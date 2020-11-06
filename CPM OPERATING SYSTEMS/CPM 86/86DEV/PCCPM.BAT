@del pccpm.h86 >NUL
@del pccpm.cmd >NUL
@del pccpm.sys >NUL
copy cpm.h86+pcbios.h86 pccpm.h86
cpm gencmd pccpm 8080 code[a51]
ren pccpm.cmd pccpm.sys

