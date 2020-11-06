-ARCHIVE- $main.a86 2887
	title	'c86 basic support package'
	pagesize	51
;	7/28/81

;notes
;	the  first 4 words of usable ds space contain
;
;	100h	The address of the first byte of uninitialised global storage
;	102h	The address of the last+1 byte of uninitialized global stor
;	104h	The minimum number of reserved bytes between top of heap and
;			the bottom of the system stack
;	106h	The address of the next available cell to add to the heap
;			Note that this value is biased up by (104h) for 
;			efficient testing at run time.

DEFGBL	equ	91h		;define a global
REFGBL	equ	0a1h		;refer to a global

	cseg

;	$main	entry point for c programs

dollar_main:
	mov	ax,ds
	mov	ss,ax		;set up the stack
	mov	es,ax		;just in case
	mov	sp,.6		;get stack top value
	mov	bp,sp		;set the base pointer too

;	set up the reverved bytes value
;	and the heap top value

	mov	ax,128		;default reserved bytes
	mov	.260,ax		;save for program use
	add	ax,.258		;s-break value
	mov	.262,ax		;save for later

;	clear the uninitialized global storage region

	cld			;to go up
	mov	di,.256		;get start of static uninitialised area
	mov	cx,.258		;get end address +1 of static uninit
	sub	cx,di		;number of bytes in area
	xor	ax,ax		;zero ax
	rep	stos	al	;clear the area

;	call the routine _main to do other initialisation

umain	equ	$+1
	call	dollar_main	;enter c system at '_main'

;	_exit	abort exit point for c programs

under_exit:
	mov	cl,0
	mov	dl,cl
	int	224		;we are finished

;	set up on entry to a function

dollar_entry0:
	pop	si		;get address we came from
	push	bp		;save frame pointer
	mov	bp,sp		;get new frame pointer
	cmp	sp,.262		;how is heap space
	jbe	heap_error	;crash
	jmp	si		;go to program
dollar_entry1:
	pop	si		;get address we came from
	push	bp		;save frame pointer
	mov	bp,sp		;get new frame pointer
	cld
	db	2eh		;seg override prefix to cs:
	lods	al		;get offset
	mov	ah,0		;as a word value
	sub	sp,ax		;adjust stack
	cmp	sp,.262		;how is heap space
	jbe	heap_error	;crash
	jmp	si		;go to program
dollar_entry2:
	pop	si		;get address we came from
	push	bp		;save frame pointer
	mov	bp,sp		;get new frame pointer
	cld
	db	2eh		;seg override prefix to cs:
	lods	ax		;get the offset
	sub	sp,ax		;adjust stack
	cmp	sp,.262		;how is heap space
	jbe	heap_error	;crash
	jmp	si		;go to program

;	a heap error has occured

heap_error:
	mov	sp,bp		;reset the stack pointer
	sub	sp,2		;save the base pointer
	mov	ax,8000h	;set error flag
	push	ax
	jmps	under_exit	;go to the exit mechanism

;	define the addresses to relocate in the above code

	eseg

	dw	dollar_main
	db	DEFGBL,'$main',0
	dw	under_exit
	db	DEFGBL,'_exit',0
	dw	dollar_entry0
	db	DEFGBL,'$entry0',0
	dw	dollar_entry1
	db	DEFGBL,'$entry1',0
	dw	dollar_entry2
	db	DEFGBL,'$entry2',0
	dw	umain
	db	REFGBL,'_main',0
	dw	0,0

	end
-ARCHIVE- \cpmread.c 2171
/*	read characters from cp/m
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* true if ascii data in file */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];

/*	fill a buffer
*/

_cpmread(buf)
struct bufstr *buf;
{
  int j;
  unsigned status;

