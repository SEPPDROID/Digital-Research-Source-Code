/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) sys/types.h - Jul 26, 1983  REGULUS 4.1
*/

/*
	Unix compatable type definitions
*/

typedef struct { int r[1];} *physadr;
typedef long daddr_t;
typedef char *caddr_t;
typedef unsigned short ushort;
typedef ushort ino_t;
typedef char cnt_t;
typedef long time_t;
typedef int label_t[6];
typedef short dev_t;
typedef long off_t;
typedef long paddr_t;
