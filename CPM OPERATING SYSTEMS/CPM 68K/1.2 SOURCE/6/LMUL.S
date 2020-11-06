*// long multiply routine without floating point
*//  call with:
*//		two long values on stack
*//  returns:
*//		long value in R0 and R1
*//
*// warning:  no overflow checking or indication!!!!
*struct {
*	int hiword;
*	int loword;
*};
*long lmul(l1,l2)
*long l1,l2;
*{
*
*	long t1;
*	register int sign;
*	register int t2;
*
*	sign = 0;
*	if(l1 < 0) {
*		l1 = -l1;	//make it positive
*		sign++;
*	}
*	if(l2 < 0) {
*		l2 = -l2;	//make it positive
*		sign++;
*	}
*	t1 = l1.loword*l2.loword;
*	t2 = l1.hiword*l2.loword + l2.hiword*l1.loword;
*	t1.hiword = t1.hiword + t2;
*	if(sign&1)
*		t1 = -t1;	//negate results
*	return(t1);
*}
*
*
.globl	lmul
.text
lmul:
~~lmul:
~sign=R2
~l1=8
~l2=12
~t1=-4
~t2=R6
link	R14,#-4
clr	R2
tst.l	8(R14)		//is first arg negative?
bge L2
neg.l	8(R14)		//yes, negate it
inc	R2				// increment sign flag
L2:tst.l	12(R14)		//is second arg negative?
bge L3
neg.l	12(R14)		//yes, make it positive
inc	R2				//increment sign flag
L3:move	10(R14),R0		//arg1.loword
mulu	14(R14),R0		//arg2.loword
move.l	R0,-4(R14)	//save in temp
move	8(R14),R0		//arg1.hiword
mulu	14(R14),R0		//arg2.loword
move	12(R14),R1		//arg2.hiword
mulu	10(R14),R1		//arg1.loword
add	R1,R0			//form the sum of 2 lo-hi products
add	-4(R14),R0	//add to temp hiword
move	R0,-4(R14)	//store back in temp hiword
move.l	-4(R14),R0	//long results
btst	#0,R2			//test sign flag
beq L4
neg.l	R0		//complement the results
L4:
unlk	R14
rts
ve.l	-4(R14),R0	//long results
btst	#,R2			//test sign flag
beq L4
neg.l	R0		//complement the results
L4:
unlk	R14
rts
ve.l	-4(R14),R0	//long results
btst	#,R2			//test sign flag
beq L4
neg.l	R0		//complement the results
L4:
unlk	R14
rts
ve.l	-4(R14),R0	//long results
btst	#