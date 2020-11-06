/* this is the archiver for the Zilog edition of the C compiler */

typedef	long       	time_t;   	/* a time */
typedef	int        	dev_t;    	/* device code */
typedef	long       	off_t;    	/* offset in file */

struct	stat
{
	dev_t	st_dev;
	unsigned int	st_ino;
	unsigned short st_mode;
	short	st_nlink;
	short  	st_uid;
	short  	st_gid;
	dev_t	st_rdev;
	off_t	st_size;
	time_t	st_atime;
	time_t	st_mtime;
	time_t	st_ctime;
};

#define	S_IFMT	0170000		/* type of file */
#define		S_IFDIR	0040000	/* directory */
#define		S_IFCHR	0020000	/* character special */
#define		S_IFBLK	0060000	/* block special */
#define		S_IFREG	0100000	/* regular */
#define		S_IFMPC	0030000	/* multiplexed char special */
#define		S_IFMPB	0070000	/* multiplexed block special */
#define	S_ISUID	0004000		/* set user id on execution */
#define	S_ISGID	0002000		/* set group id on execution */
#define	S_ISVTX	0001000		/* save swapped text even after use */
#define	S_IREAD	0000400		/* read permission, owner */
#define	S_IWRITE	0000200		/* write permission, owner */
#define	S_IEXEC	0000100		/* execute/search permission, owner */

#define AR8KMAGIC 0177545
#define EVEN 0
#define ODD 1


struct ar8k_hd {
	char	ar8k_name[14];
	long	ar8k_date;
	char	ar8k_uid,	/* not used except for compatibility */
		ar8k_gid;	/* only for compatibility */
	int	ar8k_mode;	/* compatibility */
	long	ar8k_size;
}
	head;		/* header from the old archive */

struct stat statbuf;

char	**filvec,		/* pointer to start of vertor of files */
	*filename,		/* current file being processed */
	*archive,		/* the name of the archive file */
	*tail(),		/* find the real name of the file */
	filmrk[256],		/* marks for up to 256 files */
	buffer[512];		/* the copying buffer */

char	*tmpname = "ar8kxxxx.tmp";

int	key = 0,		/* one of d r q t x */
	verbose,		/* set if the 'v' key is used */
	arfile,			/* file number of the archive file */
	tpfile,			/* file number of the temporary file */
	tmpflag = 0,		/* flag set if a tempfile created */
	file,			/* file number of the input or output file */
	magic = {AR8KMAGIC},	/* the output magic word */
	inmagic,		/* the file magic word is read here */
	filyet,			/* count of files not yet processed */
	filcnt;			/* count of files in filvec to process */

main(argc,argv) char **argv;{

register i;
register char *cp;

	if( argc < 3 ){
		printf("usage:  ar8k  key  archfile [files ...]\n");
		exit(1);
	}

	cp = argv[1];
	while( i = *cp++ ) switch( i ){

case 'd':
case 'q':
case 'r':
case 't':
case 'x':
		if( key ) errex("only one of d q r t x allowed");
		key = i;
		continue;

case 'v':	verbose++;
		continue;

default:	errex("illegal key");
	}
	if( key == 0 ) errex("no key specified");

	archive = argv[2];
	filvec = &argv[3];
	filyet = filcnt = argc-3;
	if( filcnt > 256 ) errex( "too many file names" );

/* now create a temporary file for the functions needing it */

	switch( key ){

case 'd':
case 'r':
		tmpflag = settmp();
	}

/* now open the archive for the functions needing to read it */

	switch( key ){

case 'd':
case 'r':
case 't':
case 'x':
		arfile = openb( archive , 0 );
		if( arfile < 0 ){  /* can't read */
			if( key != 'r' ) errex("can't read %s\n",archive );
			key = 'q';
		} else {
			chmagic( arfile );
		}
	}

	if( key == 'q' ){  /* quick append to end */
		arfile = openb( archive , 2 );
		if( arfile < 0 ){
			arfile = creatb( archive , 0666 );
			if( arfile >= 0 ) wrmagic( arfile );
				else  errex("can't append to %s",archive);
		} else {
			chmagic( arfile );
			lseek( arfile , 0L , 2 );  /* seek to end */
		}
		append( arfile );
		close(arfile);
		exit(0);
	}

/* now do the main body of processing */

	while( read( arfile, (char *) &head, sizeof head ) == sizeof head ){

		switch( key ){

case 'd':			/* deletion */
			if( inlist() ){
				comment("deleting");
				skip();
			} else {
				puthead( tpfile );
				copy( tpfile , EVEN , arfile , EVEN );
			}	
			break;

case 'r':			/* replacement */
			if( inlist() && (file = openb(filename,0)) >= 0 ){
				skip();
				getfile( tpfile , "replacing");
			} else {
				puthead( tpfile );
				copy( tpfile , EVEN , arfile , EVEN );
			}
			break;

case 't':			/* listing */
			if( filcnt == 0 || inlist() ){
				if( verbose ){
					printf(" %7D ", head.ar8k_size );
				}
				printf("%-14.14s\n",head.ar8k_name);
			}
			skip();
			break;

case 'x':			/* extracting */
			if( (filcnt == 0 || inlist()) &&
			    (file=creatb(head.ar8k_name,0666)) >=0 ){
				comment("extracting");
				copy( file , ODD , arfile , EVEN );
				close( file );
			} else {
				skip();
			}
			break;
		}
	}

/* the main body of the work is done, copy the archive file back */

	switch( key ){

case 'r':
		if( filyet )append( tpfile );

case 'd':
		close( arfile );
		arfile = creatb( archive , 0666 );
		if( arfile < 0 ){
			printf("cannot create %s\n",archive );
		} else {
			lseek( tpfile , 0L , 0 );
			while( (i = read( tpfile , buffer , 512)) > 0 )
				if( write( arfile , buffer , i ) != i )
					errex("botch in recopying the archive");
		}
	}
	if( filyet != 0 ){
		printf("\nfiles not processed:\n\n");
		for( i=0; i<filcnt; i++ )
			if( !filmrk[i] )
				printf("\t%s\n",filvec[i] );
	}
	if (tmpflag) {
		close(tpfile);
		unlink(tmpname);
	}

}


