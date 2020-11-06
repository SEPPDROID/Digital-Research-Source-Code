#define VN "\nPROTOCOL Vn 1.0 02/17/83"
/* PROTOCOL - This utility sets the serial port protocol for the
   specified physical device. Alternatively, it displays the
   current protocols for all of the serial devices. */

#include <LIBRARY.H>

	/* code tables used to relate ASCII strings to code values */
struct _ct ct_iproto[3];	/* code table for input protocols */
struct _ct ct_oproto[4];	/* code table for output protocols */
struct _ct ct_dproto[7];	/* code table for displaying protocols */
struct _ct ct_pdev[MAXPDEV + 2];	/* physical device table */
struct _ct ct_io[3];		/* input, output */


	/* parameters on the command line */
#define PDEV argv[1]	/* physical device */
#define IO argv[2]	/* input/output */
#define PROTO argv[3]	/* protocol */
#define PROTOL argv[4]	/* protocol message length */


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
	set_proto(get_pdev(PDEV),	/* physical device */
			/* Input/Output and Protocol */
		get_proto(get_io(IO),PROTO),
		PROTOL);	/* Protocol message length */
	}
show_proto();

} /* end of program */

setup()			/* setup the code tables for this program */
{
	/* initialize the physical device table */
ct_init(ct_pdev[0],0,PN_T);	/* terminal */
ct_init(ct_pdev[1],1,PN_P);	/* printer */
ct_init(ct_pdev[2],2,PN_M);	/* modem */
ct_init(ct_pdev[3],CT_SNF,"*");	/* terminator */

	/* initialize the input/output table */
ct_init(ct_io[0],0,"INPUT");
ct_init(ct_io[1],1,"OUTPUT");
ct_init(ct_io[2],CT_SNF,"*");		/* terminator */

	/* initialize the output protocol table */
ct_init(ct_oproto[0],DT_ODTR,"DTR");
ct_init(ct_oproto[1],DT_OXON,"XON");
ct_init(ct_oproto[2],DT_OETX,"ETX");
ct_init(ct_oproto[3],CT_SNF,"*");	/* terminator */

	/* initialize the input protocol table */
ct_init(ct_iproto[0],DT_IRTS,"RTS");
ct_init(ct_iproto[1],DT_IXON,"XON");
ct_init(ct_iproto[2],CT_SNF,"*");	/* terminator */

	/* initialize the display protocol */
ct_init(ct_dproto[0],DT_ODTR,"Output DTR");
ct_init(ct_dproto[1],DT_OXON,"Output XON");
ct_init(ct_dproto[2],DT_OETX,"Output ETX");
ct_init(ct_dproto[3],DT_IRTS,"Input RTS");
ct_init(ct_dproto[4],DT_IXON,"Input XON");
ct_init(ct_dproto[5],CT_SNF,"*");
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

unsigned
get_proto(output,pproto)
/* This function returns the protocol code for the
   protocol specified by the user in the command line. */
int output;		/* =1 for output, =0 for input */
char *pproto;		/* pointer to character string */

{
unsigned retval;		/* return value */

if (output)			/* OUTPUT specified */
	{
		/* get code for ASCII string */
	retval = ct_parc(ct_oproto,pproto);
	if (retval == CT_SNF)		/* if string not found */
		{
		printf("\n\007Output Protocol '%s' is invalid or ambiguous.",
	pproto);
		printf("\nLegal Output Protocols are : ");
		ct_disps(ct_oproto);	/* display valid protocols */
		exit();
		}
	}
else				/* INPUT specified */
	{
		/* get code for ASCII string */
	retval = ct_parc(ct_iproto,pproto);
	if (retval == CT_SNF)		/* if string not found */
		{
		printf("\n\007Input Protocol '%s' is invalid or ambiguous.",
	pproto);
		printf("\nLegal Input Protocols are : ");
		ct_disps(ct_iproto);	/* display valid protocols */
		exit();
		}
	}
return retval;			/* return code */
}


set_proto(pdevc,protoc,pplength)	/* set the protocol for physical device */
int pdevc;			/* physical device code */
unsigned protoc;			/* protocol byte */
char *pplength;			/* pointer to protocol length */
{
struct _ppdt
{
char *pdt[16];		/* array of 16 pointers to the device tables */
} ;
struct _ppdt *ppdt;		/* pointer to the device table array */
struct _dt *dt;			/* pointer to a device table */

ppdt = get_cba(CB_DTA);	/* set pointer to array of pointers */
dt = ppdt -> pdt[pdevc];

if (!dt)		/* chk if pointer in array is valid */
	{
	printf("\nError - Array of Device Table Addresses is not set for device #%d.",
		pdevc);
	exit();
	}

if (protoc & 0x8000)	/* check if protocol byte to be set
			   directly or to be OR'ed in */
	{		/* OR'ed */
	dt -> dt_st1 |= (protoc & 0x7F);
	}
else
	{		/* set directly */
	dt -> dt_st1 = (protoc & 0x7F);
	}	

if ((protoc & 0x7F) == DT_OETX)	/* if etx/ack, check for message
				   length */
	{
	if (isdigit(*pplength))		/* check if length present */
		{
			/* convert length to binary and set device
			   table field. */
		dt -> dt_etxml = atoi(pplength);
		}
	}
}


show_proto()		/* show the current protocol settings */
{
struct _ppdt
{
char *pdt[16];		/* array of 16 pointers to the device tables */
} ;
struct _ppdt *ppdt;		/* pointer to the device table array */
struct _dt *dt;			/* pointer to a device table */
int pdevc;			/* physical device code */
struct _ct *dproto;		/* pointer to display protocols */

ppdt = get_cba(CB_DTA);	/* set pointer to array of pointers */

	/* for all physical devices */
for (pdevc = 0; pdevc <= MAXPDEV; pdevc++)
	{
		/* set pointer to device table */
	dt = ppdt -> pdt[pdevc];

	if (dt) /* chk if pointer in array is valid */
		{
		printf("\n\tProtocol for %s - ",ct_strc(ct_pdev,pdevc));
			/* check if any protocols set */
		if (!(dt -> dt_st1 & ALLPROTO))
			{
			printf("None.");
			continue;
			}

			/* set pointer to display protocol table */
		dproto = ct_dproto;
		while (dproto -> _ct_code != CT_SNF)
			{
				/* check if protocol bit set */
			if (dproto -> _ct_code & dt -> dt_st1)
				{	/* display protocol */
				printf("%s ",dproto -> _ct_sp);
				}
			++dproto;	/* move to next entry */
			}
			/* chk if etx/ack protocol and
			   message length to be displayed */
		if (dt -> dt_st1 & DT_OETX)
			printf(" Message Length %d bytes.",
				dt -> dt_etxml);
		}
	}
}


chk_use(argc)		/* check for correct usage */
int argc;		/* argument count on commmand line */
{
if (argc == 1)
	{
	printf("\nPROTOCOL sets the physical device's serial protocols.");
	printf("\n\tPROTOCOL physical-device direction protocol {message-length}");
	printf("\n\nLegal physical devices are :");
	ct_disps(ct_pdev);
	printf("\nLegal direction/protocols are :");
	ct_disps(ct_dproto);
	printf("\n\tMessage length can be specifed with Output Etx.\n");
	exit();
	}
}
