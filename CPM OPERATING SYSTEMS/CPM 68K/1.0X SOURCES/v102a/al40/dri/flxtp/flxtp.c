/*
 * flxtp keys [name ...]
 *	keys are:
 *		w write to beginning of tape
 *		a append to end of tape
 *		r read named files from tape; if no names, extract all
 *		t table named files; or table all
 *	also may add:
 *		v verbose
 *		i wait for user 'y' before performing function
 *		u leave extracted filenames in upper case (usually
 *		  filenames are mapped to lower case)
 *		n don't rewind tape before performing action
 *		[x,y]	specify uic for tape
 *		pooo	use protection ooo on tape
 *		bddd	use blocksize ddd to write on tape
 *			(tape read automagically determines blksize)
 *		ddddd	use density dddd (800 or 1600)
 *		o ok to overwrite existing files when extracting
 *		  (default will not overwrite existing files)
 *		m make a new directory for each uic found on tape and
 *		  put the extracted files in them
 *
 *	name is a unix path name (for writing tapes) or the name of
 *	a file on the tape. When writing tapes, a name may be a uic
 *	in the same form as above. This causes all following files
 *	to be written on the tape in that uic.
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#define FLXHD	14
#define FLXDATA 512
/*
 * len of longest unpacked filename, including the null at end 
 * and one extra space for version 'number'
 */
#define FLXNM	15
#define MAXBASE	(9+1)
#define MAXEXT	(3+1)

/* note byte swap for DEC machines */
#define OFFNAME	0
#define OFFEXT	4
#define OFFUIC2	6
#define OFFUIC1 7
#define OFFPROT2 8
#define OFFPROT1 9
#define OFFDATE2 10
#define OFFDATE1 11
#define OFFOVER	12

#define DEFUIC1	'\001'
#define DEFUIC2	'\001'
#define DEFPROT1	'\000'
#define DEFPROT2	'\233'


#define CR	'\015'
#define LF	'\012'
#define NL	'\012'

#define TPWRITE	'w'
#define TPAPPEND	'a'
#define TPREAD	'r'
#define TPTABLE	't'

#define	IOPT	'i'
#define	VOPT	'v'
#define UOPT	'u'
#define NOPT	'n'
#define POPT	'p'
#define BOPT	'b'
#define DOPT	'd'
#define OOPT	'o'
#define MOPT	'm'

#define NOCMD	'\0'

struct parsehdr {
	char	phname[FLXNM];
	char	phuic1;
	char	phuic2;
	char	phprot1;
	char	phprot2;
	char	phdate[11];
};

int verbose, mkdirs, overwrite, inquire, allfiles, uicfiles, upcase, norewind = 0;
int argsleft, filenbr, actnbr = 0;
char tp1600[] = "/dev/nrmt0";
char tp1600rw[] = "/dev/rmt0";
char bsr1600[] = "mt -t /dev/nrmt0 bsr 1";
char tp800[] = "/dev/nrmt0";
char tp800rw[] = "/dev/rmt0";
char bsr800[] = "mt -t /dev/nrmt0 bsr 1";
char *devtp = tp1600;
char *devtprw = tp1600rw;
char *bsrcmd = bsr1600;
int tpfd;
FILE *dfd;

char uic1 = DEFUIC1;
char uic2 = DEFUIC2;
char prot1 = DEFPROT1;
char prot2 = DEFPROT2;
char date1 = '\000';
char date2 = '\000';

#define BIGBLK 32768

unsigned tpblksiz = FLXDATA;
char *tpbuf, *malloc();

char cmd = NOCMD;

/*
 * flxtp - read or write flx format tape
 */