  if(buf->bfcb<256){
    buf->bptr=0;
    buf->bcnt=1;
    if(buf->blff){
      buf->blff=0;
      buf->bdata[0]='\n';
    }
    else {
      buf->bdata[0]=bdos(buf->bfcb-1,0)&0x7f;	/* strip parity */
      if(buf->bdata[0]=='\r')buf->blff=1;	/* next one out is a l/f */
      if(buf->bdata[0]==26)buf->bptr=0;		/* end of file */
    }
    return;
  }
		/* its a disk read */
  buf->bsec+=((buf->bcnt+(SECSIZE-1))/SECSIZE);	/* select sector to read */
  for(j=0;j<NUMBSEC;++j){
    bdos(0x1a,buf->bdata+(j*SECSIZE));		/* set transfer address */
    buf->bfcb->rr=buf->bsec+j;			/* set sector number */
    status=bdos(0x21,buf->bfcb)&0xff;		/* do the read */
    if(status==3 || status>4)exit(0xff);	/* terminal error */
    if(status)break;
  }
  buf->bcnt=j*SECSIZE;
  buf->bptr=0;					/* no chars read yet */
}
-ARCHIVE- \cpmwrit.c 1885
/*	write characters to a file
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char bfd;			/* fd for this file */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];

/*	empty a buffer
*/

_cpmwrite(buf)
struct bufstr *buf;
{
  int j;
  unsigned status;

  if(buf->bdirty){
    if(buf->bfcb>255){				/* disk file if not zero */
      for(j=0;j*SECSIZE<buf->bcnt;++j){
	bdos(0x1a,buf->bdata+(j*SECSIZE));	/* set transfer address */
	buf->bfcb->rr=buf->bsec++;		/* set sector number */
	status=bdos(0x22,buf->bfcb)&0xff;	/* do the write */
	if(status)exit(0x80ff);			/* terminal error */
      }
    }
    else {					/* character device */
      bdos(buf->bfcb,buf->bdata[0]);
    }
  }
  buf->bcnt=0;
  buf->bptr=0;
  buf->bdirty=0;
  setmem(buf->bdata,buf->bsize,0x1a);		/* force buffer to EOF */
}
-ARCHIVE- \main.c 1068
/*	_main	7/20/81
*/

/*	set up arguments from input command line
*/

#define MAXARG 20
#define STDIN 1
#define STDOUT 2

_main()
{
  char *cp,*cp1,*cp2,j,k,argc,*argv[MAXARG];

  cp=0x80;			/* get address of cp/m input line */
  j=*cp++;			/* get number of characters in command line */
  argv[0]=cp1=sbrk(j+2);	/* a place to build arg list(s) */
  open("con:",0);		/* open STDIN */
  open("con:",1);		/* open STDOUT */
  open("con:",1);		/* open STDERR */
  *cp1++='c';
  *cp1++=0;
  for(argc=1;argc<MAXARG;){
    while(j && (*cp==' ' || *cp=='\t')){
      --j;
      ++cp;
    }
    if(!j)break;
    argv[argc]=cp2=cp1;
    while(j && *cp!=' ' &&  *cp!='\t'){
      --j;
      k=*cp++;
      if(k>='A' && k<='Z')k+=0x20;
      *cp1++=k;
    }
    *cp1++=0;
    if(*cp2=='<'){			/* redirect input */
      close(STDIN);
      open(cp2+1,0);
    }
    else if(*cp2=='>'){			/* redirect output */
      close(STDOUT);
      creat(cp2+1,1);
    }
    else ++argc;			/* count the argument */
  }
  exit(main(argc,argv));
}
-ARCHIVE- \open.c 2706
/*	common open routine
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* flag non zero if ascii file */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

char *_opentab[MAXFILES];

_open(filename,mode,new)
char *filename;
unsigned mode,new;
{
  int j;
  unsigned status,size;
  struct cpmfcb *fcb;
  struct bufstr *buf;

  if(mode>5)return -1;			/* bad open mode */
  if(mode>2)++mode;
  for(j=1;j<MAXFILES;++j)if(!_opentab[j])break;
  if(j==MAXFILES)return -1;		/* all possable files open */
  if(fcb=_opencdv(filename,++mode))size=1;
  else {
    if(!(fcb=makefcb(filename)))return -1;/* bad file name */
    if(new){
      bdos(0x13,fcb);			/* delete the file if it exists */
      new=7;				/* set up for a create */
    }
    status=bdos(0x0f+new,fcb)&0xff;	/* open/create the file */
    if(status>3){
      free(fcb);
      return -1;			/* file not found */
    }
    size=SECSIZE*NUMBSEC;
  }
  buf=alloc(SZBUFSTR-1+size);		/* get the buffer */
  if(!buf){
    if(fcb>255)free(fcb);
    return -1;
  }
  _opentab[j]=buf;
  buf->basc=(mode<4);
  buf->bfcb=fcb;
  buf->bmode=mode&3;
  buf->bsize=size;
  setmem(buf->bdata,size,0x1a);		/* fill with eof chars */
  return j;
}