settmp(){   /* create a temporary archive file */

	close( creatb( tmpname ) , 0600 );
	tpfile = openb( tmpname , 2 );
	/* unlink( "/tmp/ar8kxxxxxxx" );  */
	if( tpfile < 0 ){
		printf("cannot create tempfile\n" );
		exit(1);
	}
	wrmagic( tpfile );
	return(1);
}

wrmagic( f ){  /* write the magic word */

	if( write( f , (char *) &magic , sizeof(int) ) != sizeof(int) )
		errex("can't write magic word\n");
}


char *
tail( s ) char *s; {

register i;
register char *t,*u;

	for(;;){
		u = t = s;
		while( i = *u++ )
			if( i == '/' ) t = u;
		if( *t ) return( t );
		if( t == s )errex("bad file name");
	}
}


comment(s) register char *s; {

	if( verbose ) printf("%s:\t%-14.14s\n",s,head.ar8k_name);
}


errex(s,a,b,c) char *s; {

	printf("fatal:\t");
	printf(s,a,b,c);
	printf("\n");
	exit(1);
}


getfile( f , s ) char *s;{  /* make a header and copy a file to f */

register char *cp;
register i;


	cp = tail( filename );
	fstat( file , &statbuf );
	lseek(file, 0L, 0);		/* reposition at head; "fstat"	*/
					/* corrupts the file position.	*/
	head.ar8k_size = statbuf.st_size;
	head.ar8k_uid = statbuf.st_uid;
	head.ar8k_gid = statbuf.st_gid;
	head.ar8k_date = statbuf.st_mtime;
	head.ar8k_mode = statbuf.st_mode;
	for( i=0; i<14; i++ )
		if( head.ar8k_name[i] = *cp )
			cp++;
	comment( s );
	puthead( f );
	copy( f , EVEN , file , ODD );
	close( file );
}


inlist(){  /* check to see if the current header name is in the list */

register char *cp,*bp;
register j;
int	i;

	for( i=0; i<filcnt; i++ ){
		if( !filmrk[i] ){
			cp = tail( filvec[i] );
			bp = head.ar8k_name;
			for( j=0; j<14; j++ ){
				if( *cp != *bp++ ) break;
				if( *cp ) cp++;
			}
			if( j == 14 ){
				filmrk[i]++;
				filyet--;
				filename = filvec[i];
				return( 1 );
			}
		}
	}
	return( 0 );
}


copy( outf, outflag , inf, inflag ){


register i,j;

	i = head.ar8k_size >> 9;
	while( --i >= 0 ){
		if( read( inf , buffer , 512 ) != 512 ) errex("bad read");
		if( write( outf , buffer , 512) != 512 ) errex("bad write");
	}
	i = head.ar8k_size;
	j = i = i & 0777;
	if( i & 1 ){  /* odd length */
		if( inflag == EVEN ) i++;
		if( outflag == EVEN ) j++;
	}
	if( i ){
		if( read( inf , buffer , i ) != i ) errex("bad read.");
		buffer[i] = 0;
		if( write( outf , buffer , j ) != j ) errex("bad write.");
	}
}


skip(){  /* skip over a file */

long size;

	size = head.ar8k_size;
	if( size & 1 ) size++;
	lseek( arfile , size , 1 );
}


puthead( f ){  /* write the header */

	if( write( f , (char *) &head , sizeof head ) != sizeof head )
		errex( "header write error" );
}


append( f ){

register i;


	for( i=0; i<filcnt; i++ ){
		if( !filmrk[i] ){
			filename = filvec[i];
			if( (file = openb(filename,0)) >= 0 ){
				filmrk[i]++;
				filyet--;
				getfile( f ,"appending");
			} else {
				printf("cannot read %s\n",filename);
			}
		}
	}
}

chmagic( f ){  /* check the file for the magic code */

	read( f , (char *) &inmagic , sizeof(int) );
	if( inmagic != AR8KMAGIC ) errex("%s is not an archive",archive);
}

/* Dummy fstat since we're not in UNIX anymore.  The call to _filesz	*/
/*  has the side effect of positioning the file at the end.		*/

fstat(fd,stp)
int	fd;
struct	stat	*stp;
{	
	register long fsize;

	fsize = _filesz(fd);
	stp->st_size = fsize;
	stp->st_uid = 0;
	stp->st_gid = 0;
	stp->st_mtime = 0;
	stp->st_mode = 0666;
}