main (argc, argv)
int argc;
char **argv;
{
int k;
int density;
char c = '\0';
	if (argc < 2) usage();
	if (argc == 2) allfiles++;
	else argsleft = argc - 2;
	argv++;	/* skip over command name */

	k=0;
	while ( (c=(*argv)[k++]) != '\0') {
		switch (c) {
			case TPAPPEND:
#ifndef PICKLE
				error ("append function not implemented");
#endif
				/* fall thru to write */
			case TPWRITE:
				if (cmd != NOCMD) error ("Only one of rwat may be selected");
				if (allfiles) error ("Name(s) must be given for w or a");
				cmd = c;
				break;
			case TPREAD:
				/* fall thru */
			case TPTABLE:
				if (cmd != NOCMD) error ("Only one of rwat may be selected");
				cmd = c;
				break;
			
			case MOPT: mkdirs++; break;
			case OOPT: overwrite++; break;
			case NOPT: norewind++; break;
			case IOPT: inquire++; break;
			case VOPT: verbose++; break;
			case UOPT: upcase++; break;
			case '[':
				if(sscanf(&(*argv)[k],"%3o,%3o]",&uic1,&uic2)==2 ) {
					while ( (*argv)[k++] != ']');
				}
				else error ("Uic must be given as [xxx,yyy]");
				uicfiles++;
				break;
			case POPT:
				if(sscanf(&(*argv)[k],"%3o",&prot2) == 1 ) {
					while ( isdigit ( (*argv)[k]) )
						k++;
				}
				else error ("Protection must be given as pooo");
				break;
			case BOPT:
				if (sscanf(&(*argv)[k],"%d",&tpblksiz) == 1) {
					while ( isdigit ( (*argv)[k] ) )
						k++;
				}
				else error ("Tape blocksize must be given as bddd");
			
				break;
	
			case DOPT:
				if (sscanf(&(*argv)[k],"%d",&density) == 1) {
					while ( isdigit ( (*argv)[k] ) )
						k++;
					if (density==1600) {/* already set */}
					else if (density==800) {
						devtp=tp800;
						devtprw=tp800rw;
						bsrcmd=bsr800;
					}
					else error ("Density must be given as d800 or d1600");
				}
				else error ("Density must be given as d800 or d1600");
				break;
			case '-': /* ignore */ break;
			default:
				usage();
			} /* switch */
		} /* while */


	if (cmd == TPREAD | cmd == TPTABLE) tpblksiz = BIGBLK;

	if ( (tpbuf=malloc(tpblksiz)) == (char *) 0)
		error ("Sorry, can't get buffer for your blocksize");

	argv++;	/* skip over keys */

	if ( !norewind) {
		if((tpfd=open(devtprw,0)) == -1)
			error ("Can't access tape");
		close(tpfd);
	}
	switch (cmd) {
	case TPAPPEND:
		seekeot();
		/* fall thru to put files */
	case TPWRITE:
		putfiles (argv);
		break;
	case TPREAD:
		getfiles (argv);
		break;
	case TPTABLE:
		tblfiles (argv);
		break;
	default:
		error ("Pgm error; see sys msg");
	} /* switch */

	exit (0);
} /* flxtp */

error (msg)
char *msg;
{
	fprintf (stderr,"%s\n",msg);
	exit (-1);
} /* error */

usage ()
{
	fprintf (stderr,"Usage: flxtp (rwat)[vilnupOOObDDDdDDDD] [name ...]\nwhere u = [x,y]\n");
	exit (-1);
} /* usage */
int tblfiles (list)
char **list;
{
char hdr[FLXHD];
struct parsehdr phdr;
int nr, nbrblk, totblks  = 0;
	if ( (tpfd=open(devtp,0)) == -1) error ("Can't open tape");
	while (allfiles | argsleft) {
		if ((nr=read(tpfd,hdr,FLXHD)) == 0) break;
		expandhdr (hdr, &phdr);
		filenbr++;
		if (amatch (&phdr, list)) {
			nbrblk = gobblefile();
			totblks += nbrblk;
			fprintf(stdout,"File %4d: %-13s %5d <%3o> [%3o,%3o]\n",
			  filenbr, phdr.phname, nbrblk,
			  phdr.phprot2&0xff, phdr.phuic1&0xff, phdr.phuic2&0xff);
			actnbr++;
			if (!allfiles) argsleft--;
		}
		else nbrblk = gobblefile();
	}
	if(allfiles) fprintf (stdout,"Total %d data blocks in %d files listed\n",
	  totblks,actnbr);
	if (!allfiles & argsleft) fprintf (stderr,"Other files not found\n");
	close (tpfd);
} /* tblfiles */