/*	test for opening of a character device
*/

_opencdv(filename,mode)
char *filename;
unsigned mode;
{
  char temp[5];
  int j;

  if(strlen(filename)!=4)return 0;		/* not one of ours */
  for(j=0;j<5;++j)temp[j]=toupper(*filename++);	/* to upper case */
  if(!strcmp(temp,"CON:"))return 2;		/* its the console */
  if(!strcmp(temp,"PRN:") && (mode&2))return 5;	/* its a printer */
  return 0;					/* none of the above */
}
-ARCHIVE- \printf.c 1285
/*	a service routine for fprintf et al
*/

_fprintf(fd,string,ip)
unsigned fd;
char *string;
int *ip;
{
  char tbuff[17],*cp;
  unsigned leftadj,padchar,width,precflg,precisn,longflg,length;

  while(*string){
    if(*string=='%'){
      if(leftadj=(*++string=='-'))++string;
      padchar=*string;
      if(padchar!='0')padchar=' ';
      for(width=0;isdigit(*string);)width=width*10+(*string++-'0');
      if(precflg=(*string=='.'))
	for(precisn=0;isdigit(*++string);)precisn=precisn*10+(*string-'0');
      if(longflg=(toupper(*string)=='L'))++string;
      switch(toupper(*string)){
	case 'X':
	  itoh(*ip++,tbuff);
	  cp=tbuff;
	  length=strlen(cp);
	  break;
	case 'D':
	  itoa(*ip++,tbuff);
	  cp=tbuff;
	  length=strlen(cp);
	  break;
	case 'S':
	  cp=*ip++;
	  length=strlen(cp);
	  if(precflg && precisn<length)length=precisn;
	  break;
	case 'C':
	  cp=ip++;
	  length=1;
	  break;
	default:
	  cp=string;
	  length=1;
	  break;
      }
      if(!leftadj && width>length)
	while(width-- >length)write(fd,&padchar,1);
      if(width>length)width-=length; else width=0;
      write(fd,cp,length);
      if(leftadj && width)
	while(width--)write(fd,&padchar,1);
      ++string;
    }
    else write(fd,string++,1);
  }
}
-ARCHIVE- abort.c 215
/*	print a message and abort
*/

#define STDERR 3

abort(string,arg1)
char *string,arg1;
{

  _fprintf(STDERR,"\nABORT:- ");
  _fprintf(STDERR,string,&arg1);
  _fprintf(STDERR,"\n");
  exit(0x7fff);
}
-ARCHIVE- alloc.c 728
/*	alloc.c		7/16/81
*/

struct header
{
  char *addr;
  unsigned hsize;
};

#define HEADSIZE 4

struct header _allocb;		/* base block for control purposes */
char _allocd;			/* dummy so above wont be last entry */

alloc(size)
unsigned size;
{
  char *pp,*cp,*np;

  for(pp=&_allocb;cp=pp->addr;pp=cp){
    if(cp->hsize>=size){
      if(cp->hsize>=size+HEADSIZE){
	np=cp+size+HEADSIZE;
	np->addr=cp->addr;
	np->hsize=cp->hsize-size-HEADSIZE;
	cp->addr=np;
      }
      else size=cp->hsize;
      pp->addr=cp->addr;
      break;
    }
  }
  if(!cp)if(!(cp=sbrk(size+HEADSIZE)))abort("alloc-heap full");
  cp->hsize=size;
  cp->addr=cp;
  setmem(cp+HEADSIZE,size,0);
  return cp+HEADSIZE;
}
-ARCHIVE- aseek.c 2126
/*	do a short seek using an array
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* non zero if ascii file */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];

