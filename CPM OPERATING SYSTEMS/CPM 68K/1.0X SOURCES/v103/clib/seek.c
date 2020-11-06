
/*
	Copyright 1983
	Alcyon Corporation
	8716 Production Ave.
	San Diego, CA  92121

	@(#) seek.c - Sep 12, 1983  REGULUS 4.1
*/

/* fake v6 seek system call */

seek(fd,offset,position)
int fd, offset, position;
{
	register long loffset;

	loffset = offset;
	if (position >= 3) {
		loffset *= 512;
		position -= 3;
	}
	lseek(fd,loffset,position);
}
