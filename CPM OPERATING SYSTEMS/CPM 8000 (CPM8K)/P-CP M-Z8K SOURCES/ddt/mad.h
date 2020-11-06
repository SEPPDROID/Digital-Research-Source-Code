
/*
********	Z800 opcode table	(from z800.c)
*/

#define	OFFSET	0x000F

/* standard data types */
#define	B1	0x0100	/* bit */
#define	B2	0x0200	/* nib */
#define	B3	0x0300	/* oct */
#define	B4	0x0400	/* nibble */
#define B5	0x0500	/* 5-bit (used for bitL) */

#define	X1	0x1100	/* byte */
#define	X2	0x1200	/* word */
#define	X3	0x1300	/* triple-byte? */
#define	X4	0x1400	/* long */
#define	X5	0x1500	/* quad */
#define	X6	0x1600	/* float */
#define	X7	0x1700	/* double */

#define	AA	0x2100	/* address */
#define	AL	0x2200	/* long address */

#define	RB	0x3100	/* byte register */
#define	RR	0x3200	/* register */
#define	RL	0x3300	/* long register */
#define	RQ	0x3400	/* quad register */

#define	II	0x4100	/* indirect register either long or short, but not 0 */
#define	I0	0x4200	/* indirect register either long or short, incl 0 */
#define	IR	0x4300	/* indirect register, but not r0 */
#define	IS	0x4400	/* indirect register including r0 */
#define	IL	0x4500	/* indirect long register, but not rr0 */
#define	L0	0x4600	/* indirect long register including rr0 */
#define	PP	0x4800	/* parenthesized register, either long or short, not 0*/
#define	PR	0x4900	/* parenthesized register, but not (R0)  */
#define	P0	0x4A00	/* parenthesized register, but not rr0 */
#define	PL	0x4B00	/* parenthesized long register */
#define	BB	0x4C00	/* base register (an RR or RL but not R0 or RR0) */
#define	BR	0x4D00	/* base register (an RR but not RR), not R0 */
#define	BL	0x4E00	/* long base register (an RL but not RR), not RR0 */

#define	CC	0x5100	/* condition code */
#define	CF	0x5200	/* conditional flag for comflg,resflg,setflg */

#define	D1	0x6100	/*  7-bit displacement (for djnz,dbjnz) */
#define	D2	0x6200	/*  8-bit displacement (for jr) */
#define	D3	0x6300	/* 12-bit displacement (for calr) */
#define	D4	0x6400	/* 16-bit displacement (for ld BA) */
#define	D5	0x6500	/* 16-bit displacement (for ldr) */

#define	SR	0x7100	/* special (control) register */
#define	UR	0x7200	/* user special (control) register */
#define	VI	0x7400	/* interrupts (vi,nvi) */
#define	NI	0x7500	/* interrupts (vi,nvi) */
#define	BN	0x7600	/* 1,2 bit field for rl,rlc,rr,rrc */
#define	ID	0x7700	/* increment and decrement fields (to be bumped) */
#define	P1	0x7800	/* +b field for sla,sll */
#define	P2	0x7900	/* +b field for slab,sllb */
#define	P3	0x7A00	/* +b field for slal,slll */
#define	N1	0x7B00	/* -b field for sra,srl */
#define	N2	0x7C00	/* -b field for srab,srlb */
#define	N3	0x7D00	/* -b field for sral,srll */
#define SL	0x7E00	/* long special (control) register */

/* bizarre data types */
#define	XRST	0x8100
#define	XBOF	0x8200
#define	XBWD	0x8300

/* register data types */
#define	RNS1	0x8400 /* normal	single	reg(b, c, d, e, h, l, m ) */
#define	RNS2	0x8500 /* normal	single	reg(b, c, d, e, h, l, m ) */
#define	XPSR	0x8600 /* prime	single	reg(b',c',d',e',h',l',m') */
#define	XIXY	0x8700 /* XY	single	reg(ixh, ixl, iyh, iyl) */
#define	XISR	0x8800 /* indexed	single	reg(@(c))	*/
#define	XNDR	0x8900 /* normal	double	reg(bc, de, hl, pc, sp) */
#define	XPDR	0x8a00 /* prime	double	reg(bc',de',hl') */
#define	XIDR	0x8b00 /* indexed	double	reg(@(bc,de,hl,sp,ix,iy)) */
#define	XIRR	0x8c00 /* indexed	registr	reg(@(hl+ix,hl+iy,ix+iy))*/
#define	XIII	0x8d00 /* i register */
#define	XRRR	0x8e00 /* r register */
#define	XTXR	0x8f00 /* TX register */
#define	DREG	0xA000
#define	PSRG	0xA100
#define	PDRG	0xA200

#define	XOPC	0xA300

#define	EM	0xB000
#define	BP	0xB100
#define	BS	0xB200

struct op {
 BYTE	opc;
 BYTE	opb[4];
 WORD	opa[4];
};
extern	struct	op	ops[];

/*
********	reserved symbol table	(from z800.c)
*/

extern	char	*opcodes[];
extern	struct	symbol	oprands[];
/*
**	struct	rsym	{
**	 int	ltype;
**	 int	lvalu;
**	 char	*label;
**	};
**	
**	extern	struct	rsym oprands[];
*/

/*
********	globals for assembler / disassembler
**
**	int	types[MAXARGS]	- array for all of the fields
**	int	fields[MAXARGS]	- pointers to start of fields in formatted str2.
**	long	values[MAXARGS]	- array of numeric decode
**
*/

			/* maxargs (including comment field) */
#define	MAXA	4
#define	MAXB	4
#define	MAXF	10

extern	int	args,opcode;
extern	int	types[];
extern	long	values[];
extern	char	*fields[];
extern	char	tline[];


extern	int	opsa,opsb;
extern	int	ttype,fldtype,bitoff,offset,bitnum;
extern	int	ivalue;
extern	long	tvalue;
extern	int	z12,segmode;
extern	POINTER	slide;		/* relative pc offset for assembler */
