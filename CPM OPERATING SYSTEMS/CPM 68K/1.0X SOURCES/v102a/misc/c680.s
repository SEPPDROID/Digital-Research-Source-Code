*	Initial start up for programs on Regulus, loaded at location 0 of
*	program.
			.globl	_main,_exit,_environ
			ori.b	#0,d0				// 0 at 0 changed 29 Aug 1983
			move.l	6(sp),_environ		//save ptr to environment block
			jsr		_main				//call main routine
			move	#0,-(sp)			//do an exit(0)
			jsr		_exit
			.bss
_environ:	.ds		4					//four bytes for ptr
			.end
