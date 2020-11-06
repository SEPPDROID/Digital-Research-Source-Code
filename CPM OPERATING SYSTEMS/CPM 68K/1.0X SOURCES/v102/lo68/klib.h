/*
 *	This file defines procedure names used by the assembler, compiler,
 *	etc. to be the names used in this library.  
 *
 *	(In case you haven't guessed, the "k" in "klib" stands for:
 *
 *			K L U D G E !)
 */
#define	creat	xcreat
#define	open	xopen
#define	fopen	xfopen
#define	getc	xgetc
#define	getw	xgetw
#define	fcreat	xfcreat
#define	putw	xputw
#define	strcmp	xstrcmp
#define	index	xindex
#define	strcpy	xstrcpy
/*
 *	Defining the names in this manner allows using the V7 UNIX library
 *	to run the asinine V6 calls in the Alcyon software.
 *
 */