aseek(fd,asposn,offset)
unsigned fd,asposn[2],offset;
{
  unsigned reqdposn[2];
  struct bufstr *buf;

  if(fd>=MAXFILES)return -1;			/* bad fd */
  if(!(buf=_opentab[fd]))return -1;		/* file not open */
  if(buf->bfcb<256)return;			/* no seek character device */
  reqdposn[1]=asposn[1]+asposn[0]/SECSIZE;	/* standardize */
  reqdposn[0]=asposn[0]%SECSIZE;		/* the reqd position */
  if((reqdposn[1]<buf->bsec) || (reqdposn[1]>=buf->bsec+NUMBSEC)){
    if(buf->bdirty)_cpmwrite(buf);
    buf->bsec=(reqdposn[1]/NUMBSEC)*NUMBSEC;	/* to correct base */
    reqdposn[0]+=(reqdposn[1]%NUMBSEC)*SECSIZE;	/* get correct offset */
    buf->bcnt=0;				/* say no data in buffer */
    _cpmread(buf);				/* read the buffer in */
  }
  else reqdposn[0]+=(reqdposn[1]-buf->bsec)*SECSIZE;
  if(reqdposn[0]>buf->bcnt)buf->bcnt=reqdposn[0];
  buf->bptr=reqdposn[0];
  return 0;
}
-ARCHIVE- atell.c 1603
/*	tell the current position using an array
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* non zero if ascii file */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];

atell(fd,asposn)
unsigned fd,asposn[2];
{
  struct bufstr *buf;

  if(fd>=MAXFILES)return -1;			/* bad fd */
  if(!(buf=_opentab[fd]))return -1;		/* file not open */
  if(buf->bfcb<256)asposn[0]=asposn[1]=0;	/* no seek character device */
  else {
    asposn[1]=buf->bsec+buf->bptr/SECSIZE;
    asposn[0]=buf->bptr%SECSIZE;
  }
  return 0;
}
-ARCHIVE- bdos.a86 388
	title	'c86 basic support package'
	pagesize	51
;	5/18/81

DEFGBL	equ	91h		;define a global
REFGBL	equ	0a1h		;refer to a global

	cseg

;	bdos	call bdos to execute a call


bdos:
	push	bp
	mov	bp,sp
	mov	cx,4[bp]
	mov	dx,6[bp]
	int	224
	pop	bp
	ret



;	define the addresses to relocate in the above code

	eseg

	dw	bdos
	db	DEFGBL,'bdos',0
	dw	0,0

	end
-ARCHIVE- close.c 1596
/*	close a file
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* ascii data flag */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];

close(fd)
unsigned fd;
{
  struct bufstr *buf;

  if(fd>=MAXFILES)return -1;			/* bad fd */
  if(!(buf=_opentab[fd]))return -1;		/* file not open */
  if(buf->bdirty)_cpmwrite(buf);		/* empty the buffer */
  if(buf->bfcb>255){
    if((bdos(0x10,buf->bfcb)&0xff)>3)return -1;/* fatal close error */
    free(buf->bfcb);
  }
  free(buf);
  _opentab[fd]=0;
  return 0;
}
-ARCHIVE- creat.c 189
/*	create a new file, deleting any existing file
*/

creat(filename,mode)
char *filename;
unsigned mode;
{

  return _open(filename,mode,7);	/* do common open for a new file */
}
-ARCHIVE- exit.c 244
/*	do exit processing for a c program
*/

#define MAXFILES 8

extern char *_opentab[MAXFILES];

exit(val)
int val;
{
  int j;

  for(j=0;j<MAXFILES;++j){
    if(_opentab[j])close(j);
  }
  _exit(val);			/* thats all folks */
}
-ARCHIVE- fclose.c 101
/*	this is a convenience function only
*/

fclose(fd)
unsigned fd;
{

  return close(fd);
}
-ARCHIVE- fgets.c 1974
/*	get an input string from a file
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* non vero if ascii */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];


fgets(line,maxline,fd)
char *line;
int maxline;
unsigned fd;
{
  int j,cc,k;
  struct bufstr *buf;

  if(fd>=MAXFILES)return 0;			/* bad fd */
  if(!(buf=_opentab[fd]))return 0;		/* file not open */
  if(buf->bfcb==2 && (!buf->blff)){		/* do direct console input */
    line[0]=maxline-2;
    bdos(10,line);				/* read the line */
    bdos(2,'\n');				/* put out a l/f */
    cc=line[1];
    movmem(line+2,line,cc);			/* position the data */
    for(j=0;j<cc;++j)if(line[j]==26)break;	/* check for cpm EOF */
    line[j++]='\n';
  }
  else {
    for(j=0;j<maxline-1;){
      cc=read(fd,line+j,1);
      if(cc<1)break;
      if(line[j++]=='\n')break;
    }
  }
  line[j]=0;
  if(j)return line; else return 0;
}
-ARCHIVE- fopen.c 396
/*	open a file
*/

