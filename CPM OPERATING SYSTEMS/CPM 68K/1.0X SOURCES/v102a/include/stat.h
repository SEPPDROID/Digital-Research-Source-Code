/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) sys/stat.h - Jul 26, 1983  REGULUS 4.1
*/

long stat();
long fstat();

#define STATSIZE	32		/*  REGULUS stat returns 32 bytes */

/*  index record flags -- same as on the disk */
/*   0777 - owner, group, and public access bits */

#define		FIDIR		0100000		/* directory */
#define		FICHR		0040000		/* char special file */
#define		FIBLK		0020000		/* block special file */
#define		FINPIPE		0010000		/* named pipe (fifo) file */
#define		FISUID		0004000		/* set user id upon execution */
#define		FISGID		0002000		/* set group id upon execution */
#define		FISTICKY	0001000		/* sticky file */
#define		USRFBITS	0000777		/* access bits */

#define		AREAD	4		/* file access read bit */
#define		AWRITE	2		/* file access write bit */
#define		AEXEC	1		/* file access execute bit */

#define		ROOTIDB		2L			/*  root index record number */

/* this is the structure of a file indirect block */
/*  32 bytes are returned by stat and fstat. */

struct indblk {
	int		i_magic;		/* magic number to id indirect blocks */
	long	i_size;			/* file size */
	int		i_ownerid;		/* file owner id */
	char	i_groupid;		/* group id of file */
	char	i_nlinks;		/* file reference count */
	long	i_moddate;		/* file modification date and time */
	long	i_accdate;		/* file access date and time */
	int		i_flags;		/* file type and access mode */
	long	i_blkptr[2];	/* block # of data blocks */
							/* first blk ptr contains major/minor device # */
							/*  for special files */
	int		i_notused;
	int		i_dev;			/*  device number where file resides */
};

/*
 the majority of Unix programs that use stat or fstat should be
	happy with the following defines, if not, they should use the
	version 6 or version 7 interface libraries....
*/

struct stat {
	int		i_magic;		/* magic number to id indirect blocks */
	long	st_size;		/* file size */
	int		st_uid;			/* file owner id */
	char	st_gid;			/* group id of file */
	char	st_nlink;		/* file reference count */
	long	st_mtime;		/* file modification date and time */
	long	st_atime;		/* file access date and time */
	int		st_mode;		/* file type and access mode */
	int		i_nu1;			/* pad for word */
	int		st_rdev;		/* device for character and block devs */
	long	i_nu2;			/* pad for block */
	int		i_nu3;			/* pad for word */
	int		st_dev;			/*  device number where file resides */
};
#define	S_IFMT	(FIBLK|FICHR|FIDIR|FINPIPE)
#define	S_IFDIR	FIDIR
#define	S_IFCHR	FICHR
#define	S_IFBLK	FIBLK
#define	S_IFREG	0
#define	S_IFIFO	FINPIPE
#define	S_ISUID	FISUID
#define	S_ISGID	FISGID
#define	S_ISVTX	FISTICKY
#define	S_IREAD	0400
#define	S_IWRITE	0200
#define	S_IEXEC	0100