int gobblefile ()
{

int nr, nbrblk = 0;
	while ((nr=read(tpfd,tpbuf,tpblksiz)) > 0)
		nbrblk++;
	return (nbrblk);
} /* gobblefile */
int getfiles (list)
char **list;
{
char wuic1, wuic2 = 0;
char hdr[FLXHD], reply[2], *pfull, *p, *q, *v, *fname, overname[FLXNM];
static char versions[] = "!@#$%^&*()";
static char mkdircmd [] = "mkdir 000,000.dir";
static char *pathname = &mkdircmd[6];
static char fullname[27];
struct parsehdr phdr;
int nr, ip, jp, nbrblk = 0;
	if ( (tpfd=open(devtp,0)) == -1) error ("Can't open tape");
	while (allfiles | argsleft) {
		if ((nr=read(tpfd,hdr,FLXHD)) == 0)
			break;
		expandhdr ( hdr, &phdr);
		filenbr++;
		if (amatch (&phdr, list) ) {
			if (!allfiles) argsleft--;
			if (inquire) {
				fprintf (stdout,"Extract %s? (y/n) ",phdr.phname);
				fread(reply,1, 2, stdin);
				if (*reply != 'y') goto skip;
			}
			if (mkdirs) {
				if (phdr.phuic1 != wuic1 | phdr.phuic2 != wuic2) {
					wuic1=phdr.phuic1;
					wuic2=phdr.phuic2;
					sprintf(pathname,
					  "%03o,%03o.dir",
					   wuic1&0xff, wuic2&0xff);
					if (access(pathname,0) != 0) {
						if (system(mkdircmd) != 0)
						error ("Can't make directory");
					}
				}
				pfull = fullname;
				p = pathname;
				while ( *pfull++ = *p++ );
				pfull[-1] = '/';
				p = phdr.phname;
				while ( *pfull++ = *p++);
				fname = fullname;
			}
			else fname = phdr.phname;

			if (verbose) fprintf (stdout,
			  "get [%3o,%3o] %s file # %d\n",
			  phdr.phuic1&0xff, phdr.phuic2&0xff, fname, filenbr);
			if (!overwrite && (access(fname,0 /* exists */) == 0)) {
				/* try adding 'version' char at end of name */
				v=versions;
				p=fname; q=overname;
				while (*p) *q++ = *p++;
				*(q+1) = '\0';
				while (*v) {
					*q = *v;
					if (access(overname,0)) break;
					v++; /* try next 'version' nbr */
				}
				if (*v == 0) { /*already 10 versions */
					printf("Can't overwrite %s\n",
					  overname);
					goto skip;
				}
				printf("Duplicate filename; %s used\n",
				  overname);
				fname = overname;
			}
			if ( (dfd=fopen(fname, "w")) == NULL)
				error ("Can't open disk file");
			while ( (nr=read(tpfd, tpbuf, tpblksiz)) > 0){
				ip = jp = 0;
				while (ip<nr) {
					tpbuf[jp] = tpbuf[ip];
					if (tpbuf[jp] == CR
					  || tpbuf[jp] == '\0')
						jp--;
					ip++; jp++;
				} /* while */
				if (fwrite(tpbuf, 1, jp, dfd) != jp)
					error ("Disk write error");
			} /* while */
			if (nr == -1) error ("Tape read error");
			fclose(dfd);
			actnbr++;
			continue;
		}
		skip: nbrblk=gobblefile();
	}
	if (allfiles | verbose) fprintf (stdout,"Total %d files extracted\n",actnbr);
	if ((!allfiles) & argsleft) fprintf (stderr,"%d files not found\n",argsleft);
	close (tpfd);
} /* getfiles */
seekeot () {
int nr, stat;
	if ( (tpfd=open(devtp,0)) == -1) error ("Can't open tape");
	if (verbose) fprintf (stdout,"Seeking to end of tape\n");
	while (1) {
		stat = read(tpfd,tpbuf,tpblksiz);

		if(stat <= 0) break;
		nr = gobblefile ();
		filenbr++;
	}
	if (stat == -1) error ("Can't find end of tape");
	close (tpfd);
	/* back up before second eot mark */
	if (system(bsrcmd)) error ("Can't find end of tape");
} /* seekeot */
putfiles (list)
char **list;
{
int n, nbrblk = 0;
char reply[2];
struct stat sbuf;

	while ( *list != NULL) {
		actnbr++;
		filenbr++;
		if (**list == '[') {
			/* set new uic for writing */
			if (sscanf(*list, "[%3o,%3o]", &uic1, &uic2) != 2)
				error ("Uic must be given as [xxx,yyy]");
			list++;
			actnbr--;
			filenbr--;
			continue;
		}
		if ( (stat(*list,&sbuf) == 0) &&
		  (sbuf.st_mode & S_IFDIR) ) {
			printf("Directory %s ignored\n", *list);
			list++;
			actnbr--;
			filenbr--;
			continue;
		}
		if (inquire) {
			fprintf (stdout, "%c File # %d %s? (y/n) ",cmd, actnbr,  *list);
			fread (reply, 1, 2, stdin);
			if (*reply != 'y') continue;
			actnbr--;
			filenbr--;
		}
		if (verbose)
		  fprintf (stdout,"%c File #%4d [%3o,%3o] %s ",
		  cmd, filenbr, uic1&0xff, uic2&0xff, *list);
		if ( (dfd=fopen(*list, "r")) != NULL) {
			if ( (tpfd=open(devtp,2)) == -1)
				error ("Can't open tape");
			if (puthdr(*list)) error ("Can't write tape");
			nbrblk=0;
			while ( n=buildbuf()) {
				if (write(tpfd,tpbuf,n) != n)
					error ("Tape write error");
				nbrblk++;
			}
			close(tpfd);
			fclose(dfd);
			if (verbose) fprintf (stdout,"%d data blocks\n",nbrblk);
		}
		else fprintf (stderr,"Can't open diskfile %s\n", *list);
	list++;
	} /* while */
	if (verbose) fprintf (stdout, "Total %d files written to tape\n",actnbr);
} /* putfiles */
int puthdr (name)
char *name;
{
char buf[FLXHD], base[MAXBASE], ext[MAXEXT], *nend;
int i;

	/* get parts of filename */

	fnparts(name,base,MAXBASE,ext,MAXEXT);

	/* pad both parts with blanks */

	nend=base;
	i=0;
	while (*nend) {nend++; i++;}
	while ( i < MAXBASE ) base[i++] = ' ';

	nend=ext;
	i=0;
	while (*nend) {nend++; i++;}
	while ( i < MAXEXT ) ext[i++] = ' ';

	/* convert both parts to rad50 */

	ator50 (base, 6, &buf[OFFNAME]);
	ator50 (&base[6], 3, &buf[OFFOVER]);
	ator50 (ext, 3, &buf[OFFEXT]);

	buf[OFFUIC1] = uic1;
	buf[OFFUIC2] = uic2;
	buf[OFFPROT1] = prot1;
	buf[OFFPROT2] = prot2;
	buf[OFFDATE1] = date1;
	buf[OFFDATE2] = date2;

	if (write(tpfd,buf,FLXHD) != FLXHD) return (-1);
	return (0);
} /* puthdr */
int buildbuf ()
{
static int crlf, deof = 0;
int n, c;
	n = 0;
	if (crlf) {
		tpbuf[n++] = LF;
		crlf = 0;
	}

	if (deof) {deof=0; return(0);}

	while (n <tpblksiz) {
		c=getc(dfd);
		if (c == EOF) {deof++; break;}
		if (c == NL) {
			tpbuf[n++] = CR;
			if (n<tpblksiz) tpbuf[n++] = LF;
			else {crlf++; break;}
		}
		else tpbuf[n++] = c;
	}
	if (n%2 != 0) tpbuf[n++] = '\0';
	return (n);
} /* buildbuf */
char unmap (n)
unsigned n;