#define AREAD 0
#define AWRITE 1

fopen(filename,fomode)
char *filename,*fomode;
{
  int fd;

  switch(tolower(*fomode)){
    case 'r':
      fd=open(filename,AREAD);
      break;
    case 'w':
      fd=creat(filename,AWRITE);
      break;
    default:
      abort("fopen-illegal mode %s opening %s",fomode,filename);
  }
  if(fd<1)fd=0;
  return fd;
}
-ARCHIVE- fprintf.c 131
/*	print a field to a file
*/

fprintf(fd,string,arg1)
unsigned fd;
char *string,arg1;
{

  _fprintf(fd,string,&arg1);
}
-ARCHIVE- fputs.c 155
/*	output a string expanding newlines
*/

fputs(line,fd)
char *line;
unsigned fd;
{
  int leng;

  leng=strlen(line);
  write(fd,line,leng);
}
-ARCHIVE- free.c 639
/*	return a free block to the heap
*/

struct header
{
  char *addr;
  unsigned hsize;
};

#define HEADSIZE 4

extern struct header _allocb;		/* base block for control purposes */


free(fp)
char *fp;
{
  char *pp,*cp,*np;

  fp-=HEADSIZE;
  if(fp!=fp->addr)abort("free-block address");
  for(cp=&_allocb;cp->addr && cp->addr<fp;cp=cp->addr);
  fp->addr=cp->addr;
  cp->addr=fp;
  if(fp==cp+cp->hsize+HEADSIZE){
    cp->hsize+=(fp->hsize+HEADSIZE);
    cp->addr=fp->addr;
  }
  else cp=fp;
  fp=fp->addr;
  if(fp==cp+cp->hsize+HEADSIZE){
    cp->hsize+=(fp->hsize+HEADSIZE);
    cp->addr=fp->addr;
  }
}
-ARCHIVE- getc.c 192
/*	get a character from the file
*/

getc(fd)
unsigned fd;
{
  unsigned cc;

  if(read(fd,&cc,1)!=1)return -1;		/* read 1 character */
  return cc&0xff;				/* return character */
}
-ARCHIVE- getchar.c 211
/*	get a character from the standard input
*/

#define STDIN 1

getchar()
{
  unsigned cc;

  if(read(STDIN,&cc,1)!=1)return -1;		/* read 1 character */
  return cc&0xff;				/* return character */
}
-ARCHIVE- getw.c 275
/*	get a word from a file
*/

getw(fd)
unsigned fd;
{
  unsigned cc,count;

  count=read(fd,&cc,2);				/* read 2 characters */
  switch(count){
    case 2:
      return cc;
    case 1:
      return cc&0xff;
  }
  return -1;					/* EOF, not a good value */
}
-ARCHIVE- inportb.a86 361
;	read an b bit value from a port

;	input	a port number
;	returns the input byte, zero filled

	cseg

inportb:
	push	bp
	mov	bp,sp		;set the frame pointer
	mov	dx,4[bp]	;get the port number
	in	al,dx		;get a byte value
	mov	ah,0		;zero the top byte
	pop	bp
	ret

	eseg

DEFGBL	equ	91h

	dw	inportb
	db	DEFGBL,'inportb',0
	dw	0,0

	end
-ARCHIVE- inportw.a86 317
;	read an w bit value from a port

;	input	a port number
;	returns the input word

	cseg

inportw:
	push	bp
	mov	bp,sp		;set the frame pointer
	mov	dx,4[bp]	;get the port number
	in	ax,dx		;get a byte value
	pop	bp
	ret

	eseg

DEFGBL	equ	91h

	dw	inportw
	db	DEFGBL,'inportw',0
	dw	0,0

	end
-ARCHIVE- isalpha.c 139
/*	return true if input character is alphabetic
*/

isalpha(c)
char c;
{

  return ((('A'<=c)&(c<='Z')) | (('a'<=c)&(c<='z')));
}
-ARCHIVE- isdigit.c 112
/*	return true if input character is a digit
*/

isdigit(c)
char c;
{

  return (('0'<=c)&(c<='9'));
}
-ARCHIVE- isspace.c 139
/*	returns true if chararcter is a blank,tab or newline
*/

isspace(cc)
char cc;
{

  return (cc==' ' || cc=='\t' || cc=='\n');
}
-ARCHIVE- itoa.c 202
/*	integer to ascii
*/

