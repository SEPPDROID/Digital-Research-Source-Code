/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) signal.h - Jul 26, 1983  REGULUS 4.1
*/

/*
 * The following define the REGULUS signals
 * Those signals with *'s in the comment field cause core dumps if
 *	not caught or ignored.
 */

#define SIG_HUP		1	/*  	Hangup					 */
#define	SIGHUP		1
#define SIG_INT		2	/*  	Interupt				 */
#define	SIGINT		2
#define SIG_QUIT	3	/* *	Quit					 */
#define	SIGQUIT		3
#define SIG_ILL		4	/* *	Illegal Instruction		 */
#define	SIGILL		4
#define SIG_TRACE	5	/* *	Trace Trap				 */
#define	SIGTRAP		5
#define	SIG_ABRT	6	/* *	IOT on PDP-11, abort trap */
#define	SIGIOT		6
#define	SIGEMT		7
#define SIG_FLT		8	/* *	Floating Point Exception  */
#define	SIGFPE		8
#define SIG_KILL	9	/*  	Killed					  */
#define	SIGKILL		9
#define SIG_BUS		10	/* *	Bus Error				 */
#define	SIGBUS		10
#define SIG_SEG		11	/* *	Segmentation Violation */
#define	SIGSEGV		11
#define SIG_SYS		12	/* *	Bad Argument to System Call */
#define	SIGSYS		12
#define SIG_PIPE	13	/*  	Write on pipe Trap	 */
#define	SIGPIPE		13
#define	SIG_ALRM	14	/*		Alarm clock */
#define	SIGALRM		14
#define	SIG_TERM	15	/*		Catchable kill */
#define	SIGTERM		15
#define	SIGUSR1		16	/*		Unix User 1 */
#define	SIGUSR2		17	/*		Unix User 2 */
#define	SIGCLD		18	/*		Unix Death of child */
#define	SIGCHLD		18	/*		BSD 4.1 Death of child */
#define	SIGPWR		19	/*		Unix Power Failure */
#define SIG_ADD		20	/*  	Address error			 */
#define SIG_DIV		21	/*  	Zero divide			 */
#define SIG_CHK		22	/*  	Chk instruction		 */
#define SIG_DVR		23	/*  	Trapv instruction		 */
#define SIG_PRIV	24	/*  	Priviledge violation	 */
#define SIG_U1T		25	/*  	1010 instruction trap	 */
#define SIG_U2T		26	/*  	1111 instruction trap	 */
#define SIG_INV		27	/*  	Invalid trap			 */
#define SIG_BRK		28	/*  	Breakpoint trap		 */
#define SIG_MEM		29	/*  	Out of memory	(CIE systems, no MMU) */
#define SIG_REL		30	/*  	Bad relocation bits	(CIE systems, no MMU) */
#define SIG_UNDER	31	/*  	User stack underflow	(CIE systems, no MMU) */
#define	NUMSIG		32	/*		Number of system signals */
#define	NSIGS		64	/*		Total number of signals */
#define	NSIG		32	/*		For 4.1 BSD */

/* These are the default signal action and the ignore signal action */
#define SIG_DFL		((int (*)())0)	/* address of zero */
#define SIG_IGN		((int (*)())1)	/* address of long 1 */
int (*(signal()))();

#define	COREDUMPED	0200