{
static char rm[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ$.!0123456789";
static char rml[]= " abcdefghijklmnopqrstuvwxyz$.!0123456789";
	if (n<40) {
		if (upcase) return (rm[n]);
		else return (rml[n]);
	}
	return ('!');	/* error */
} /* unmap */

r50toa (s, slen, d)
char *s, *d;
int slen;
{
unsigned word;
int  i, j, i2;
	for (i=j=0; i<slen; i++) {
		i2= i*2;
		/* note byte swap for DEC machines */
		word = ( (s[i2+1]<<8) | (s[i2]&0xff) ) & 0xffff;
		d[j++]=unmap(word/1600);
		d[j++]=unmap((word%1600)/40);
		d[j++]=unmap((word%1600)%40);
	} /* for */
	d[j++]='\0';
} /* r50toa */
ator50 (s, slen, d)
char *s, *d;
int slen;
{
int i, word;
	for (i=0; i<slen; i+=3) {
		word = radmap(s[i])*1600 + radmap(s[i+1])*40 + radmap(s[i+2]);
		/* note byte swap for DEC machine */
		*d++ = word;
		*d++ = word>>8;
	} /* for */
} /* ator50 */

int radmap (c)
char c;
{
	if (islower(c)) return (c-96);
	if (isupper(c)) return (c-64);
	if (isdigit(c)) return (c-18);
	if (c==' ') return (0);
	if (c=='$') return (27);
	if (c=='.') return (28);
	return (29);
} /* radmap */

#define MATCH	1
#define NOMATCH	0

int amatch (ph, list)
struct parsehdr *ph;
char **list;
{
	if (allfiles) {
		if (uicfiles) {
			if (ph->phuic1 == uic1 & ph->phuic2 == uic2)
				return (MATCH);
			else return (NOMATCH);
		}
		else return (MATCH);
	}
	if (uicfiles)
		if (ph->phuic1 != uic1 | ph->phuic2 != uic2) return (NOMATCH);
	while (*list) {
		if (strncmp (ph->phname, *list, FLXNM) == 0)
			return (MATCH);
		list++;
	} /* while */
	return (NOMATCH);
} /* amatch */
expandhdr (hdr, ph)
char *hdr;
struct parsehdr *ph;
{
char *nm;
	nm = ph->phname;
	r50toa(&hdr[OFFNAME], 2 /* words */, nm);
	while (*nm > ' ') nm++;
	r50toa(&hdr[OFFOVER], 1 /* word */, nm);
	while (*nm > ' ') nm++;
	*nm++ = '.';
	r50toa (&hdr[OFFEXT], 1 /* word */, nm);
	while (*nm > ' ') nm++;
	/* if filename ends in '.', just drop it */
	if ( *(nm - 1) == '.') nm--;
	*nm = '\0';
	ph->phuic1 = hdr[OFFUIC1];
	ph->phuic2 = hdr[OFFUIC2];
	ph->phprot1 = hdr[OFFPROT1];
	ph->phprot2 = hdr[OFFPROT2];
	ph->phdate[0] = '\0'; /* don't yet know how to parse date */
} /* expandhdr */