itoa(n,cp)
int n;
char *cp;
{
  int j;

  if(n<0)*cp++='-';
  else n=-n;
  if(n<-9)j=itoa(n/-10,cp);
  else j=0;
  cp[j]='0'-(n%-10);
  cp[++j]=0;
  return j;
}
-ARCHIVE- itoh.c 198
/*	integer to hex
*/

itoh(n,cp)
unsigned n;
char *cp;
{
  int j,k;

  cp[4]=0;
  for(j=3;j>=0;--j){
    k=(n&0xf)+'0';
    n=n>>4;
    if(k>'9')k=k+('A'-'9'-1);
    cp[j]=k;
  }
}
-ARCHIVE- makefcb.c 2031
/*	make a cp/m file control block
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* non zero if ascii */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];

#define SZCPMFCB 36

makefcb(filename)
char *filename;
{
  char j,u;
  struct cpmfcb *fcb;

  if(!(fcb=alloc(SZCPMFCB)))return 0;		/* heap full */
  if(filename[1]==':'){
    u=toupper(*filename);
    if(u>='A' && u<='D')fcb->dr=u-('A'-1);	/* set drive number */
    else goto error;
    filename+=2;
  }
  setmem(fcb->fn,11,' ');			/* space fill fn and fe */
  for(j=0;*filename;){				/* do file name */
    u=toupper(*filename++)&0x7f;
    if(u=='.')break;				/* that part done */
    if(u<0x21)goto error;			/* some protection */
    if(j<8)fcb->fn[j++]=u;
  }
  for(j=0;*filename;){				/* do fn extension */
    u=toupper(*filename++)&0x7f;
    if(u<0x21)goto error;
    if(j<3)fcb->fe[j++]=u;
  }
  return fcb;					/* all ok */
error:
  free(fcb);
  return 0;
}
-ARCHIVE- movmem.a86 826
	title	'move a block of memory'
	pagesize	51
;	7/28/81

DEFGBL	equ	91h		;define a global

	cseg

;	movmem

;	entry	1,the source address
;		2,the destination address
;		3,the number of bytes to move

movmem:
	push	bp
	mov	bp,sp
	mov	ax,ds		;ensure that extra seg is correct
	mov	es,ax
	mov	si,4[bp];	;the source address
	mov	di,6[bp]	;the destination address
	mov	cx,8[bp]	;the number of bytes to move
	cmp	si,di		;which way to do the move ?
	jb	movmem01	;do it in reverse order
	cld
	jmps	movmem02	;must be this
movmem01:
	add	si,cx		;point to other end of string
	dec	si
	add	di,cx
	dec	di
	std			;backwards in memory
movmem02:
	rep	movs	al,al	;do the move
	pop	bp
	ret

;	define the addresses to relocate in the above code

	eseg

	dw	movmem
	db	DEFGBL,'movmem',0
	dw	0,0

	end
-ARCHIVE- open.c 167
/*	open an existing file
*/

open(filename,mode)
char *filename;
unsigned mode;
{

  return _open(filename,mode,0);	/* do common open for existing file */
}
-ARCHIVE- outportb.a86 420
;	write an 8 bit value to port

;	input	a port number
;		the value to write 
;	returns the input byte, zero filled

	cseg

outportb:
	push	bp
	mov	bp,sp		;set the frame pointer
	mov	dx,4[bp]	;get the port number
	mov	ax,6[bp]	;get the value to output
	mov	ah,0		;zero the top byte
	out	dx,al		;put it out
	pop	bp
	ret

	eseg

DEFGBL	equ	91h

	dw	outportb
	db	DEFGBL,'outportb',0
	dw	0,0

	end
-ARCHIVE- outportw.a86 377
;	write an 16 bit value to port

;	input	a port number
;		the value to write 
;	returns the input word

	cseg

outportw:
	push	bp
	mov	bp,sp		;set the frame pointer
	mov	dx,4[bp]	;get the port number
	mov	ax,6[bp]	;get the value to output
	out	dx,ax		;put it out
	pop	bp
	ret

	eseg

DEFGBL	equ	91h

	dw	outportw
	db	DEFGBL,'outportw',0
	dw	0,0

	end
-ARCHIVE- printf.c 150
/*	print a field to the standard output
*/

