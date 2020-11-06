#define VN "\nASSIGN Vn 1.0 02/17/83"

#include <LIBRARY.H>

struct _ct ct_pdev[MAXPDEV + 2];	/* physical device table */


	/* names of logical devices */
#define LN_C	"CONSOLE"
#define LN_A	"AUXILIARY"
#define LN_L	"LIST"
struct _ct ct_ldev[4];		/* logical device table */

struct _ct ct_io[3];		/* input, output */

	/* parameters on the command line */
#define LDEV argv[1]	/* logical device */
#define IO argv[2]	/* input/output */


main(argc,argv)
int argc;
char *argv[];
{

printf(VN);	/* display signon message */
setup();	/* setup code tables */
chk_use(argc);	/* check correct usage */

	/* check if request to show current settings */
if (usstrcmp("SHOW",argv[1]))
	{		/* no, assume a set is required */
		/* NOTE : the number of physical devices to
		   process is given by argc -3 */
	set_assign(get_ldev(LDEV),get_io(IO),argc - 3,argv);
	}
show_assign();

}

setup()			/* setup the code tables for this program */
{
	/* initialize the physical device table */
ct_init(ct_pdev[0],0,PN_T);	/* terminal */
ct_init(ct_pdev[1],1,PN_P);	/* printer */
ct_init(ct_pdev[2],2,PN_M);	/* modem */
ct_init(ct_pdev[3],CT_SNF,"*");	/* terminator */

	/* initialize the logical device table */
ct_init(ct_ldev[0],0,LN_C);	/* terminal */
ct_init(ct_ldev[1],1,LN_A);	/* auxiliary */
ct_init(ct_ldev[2],2,LN_L);	/* list */
ct_init(ct_ldev[3],CT_SNF,"*");	/* terminator */

	/* initialize the input/output table */
ct_init(ct_io[0],0,"INPUT");
ct_init(ct_io[1],1,"OUTPUT");
ct_init(ct_io[2],CT_SNF,"*");		/* terminator */

}

unsigned
get_ldev(pldev)		/* get logical device */
/* This function returns the logical device code
   specified by the user in the command line. */
char *pldev;		/* pointer to character string */
{
unsigned retval;			/* return value */
retval = ct_parc(ct_ldev,pldev);	/* get code for ASCII string */
if (retval == CT_SNF)		/* if string not found */
	{
	printf("\n\007Logical Device '%s' is invalid or ambiguous.",
		pldev);
	printf("\nLegal Logical Devices are : ");
	ct_disps(ct_ldev);	/* display all values */
	exit();
	}
return retval;			/* return code */
}

unsigned
get_io(pio)		/* get input/output parameter */
char *pio;		/* pointer to character string */
{
unsigned retval;			/* return value */

retval = ct_parc(ct_io,pio);	/* get code for ASCII string */
if (retval == CT_SNF)		/* if string not found */
	{
	printf("\n\007Input/Output direction '%s' is invalid or ambiguous.",
		pio);
	printf("\nLegal values are : ");
	ct_disps(ct_io);	/* display all values */
	exit();
	}
return retval;			/* return code */
}

set_assign(ldevc,output,argc,argv)	/* set assignment (i/o redirection) */
int ldevc;			/* logical device code */
int output;			/* i/o redirection code */
int argc;			/* count of arguments to process */
char *argv[];			/* replica of parameter to main function */
{
unsigned *redir;		/* pointer to redirection word */
int pdevc;			/* physical device code */
unsigned rd_val;		/* redirection value */

	/* get the address of the i/o redirection word.
	   this code assumes that get_cba code values
	   are ordered :
		Device #0, Input & Output
		Device #1, Input & Output
		Device #2, Input & Output.
	   the get_cba code is computed by multiplying the
	   logical device code * 2 (that is, shift left 1)
	   and added onto the code for Device #0, Input.
	   then the output variable (0 = Input, 1 = Output)
	   is added on.  */
redir = get_cba(CB_CI + (ldevc << 1) + output);

rd_val = 0;	/* initialize redirection value */

	/* for output, assignment can be made to several physical
	   devices, so this code may be executed several times. */
do
	{	
		/* get code for ASCII string */
		/* NOTE : the physical device parameters start
		   with parameter #3 (argv[3]). However argc
		   is a decreasing count of the number of physical
		   devices to be processed - therefore argc + 2
		   causes them to be processed in reverse order
		   (i.e. from right to left on the command line) */

	pdevc = ct_parc(ct_pdev,argv[argc + 2]);

	if (pdevc == CT_SNF)		/* if string not found */
		{
		printf("\n\007Physical Device '%s' is invalid or ambiguous.",
		argv[argc + 2]);
		printf("\nLegal Physical Devices are : ");
		ct_disps(ct_pdev);	/* display all values */
		exit();
		}
		/* repeat this loop for as long as there are 
		   more parameters (for output only) */
	else
		{
		/* build new redirection value by OR'ing in
		   a 1-bit shifted left pdevc places. */
		rd_val |= (1 << pdevc);
		}
	} while (--argc && output);

*redir = rd_val;	/* set the value into the config. block */
}

show_assign()			/* show current baud rate */
{
int rd_code;			/* redirection code for get_cba */
int ldevn;			/* logical device number */
int pdevn;			/* physical device number */
unsigned rd_val;		/* re-direction value */
unsigned *prd_val;		/* pointer to the redirection value */

/* Note : the respective codes for accessing the redirection values
   via the get_cba (get configuration block address) function are:
	Device #0 Console Input  - 5
	Device #0 Console Output - 6
	Device #1 Auxiliary Input  - 7
	Device #1 Auxiliary Output - 8
	Device #2 List Input  - 9
	Device #2 List Output - 10

   This function uses this mathematical relationship. */

printf("\nCurrent Device Assignments are :");


	/* for all get_cba codes */
for (rd_code = CB_CI; rd_code <= CB_LO; rd_code++)
	{
		/* set pointer to redirection value */
	prd_val = get_cba(rd_code);
		/* get the input redirection value */
	rd_val = *prd_val;	/* this also performs byte-reversal */

		/* display device name. rd_code is converted to a 
		   device number by subtracting the first code number
		   from it and dividing by 2 (shift right one place).
		   the input/output direction is derived from the
		   least significant bit of the rd_code. */

	printf("\n\t%s %s is assigned to - ",
		ct_strc(ct_ldev,(rd_code - CB_CI) >> 1),
		ct_strc(ct_io,((rd_code & 0x01) ^ 1)));

		/* for all physical devices */
	for (pdevn = 0; pdevn < 16; pdevn++)
		{
			/* check if current physical device is assigned
			   by ANDing with a 1-bit shifted left pdevn times */
		if (rd_val & (1 << pdevn))	/* is device active */
			{	/* display physical device name */
			printf(" %s",ct_strc(ct_pdev,pdevn) );
			}
		}

	}
}


chk_use(argc)		/* check for correct usage */
int argc;		/* argument count on commmand line */
{
if (argc == 1)
	{
	printf("\nASSIGN sets the Input/Output redirection.");
	printf("\n\tASSIGN logical-device INPUT physical-device");
	printf("\n\tASSIGN logical-device OUTPUT physical-dev1 {phy_dev2..}");
	printf("\n\tASSIGN SHOW    (to show current assignments)");
	printf("\n\nLegal logical devices are :");
	ct_disps(ct_ldev);
	printf("\nLegal physical devices are :");
	ct_disps(ct_pdev);
	exit();
	}
}
