#define VN "\nSPEED 1.0 02/17/83"
/* This utility sets the baud rate speed for each of the physical
   devices. */

#include <LIBRARY.H>

struct _ct ct_pdev[MAXPDEV + 2];	/* physical device table */

	/* hardware specific items */
				/* baud rates for serial ports */
#define B300	0x35		/* 300 baud */
#define B600	0x36		/* 600 baud */
#define B1200	0x37		/* 1200 baud */
#define B2400	0x3A		/* 2400 baud */
#define B4800	0x3C		/* 4800 baud */
#define B9600	0x3E		/* 9600 baud */
#define B19200	0x3F		/* 19200 baud */
struct _ct ct_br[10];	/* code table for baud rates (+ spare entries) */


	/* parameters on the command line */
#define PDEV argv[1]	/* physical device */
#define BAUD argv[2]	/* baud rate */


main(argc,argv)
int argc;
char *argv[];
{
printf(VN);	/* display sign on message */
setup();	/* setup code tables */
chk_use(argc);	/* check correct usage */

	/* check if request to show current settings */
if (usstrcmp("SHOW",argv[1]))
	{		/* No, assume setting is required */
	set_baud(get_pdev(PDEV),get_baud(BAUD)); /* set baud rate */
	}

show_baud();		/* display current settings */

} /* end of program */

setup()			/* setup the code tables for this program */
{
	/* initialize the physical device table */
ct_init(ct_pdev[0],T_DEVN,PN_T);	/* terminal */
ct_init(ct_pdev[1],P_DEVN,PN_P);	/* printer */
ct_init(ct_pdev[2],M_DEVN,PN_M);	/* modem */
ct_init(ct_pdev[3],CT_SNF,"*");	/* terminator */

	/* initialize the baud rate table */
ct_init(ct_br[0],B300,"300");
ct_init(ct_br[1],B600,"600");
ct_init(ct_br[2],B1200,"1200");
ct_init(ct_br[3],B2400,"2400");
ct_init(ct_br[4],B4800,"4800");
ct_init(ct_br[5],B9600,"9600");
ct_init(ct_br[6],B19200,"19200");
ct_init(ct_br[7],CT_SNF,"*");	/* terminator */
}

unsigned
get_pdev(ppdev)		/* get physical device */
/* This function returns the physical device code
   specified by the user in the command line. */
char *ppdev;		/* pointer to character string */
{
unsigned retval;			/* return value */

retval = ct_parc(ct_pdev,ppdev);	/* get code for ASCII string */
if (retval == CT_SNF)		/* if string not found */
	{
	printf("\n\007Physical Device '%s' is invalid or ambiguous.",
		ppdev);
	printf("\nLegal Physical Devices are : ");
	ct_disps(ct_pdev);	/* display all values */
	exit();
	}
return retval;			/* return code */
}


unsigned
get_baud(pbaud)
/* This function returns the baud rate time constant
   for baud rate specified by the user in the command line. */
char *pbaud;		/* pointer to character string */
{
unsigned retval;			/* return value */
retval = ct_parc(ct_br,pbaud);	/* get code for ASCII string */
if (retval == CT_SNF)		/* if string not found */
	{
	printf("\n\007Baud Rate '%s' is invalid or ambiguous.",
		pbaud);
	printf("\nLegal Baud Rates are : ");
	ct_disps(ct_br);	/* display all values */
	exit();
	}
return retval;			/* return code */
}

set_baud(pdevc,baudc)		/* set the baud rate of the specified device */
int pdevc;			/* physical device code */
short baudc;			/* baud rate code */
			/* on some systems this may have to be a 
			   two-byte (unsigned) value. */
{
short *baud_rc;		/* pointer to the baud rate constant */
			/* on some systems this may have to be a 
			   two-byte (unsigned) value. */
/* Note : the respective codes for accessing the baud rate constants
   via the get_cba (get configuration block address) function are:
	Device #0 = 19,  #1 = 21, #2 = 23. This function uses this
   mathematical relationship. */

	/* setup pointer to the baud rate constant */
baud_rc = get_cba(CB_D0_BRC + (pdevc << 1));

	/* then set the baud rate constant */
*baud_rc = baudc;

	/* then call the BIOS initialization routine */
bios(CIOINIT,pdevc);
}


show_baud()		/* show current baud rate */
{

int pdevn;			/* physical device number */
short baudc;			/* baud rate code */
			/* on some systems this may have to be a 
			   two-byte (unsigned) value. */
short *baud_rc;			/* pointer to the baud rate constant */
			/* on some systems this may have to be a 
			   two-byte (unsigned) value. */
/* Note : the respective codes for accessing the baud rate constants
   via the get_cba (get configuration block address) function are:
	Device #0 = 19,  #1 = 21, #2 = 23. This function uses this
   mathematical relationship. */

printf("\nCurrent Baud Rate settings are :");


for (pdevn = 0; pdevn <= MAXPDEV; pdevn ++)	/* all physical devices */
	{
		/* setup pointer to the baud rate constant -
		   the code for the get_cba function is computed
		   by adding the physical device number *2 to
		   the Baud Rate code for device #0 */

	baud_rc = get_cba(CB_D0_BRC + (pdevn << 1));

		/* then set the baud rate constant */
	baudc = *baud_rc;

	printf("\n\t%s set to %s baud.",
		ct_strc(ct_pdev,pdevn),	/* get ptr to device name */
		ct_strc(ct_br,baudc) ); /* get ptr to baud rate */
	}
}

chk_use(argc)		/* check correct usage */
int argc;		/* argument count */
{
if (argc == 1)
	{
	printf("\nThe SPEED utility sets the baud rate speed for each physical device.");
	printf("\nUsage is :  SPEED physical-device baud-rate, or");
	printf("\n            SPEED SHOW     (to show current settings)");
	printf("\n\nValid physical devices are: ");
	ct_disps(ct_pdev);
	printf("\nValid baud rates are: ");
	ct_disps(ct_br);
	exit();
	}
}