#define STDOUT 2

printf(string,arg1)
char *string,arg1;
{

  _fprintf(STDOUT,string,&arg1);
}
-ARCHIVE- putc.c 104
/*	put a character to a file
*/

putc(c,fd)
unsigned c,fd;
{

  write(fd,&c,1);
  return 0;
}
-ARCHIVE- putchar.c 128
/*	put a character to the standard output
*/

#define STDOUT 2

putchar(cc)
unsigned cc;
{

  write(STDOUT,&cc,1);
}
-ARCHIVE- putw.c 86
/*	put a word to a file
*/

putw(w,fd)
unsigned w,fd;
{

  write(fd,&w,2);
}
-ARCHIVE- read.c 2286
/*	read characters from the file
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* true if ascii data in file */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];


read(fd,buffer,count)
unsigned fd;
char *buffer;
unsigned count;
{
  int retval,retcc;
  struct bufstr *buf;

  if(fd>=MAXFILES)return -1;			/* bad fd */
  if(!(buf=_opentab[fd]))return -1;		/* file not open */
  if(!(buf->bmode&1))return -1;			/* not open for reading */
  for(retval=0;retval<count;++retval){
    if((retcc=_read(buf))<0)break;
    *buffer++=retcc;
    if(buf->basc){
      if(retcc==26){
	--buf->bptr;
	break;
      }
      if(retcc=='\r'){
	retcc=_read(buf);
	if(retcc=='\n') *(buffer-1)='\n';
	else if(retcc!=-1)--buf->bptr;
      }
    }
  }
  return retval;
}

/*	support routine for read
*/

_read(buf)
struct bufstr *buf;
{
  char cc;

  if(buf->bptr>=buf->bcnt){			/* no data in buffer */
    if(buf->bdirty)_cpmwrite(buf);		/* get out your dirty buff */
    _cpmread(buf);				/* fill the buffer */
  }
  if(!buf->bcnt)return -1;			/* end of file */
  return buf->bdata[buf->bptr++];
}
-ARCHIVE- rename.c 412
/*	rename a file
*/

rename(filefrom,fileto)
char *filefrom,*fileto;
{
  char *fcbfrom,*fcbto;
  unsigned status;

  if(!(fcbfrom=makefcb(filefrom)))return -1;
  if(!(fcbto=makefcb(fileto))){
    free(fcbfrom);
    return -1;
  }
  movmem(fcbto,fcbfrom+16,16);		/* build composite fcb */
  status=bdos(0x17,fcbfrom)&0xff;
  free(fcbfrom);
  free(fcbto);
  if(status)return -1;
  return 0;
}
-ARCHIVE- sbrk.c 364
/*	get some memory from the system
*/


sbrk(size)
unsigned size;
{
  unsigned *up,uv;

  up=0x106;			/* get address of control field */
  if(*up+size>=&size)return 0;	/* no room left for this one */
  uv=*up;			/* get value to return to caller */
  *up+=size;			/* say its allocated */
  uv-=*--up;			/* subtract out the reserve */
  return uv;
}
-ARCHIVE- setmem.a86 657
	title	'set memory to a specified character'
	pagesize	51
;	5/28/81

DEFGBL	equ	91h		;define a global

	cseg

;	setmem

;	entry	1,the address of first character to be set
;		2,the number of bytes to set (unsigned int)
;		3,the desired value

setmem:
	push	bp
	mov	bp,sp
	mov	ax,ds		;ensure that extra seg is correct
	mov	es,ax
	cld			;and clear the direction flag
	mov	di,4[bp]	;the address to set
	mov	cx,6[bp]	;the number of bytes to set
	mov	al,8[bp]	;the value to set
	rep	stos	al	;set the area
	pop	bp
	ret

;	define the addresses to relocate in the above code

	eseg

	dw	setmem
	db	DEFGBL,'setmem',0
	dw	0,0

	end
-ARCHIVE- strcat.c 183
/*	concatinate string t to string s
*/

strcat(s,t)
char *s,*t;
{
  char *cp;

  while(*s++);			/* find end of string */
  for(--s;*s++=*t++;);		/* do the concatinate */
}
-ARCHIVE- strcmp.c 133
/*	string compare
*/

strcmp(s,t)
char *s,*t;
{

  for(;*s==*t;t++)if(!*s++)return 0;
  if(*s<*t)return -1;
  return 1;
}
-ARCHIVE- strcpy.c 89
/*	string copy
*/

