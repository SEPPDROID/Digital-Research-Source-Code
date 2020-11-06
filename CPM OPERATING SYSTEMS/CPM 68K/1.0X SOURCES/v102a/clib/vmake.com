$ clib
$ set noon
$ !	
$ !	C library "make" file for DRI clib on VMS using Whitesmith's compiler
$ !
$ delete *.obj;*		! Clean up files from before
$ copy machine.vax machine.h	! Set up for VAX compiles
$ pur  machine.h		! Clean up our toys
$ cx access
$ cx atoi
$ cx atol
$ cx calloc
$ cx chmod
$ cx chown
$ cx cleanup
$ cx creatvms
$ cx ctype
$ cx errno
$ cx fclose
$ cx fdopen
$ cx fflush
$ cx fgetc
$ cx fgets
$ cx fopen
$ cx fprintf
$ cx fputc
$ cx fputs
$ cx fread
$ cx freopen
$ cx fscanf
$ cx fseek
$ cx ftell
$ cx ftoavms
$ cx fwrite
$ cx getl
$ cx getpid
$ cx gets
$ cx getw
$ cx index
$ cx ldiv
$ cx lseekvms
$ cx malloc
$ cx mktemp
$ cx openvms
$ cx perror
$ cx printf
$ cx putl
$ cx puts
$ cx putw
$ cx qsort
$ cx rand
$ cx rewind
$ cx rindex
$ cx sbrkvms
$ cx scanf
$ cx setbuf
$ cx sprintf
$ cx sscanf
$ cx strcat
$ cx strcmp
$ cx strcpy
$ cx strins
$ cx strlen
$ cx strncat
$ cx strncmp
$ cx strncpy
$ cx swab
$ cx uldivvms
$ cx ungetc
$ cx unlinkvms
$ cx xdoprt
$ cx xdoprtfp
$ cx xdoscan
$ cx xfdecls
$ cx xfilbuf
$ cx xflsbfvms
$ cx xprtint
$ cx xprtld
$ cx xprtshrt
$ cx xstrcmp
$ mac mopen
$ mac sbreak
$ delete clibdri.*;*
$ libr/creat clibdri *.obj
$ mac xhdr