strcpy(to,from)
char *to,*from;
{

  while(*to++=*from++);
}
-ARCHIVE- strlen.c 141
/*	return the length of a string
*/

strlen(string)
char *string;
{
  char *cp;

  for(cp=string++;*cp++;);
  return cp-string;
}
-ARCHIVE- tolower.c 133
/*	convert character to lowercase if upper case */

tolower(c)
char c;
{

  if('A'<=c && c<='Z')c+=('a'-'A');
  return c;
}
-ARCHIVE- toupper.c 133
/*	convert character to uppercase if lower case */

toupper(c)
char c;
{

  if('a'<=c && c<='z')c+=('A'-'a');
  return c;
}
-ARCHIVE- ungetc.c 1485
/*	ungetc a char from an input file
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* non zero if ascii data */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];


ungetc(cc,fd)
unsigned cc,fd;
{
  struct bufstr *buf;

  if(fd>=MAXFILES)return -1;			/* bad fd */
  if(!(buf=_opentab[fd]))return -1;		/* file not open */
  if(buf->bptr)buf->bdata[--buf->bptr]=cc;
  else return -1;
  return 0;
}

-ARCHIVE- unlink.c 255
/*	unlink (delete) a disk file
*/

unlink(filename)
char *filename;
{
  char *fcb;
  unsigned status;

  if(!(fcb=makefcb(filename)))return -1;
  status=bdos(0x13,fcb)&0xff;
  free(fcb);
  if(status)return -1;
  return 0;				/* all ok */
}
-ARCHIVE- utoa.c 239
/*	unsigned integer to ascii
*/

utoa(val,str)
unsigned val;
char *str;
{
  char digit,offset;

  digit=val%10+'0';
  offset=0;
  if(val>9)offset=utoa(val/10,str);
  str[offset]=digit;
  str[++offset]=0;
  return offset;
}
-ARCHIVE- write.c 2026
/*	write characters to a file
*/

/*	define the cp/m86 file control block
*/

struct cpmfcb
{
  char dr;		/* drive specification */
  char fn[8];		/* the file name */
  char fe[3];		/* the file extent */
  char ex;		/* the current extent number */
  char s1;		/* cp/m use */
  char s2;		/* cp/m use */
  char rc;		/* record count */
  char dn[16];		/* cp/m use */
  char cr;		/* current record */
  unsigned rr;		/* random record number */
  char ro;		/* random record overflow */
};

struct bufstr
{
  char *bfcb;			/* really struct pointer to cp/m fcb block */
  char basc;			/* non vero if ascii file */
  unsigned bsize;		/* size of the buffer in bytes */
  unsigned bsec;		/* the first cp/m record in the buffer */
  int bptr;			/* next character to be read/written */
  int bcnt;			/* the number of characters in buffer */
  char bmode;			/* the open mode */
  char bdirty;			/* data in the buffer to write */
  char blff;			/* next character read will be a l/f */
  char bdata[1];		/* the data itself */
};

#define SZBUFSTR 15
#define SECSIZE 128		/* size of cp/m sector */
#define NUMBSEC 8		/* number of sectors in buffer */

#define MAXFILES 8

extern char *_opentab[MAXFILES];

write(fd,buffer,count)
unsigned fd,count;
char *buffer;
{
  int retval;
  struct bufstr *buf;

  if(fd>=MAXFILES)return -1;			/* bad fd */
  if(!(buf=_opentab[fd]))return -1;		/* file not open */
  if(!(buf->bmode&2))return -1;			/* not open for writing */
  for(retval=0;retval<count;++retval){
    if(*buffer=='\n' && buf->basc)_write('\r',buf);
    _write(*buffer++,buf);
  }
  return retval;
}

/*	service routine for write
*/

_write(cc,buf)
unsigned cc;
struct bufstr *buf;
{

  if(!buf->bcnt && buf->bmode==3 && buf->bfcb>255)_cpmread(buf);
  buf->bdata[buf->bptr++]=cc;			/* put the character */
  buf->bdirty=1;				/* say data to write */
  if(buf->bptr>buf->bcnt)buf->bcnt=buf->bptr;
  if(buf->bptr>=buf->bsize)_cpmwrite(buf);	/* empty the buffer */
}
