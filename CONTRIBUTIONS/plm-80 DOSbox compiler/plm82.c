/* plm82.f -- translated by f2c (version 20060506).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "f2c.h"

/* Common Block Declarations */

struct symbl_1_ {
    integer symbol[3000], symax, sytop, syinfo, lmem;
};

#define symbl_1 (*(struct symbl_1_ *) &symbl_)

struct cntrl_1_ {
    integer contrl[64];
};

#define cntrl_1 (*(struct cntrl_1_ *) &cntrl_)

struct titles_1_ {
    integer title[10], vers;
};

#define titles_1 (*(struct titles_1_ *) &titles_)

struct terrr_1_ {
    integer terr[22];
    logical errflg;
};

#define terrr_1 (*(struct terrr_1_ *) &terrr_)

struct files_1_ {
    integer ibuff[80], obuff[120], ibp, obp, itran[256], otran[64];
};

#define files_1 (*(struct files_1_ *) &files_)

struct factor_1_ {
    integer wdsize, wfact, two8, fact[5];
};

#define factor_1 (*(struct factor_1_ *) &factor_)

struct memory_1_ {
    integer maxmem, memtop, membot, mem[2500], maxvm, offset, preamb;
};

#define memory_1 (*(struct memory_1_ *) &memory_)

struct pstack_1_ {
    integer prstk[15], maxdep[16], curdep[16], prsmax, prsp, lxis;
};

#define pstack_1 (*(struct pstack_1_ *) &pstack_)

struct messg_1_ {
    integer mssg[77];
};

#define messg_1 (*(struct messg_1_ *) &messg_)

struct code_1_ {
    integer codloc, alter, cbits[43];
};

#define code_1 (*(struct code_1_ *) &code_)

struct regall_1_ {
    integer regs[7], regv[7], lock[7], prec[16], st[16], rasn[16], litv[16], 
	    sp, maxsp, intbas;
};

#define regall_1 (*(struct regall_1_ *) &regall_)

struct ops_1_ {
    integer ld, in, dc, ad, ac, su, sb, nd, xr, or, cp, rot, jmp, jmc, cal, 
	    clc, rtn, rtc, rst, inp, out, halt, sta, lda, xchg, sphl, pchl, 
	    cma, stc, cmc, daa, shld, lhld, ei, di, lxi, push, pop, dad, stax,
	     ldax, incx, dcx, ra, rb, rc, rd, re, rh, rl, rsp, me, lft, rgt, 
	    tru, fal, cy, acc, carry, zero, sign, parity;
};

#define ops_1 (*(struct ops_1_ *) &ops_)

struct types_1_ {
    integer varb, intr, proc, label, liter;
};

#define types_1 (*(struct types_1_ *) &types_)

struct peep_1_ {
    integer lapol, lastld, lastrg, lastin, lastex, lastir;
};

#define peep_1 (*(struct peep_1_ *) &peep_)

struct inter_1_ {
    integer intpro[8];
};

#define inter_1 (*(struct inter_1_ *) &inter_)

struct base_1_ {
    integer debase;
};

#define base_1 (*(struct base_1_ *) &base_)

struct {
    integer acclen, accum[32], type__, stype, eoflag, ident, numb, specl, str,
	     cont, value, ascii[48];
} scanc_;

#define scanc_1 scanc_

struct inst_1_ {
    integer ctran[256], insym[284], ibytes[23];
};

#define inst_1 (*(struct inst_1_ *) &inst_)

struct rgmapp_1_ {
    integer regmap[9];
};

#define rgmapp_1 (*(struct rgmapp_1_ *) &rgmapp_)

struct bifloc_1_ {
    integer inloc, outloc, timloc, casjmp;
};
struct bifloc_2_ {
    integer inloc, outloc, firsti, casjmp;
};

#define bifloc_1 (*(struct bifloc_1_ *) &bifloc_)
#define bifloc_2 (*(struct bifloc_2_ *) &bifloc_)

struct smessg_1_ {
    integer smssg[29];
};

#define smessg_1 (*(struct smessg_1_ *) &smessg_)

struct bifcod_1_ {
    integer biftab[41], bifpar;
};

#define bifcod_1 (*(struct bifcod_1_ *) &bifcod_)

struct opcod_1_ {
    integer polchr[18], opcval[51];
};

#define opcod_1 (*(struct opcod_1_ *) &opcod_)

struct ilcod_1_ {
    integer opr, adr, vlu, def, lit, lin, nop, add, adc, sub, sbc, mul, div, 
	    mdf, neg, and, ior, xor, not, eql, lss, gtr, neq, leq, geq, inx, 
	    tra, trc, pro, ret, sto, std, xch, del, dat, lod, bif, inc, cse, 
	    end, enb, enp, hal, rtl, rtr, sfl, sfr, hiv, lov, cva, org, drt, 
	    ena, dis, ax1, ax2, ax3;
};

#define ilcod_1 (*(struct ilcod_1_ *) &ilcod_)

struct xfropt_1_ {
    integer xfrloc, xfrsym, tstloc, conloc, defsym, defrh, defrl;
};

#define xfropt_1 (*(struct xfropt_1_ *) &xfropt_)

struct sthed_1_ {
    integer sthead[12];
};

#define sthed_1 (*(struct sthed_1_ *) &sthed_)

/* Initialized data */

struct {
    integer fill_1[3000];
    integer e_2[3];
    integer fill_3[1];
    } symbl_ = { {0}, 3000, 0, 3000 };

struct {
    integer e_1;
    integer fill_2[1];
    integer e_3[43];
    } code_ = { 0, {0}, 64, 4, 5, 128, 136, 144, 152, 160, 168, 176, 184, 7, 
	    195, 194, 205, 196, 201, 192, 199, 219, 211, 118, 50, 58, 235, 
	    249, 233, 47, 55, 63, 39, 34, 42, 251, 243, 1, 197, 193, 9, 2, 10,
	     3, 11 };

struct {
    integer e_1[21];
    integer fill_2[64];
    integer e_3[3];
    } regall_ = { 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -1, -1, 0, 0, 0, 0,
	     0, 0, 0, {0}, 0, 16, 23 };

struct {
    integer e_1[62];
    } ops_ = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 
	    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 
	    35, 36, 37, 38, 39, 40, 41, 42, 43, 1, 2, 3, 4, 5, 6, 7, 9, 8, 9, 
	    10, 12, 11, 13, 14, 15, 16, 17, 18 };

struct {
    integer e_1[5];
    } types_ = { 1, 2, 3, 4, 6 };

struct {
    integer e_1[64];
    } cntrl_ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	    ;

struct {
    integer e_1[11];
    } titles_ = { 27, 23, 24, 4, 1, 33, 16, 29, 30, 1, 20 };

struct {
    integer e_1[22];
    logical e_2;
    } terrr_ = { 14, 26, 24, 27, 20, 23, 12, 31, 20, 26, 25, 1, 31, 16, 29, 
	    24, 20, 25, 12, 31, 16, 15, FALSE_ };

struct {
    integer e_1[6];
    } peep_ = { -1, 0, 0, 0, 0, 0 };

struct {
    integer fill_1[200];
    integer e_2[2];
    integer fill_3[256];
    char e_4[208];
    integer e_5[12];
    } files_ = { {0}, 81, 0, {0}, "    0   1   2   3   4   5   6   7   8   9"
	    "   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P"
	    "   Q   R   S   T   U   V   W   X   Y   Z   $   =   .   /   (   )"
	    "   +   -   '   *   ,   <   >   :   ;   ", 0, 0, 0, 0, 0, 0, 0, 0, 
	    0, 0, 0, 0 };

struct {
    integer e_1[8];
    } inter_ = { 0, 0, 0, 0, 0, 0, 0, 0 };

struct {
    integer e_1;
    } base_ = { 16 };

struct {
    integer e_1;
    integer fill_2[1];
    integer e_3;
    integer fill_4[5];
    } factor_ = { 31, {0}, 256 };

struct {
    integer e_1[563];
    } inst_ = { 835, 36, 40, 42, 1057, 2081, 1280, 35, 995, 39, 41, 43, 1089, 
	    2113, 2304, 67, 995, 100, 104, 106, 1121, 2145, 3328, 99, 995, 
	    103, 105, 107, 1153, 2177, 4352, 131, 995, 164, 707, 170, 1185, 
	    2209, 5376, 675, 995, 167, 739, 171, 1217, 2241, 6400, 579, 995, 
	    292, 387, 298, 1249, 2273, 7424, 611, 995, 295, 419, 299, 1025, 
	    2049, 256, 643, 1056, 1088, 1120, 1152, 1184, 1216, 1248, 1024, 
	    2080, 2112, 2144, 2176, 2208, 2240, 2272, 2048, 3104, 3136, 3168, 
	    3200, 3232, 3264, 3296, 3072, 4128, 4160, 4192, 4224, 4256, 4288, 
	    4320, 4096, 5152, 5184, 5216, 5248, 5280, 5312, 5344, 5120, 6176, 
	    6208, 6240, 6272, 6304, 6336, 6368, 6144, 7200, 7232, 7264, 7296, 
	    7328, 7360, 355, 7168, 32, 64, 96, 128, 160, 192, 224, 0, 3105, 
	    3137, 3169, 3201, 3233, 3265, 3297, 3073, 4129, 4161, 4193, 4225, 
	    4257, 4289, 4321, 4097, 5153, 5185, 5217, 5249, 5281, 5313, 5345, 
	    5121, 6177, 6209, 6241, 6273, 6305, 6337, 6369, 6145, 7201, 7233, 
	    7265, 7297, 7329, 7361, 7393, 7169, 8225, 8257, 8289, 8321, 8353, 
	    8385, 8417, 8193, 9249, 9281, 9313, 9345, 9377, 9409, 9441, 9217, 
	    10273, 10305, 10337, 10369, 10401, 10433, 10465, 10241, 3106, 38, 
	    1058, 163, 2082, 37, 3329, 259, 3234, 227, 1186, 995, 2210, 195, 
	    4353, 1283, 3074, 102, 1026, 323, 2050, 101, 5377, 2307, 3202, 
	    995, 1154, 291, 2178, 995, 6401, 3331, 3170, 166, 1122, 483, 2146,
	     165, 7425, 4355, 3298, 547, 1250, 451, 2274, 995, 8449, 5379, 
	    3138, 6, 1090, 803, 2114, 5, 9473, 6403, 3266, 515, 1218, 771, 
	    2242, 995, 10497, 7427, 15, 38, 60, 66, 108, 116, 234, 240, 247, 
	    253, 259, 266, 273, 279, 10, 27, 28, 29, 30, 31, 32, 33, 34, 35, 
	    36, 38, 12, 13, 14, 15, 16, 19, 23, 24, 20, 30, 27, 8, 48, 50, 52,
	     53, 55, 56, 57, 58, 60, 25, 14, 25, 37, 27, 27, 26, 14, 37, 24, 
	    27, 16, 1, 63, 66, 24, 26, 33, 10, 78, 81, 84, 87, 90, 93, 96, 99,
	     102, 105, 108, 20, 25, 29, 15, 14, 29, 12, 15, 15, 12, 15, 14, 
	    30, 32, 13, 30, 13, 14, 12, 25, 12, 35, 29, 12, 26, 29, 12, 14, 
	    24, 27, 3, 113, 114, 115, 116, 21, 14, 29, 31, 149, 152, 155, 158,
	     161, 164, 168, 171, 174, 176, 179, 182, 185, 188, 192, 196, 200, 
	    204, 207, 210, 213, 216, 220, 224, 226, 228, 231, 231, 231, 231, 
	    231, 234, 29, 23, 14, 29, 29, 14, 29, 12, 23, 29, 12, 29, 21, 24, 
	    27, 14, 12, 23, 23, 29, 16, 31, 29, 30, 31, 20, 25, 26, 32, 31, 
	    19, 23, 31, 30, 31, 12, 23, 15, 12, 35, 14, 19, 18, 35, 31, 19, 
	    23, 30, 27, 19, 23, 27, 14, 19, 23, 14, 24, 12, 30, 31, 14, 14, 
	    24, 14, 15, 12, 12, 30, 19, 23, 15, 23, 19, 23, 15, 16, 20, 15, 
	    20, 25, 26, 27, 45, 45, 45, 1, 237, 240, 23, 35, 20, 1, 243, 247, 
	    27, 32, 30, 19, 1, 250, 253, 27, 26, 27, 1, 256, 259, 15, 12, 15, 
	    1, 262, 266, 30, 31, 12, 35, 1, 269, 273, 23, 15, 12, 35, 1, 276, 
	    279, 20, 25, 35, 1, 282, 285, 15, 14, 35, 0, 0, 0, 0, 2, 2, 0, 0, 
	    1, 1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2 };

struct {
    integer e_1[9];
    } rgmapp_ = { 7, 0, 1, 2, 3, 4, 5, 6, 6 };

struct {
    integer e_1[4];
    } bifloc_ = { 16, 17, 7, 0 };

struct {
    integer e_1[29];
    } smessg_ = { 30, 31, 12, 14, 22, 1, 30, 20, 37, 16, 1, 39, 1, 26, 33, 16,
	     29, 29, 20, 15, 15, 16, 25, 1, 13, 36, 31, 16, 30 };

struct {
    integer e_1[42];
    } bifcod_ = { -3, -20, 35, 3, 5, 27, 33, 7902073, 848538, 6905856, 
	    5063915, 33, 11630827, 7924680, 7948063, 13782815, 1638430, 
	    12790251, 16, 45, 2, 15, 35, 5713786, 6238075, 8467, 1129984, 
	    13769189, 14876690, 7992801, 7884567, 8210199, 8154903, 15820567, 
	    836157, 8173312, 8214303, 13197087, 0, 0, 0, 0 };

struct {
    integer e_1[69];
    } opcod_ = { 26, 27, 29, 12, 15, 29, 33, 12, 23, 15, 16, 17, 23, 20, 31, 
	    23, 20, 25, 104091, 50127, 50126, 124941, 123726, 100375, 62753, 
	    119832, 103442, 50767, 83613, 145053, 104095, 67351, 96158, 75741,
	     103452, 95260, 74780, 83555, 128844, 128846, 112474, 119839, 
	    124890, 124879, 144275, 62487, 62239, 95887, 54545, 83534, 59280, 
	    67151, 67149, 67163, 78615, 120791, 120797, 123991, 123997, 79137,
	     95905, 59468, 108370, 63327, 67148, 62750, 51395, 51396, 51397 };

struct {
    integer e_1;
    integer fill_2[2503];
    integer e_3;
    integer fill_4[1];
    } memory_ = { 2500, {0}, 0 };

struct {
    integer e_1[57];
    } ilcod_ = { 0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
	    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 
	    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 
	    45, 46, 47, 48, 49, 50 };

struct {
    integer e_1[7];
    } xfropt_ = { -1, 0, -1, -1, 0, -1, -1 };

struct {
    integer e_1[12];
    } sthed_ = { 27, 29, 30, 31, 29, 12, 30, 25, 23, 20, 31, 33 };

struct {
    integer e_1[50];
    } pstack_ = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	    0, 0, 0, 0, 0, 15, 0, 0 };

struct {
    integer e_1[77];
    } messg_ = { 27, 12, 30, 30, 45, 25, 26, 27, 29, 26, 18, 29, 12, 24, 1, 
	    16, 29, 29, 26, 29, 42, 43, 25, 16, 12, 29, 12, 31, 27, 12, 29, 
	    30, 16, 1, 30, 31, 12, 14, 22, 51, 1, 30, 36, 24, 13, 26, 23, 1, 
	    1, 12, 15, 15, 29, 1, 34, 15, 30, 1, 14, 19, 29, 30, 1, 1, 1, 23, 
	    16, 25, 18, 31, 19, 1, 27, 29, 1, 31, 36 };


/* Table of constant values */

static integer c__0 = 0;
static integer c__4 = 4;
static integer c__8080 = 8080;
static integer c__10 = 10;
static integer c__1 = 1;
static integer c__40 = 40;
static integer c__144 = 144;
static integer c__150 = 150;
static integer c__16 = 16;
static integer c__30 = 30;
static integer c__5 = 5;
static integer c__38 = 38;
static integer c__6 = 6;
static integer c__7 = 7;
static integer c__77 = 77;
static integer c__2 = 2;
static integer c_n5 = -5;
static integer c__8 = 8;
static integer c__20 = 20;
static integer c__101 = 101;
static integer c__102 = 102;
static integer c__103 = 103;
static integer c__104 = 104;
static integer c__80 = 80;
static integer c__3 = 3;
static integer c__105 = 105;
static integer c__39 = 39;
static integer c_n10 = -10;
static integer c__19 = 19;
static integer c__42 = 42;
static integer c__43 = 43;
static integer c_n4 = -4;
static integer c__22 = 22;
static integer c__106 = 106;
static integer c_n2 = -2;
static integer c__152 = 152;
static integer c__107 = 107;
static integer c__108 = 108;
static integer c__109 = 109;
static integer c__110 = 110;
static integer c__111 = 111;
static integer c__147 = 147;
static integer c__112 = 112;
static integer c__113 = 113;
static integer c__148 = 148;
static integer c__114 = 114;
static integer c__115 = 115;
static integer c__116 = 116;
static integer c__117 = 117;
static integer c_n6 = -6;
static integer c__48 = 48;
static integer c__45 = 45;
static integer c__47 = 47;
static integer c_n8 = -8;
static integer c__51 = 51;
static integer c_n255 = -255;
static integer c_n1 = -1;
static integer c__118 = 118;
static integer c__119 = 119;
static integer c__120 = 120;
static integer c__121 = 121;
static integer c__122 = 122;
static integer c__11 = 11;
static integer c__29 = 29;
static integer c__12 = 12;
static integer c__13 = 13;
static integer c__24 = 24;
static integer c__14 = 14;
static integer c__23 = 23;
static integer c__123 = 123;
static integer c__124 = 124;
static integer c__125 = 125;
static integer c__126 = 126;
static integer c__9 = 9;
static integer c__127 = 127;
static integer c__18 = 18;
static integer c__128 = 128;
static integer c__129 = 129;
static integer c__130 = 130;
static integer c__145 = 145;
static integer c__131 = 131;
static integer c__256 = 256;
static integer c__134 = 134;
static integer c__135 = 135;
static integer c_n12 = -12;
static integer c__133 = 133;
static integer c__136 = 136;
static integer c__146 = 146;
static integer c__138 = 138;
static integer c__139 = 139;
static integer c__149 = 149;
static integer c__140 = 140;
static integer c__141 = 141;
static integer c__32 = 32;
static integer c__143 = 143;

/* *********************************************************************** */

/*         8 0 8 0   P L / M   C O M P I L E R ,   P A S S - 2 */
/*                                 PLM82 */
/*                              VERSION 2.0 */
/*                             JANUARY, 1975 */

/*                          COPYRIGHT (C) 1975 */
/*                          INTEL CORPORATION */
/*                          3065 BOWERS AVENUE */
/*                          SANTA CLARA, CALIFORNIA 95051 */

/*  MODIFYED BY JEFF OGDEN (UM), DECEMBER 1977. */

/* *********************************************************************** */


/*         P A S S - 2     E R R O R    M E S S A G E S */

/*  ERROR                     MESSAGE */
/*  NUMBER */
/*  ------  --- ------------------------------------------------------- */

/*   101     REFERENCE TO STORAGE LOCATIONS OUTSIDE THE VIRTUAL MEMORY */
/*           OF PASS-2.  RE-COMPILE PASS-2 WITH LARGER 'MEMORY' ARRAY. */

/*   102         " */

/*   103     VIRTUAL MEMORY OVERFLOW.  PROGRAM IS TOO LARGE TO COMPILE */
/*           WITH PRESENT SIZE OF 'MEMORY.'  EITHER SHORTEN PROGRAM OR */
/*           RECOMPILE PASS-2 WITH A LARGER VIRTUAL MEMORY. */

/*   104     (SAME AS 103). */


/*   105     $TOGGLE USED IMPROPERLY IN PASS-2.  ATTEMPT TO COMPLEMENT */
/*           A TOGGLE WHICH HAS A VALUE OTHER THAN 0 OR 1. */

/*   106     REGISTER ALLOCATION TABLE UNDERFLOW.  MAY BE DUE TO A PRE- */

/*   107     REGISTER ALLOCATION ERROR.  NO REGISTERS AVAILABLE.  MAY */
/*           BE CAUSED BY A PREVIOUS ERROR, OR PASS-2 COMPILER ERROR. */

/*   108     PASS-2 SYMBOL TABLE OVERFLOW.  REDUCE NUMBER OF */
/*           SYMBOLS, OR RE-COMPILE PASS-2 WITH LARGER SYMBOL TABLE. */

/*   109     SYMBOL TABLE OVERFLOW (SEE ERROR 108). */

/*   110     MEMORY ALLOCATION ERROR.  TOO MUCH STORAGE SPECIFIED IN */
/*           THE SOURCE PROGRAM (16K MAX).  REDUCE SOURCE PROGRAM */
/*           MEMORY REQUIREMENTS. */

/*   111     INLINE DATA FORMAT ERROR.  MAY BE DUE TO IMPROPER */
/*           RECORD SIZE IN SYMBOL TABLE FILE PASSED TO PASS-2. */

/*   112     (SAME AS ERROR 107). */

/*   113     REGISTER ALLOCATION STACK OVERFLOW.  EITHER SIMPLIFY THE */
/*           PROGRAM OR INCREASE THE SIZE OF THE ALLOCATION STACKS. */

/*   114     PASS-2 COMPILER ERROR IN 'LITADD' -- MAY BE DUE TO A */
/*           PREVIOUS ERROR. */

/*   115     (SAME AS 114). */

/*   116     (SAME AS 114). */

/*   117     LINE WIDTH SET TOO NARROW FOR CODE DUMP (USE $WIDTH=N) */

/*   118     (SAME AS 107). */

/*   119     (SAME AS 110). */

/*   120     (SAME AS 110, BUT MAY BE A PASS-2 COMPILER ERROR). */

/*   121     (SAME AS 108). */

/*   122     PROGRAM REQUIRES TOO MUCH PROGRAM AND VARIABLE STORAGE. */
/*           (PROGRAM AND VARIABLES EXCEED 16K). */

/*   123     INITIALIZED STORAGE OVERLAPS PREVIOUSLY INITIALIZED STORAGE. */

/*   124     INITIALIZATION TABLE FORMAT ERROR.  (SEE ERROR 111). */

/*   125     INLINE DATA ERROR.  MAY HAVE BEEN CAUSED BY PREVIOUS ERROR. */

/*   126     BUILT-IN FUNCTION IMPROPERLY CALLED. */

/*   127     INVALID INTERMEDIATE LANGUAGE FORMAT. (SEE ERROR 111). */

/*   128     (SAME AS ERROR 113). */

/*   129     INVALID USE OF BUILT-IN FUNCTION IN AN ASSIGNMENT. */

/*   130     PASS-2 COMPILER ERROR.  INVALID VARIABLE PRECISION (NOT */
/*           SINGLE BYTE OR DOUBLE BYTE).  MAY BE DUE TO PREVIOUS ERROR. */

/*   131     LABEL RESOLUTION ERROR IN PASS-2 (MAY BE COMPILER ERROR). */

/*   132     (SAME AS 108). */

/*   133     (SAME AS 113). */

/*   134     INVALID PROGRAM TRANSFER (ONLY COMPUTED JUMPS ARE ALLOWED */
/*           WITH A 'GO TO'). */

/*   135     (SAME AS 134). */

/*   136     ERROR IN BUILT-IN FUNCTION CALL. */

/*   137     (NOT USED) */

/*   138     (SAME AS 107). */

/*   139     ERROR IN CHANGING VARIABLE TO ADDRESS REFERENCE. MAY */
/*           BE A PASS-2 COMPILER ERROR, OR MAY BE CAUSED BY PRE- */
/*           VOUS ERROR. */

/*   140     (SAME AS 107). */

/*   141     INVALID ORIGIN.  CODE HAS ALREADY BEEN GENERATED IN THE */
/*           SPECIFIED LOCATIONS. */

/*   142     A SYMBOL TABLE DUMP HAS BEEN SPECIFIED (USING THE $MEMORY */
/*           TOGGLE IN PASS-1), BUT NO FILE HAS BEEN SPECIFIED TO RE- */
/*           CEIVE THE BNPF TAPE (USE THE $BNPF=N CONTROL). */

/*   143     INVALID FORMAT FOR THE SIMULATOR SYMBOL TABLE DUMP (SEE */
/*           ERROR 111). */

/*   144     STACK NOT EMPTY AT END OF COMPILATION.  POSSIBLY CAUSED */
/*           BY PREVIOUS COMPILATION ERROR. */

/*   145     PROCEDURES NESTED TOO DEEPLY (HL OPTIMIZATION) */
/*           SIMPLIFY NESTING, OR RE-COMPILE WITH LARGER PSTACK */

/*   146     PROCEDURE OPTIMIZATION STACK UNDERFLOW.  MAY BE A */
/*           RETURN IN OUTER BLOCK. */

/*   147     PASS-2 COMPILER ERROR IN LOADV. REGISTER */
/*           STACK ORDER IS INVALID.  MAY BE DUE TO PREVIOUS ERROR. */

/*   148     PASS-2 COMPILER ERROR.  ATTEMPT TO UNSTACK TOO */
/*           MANY VALUES.  MAY BE DUE TO PREVIOUS ERROR. */

/*   149     PASS-2 COMPILER ERROR. ATTEMPT TO CONVERT INVALID */
/*           VALUE TO ADDRESS TYPE.  MAY BE DUE TO PREVIOUS ERROR. */

/*   150     (SAME AS 147) */

/*   151     PASS-2 COMPILER ERROR. UNBALANCED  EXECUTION STACK */
/*           AT BLOCK END.  MAY BE DUE TO A PREVIOUS ERROR. */

/*    152    INVALID STACK ORDER IN APPLY.  MAY BE DUE TO PREVIOUS */
/*           ERROR. */


/*              I M P L E M E N T A T I O N    N O T E S */
/*              - - - - - - - - - - - - - -    - - - - - */
/*    THE PL/M COMPILER IS INTENDED TO BE WRITTEN IN ANSI STANDARD */
/*    FORTRAN - IV, AND THUS IT SHOULD BE POSSIBLE TO COMPILE AND */
/*    EXECUTE THIS PROGRAM ON ANY MACHINE WHICH SUPPORTS THIS FORTRAN */
/*    STANDARD.  BOTH PASS-1 AND PASS-2, HOWEVER, ASSUME THE HOST */
/*    MACHINE WORD SIZE IS AT LEAST 31 BITS, EXCLUDING THE SIGN BIT */
/*    (I.E., 32 BITS IF THE SIGN IS INCLUDED). */

/*    THE IMPLEMENTOR MAY FIND IT NECESSARY TO CHANGE THE SOURCE PROGRAM */
/*    IN ORDER TO ACCOUNT FOR SYSTEM DEPENDENCIES.  THESE CHANGES ARE */
/*    AS FOLLOWS */

/*    1)   THE FORTRAN LOGICAL UNIT NUMBERS FOR VARIOUS DEVICES */
/*         MAY HAVE TO BE CHANGED IN THE 'GNC' AND 'WRITEL' SUBROU- */
/*         TINES (SEE THE FILE DEFINITIONS BELOW). */

/*     2)   THE HOST MACHINE MAY NOT HAVE THE PL/M 52 CHARACTER SET */
/*           0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ$=./()+-'*,<>:; */
/*         (THE LAST 15 SPECIAL CHARACTERS ARE */
/*         DOLLAR,  EQUAL,  PERIOD,  SLASH, LEFT PAREN, */
/*         RIGHT PAREN, PLUS,   MINUS,  QUOTE, ASTERISK, */
/*         COMMA, LESS-THAN, GREATER-THAN, COLON, SEMI-COLON) */
/*         IN THIS CASE, IT IS NECESSARY TO CHANGE THE 'OTRAN' VECTOR IN */
/*         BLOCK DATA TO A CHARACTER SET WHICH THE HOST MACHINE SUPPORTS */

/*     3)  ALTHOUGH THE DISTRIBUTION VERSION OF PASS-2 ASSUMES A */
/*        MINIMUM OF 31 BITS PER WORD ON THE HOST MACHINE, BETTER */
/*        STORAGE UTILIZATION IS OBTAINED BY ALTERING THE 'WDSIZE' */
/*        PARAMETER IN BLOCK DATA (SECOND TO LAST LINE OF THIS PROGRAM). */
/*        THE WDSIZE IS CURRENTLY SET TO 31 BITS (FOR THE S/360), AND */
/*        THUS WILL EXECUTE ON ALL MACHINES WITH A LARGER WORD SIZE.  THE */
/*        VALUE OF WDSIZE MAY BE SET TO THE NUMBER OF USABLE BITS IN */
/*        A FORTRAN INTEGER, EXCLUDING THE SIGN BIT (E.G., ON A */
/*        CDC 6X00, SET WDSIZE TO 44, AND ON A UNIVAC 1108, SET WDSIZE */
/*        TO 35).  IN GENERAL, LARGER VALUES OF WDSIZE ALLOW LARGER 8080 */
/*        PROGRAMS TO BE COMPILED WITHOUT CHANGING THE SIZE OF THE */
/*        'MEM' VECTOR. */

/*     4)  THE HOST FORTRAN SYSTEM MAY HAVE A LIMITATION ON THE NUMBER */
/*         OF CONTIGUOUS COMMENT RECORDS (E.G. S/360 LEVEL G). IF SO, */
/*         INTERSPERSE THE DECLARATION STATEMENTS INTEGER I1000, INTEGER */
/*         I1001, ETC., AS NECESSARY TO BREAK UP THE LENGTH OF COMMENTS. */
/*         THE SYMBOLS I1XXX ARE RESERVED FOR THIS PURPOSE. */

/*    THERE ARE A NUMBER OF COMPILER PARAMETERS WHICH MAY HAVE TO */
/*    BE CHANGED FOR YOUR INSTALLATION.  THESE PARAMETERS ARE DEFINED */
/*    BELOW (SEE 'SCANNER COMMANDS'), AND THE CORRESPONDING DEFAULT */
/*    VALUES ARE SET FOLLOWING THEIR DEFINITION.  FOR EXAMPLE, THE */
/*                  $RIGHTMARGIN = I */
/*    PARAMETER DETERMINES THE RIGHT MARGIN OF THE INPUT SOURCE LINE. */
/*    THE PARAMETER IS SET EXTERNALLY BY A SINGLE LINE STARTING WITH */
/*    '$R' IN COLUMNS ONE AND TWO (THE REMAINING CHARACTERS UP TO */
/*    THE '=' ARE IGNORED).  THE INTERNAL COMPILER REPRESENTATION */
/*    OF THE CHARACTER 'R' IS 29 (SEE CHARACTER CODES BELOW), AND THUS */
/*    THE VALUE OF THE $RIGHTMARGIN PARAMETER CORRESPONDS TO ELEMENT 29 */
/*    OF THE 'CONTRL' VECTOR. */

/*    1)  IF OPERATING IN  AN INTERACTIVE MODE, IT IS OFTEN */
/*        DESIRABLE TO MINIMIZE OUTPUT FROM PASS-2.  THUS, THE FOLLOWING */
/*        PARAMETERS ARE USUALLY SET AS DEFAULTS */
/*               $TERMINAL   =  1 */
/*               $INPUT      =  1 */
/*               $OUTPUT     =  1 */
/*               $GENERATE   =  0 */
/*               $FINISH     =  0 */

/*        ALL OTHER PARAMETERS ARE THEN SELECTED FROM THE CONSOLE */

/*    2)  IF OPERATING IN BATCH MODE, A NUMBER OF DEFAULT TOGGLES ARE */
/*        OFTEN SET WHICH PROVIDE USEFUL INFORMATION WHEN DEBUGGING */
/*        THE FINAL PROGRAM */
/*               $TERMINAL   =  0 */
/*               $INPUT      =  2 */
/*               $OUTPUT     =  2 */
/*               $GENERATE   =  1 (LINE NUMBER VS. CODE LOCATIONS) */
/*               $FINISH     =  1 (DECODE PROGRAM INTO MNEMONICS AT END) */

/*    3)  IF OPERATING WITH AN INTELLEC 8/80, IT MAY BE USEFUL TO SET */
/*        THE CODE GENERATION HEADER AT 16, PAST THE MONITOR'S VARIABLES. */
/*               $HEADER     = 16 */

/*    RECALL, OF COURSE, THAT THE PROGRAMMER CAN ALWAYS OVERRIDE THESE */
/*    DEFAULT TOGGLES -- THEY ARE ONLY A CONVENIENCE TO THE PROGRAMMER. */

/*    5)  THE CHARACTERISTICS OF THE INTERMEDIATE LANGUAGE FILES */
/*        PRODUCED BY PASS-1 ARE MONITORED BY THE $J, $R, $U, AND */
/*        $Z PARAMETERS.  THESE PARAMETERS CORRESPOND TO THE SOURCE */
/*        AND WIDTH OF THE INTERMEDIATE CODE FILE ($J AND $R), AND */
/*        SOURCE AND WIDTH OF THE INTERMEDIATE SYMBOL TABLE ($U */
/*        AND $R).  SOME FORTRAN SYSTEMS DELETE THE LEADING CHARACTER */
/*        OF THE FILES PRODUCED BY OTHER FORTRAN PROGRAMS.  THE $Z */
/*        PARAMETER MAY BE USED TO READ EXTRA BLANKS AT THE BEGINNING OF */
/*        THE INTERMEDIATE FILES IF THIS BECOMES A PROBLEM ON THE HOST */
/*        SYSTEM. */

/*         UNDER NORMAL CIRCUMSTANCES, THESE PARAMETERS WILL NOT */
/*        HAVE TO BE CHANGED.  IN ANY CASE, EXPERIMENT WITH VARIOUS */
/*        VALUES OF THE $ PARAMETERS BY SETTING THEM EXTERNALLY BE- */
/*        FORE ACTUALLY CHANGING THE DEFAULTS. */

/*    THE IMPLEMENTOR MAY ALSO WISH TO INCREASE OR DECREASE THE SIZE */
/*    OF PASS-1 OR PASS-2 TABLES.  THE TABLES IN PASS-2 THAT MAY BE */
/*    CHANGED IN SIZE ARE 'MEM' AND 'SYMBOL' WHICH CORRESPOND TO */
/*    THE AREAS WHICH HOLD THE COMPILED PROGRAM AND PROGRAM SYMBOL */
/*    ATTRIBUTES, RESPECTIVELY.  IT IS IMPOSSIBLE TO PROVIDE AN */
/*    EXACT FORMULA WHICH RELATES THE NUMBER OF SYMBOLS HELD BY */
/*    THE SYMBOL TABLE SINCE THE VARIOUS TYPES OF SYMBOLS REQUIRE */
/*    DIFFERING AMOUNTS OF STORAGE IN THE TABLE. */

/*    1)  IN THE CASE OF THE MEM VECTOR, THE LENGTH IS DETERMINED */
/*        BY THE WDSIZE PARAMETER AND THE LARGEST PROGRAM WHICH YOU */
/*        WISH TO COMPILE.  THE NUMBER OF 8080 (8-BIT) WORDS WHICH ARE */
/*        PACKED INTO EACH MEM ELEMENT IS */

/*                      P = WDSIZE/8 */

/*        AND THUS THE LARGEST PROGRAM WHICH CAN BE COMPILED IS */

/*                      T = P * N */

/*        WHERE N IS THE DECLARED SIZE OF THE MEM VECTOR.  TO CHANGE */
/*        THE SIZE OF MEM, ALTER ALL OCCURRENCES OF */

/*                         MEM(2500) */

/*        IN EACH SUBROUTINE TO MEM(N), WHERE N REPRESENTS THE NEW */
/*        INTEGER CONSTANT SIZE.  IN ADDITION, THE 'DATA' STATEMENT */
/*        IN BLOCK DATA (LAST PROGRAM SEGMENT) MUST BE CHANGED FOR THE */
/*        MACRO PARAMETERS BASED UPON THE CONSTANT VALUE N TO */

/*          DATA WDSIZE /31/, TWO8 /256/, MAXMEM /N/ */

/*    2)  IF THE IMPLEMENTOR WISHES TO INCREASE OR DECREASE THE SIZE */
/*        OF THE SYMBOL TABLE, THEN ALL OCCURRENCES OF */

/*                          SYMBOL(3000) */

/*        MUST BE CHANGED TO SYMBOL(M), WHERE M IS THE DESIRED INTEGER */
/*        CONSTANT SIZE.  THE 'DATA' STATEMENTS FOR SYMBOL TABLE PARA- */
/*        METERS MUST ALSO BE ALTERED AS SHOWN BELOW. */

/*             DATA SYMAX /M/, SYTOP /0/, SYINFO /M/ */

/*    GOOD  LUCK (AGAIN) ... */

/*     F  I  L  E     D  E  F  I  N  I  T  I  O  N  S */
/*            INPUT                        OUTPUT */

/*     FILE   FORTRAN  MTS       DEFAULT   FORTRAN  MTS      DEFAULT */
/*     NUM    I/O UNIT I/O UNIT  FDNAME    I/O UNIT I/O UNIT FDNAME */

/*      1       1      GUSER     *MSOURCE*   11     SERCOM   *MSINK* */
/*      2       2      SCARDS    *SOURCE*    12     SPRINT   *SINK* */
/*      3       3      3                     13     13 */
/*      4       4      4         -PLM16##    14     14 */
/*      5       5      5                     15     15 */
/*      6       6      6                     16     16 */
/*      7       7      7         -PLM17##    17     SPUNCH   -LOAD */


/*   ALL INPUT RECORDS ARE 80 CHARACTERS OR LESS.  ALL */
/*   OUTPUT RECORDS ARE 120 CHARACTERS OR LESS. */
/*   THE FORTRAN UNIT NUMBERS CAN BE CHANGED IN THE */
/*   SUBROUTINES GNC AND WRITEL (THESE ARE THE ONLY OC- */
/*   CURRENCES OF REFERENCES TO THESE UNITS). */



/*    0 1 2 3 4 5 6 7 8 9 */
/*    0 0 0 0 0 0 0 0 1 1 */
/*    2 3 4 5 6 7 8 9 0 1 */


/*    $ = . / ( ) + - ' * , < > : ; */
/*    3 3 4 4 4 4 4 4 4 4 4 4 5 5 5 */
/*    8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 */

/*    A B C D E F G H I J K L M N O P Q R S T U V W X Y Z */
/*    1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3 3 3 3 3 3 */
/*    2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 */


/*  SEQNO               SUB/FUNC NAME */
/*  16280000      SUBROUTINE INITAL */
/*  16560000      INTEGER FUNCTION GET(IP) */
/*  16740000      SUBROUTINE PUT(IP,X) */
/*  16960000      INTEGER FUNCTION ALLOC(I) */
/*  17150000      FUNCTION ICON(I) */
/*  17340000      INTEGER FUNCTION GNC(Q) */
/*  18690000      FUNCTION IMIN(I,J) */
/*  18760000      SUBROUTINE FORM(CC,CHARS,START,FINISH,LENGTH) */
/*  19040000      SUBROUTINE WRITEL(NSPACE) */
/*  19580000      SUBROUTINE CONOUT(CC,K,N,BASE) */
/*  19900000      SUBROUTINE PAD(CC,CHR,I) */
/*  20010000      SUBROUTINE ERROR(I,LEVEL) */
/*  20310000      INTEGER FUNCTION SHR(I,J) */
/*  20350000      INTEGER FUNCTION SHL(I,J) */
/*  20390000      INTEGER FUNCTION RIGHT(I,J) */
/*  20430000      SUBROUTINE DELETE(N) */
/*  20680000      SUBROUTINE APPLY(OP,OP2,COM,CYFLAG) */
/*  23380000      SUBROUTINE GENREG(NP,IA,IB) */
/*  24400000      SUBROUTINE LOADSY */
/*  26100000      SUBROUTINE LOADV(IS,TYPV) */
/*  28330000      SUBROUTINE SETADR(VAL) */
/*  28790000      SUBROUTINE USTACK */
/*  28900000      INTEGER FUNCTION CHAIN(SY,LOC) */
/*  29070000      SUBROUTINE GENSTO(KEEP) */
/*  30880000      SUBROUTINE LITADD(S) */
/*  32120000      SUBROUTINE DUMP(L,U,FA,FE) */
/*  33080000      INTEGER FUNCTION DECODE(CC,I,W) */
/*  34540000      SUBROUTINE EMIT(OPR,OPA,OPB) */
/*  36950000      SUBROUTINE PUNCOD(LB,UB,MODE) */
/*  38010000      SUBROUTINE CVCOND(S) */
/*  38730000      SUBROUTINE SAVER */
/*  40000000      SUBROUTINE RELOC */
/*  41970000      SUBROUTINE LOADIN */
/*  42770000      SUBROUTINE EMITBF(L) */
/*  43510000      SUBROUTINE INLDAT */
/*  44780000      SUBROUTINE UNARY(IVAL) */
/*  45950000      SUBROUTINE EXCH */
/*  46690000      SUBROUTINE STACK(N) */
/*  46790000      SUBROUTINE READCD */
/*  52230000      SUBROUTINE OPERAT(VAL) */
/*  66220000      SUBROUTINE SYDUMP */

/*     GLOBAL VARIABLES */
/* Main program */ int MAIN__(void)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    /* Subroutine */ int s_stop(char *, ftnlen);

    /* Local variables */
    static integer i__, j, k, l, m, n, jp, jl, jn, np;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer gnc_(integer *), icon_(integer *);
    extern /* Subroutine */ int form_(integer *, integer *, integer *, 
	    integer *, integer *), dump_(integer *, integer *, integer *, 
	    integer *), reloc_(void), error_(integer *, integer *), readcd_(
	    void), loadin_(void), inital_(void), puncod_(integer *, integer *,
	     integer *), loadsy_(void), conout_(integer *, integer *, integer 
	    *, integer *), writel_(integer *), sydump_(void);


/*     INITIALIZE MEMORY */
    inital_();
/*     THE FOLLOWING SCANNER COMMANDS ARE DEFINED */
/*     ANALYSIS         (12) */
/*     BPNF             (13) */
/*     COUNT = I        (14) */
/*     DELETE = I       (15) */
/*     EOF              (16) */
/*     FINISH           (17)  DUMP CODE AT FINISH */
/*     GENERATE         (18) */
/*     HEADER           (19) */
/*     INPUT = I        (20) */
/*     JFILE (CODE)= I  (21) */
/*     LEFTMARGIN = I   (23) */
/*     MAP              (24) */
/*     NUMERIC (EMIT)   (25) */
/*     OUTPUT = I       (26) */
/*     PRINT (T OR F)   (27) */
/*     QUICKDUMP = N    (28)  HEXADECIMAL DUMP */
/*     RIGHTMARG = I    (29) */
/*     SYMBOLS          (30) */
/*     TERMINAL         (31) (0=BATCH, 1=TERM, 2=INTERLIST) */
/*     USYMBOL = I      (32) */
/*     VARIABLES        (33) */
/*     WIDTH = I        (34) */
/*     YPAD = N         (36)  BLANK PAD ON OUTPUT */
/*     ZMARGIN = I      (37)  SETS LEFT MARGIN FOR I.L. */
/*     * = N            (47)  0 - COMPILER HANDLES STACK POINTER */
/*                            1 - PROGRAMMER HANDLES  STACK POINTER */
/*                            N > 1 (MOD 65536) N IS BASE VALUE OF SP */

/*     CONTRL(1) HOLDS THE ERROR COUNT */
    for (i__ = 1; i__ <= 64; ++i__) {
/* L2: */
	cntrl_1.contrl[i__ - 1] = -1;
    }
    cntrl_1.contrl[0] = 0;
    cntrl_1.contrl[11] = 0;
    cntrl_1.contrl[12] = 7;
    cntrl_1.contrl[13] = 0;
    cntrl_1.contrl[14] = 120;
    cntrl_1.contrl[15] = 0;
    cntrl_1.contrl[16] = 1;
    cntrl_1.contrl[17] = 1;
    cntrl_1.contrl[18] = 0;
    cntrl_1.contrl[19] = 1;
    cntrl_1.contrl[20] = 4;
    cntrl_1.contrl[22] = 1;
    cntrl_1.contrl[23] = 1;
    cntrl_1.contrl[24] = 0;
    cntrl_1.contrl[25] = 2;
    cntrl_1.contrl[26] = 0;
    cntrl_1.contrl[27] = 1;
    cntrl_1.contrl[28] = 73;
    cntrl_1.contrl[29] = 0;
    cntrl_1.contrl[30] = 1;
    cntrl_1.contrl[31] = 7;
    cntrl_1.contrl[32] = 0;
    cntrl_1.contrl[33] = 120;
    cntrl_1.contrl[35] = 1;
    cntrl_1.contrl[36] = 2;
    cntrl_1.contrl[46] = 0;

    for (i__ = 1; i__ <= 256; ++i__) {
	files_1.itran[i__ - 1] = 1;
/* L8: */
    }

    for (i__ = 53; i__ <= 64; ++i__) {
	files_1.otran[i__ - 1] = files_1.otran[0];
/* L5: */
    }

    for (i__ = 1; i__ <= 52; ++i__) {
	j = files_1.otran[i__ - 1];
	j = icon_(&j);
/* L10: */
	files_1.itran[j - 1] = i__;
    }
    conout_(&c__0, &c__4, &c__8080, &c__10);
    pad_(&c__1, &c__1, &c__1);
    form_(&c__1, titles_1.title, &c__1, &c__10, &c__10);
    i__1 = titles_1.vers / 10;
    conout_(&c__1, &c__1, &i__1, &c__10);
    pad_(&c__1, &c__40, &c__1);
    i__1 = titles_1.vers % 10;
    conout_(&c__1, &c__1, &i__1, &c__10);
    writel_(&c__1);
    i__ = gnc_(&c__0);
/*     CHANGE MARGINS FOR READING INTERMEDIATE LANGUAGE */
    cntrl_1.contrl[22] = cntrl_1.contrl[36];
    writel_(&c__0);
    code_1.codloc = cntrl_1.contrl[18];
    loadsy_();
    readcd_();
    if (terrr_1.errflg) {
	goto L10100;
    }
/*     MAKE SURE COMPILER STACK IS EMPTY */
    if (regall_1.sp != 0) {
	error_(&c__144, &c__1);
    }
/*     MAKE SURE EXECUTION STACK IS EMPTY */
    if (pstack_1.curdep[0] != 0) {
	error_(&c__150, &c__1);
    }
    reloc_();
/*     MAY WANT A SYMBOL TABLE FOR THE SIMULATOR */
    writel_(&c__0);
    sydump_();
    if (cntrl_1.contrl[16] == 0) {
	goto L90;
    }
/*         DUMP THE PREAMBLE */
    i__ = memory_1.offset;
    memory_1.offset = 0;
    if (memory_1.preamb > 0) {
	i__1 = memory_1.preamb - 1;
	dump_(&c__0, &i__1, &c__16, &c__1);
    }
    memory_1.offset = i__;

/*     DUMP THE SYMBOL TABLE BY SEGMENTS UNTIL CODLOC-1 */
    i__ = memory_1.offset + memory_1.preamb;
L15:
    jp = 99999;
    jl = 0;
/*     LOCATE NEXT INLINE DATA AT OR ABOVE I */
    jn = 0;
    np = regall_1.intbas + 1;
    if (np > symbl_1.sytop) {
	goto L22;
    }
    i__1 = symbl_1.sytop;
    for (n = np; n <= i__1; ++n) {
	l = symbl_1.symbol[n - 1];
	m = symbl_1.symbol[l - 2];
	if (m < 0) {
	    goto L20;
	}
	if (m % 16 != types_1.varb) {
	    goto L20;
	}
	j = (i__2 = symbl_1.symbol[l - 1], abs(i__2));
	j %= 65536;
	if (j > jp) {
	    goto L20;
	}
	if (j < i__) {
	    goto L20;
	}
/*         CANDIDATE AT J */
	k = m / 16 % 16;
	if (k > 2) {
	    k = 1;
	}
	k *= m / 256;
	if (k == 0) {
	    goto L20;
	}
/*         FOUND ONE AT J WITH LENGTH K BYTES */
	jp = j;
	jn = n;
	jl = k;
L20:
	;
    }
L22:
/*     JP IS BASE ADDRESS OF NEXT DATA STMT, JL IS LENGTH IN BYTES */

    if (i__ >= jp) {
	goto L30;
    }
/*     CODE IS PRINTED BELOW */
    l = jp - 1;
    if (l > code_1.codloc - 1) {
	l = code_1.codloc - 1;
    }
    dump_(&i__, &l, &c__16, &c__1);
L30:
    if (jp >= code_1.codloc) {
	goto L40;
    }
/*     THEN THE DATA SEGMENTS */
    if (cntrl_1.contrl[29] == 0) {
	goto L35;
    }
    pad_(&c__0, &c__30, &c__1);
    conout_(&c__1, &c__5, &jn, &c__10);
L35:
    i__1 = jp + jl - 1;
    dump_(&jp, &i__1, &c__16, &c__16);
L40:
    i__ = jp + jl;
    if (i__ < code_1.codloc) {
	goto L15;
    }
L90:
    i__ = code_1.codloc;
    loadin_();
    if (code_1.codloc == i__) {
	goto L100;
    }
/*     DUMP THE INITIALIZED VARIABLES */
    if (cntrl_1.contrl[16] != 0) {
	i__1 = code_1.codloc - 1;
	dump_(&i__, &i__1, &c__16, &c__16);
    }
L100:
    if (cntrl_1.contrl[12] == 0) {
	goto L9999;
    }

/*     PUNCH DECK */
    writel_(&c__0);
    i__ = cntrl_1.contrl[25];
    cntrl_1.contrl[25] = cntrl_1.contrl[12];
    k = memory_1.offset;
    memory_1.offset = 0;
    if (memory_1.preamb > 0) {
	i__1 = memory_1.preamb - 1;
	puncod_(&c__0, &i__1, &c__1);
    }
    memory_1.offset = k;
    j = 2;
    if (memory_1.preamb == 0) {
	j = 3;
    }
    i__1 = memory_1.offset + memory_1.preamb;
    i__2 = code_1.codloc - 1;
    puncod_(&i__1, &i__2, &j);
    pad_(&c__0, &c__1, &c__1);
/*         WRITE A $ */
    pad_(&c__1, &c__38, &c__1);
    writel_(&c__0);
    cntrl_1.contrl[25] = i__;

L9999:
/*     WRITE ERROR COUNT */
    j = cntrl_1.contrl[25];
    k = j;
L10000:
    writel_(&c__0);
    cntrl_1.contrl[25] = j;
    i__ = cntrl_1.contrl[0];
    if (i__ == 0) {
	form_(&c__0, messg_1.mssg, &c__6, &c__7, &c__77);
    }
    if (i__ != 0) {
	conout_(&c__2, &c_n5, &i__, &c__10);
    }
    pad_(&c__1, &c__1, &c__1);
    form_(&c__1, messg_1.mssg, &c__8, &c__20, &c__77);
    if (i__ != 1) {
	pad_(&c__1, &c__30, &c__1);
    }
    pad_(&c__0, &c__1, &c__1);
    writel_(&c__0);
/*     CHECK FOR TERMINAL CONTROL OF A BATCH JOB */
    if (j == 1 || cntrl_1.contrl[30] == 0) {
	goto L10100;
    }
/*     ARRIVE HERE IF TERMINAL TOGGLE GT 0, AND OUTPUT NOT CONSOLE */
    j = 1;
    goto L10000;
L10100:
    s_stop("", (ftnlen)0);
    return 0;
} /* MAIN__ */

/* Subroutine */ int inital_(void)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k;

    factor_1.wfact = factor_1.wdsize / 8;
    memory_1.maxvm = memory_1.maxmem * factor_1.wfact - 1;
    memory_1.memtop = memory_1.maxvm + 1;
    memory_1.membot = -1;

    for (i__ = 1; i__ <= 5; ++i__) {
	factor_1.fact[i__ - 1] = 0;
/* L5: */
    }


    factor_1.fact[factor_1.wfact - 1] = 1;
    j = factor_1.wfact - 1;
    i__1 = j;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k = factor_1.wfact - i__;
	factor_1.fact[k - 1] = factor_1.fact[k] * factor_1.two8;
/* L10: */
    }

    i__1 = memory_1.maxmem;
    for (i__ = 1; i__ <= i__1; ++i__) {
	memory_1.mem[i__ - 1] = 0;
/* L15: */
    }
    return 0;
} /* inital_ */

integer get_(integer *ip)
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
    static integer i__, j, k;
    extern /* Subroutine */ int error_(integer *, integer *);

    i__ = *ip - memory_1.offset;
    j = i__ / factor_1.wfact + 1;
    if (j > memory_1.maxmem) {
	goto L9999;
    }
    j = memory_1.mem[j - 1];
    k = i__ % factor_1.wfact + 1;
    ret_val = j / factor_1.fact[k - 1] % factor_1.two8;
    return ret_val;
L9999:
    ret_val = 0;
    error_(&c__101, &c__5);
    return ret_val;
} /* get_ */

/* Subroutine */ int put_(integer *ip, integer *x)
{
    static integer i__, j, k, m, mh, ifact;
    extern /* Subroutine */ int error_(integer *, integer *);

    i__ = *ip - memory_1.offset;
    j = i__ / factor_1.wfact + 1;
    if (j > memory_1.maxmem) {
	goto L9999;
    }
    m = memory_1.mem[j - 1];
    k = i__ % factor_1.wfact + 1;
    mh = 0;
    if (k == 1) {
	goto L10;
    }
    ifact = factor_1.fact[k - 2];
    mh = m / ifact * ifact;
L10:
    ifact = factor_1.fact[k - 1];
    m %= ifact;
    memory_1.mem[j - 1] = mh + *x * ifact + m;
    return 0;
L9999:
    error_(&c__102, &c__5);
    return 0;
} /* put_ */

integer alloc_(integer *i__)
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
    extern /* Subroutine */ int error_(integer *, integer *);

    if (*i__ < 0) {
	goto L10;
    }
/*     ALLOCATION IS FROM BOTTOM */
    ret_val = memory_1.membot + memory_1.offset + 1;
    memory_1.membot += *i__;
    if (memory_1.membot > memory_1.memtop) {
	error_(&c__103, &c__5);
    }
    return ret_val;

/*     ALLOCATION IS FROM TOP */
L10:
    memory_1.memtop += *i__;
    if (memory_1.memtop <= memory_1.membot) {
	error_(&c__104, &c__5);
    }
    ret_val = memory_1.memtop + memory_1.offset;
    return ret_val;
} /* alloc_ */

integer icon_(integer *i__)
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
    static integer j, k;

/*     ICON IS CALLED WITH AN INTEGER VARIABLE I WHICH CONTAINS A */
/*     CHARACTER READ WITH AN A1 FORMAT.  ICON MUST REDUCE THIS CHARACTER */
/*     TO A VALUE SOMEWHERE BETWEEN 1 AND 256.  NORMALLY, THIS WOULD BE */
/*     ACCOMPLISHED BY SHIFTING THE CHARACTER TO THE RIGHTMOST BIT POSI- */
/*     TIONS OF THE WORD AND MASKING THE RIGHT 8 BITS.  IT IS DONE RATHER */
/*     INEFFICIENTLY HERE, HOWEVER, TO GAIN SOME MACHINE INDEPENDENCE. */
    for (k = 1; k <= 52; ++k) {
	j = k;
	if (*i__ == files_1.otran[k - 1]) {
	    goto L200;
	}
/* L100: */
    }
    j = 1;
L200:
    ret_val = j;
    return ret_val;
} /* icon_ */

integer gnc_(integer *q)
{
    /* Format strings */
    static char fmt_1000[] = "(80a1)";

    /* System generated locals */
    integer ret_val, i__1;

    /* Builtin functions */
    integer s_rsfe(cilist *), do_fio(integer *, char *, ftnlen), e_rsfe(void);

    /* Local variables */
    static integer i__, j, k, l, ii, lp;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer icon_(integer *);
    extern /* Subroutine */ int form_(integer *, integer *, integer *, 
	    integer *, integer *);
    static integer ifile;
    extern /* Subroutine */ int error_(integer *, integer *), writel_(integer 
	    *), conout_(integer *, integer *, integer *, integer *);

    /* Fortran I/O blocks */
    static cilist io___26 = { 0, 0, 0, fmt_1000, 0 };


/*     GET NEXT CHARACTER FROM THE INPUT STREAM (OR 0 IF */
/*     NO CHARACTER IS FOUND) */

    if (files_1.ibp <= cntrl_1.contrl[28]) {
	goto L200;
    }
/*     READ ANOTHER RECORD FROM COMMAND STREAM */
    if (cntrl_1.contrl[30] == 0) {
	goto L1;
    }
    if (cntrl_1.contrl[19] != 1) {
	goto L1;
    }
/*     INPUT IS FROM TERMINAL, SO GET RID OF LAST LINE */
    pad_(&c__0, &c__1, &c__1);
    writel_(&c__0);
L1:
    ifile = cntrl_1.contrl[19];
    if (cntrl_1.contrl[15] == 1) {
	goto L999;
    }
/* L10: */
    io___26.ciunit = ifile;
    s_rsfe(&io___26);
    do_fio(&c__80, (char *)&files_1.ibuff[0], (ftnlen)sizeof(integer));
    e_rsfe();
/* L100: */
    for (i__ = 1; i__ <= 80; ++i__) {
	j = files_1.ibuff[i__ - 1];
	j = icon_(&j);
	files_1.ibuff[i__ - 1] = files_1.itran[j - 1];
/* L110: */
    }

    lp = cntrl_1.contrl[22];
    if (files_1.ibuff[lp - 1] == 38) {
	goto L300;
    }
L115:
    files_1.ibp = lp;
    if (cntrl_1.contrl[26] == 0) {
	goto L200;
    }
    if (cntrl_1.contrl[22] == 1) {
	goto L120;
    }
    i__1 = cntrl_1.contrl[22] - 1;
    form_(&c__1, files_1.ibuff, &c__1, &i__1, &c__80);
    pad_(&c__1, &c__1, &c__3);
L120:
    form_(&c__1, files_1.ibuff, &cntrl_1.contrl[22], &cntrl_1.contrl[28], &
	    c__80);
    if (cntrl_1.contrl[28] == 80) {
	goto L130;
    }
    pad_(&c__1, &c__1, &c__3);
    i__1 = cntrl_1.contrl[28] + 1;
    form_(&c__1, files_1.ibuff, &i__1, &c__80, &c__80);
L130:
L200:
    ret_val = files_1.ibuff[files_1.ibp - 1];
    ++files_1.ibp;
    return ret_val;
L300:
    if (files_1.ibuff[1] == 1) {
	goto L115;
    }
/*     SCANNER PARAMETERS FOLLOW */
    ++lp;
L305:
    j = files_1.ibuff[lp - 1];
    if (j == 38) {
	goto L400;
    }
    ++lp;

    for (i__ = lp; i__ <= 80; ++i__) {
	ii = i__;
	if (files_1.ibuff[i__ - 1] == 39) {
	    goto L330;
	}
	if (files_1.ibuff[i__ - 1] == 38) {
	    goto L315;
	}
/* L310: */
    }

L315:
    k = cntrl_1.contrl[j - 1];
    if (k > 1) {
	goto L320;
    }
    cntrl_1.contrl[j - 1] = 1 - k;
    goto L325;
L320:
    error_(&c__105, &c__1);
L325:
    if (ii == 80) {
	goto L1;
    }
    lp = ii + 1;
    goto L305;
L330:
    k = 0;
    ++ii;

    for (i__ = ii; i__ <= 80; ++i__) {
	l = files_1.ibuff[i__ - 1];
	if (l <= 1) {
	    goto L340;
	}
	if (l > 11) {
	    goto L350;
	}
	k = k * 10 + (l - 2);
L340:
	;
    }

L350:
    cntrl_1.contrl[j - 1] = k;
/*     MAY BE MORE $ IN INPUT LINE */
L360:
    ii = lp + 1;
    for (i__ = ii; i__ <= 80; ++i__) {
	lp = i__;
	if (files_1.ibuff[i__ - 1] == 38) {
	    goto L380;
	}
/* L370: */
    }
/*     NO MORE $ FOUND */
    goto L1;
L380:
    ++lp;
    goto L305;
L400:
/*     DISPLAY $ PARAMETERS */
    l = 2;
    k = 64;
    ++lp;
    j = files_1.ibuff[lp - 1];
    if (j == 1) {
	goto L410;
    }
    l = j;
    k = j;
L410:
    i__1 = k;
    for (i__ = l; i__ <= i__1; ++i__) {
	j = cntrl_1.contrl[i__ - 1];
	if (j < 0) {
	    goto L420;
	}
	pad_(&c__0, &c__38, &c__1);
	pad_(&c__1, &i__, &c__1);
	pad_(&c__1, &c__39, &c__1);
	conout_(&c__2, &c_n10, &j, &c__10);
L420:
	;
    }
    if (cntrl_1.contrl[30] != 0) {
	pad_(&c__0, &c__1, &c__1);
    }
    writel_(&c__0);
    goto L360;
L999:
    ret_val = 0;
    return ret_val;
} /* gnc_ */

integer imin_(integer *i__, integer *j)
{
    /* System generated locals */
    integer ret_val;

    if (*i__ < *j) {
	goto L10;
    }
    ret_val = *j;
    goto L20;
L10:
    ret_val = *i__;
L20:
    return ret_val;
} /* imin_ */

/* Subroutine */ int form_(integer *cc, integer *chars, integer *start, 
	integer *finish, integer *length)
{
    static integer i__, j;
    extern /* Subroutine */ int writel_(integer *);

/*     CC = 0 DUMP BUFFER, GO TO NEXT LINE */
/*     CC = 1 APPEND TO CURRENT BUFFER */
/*     CC = 2 DELETE LEADING BLANKS AND APPEND */
    /* Parameter adjustments */
    --chars;

    /* Function Body */
    j = *start;
    i__ = *cc + 1;
    switch (i__) {
	case 1:  goto L100;
	case 2:  goto L200;
	case 3:  goto L300;
    }
L100:
    writel_(&c__0);
L200:
    if (j > *finish) {
	goto L999;
    }
    ++files_1.obp;
    files_1.obuff[files_1.obp - 1] = chars[j];
    ++j;
    if (files_1.obp >= cntrl_1.contrl[33]) {
	goto L100;
    }
    goto L200;
L300:
    if (j > *finish) {
	goto L999;
    }
    if (chars[j] != 1) {
	goto L200;
    }
    ++j;
    goto L300;
L999:
    return 0;
} /* form_ */

/* Subroutine */ int writel_(integer *nspac)
{
    /* Format strings */
    static char fmt_1000[] = "(\002 \002,121a1)";

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);

    /* Local variables */
    static integer i__, j, np;
    extern integer imin_(integer *, integer *);
    static integer ofile, nblank, nspace;

    /* Fortran I/O blocks */
    static cilist io___41 = { 0, 0, 0, fmt_1000, 0 };


    nspace = *nspac;

    np = cntrl_1.contrl[35] - 1;
    if (files_1.obp <= np) {
	goto L998;
    }
    nblank = 1;

    i__1 = files_1.obp;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = files_1.obuff[i__ - 1];
	if (j != 1) {
	    nblank = i__;
	}
/* L5: */
	files_1.obuff[i__ - 1] = files_1.otran[j - 1];
    }

    files_1.obp = imin_(&cntrl_1.contrl[14], &nblank);
    ofile = cntrl_1.contrl[25] + 10;
L9:
/* L10: */
    io___41.ciunit = ofile;
    s_wsfe(&io___41);
    i__1 = files_1.obp;
    for (i__ = 1; i__ <= i__1; ++i__) {
	do_fio(&c__1, (char *)&files_1.obuff[i__ - 1], (ftnlen)sizeof(integer)
		);
    }
    e_wsfe();
/* L11: */
    if (nspace <= 0) {
	goto L998;
    }

    i__1 = files_1.obp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L12: */
	files_1.obuff[i__ - 1] = files_1.otran[0];
    }
    --nspace;
    goto L9;
L998:
    if (np <= 0) {
	goto L997;
    }
    i__1 = np;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L999: */
	files_1.obuff[i__ - 1] = 1;
    }
L997:
    files_1.obp = np;
    return 0;
/* L1001: */
} /* writel_ */

/* Subroutine */ int conout_(integer *cc, integer *k, integer *n, integer *
	base)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, t[20], ip, kp, np;
    extern integer imin_(integer *, integer *);
    extern /* Subroutine */ int form_(integer *, integer *, integer *, 
	    integer *, integer *);
    static logical zsup;
    static integer ltemp;

    np = *n;
    zsup = *k < 0;
    i__1 = abs(*k);
    kp = imin_(&i__1, &c__19);

    i__1 = kp;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L10: */
	t[i__ - 1] = 1;
    }

    ip = kp + 1;

    i__1 = kp;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ltemp = ip - i__;
	t[ltemp - 1] = np % *base + 2;
	np /= *base;
	if (zsup && np == 0) {
	    goto L30;
	}
/* L20: */
    }

L30:
    if (*base == 8) {
	goto L40;
    }
    if (*base == 2) {
	goto L45;
    }
    if (*base != 16) {
	goto L50;
    }
    ++kp;
    t[kp - 1] = 19;
    goto L50;
L40:
    ++kp;
    t[kp - 1] = 28;
    goto L50;
L45:
    ++kp;
    t[kp - 1] = 13;
L50:
    form_(cc, t, &c__1, &kp, &c__20);
    return 0;
} /* conout_ */

/* Subroutine */ int pad_(integer *cc, integer *chr, integer *i__)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer j, k, t[20];
    extern integer imin_(integer *, integer *);
    extern /* Subroutine */ int form_(integer *, integer *, integer *, 
	    integer *, integer *);

    j = imin_(i__, &c__20);

    i__1 = j;
    for (k = 1; k <= i__1; ++k) {
/* L10: */
	t[k - 1] = *chr;
    }

    form_(cc, t, &c__1, &j, &c__20);
    return 0;
} /* pad_ */

/* Subroutine */ int error_(integer *i__, integer *level)
{
    extern /* Subroutine */ int pad_(integer *, integer *, integer *), form_(
	    integer *, integer *, integer *, integer *, integer *), writel_(
	    integer *), conout_(integer *, integer *, integer *, integer *);

/*     PRINT ERROR MESSAGE - LEVEL IS SEVERITY CODE (TERMINATE AT 5) */
    ++cntrl_1.contrl[0];
    pad_(&c__0, &c__42, &c__1);
    conout_(&c__1, &c__5, &cntrl_1.contrl[13], &c__10);
    pad_(&c__1, &c__43, &c__1);
    pad_(&c__1, &c__1, &c__2);
    form_(&c__1, messg_1.mssg, &c__16, &c__20, &c__77);
    pad_(&c__1, &c__1, &c__1);
    conout_(&c__2, &c_n4, i__, &c__10);
    writel_(&c__0);
/*     CHECK FOR SEVERE ERROR - LEVEL GREATER THAN 4 */
    if (*level <= 4) {
	goto L999;
    }
/*         TERMINATE COMPILATION */
    form_(&c__0, terrr_1.terr, &c__1, &c__22, &c__22);
    writel_(&c__0);
    terrr_1.errflg = TRUE_;
L999:
    return 0;
} /* error_ */

integer shr_(integer *i__, integer *j)
{
    /* System generated locals */
    integer ret_val;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    ret_val = *i__ / pow_ii(&c__2, j);
    return ret_val;
} /* shr_ */

integer shl_(integer *i__, integer *j)
{
    /* System generated locals */
    integer ret_val;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    ret_val = *i__ * pow_ii(&c__2, j);
    return ret_val;
} /* shl_ */

integer right_(integer *i__, integer *j)
{
    /* System generated locals */
    integer ret_val;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    ret_val = *i__ % pow_ii(&c__2, j);
    return ret_val;
} /* right_ */

/* Subroutine */ int delete_(integer *n)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, i1, i2, jp;
    extern /* Subroutine */ int error_(integer *, integer *);

/*     DELETE THE TOP N ELEMENTS FROM THE STACK */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (regall_1.sp > 0) {
	    goto L50;
	}
	error_(&c__106, &c__1);
	goto L9999;
L50:
	i1 = regall_1.rasn[regall_1.sp - 1];
	i1 %= 256;
	i2 = i1 % 16;
	i1 /= 16;
	jp = regall_1.regs[0];
	if (i1 == 0) {
	    goto L100;
	}
	if (jp == i1) {
	    regall_1.regs[0] = 0;
	}
	regall_1.lock[i1 - 1] = 0;
	regall_1.regs[i1 - 1] = 0;
L100:
	if (i2 == 0) {
	    goto L200;
	}
	if (jp == i2) {
	    regall_1.regs[0] = 0;
	}
	regall_1.lock[i2 - 1] = 0;
	regall_1.regs[i2 - 1] = 0;
L200:
	--regall_1.sp;
    }
L9999:
    return 0;
} /* delete_ */

/* Subroutine */ int apply_(integer *op, integer *op2, integer *com, integer *
	cyflag)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, ia, ib, ip, jp, lp;
    extern /* Subroutine */ int exch_(void), emit_(integer *, integer *, 
	    integer *), loadv_(integer *, integer *), error_(integer *, 
	    integer *), delete_(integer *), genreg_(integer *, integer *, 
	    integer *), cvcond_(integer *), ustack_(void);

/*     APPLY OP TO TOP ELEMENTS OF STACK */
/*     USE OP2 FOR HIGH ORDER BYTES IF DOUBLE BYTE OPERATION */
/*     COM = 1 IF COMMUTATIVE OPERATOR, 0 OTHERWISE */
/*     CYFLAG = 1 IF THE CARRY IS INVOLVED IN THE OPERATION */

/*     MAY WANT TO CLEAR THE CARRY FOR THIS OPERATION */

/*     CHECK FOR ONE OF THE OPERANDS IN THE STACK (ONLY ONE CAN BE THERE) */

    i__ = regall_1.sp - 1;
    ip = 0;
    i__1 = regall_1.sp;
    for (j = i__; j <= i__1; ++j) {
	if (regall_1.st[j - 1] != 0 || regall_1.rasn[j - 1] != 0 || 
		regall_1.litv[j - 1] >= 0) {
	    goto L90;
	}

/*         OPERAND IS STACKED */
	genreg_(&c_n2, &ia, &ib);
	regall_1.regs[ia - 1] = j;
	if (ip != 0) {
	    error_(&c__152, &c__1);
	}
	ip = ib;
	if (regall_1.prec[j - 1] > 1) {
	    goto L80;
	}

/*     SINGLE PRECISION RESULT */
	ib = 0;
	goto L85;


/*         DOUBLE BYTE OPERAND */
L80:
	regall_1.regs[ib - 1] = j;

L85:
	regall_1.rasn[j - 1] = (ib << 4) + ia;
	emit_(&ops_1.pop, &ip, &c__0);
	ustack_();
L90:
	;
    }

/*     MAKE A QUICK CHECK FOR POSSIBLE ACCUMULATOR MATCH */
/*     WITH THE SECOND OPERAND */
    ia = regall_1.rasn[regall_1.sp - 1];
    if (ia > 255) {
	cvcond_(&regall_1.sp);
    }
    ib = regall_1.rasn[regall_1.sp - 2];
    if (ib > 255) {
	i__1 = regall_1.sp - 1;
	cvcond_(&i__1);
    }
    l = regall_1.regs[0];
    if (ia * ib * l * *com == 0) {
	goto L100;
    }
/*     COMMUTATIVE OPERATOR, ONE MAY BE IN THE ACCUMULATOR */
    if (l != ia % 16) {
	goto L100;
    }
/*     SECOND OPERAND IN GPR'S, L.O. BYTE IN ACCUMULATOR */
    exch_();

L100:
    ia = 0;
    ib = 0;
/*     IS OP1 IN GPR'S */

    l = regall_1.rasn[regall_1.sp - 2];
    if (l == 0) {
	goto L140;
    }
/*     REG ASSIGNED, LOCK REGS CONTAINING VAR */
    i__ = l % 16;
    if (i__ == 0) {
	goto L9990;
    }
    ia = i__;
    regall_1.lock[i__ - 1] = 1;
    i__ = l / 16;
    if (i__ == 0) {
	goto L110;
    }
    ib = i__;
    regall_1.lock[i__ - 1] = 1;

/*     MAY HAVE TO GENERATE ONE FREE REG */
L110:
    if (regall_1.prec[regall_1.sp - 2] >= regall_1.prec[regall_1.sp - 1]) {
	goto L120;
    }
    ib = ia - 1;

/*     FORCE LOW-ORDER BYTE INTO ACCUMULATOR */
L120:
/*     CHECK FOR PENDING REGISTER STORE */
    jp = regall_1.regs[0];
    if (jp == ia) {
	goto L200;
    }
    if (jp != 0) {
	emit_(&ops_1.ld, &jp, &ops_1.ra);
    }
    regall_1.regs[0] = ia;
    emit_(&ops_1.ld, &ops_1.ra, &ia);
    goto L200;

/*     IS OP2 IN GPR'S */
L140:
    l = regall_1.rasn[regall_1.sp - 1];
    if (l == 0) {
	goto L200;
    }
/*     YES - CAN WE EXCHANGE AND TRY AGAIN */
/*     AFTER INSURING THAT A LITERAL HAS NO REGS ASSIGNED */
    regall_1.litv[regall_1.sp - 1] = -1;
    if (*com == 0) {
	goto L200;
    }
L150:
    exch_();
    goto L100;

/*     OP2 NOT IN GPR'S OR OP IS NOT COMMUTATIVE */
/*     CHECK FOR LITERAL VALUE - IS OP2 LITERAL */
L200:
    k = regall_1.litv[regall_1.sp - 1];
    if (k < 0) {
	goto L280;
    }

    if (regall_1.prec[regall_1.sp - 1] > 1 || regall_1.prec[regall_1.sp - 2] 
	    > 1) {
	goto L300;
    }
/*     MAKE SPECIAL CHECK FOR POSSIBLE INCREMENT OR DECREMENT */
    if (k != 1) {
	goto L300;
    }
/*         MUST BE ADD OR SUBTRACT WITHOUT CARRY */
    if (*op != ops_1.ad && *op != ops_1.su) {
	goto L300;
    }
/*         FIRST OPERAND MUST BE SINGLE BYTE VARIABLE */
    if (regall_1.prec[regall_1.sp - 2] != 1) {
	goto L300;
    }
    if (ia > 1) {
	goto L230;
    }
/*         OP1 MUST BE IN MEMORY, SO LOAD INTO GPR */
    i__1 = regall_1.sp - 1;
    loadv_(&i__1, &c__0);
    l = regall_1.rasn[regall_1.sp - 2];
    ia = l % 16;
    if (ia == 0) {
	goto L9990;
    }
/*             ...MAY CHANGE TO INR MEMORY IF STD TO OP1 FOLLOWS... */
    peep_1.lastir = code_1.codloc;
L230:
    jp = ia;
    if (regall_1.regs[ops_1.ra - 1] == ia) {
	jp = ops_1.ra;
    }
    if (*op == ops_1.ad) {
	emit_(&ops_1.in, &jp, &c__0);
    }
    if (*op == ops_1.su) {
	emit_(&ops_1.dc, &jp, &c__0);
    }
    goto L2000;

/*     OP1 NOT A LITERAL,  CHECK FOR LITERAL OP2 */
L280:
    if (regall_1.litv[regall_1.sp - 2] < 0) {
	goto L300;
    }
    if (*com == 1) {
	goto L150;
    }

/*     GENERATE REGISTERS TO HOLD RESULTS IN LOADV */
/*     (LOADV WILL LOAD THE LOW ORDER BYTE INTO THE ACC) */
L300:
    i__1 = regall_1.sp - 1;
    loadv_(&i__1, &c__1);
    l = regall_1.rasn[regall_1.sp - 2];
    ia = l % 16;
    if (ia == 0) {
	goto L9990;
    }
    regall_1.lock[ia - 1] = 1;
    ib = l / 16;

/*     IS THIS A SINGLE BYTE / DOUBLE BYTE OPERATION */
    if (ib > 0 || regall_1.prec[regall_1.sp - 1] == 1) {
	goto L400;
    }
/*     GET A SPARE REGISTER */
    ib = ia - 1;
    if (ib == 0) {
	goto L9990;
    }
    regall_1.lock[ib - 1] = 1;

/*     NOW READY TO PERFORM OPERATION */
/*     L.O. BYTE IS IN AC, H.O. BYTE IS IN IB. */
/*     RESULT GOES TO IA (L.O.) AND IB (H.O.) */

/*     IS OP2 IN GPR'S */
L400:
    lp = regall_1.rasn[regall_1.sp - 1];
    k = -1;
    if (lp <= 0) {
	goto L500;
    }

/*         PERFORM ACC-REG OPERATION */
    i__1 = lp % 16;
    emit_(op, &i__1, &c__0);
    goto L700;

/*     IS OP2 A LITERAL */
L500:
    k = regall_1.litv[regall_1.sp - 1];
    if (k < 0) {
	goto L600;
    }

/*         USE CMA IF OP IS XR AND OP2 IS LIT 255 */
    if (*op != ops_1.xr || k % 256 != 255) {
	goto L550;
    }
    emit_(&ops_1.cma, &c__0, &c__0);
    goto L700;
L550:

/*         PERFORM ACC-IMMEDIATE OPERATION */
    i__1 = -(k % 256);
    emit_(op, &i__1, &c__0);
    goto L700;

/*     OP2 IS IN MEMORY - SETUP ADDRESS */
L600:
    loadv_(&regall_1.sp, &c__2);
/*     PERFORM OPERATION WITH LOW ORDER BYTE */
    emit_(op, &ops_1.me, &c__0);

/*     NOW PROCESS HIGH ORDER BYTE */
L700:
/*     SET UP A PENDING REGISTER STORE */
/*     IF THIS IS NOT A COMPARE */
    if (*op != ops_1.cp) {
	regall_1.regs[0] = ia;
    }
    if (regall_1.prec[regall_1.sp - 1] == 2) {
	goto L3000;
    }
/*     SECOND OPERAND IS SINGLE BYTE */
    if (regall_1.prec[regall_1.sp - 2] < 2) {
	goto L2000;
    }

/*     MAY NOT NEED TO PERFORM OPERATIONS FOR CERTAIN OPERATORS, BUT ... */
/*     PERFORM OPERATION WITH H.O. BYTE OF OP1 */
/*     OP1 MUST BE IN THE GPR'S - PERFORM DUMMY OPERATION WITH ZERO */
    jp = regall_1.regs[0];
    if (jp == 0) {
	goto L800;
    }
    if (jp == ib) {
	goto L850;
    }
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    regall_1.regs[0] = 0;
L800:
    emit_(&ops_1.ld, &ops_1.ra, &ib);
L850:
    emit_(op2, &c__0, &c__0);

/*     MOVE ACCUMULATOR TO GPR */
L1000:
/*     SET UP PENDING REGISTER STORE */
    regall_1.regs[0] = ib;

/*     FIX STACK POINTERS AND VALUES */
L2000:
/*     SAVE THE PENDING ACCUMULATOR - REGISTER STORE */
    jp = regall_1.regs[0];
    delete_(&c__2);
    regall_1.regs[0] = jp;
    ++regall_1.sp;
    regall_1.prec[regall_1.sp - 1] = 1;
    regall_1.rasn[regall_1.sp - 1] = (ib << 4) + ia;
    regall_1.lock[ia - 1] = 0;
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.litv[regall_1.sp - 1] = -1;
    regall_1.regs[ia - 1] = regall_1.sp;
    regall_1.regv[ia - 1] = -1;
    if (ib <= 0) {
	goto L9999;
    }
    regall_1.prec[regall_1.sp - 1] = 2;
    regall_1.regs[ib - 1] = regall_1.sp;
    regall_1.lock[ib - 1] = 0;
    regall_1.regv[ib - 1] = -1;
    goto L9999;

/*     PREC OF OP2 = 2 */
L3000:
/*     IS H.O. BYTE OF OP2 IN MEMORY */
    if (k >= 0 || lp > 0) {
	goto L3100;
    }
/*     POINT TO H.O. BYTE WITH H AND L */
    emit_(&ops_1.in, &ops_1.rl, &c__0);
    ++regall_1.regv[6];

/*     DO WE NEED TO PAD WITH H.O. ZERO FOR OP1 */
L3100:
    if (regall_1.prec[regall_1.sp - 2] > 1) {
	goto L3200;
    }
/*     IS STORE PENDING */
    jp = regall_1.regs[0];
    if (jp == 0) {
	goto L3150;
    }
    if (jp == ib) {
	goto L3250;
    }
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    regall_1.regs[0] = 0;
L3150:
    if (*cyflag == 0) {
	emit_(&ops_1.xr, &ops_1.ra, &c__0);
    }
    if (*cyflag == 1) {
	emit_(&ops_1.ld, &ops_1.ra, &c__0);
    }
    goto L3250;

/*     IS H.O. BYTE OF OP2 IN GPR */
L3200:
/*     IS STORE PENDING */
    jp = regall_1.regs[0];
    if (jp == 0) {
	goto L3220;
    }
    if (jp == ib) {
	goto L3250;
    }
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    regall_1.regs[0] = 0;
L3220:
    emit_(&ops_1.ld, &ops_1.ra, &ib);
L3250:
    if (lp == 0) {
	goto L3300;
    }

/*     OP2 IN GPR'S - PERFORM ACC-REGISTER OPERATION */
    i__1 = lp / 16;
    emit_(op2, &i__1, &c__0);
    goto L1000;

/*     OP2 IS NOT IN GPR'S - IS IT A LITERAL */
L3300:
    if (k < 0) {
	goto L3400;
    }
/*     YES - PERFORM ACC-IMMEDIATE OPERATION */
/*     USE CMA IF OP1 IS XR AND OP2 IS 65535 */
    if (*op2 != ops_1.xr || k != 65535) {
	goto L3350;
    }
    emit_(&ops_1.cma, &c__0, &c__0);
    goto L1000;
L3350:
    i__1 = -(k / 256);
    emit_(op2, &i__1, &c__0);
    goto L1000;

/*     PERFORM ACC-MEMORY OPERATION */
L3400:
    emit_(op2, &ops_1.me, &c__0);
    goto L1000;

L9990:
    error_(&c__107, &c__5);
L9999:
    return 0;
} /* apply_ */

/* Subroutine */ int genreg_(integer *np, integer *ia, integer *ib)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, n, ip, jp;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *), 
	    stack_(integer *);
    static integer idump;
    extern /* Subroutine */ int saver_(void);

/*     GENERATE N FREE REGISTERS FOR SUBSEQUENT OPERATION */
    n = abs(*np);
/*     N IS NUMBER OF REGISTERS, NP NEGATIVE IF NO PUSHING ALLOWED */
/* L10: */
    *ib = 0;
    *ia = 0;
    idump = 0;

/*     LOOK FOR FREE RC OR RE AND ALLOCATE IN PAIRS (RC/RB,RE/RD) */
L100:
    k = ops_1.rc;
    if (regall_1.regs[k - 1] == 0) {
	goto L200;
    }
    k = ops_1.re;
    if (regall_1.regs[k - 1] != 0) {
	goto L9990;
    }
L200:
    *ia = k;
    if (n > 1) {
	*ib = *ia - 1;
    }
    goto L9999;

L9990:
    if (idump > 0) {
	goto L9991;
    }
    if (*np < 0) {
	goto L5000;
    }
    ip = 0;
/*     GENERATE TEMPORARIES IN THE STACK AND RE-TRY */
/*         SEARCH FOR LOWEST REGISTER PAIR ASSIGNMENT IN STACK */
    if (regall_1.sp <= 0) {
	goto L5000;
    }
    i__1 = regall_1.sp;
    for (i__ = 1; i__ <= i__1; ++i__) {
	k = regall_1.rasn[i__ - 1];
	if (k == 0) {
	    goto L3950;
	}
	if (k > 255) {
	    goto L4000;
	}
	j = k % 16;
	if (regall_1.lock[j - 1] != 0) {
	    goto L4000;
	}
	jp = k / 16;
	if (jp == 0) {
	    goto L3900;
	}
/*         OTHERWISE CHECK HO REGISTER */
	if (regall_1.lock[jp - 1] != 0 || jp != j - 1) {
	    goto L4000;
	}
L3900:
	if (ip == 0) {
	    ip = i__;
	}
	goto L4000;
L3950:
	if (regall_1.st[i__ - 1] == 0 && regall_1.litv[i__ - 1] < 0) {
	    ip = 0;
	}
L4000:
	;
    }
    if (ip == 0) {
	goto L5000;
    }
/*     FOUND ENTRY TO PUSH AT IP */
    j = regall_1.rasn[ip - 1];
    jp = j / 16;
    j %= 16;
    regall_1.regs[j - 1] = 0;
    if (jp > 0) {
	regall_1.regs[jp - 1] = 0;
    }
/*     CHECK PENDING REGISTER STORE */
    k = regall_1.regs[0];
    if (k == 0) {
	goto L4500;
    }
    if (k == j) {
	goto L4200;
    }
    if (k != jp) {
	goto L4500;
    }
/*     STORE INTO HO REGISTER */
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    goto L4400;
/*     PENDING STORE TO LO BYTE */
L4200:
    emit_(&ops_1.ld, &j, &ops_1.ra);
L4400:
    regall_1.regs[ops_1.ra - 1] = 0;

/*     FREE THE REGISTER FOR ALLOCATION */

L4500:
    stack_(&c__1);
    i__1 = j - 1;
    emit_(&ops_1.push, &i__1, &c__0);

/*     MARK ELEMENT AS STACKED (ST=0, RASN=0) */
    regall_1.rasn[ip - 1] = 0;
    regall_1.st[ip - 1] = 0;
    regall_1.litv[ip - 1] = -1;
/*         AND THEN TRY AGAIN */
    goto L100;

/*     TRY FOR MEMORY STORE */
L5000:
    idump = 1;
    saver_();
    goto L100;
L9991:
    *ia = 0;
L9999:
    return 0;
} /* genreg_ */

/* Subroutine */ int loadsy_(void)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, m, kp, mp;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer gnc_(integer *), shr_(integer *, integer *);
    static integer sign;
    extern integer right_(integer *, integer *);
    extern /* Subroutine */ int error_(integer *, integer *);
    static integer attrib;
    extern /* Subroutine */ int writel_(integer *), conout_(integer *, 
	    integer *, integer *, integer *);

/*     SAVE THE CURRENT INPUT FILE NUMBER */
    m = cntrl_1.contrl[19];
    cntrl_1.contrl[19] = cntrl_1.contrl[31];
L5:
    i__ = gnc_(&c__0);
    if (i__ == 1) {
	goto L5;
    }
/*     LOOK FOR INITIAL '/' */
    if (i__ != 41) {
	goto L8000;
    }
/*     LOAD THE INTERRUPT VECTOR */

L10:
    i__ = gnc_(&c__0);
    if (i__ == 41) {
	goto L50;
    }
    if (i__ < 2 || i__ > 9) {
	goto L8000;
    }
    --i__;
/*     GET THE PROCEDURE NAME CORRESPONDING TO INTERRUPT I-1 */
    j = 0;
    l = 1;
L20:
    k = gnc_(&c__0);
    if (k == 41) {
	goto L30;
    }
    k += -2;
    if (k < 0 || k > 31) {
	goto L8000;
    }
    j += k * l;
    l <<= 5;
    goto L20;

L30:
    inter_1.intpro[i__ - 1] = j;
    if (cntrl_1.contrl[29] < 2) {
	goto L10;
    }
    pad_(&c__0, &c__1, &c__1);
    pad_(&c__1, &c__20, &c__1);
    i__1 = i__ - 1;
    conout_(&c__1, &c__1, &i__1, &c__10);
    pad_(&c__1, &c__39, &c__1);
    pad_(&c__1, &c__30, &c__1);
    conout_(&c__1, &c__5, &j, &c__10);
    writel_(&c__0);
    goto L10;

/*     INTERRUPT PROCEDURES ARE HANDLED. */
L50:
    i__ = gnc_(&c__0);
    if (i__ == 1) {
	goto L50;
    }

    if (i__ != 41) {
	goto L8000;
    }

/*     PROCESS NEXT SYMBOL TABLE ENTRY */
L100:
    i__ = gnc_(&c__0);
    if (i__ == 41) {
	goto L1000;
    }

    ++symbl_1.sytop;
    if (symbl_1.sytop < symbl_1.syinfo) {
	goto L200;
    }
    error_(&c__108, &c__5);
    symbl_1.syinfo = symbl_1.symax;
L200:
    if (cntrl_1.contrl[29] < 2) {
	goto L250;
    }

/*     WRITE SYMBOL NUMBER AND SYMBOL TABLE ADDRESS */
    pad_(&c__0, &c__1, &c__1);
    pad_(&c__1, &c__30, &c__1);
    conout_(&c__1, &c__5, &symbl_1.sytop, &c__10);
L250:
    symbl_1.symbol[symbl_1.sytop - 1] = symbl_1.syinfo;
    --symbl_1.syinfo;
    attrib = symbl_1.syinfo;

L300:
    sign = 0;
    if (i__ == 1) {
	sign = 1;
    }
    if (i__ == 45) {
	sign = -1;
    }
    if (sign == 0) {
	goto L8000;
    }

    l = 1;
    k = 0;
L400:
    i__ = gnc_(&c__0);
    if (i__ >= 2 && i__ <= 33) {
	goto L600;
    }

/*     END OF NUMBER */
    if (symbl_1.syinfo > symbl_1.sytop) {
	goto L500;
    }
    error_(&c__109, &c__5);
    symbl_1.syinfo = symbl_1.symax;
L500:
    if (cntrl_1.contrl[29] < 2) {
	goto L550;
    }

/*     WRITE SYMBOL TABLE ADDRESS AND ENTRY */
    pad_(&c__0, &c__1, &c__4);
    conout_(&c__1, &c__5, &symbl_1.syinfo, &c__10);
    pad_(&c__1, &c__1, &c__1);
    kp = 1;
    if (sign == -1) {
	kp = 45;
    }
    pad_(&c__1, &kp, &c__1);
    conout_(&c__1, &c__8, &k, &c__16);
L550:
    symbl_1.symbol[symbl_1.syinfo - 1] = sign * k;
    --symbl_1.syinfo;
/*     LOOK FOR '/' */
    if (i__ != 41) {
	goto L300;
    }
/*     CHECK FOR SPECIAL CASE AT END OF AN ENTRY */
    attrib = (i__1 = symbl_1.symbol[attrib - 1], abs(i__1));
    i__ = attrib % 16;
    if (i__ == types_1.proc || i__ == types_1.varb) {
	goto L545;
    }
    if (i__ != types_1.label) {
	goto L100;
    }
/*     CHECK FOR SINGLE REFERENCE TO THE LABEL */
    j = attrib / 256;
    if (j != 1) {
	goto L100;
    }
/*     ALLOCATE A CELL AND SET TO ZERO */
/*     ARRIVE HERE WITH PROC, VARB, OR SINGLE REF LABEL */
L545:
    symbl_1.symbol[symbl_1.syinfo - 1] = 0;
    --symbl_1.syinfo;
    if (i__ != types_1.proc) {
	goto L100;
    }
/*     RESERVE ADDITIONAL CELL FOR STACK DEPTH COUNT */
    i__ = 0;
    goto L545;


/*     GET NEXT DIGIT */
L600:
    k = (i__ - 2) * l + k;
    l <<= 5;
    goto L400;
L1000:
/*     ASSIGN RELATIVE MEMORY ADDRESSES TO VARIABLES IN SYMBOL TABLE */
    i__ = symbl_1.sytop;
/*     65536 = 65280 + 256 */
    symbl_1.lmem = 65280;
L1100:
    if (i__ <= 0) {
	goto L9999;
    }
/*     PROCESS NEXT SYMBOL */
    mp = symbl_1.symbol[i__ - 1];
    l = -1;
    k = symbl_1.symbol[mp - 2];
/*     K CONTAINS ATTRIBUTES OF VARIABLE */
    if (k < 0) {
	goto L1300;
    }
    if (right_(&k, &c__4) != 1) {
	goto L1300;
    }
/*     OTHERWISE TYPE IS VARB */
    k = shr_(&k, &c__4);
    l = right_(&k, &c__4);
    k = shr_(&k, &c__4);
/*         L IS ELEMENT SIZE, K IS NUMBER OF ELEMENTS */
    if (l <= 2) {
	goto L1150;
    }
/*         PROBABLY AN INLINE DATA VARIABLE */
    l = -1;
    goto L1300;
L1150:
    if (symbl_1.lmem % 2 == 1 && l == 2) {
	--symbl_1.lmem;
    }
/*     MEM IS AT THE PROPER BOUNDARY NOW */
    symbl_1.lmem -= l * k;
    if (symbl_1.lmem >= 0) {
	goto L1200;
    }
    error_(&c__110, &c__1);
    symbl_1.lmem = 65280;
L1200:
    l = symbl_1.lmem;
    if (cntrl_1.contrl[29] == 0) {
	goto L1300;
    }
    if (i__ <= 4 || i__ == 6) {
	goto L1300;
    }
/*     WRITE OUT ADDRESS ASSIGNMENT */
    pad_(&c__0, &c__1, &c__1);
    pad_(&c__1, &c__30, &c__1);
    conout_(&c__1, &c__5, &i__, &c__10);
    pad_(&c__1, &c__39, &c__1);
    conout_(&c__1, &c__5, &l, &c__10);
L1300:
    symbl_1.symbol[mp - 1] = l;
    --i__;
    goto L1100;

L8000:
    error_(&c__111, &c__1);
L9999:
/*     NOW ASSIGN THE LAST ADDRESS TO THE VARIABLE 'MEMORY' */
/*     ** NOTE THAT 'MEMORY' MUST BE AT LOCATION 5 IN THE SYMBOL TABLE ** */
    i__ = symbl_1.symbol[4];
    symbl_1.symbol[i__ - 1] = 65280;
    if (cntrl_1.contrl[29] != 0) {
	writel_(&c__0);
    }
    cntrl_1.contrl[19] = m;
    return 0;
} /* loadsy_ */

/* Subroutine */ int loadv_(integer *is, integer *typv)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, m, s, ia, ib, jp, lp, typ;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *);
    extern integer chain_(integer *, integer *);
    extern /* Subroutine */ int error_(integer *, integer *), litadd_(integer 
	    *), delete_(integer *), genreg_(integer *, integer *, integer *), 
	    cvcond_(integer *), setadr_(integer *), ustack_(void);

/*     LOAD VALUE TO REGISTER IF NOT A LITERAL */
/*     TYP = 1 IF CALL FROM 'APPLY' IN WHICH CASE THE L.O. BYTE IS */
/*     LOADED INTO THE ACCUMULATOR INSTEAD OF A GPR. */
/*     IF TYP = 2, THE ADDRESS IS LOADED, BUT THE VARIABLE IS NOT. */
/*     IF TYP = 3, A DOUBLE BYTE (ADDRESS) FETCH IS FORCED. */
/*     IF TYP = 4 THEN DO A QUICK LOAD INTO H AND L */
/*     IF TYP = 5, A DOUBLE BYTE QUICK LOAD INTO H AND L IS FORCED */
    i__ = 0;
    s = *is;
    typ = *typv;
    if (typ == 2) {
	goto L100;
    }

    if (regall_1.rasn[s - 1] > 255) {
	cvcond_(&s);
    }
    if (typ == 4 || typ == 5) {
	goto L3000;
    }
    if (regall_1.rasn[s - 1] > 0) {
	goto L9999;
    }
/*     CHECK FOR PREVIOUSLY STACKED VALUE */
    if (regall_1.st[s - 1] != 0 || regall_1.litv[s - 1] >= 0) {
	goto L40;
    }
    genreg_(&c__2, &k, &i__);
/*     CHECK TO ENSURE THE STACK IS IN GOOD SHAPE */
    i__ = s + 1;
L10:
    if (i__ > regall_1.sp) {
	goto L30;
    }
    if (regall_1.st[i__ - 1] != 0 || regall_1.rasn[i__ - 1] != 0 || 
	    regall_1.litv[i__ - 1] >= 0) {
	goto L20;
    }
/*         FOUND ANOTHER STACKED VALUE */
    error_(&c__147, &c__1);
L20:
    ++i__;
    goto L10;
L30:
/*     AVAILABLE CPU REGISTER IS BASED AT K */
    i__1 = k - 1;
    emit_(&ops_1.pop, &i__1, &c__0);
    regall_1.regs[k - 1] = s;
    if (regall_1.prec[regall_1.sp - 1] < 2) {
	goto L35;
    }
    regall_1.regs[k - 2] = s;
    k = (k - 1 << 4) + k;
L35:
    regall_1.rasn[s - 1] = k;
/*     DECREMENT THE STACK COUNT FOR THIS LEVEL */
    ustack_();
    goto L9999;

L40:
/*     NO REGISTERS ASSIGNED.  ALLOCATE REGISTERS AND LOAD VALUE. */
    i__ = regall_1.prec[s - 1];
    if (typ != 3) {
	goto L50;
    }
/*         FORCE A DOUBLE BYTE LOAD */
    i__ = 2;
    typ = 0;
L50:
    genreg_(&i__, &ia, &ib);
/*     IA IS LOW ORDER BYTE, IB IS HIGH ORDER BYTE. */
    if (ia <= 0) {
	goto L9990;
    }
/*     OTHERWISE REGISTERS HAVE BEEN FOUND. */
L100:
/*     CHECK FOR LITERAL VALUE (IN ARITH EXP) */
    l = regall_1.litv[s - 1];
    if (l >= 0 && l <= 65535) {
	goto L2000;
    }
/*     OTHERWISE FETCH FROM MEMORY */
    ++regall_1.sp;
    j = regall_1.st[s - 1];
    setadr_(&j);
    litadd_(&regall_1.sp);
/*     ADDRESS OF VARIABLE IS IN H AND L */
    jp = typ + 1;
    switch (jp) {
	case 1:  goto L200;
	case 2:  goto L300;
	case 3:  goto L1000;
    }
/*     CALL FROM GENSTO (TYP = 0) */
L200:
    emit_(&ops_1.ld, &ia, &ops_1.me);
    goto L400;
/*     CALL FROM APPLY TO LOAD VALUE OF VARIABLE */
L300:
    jp = regall_1.regs[0];
/*     CHECK FOR PENDING REGISTER STORE */
    if (jp == 0) {
	goto L350;
    }
/*     HAVE TO STORE ACC INTO REGISTER BEFORE RELOADING */
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    regall_1.regs[0] = 0;
L350:
    emit_(&ops_1.ld, &ops_1.ra, &ops_1.me);

/*     CHECK FOR DOUBLE BYTE VARIABLE */
L400:
    if (i__ <= 1) {
	goto L1000;
    }
/*     LOAD HIGH ORDER BYTE */
    emit_(&ops_1.in, &ops_1.rl, &c__0);
    ++regall_1.regv[6];
    emit_(&ops_1.ld, &ib, &ops_1.me);
/*     VALUE IS NOW LOADED */
L1000:
    delete_(&c__1);
    if (typ == 2) {
	goto L9999;
    }
    regall_1.rasn[s - 1] = (ib << 4) + ia;
    if (ib != 0) {
	regall_1.regs[ib - 1] = s;
    }
    regall_1.regs[ia - 1] = s;
    if (ib != 0) {
	regall_1.regv[ib - 1] = -1;
    }
    regall_1.regv[ia - 1] = -1;
    goto L9999;

/*     LOAD A CONSTANT INTO REGISTERS (NON-COM OPERATOR) */
L2000:
    lp = l % 256;
    regall_1.regs[ia - 1] = s;
    regall_1.regv[ia - 1] = lp;
    if (typ == 1) {
	goto L2100;
    }
/*     TYP = 0, LOAD DIRECTLY INTO REGISTERS */
/*     MAY BE POSSIBLE TO LXI */
    if (ib != ia - 1) {
	goto L2010;
    }
    emit_(&ops_1.lxi, &ib, &l);
    goto L2210;
L2010:
    i__1 = -lp;
    emit_(&ops_1.ld, &ia, &i__1);
    goto L2200;

/*     TYP = 1, LOAD INTO ACCUMULATOR */
L2100:
/*     CHECK FOR PENDING REGISTER STORE */
    jp = regall_1.regs[0];
    if (jp == 0) {
	goto L2150;
    }
/*     STORE ACC INTO REGISTER BEFORE CONTINUING */
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    regall_1.regs[0] = 0;
L2150:
    if (lp == 0) {
	emit_(&ops_1.xr, &ops_1.ra, &c__0);
    }
    if (lp != 0) {
	i__1 = -lp;
	emit_(&ops_1.ld, &ops_1.ra, &i__1);
    }

L2200:
    if (ib == 0) {
	goto L2300;
    }
    i__1 = -l / 256;
    emit_(&ops_1.ld, &ib, &i__1);
L2210:
    regall_1.regs[ib - 1] = s;
    regall_1.regv[ib - 1] = -l;

L2300:
    regall_1.rasn[s - 1] = (ib << 4) + ia;
    goto L9999;
/*     QUICK LOAD TO H AND L */
L3000:
    m = regall_1.litv[s - 1];
    i__ = regall_1.rasn[s - 1];
    k = regall_1.st[s - 1];
    if (i__ != 0) {
	goto L3100;
    }
    if (k != 0) {
	goto L3200;
    }
    if (m >= 0) {
	goto L3400;
    }

/*     VALUE STACKED, SO... */
    ustack_();
    emit_(&ops_1.pop, &ops_1.rh, &c__0);
    if (regall_1.prec[s - 1] < 2) {
	emit_(&ops_1.ld, &ops_1.rh, &c__0);
    }
    goto L3160;

/*     REGISTERS ARE ASSIGNED */
L3100:
    j = regall_1.regs[0];
    l = i__ % 16;
    i__ /= 16;
    if (j != 0 && j == i__) {
	i__ = ops_1.ra;
    }
    if (j != 0 && j == l) {
	l = ops_1.ra;
    }
    if (l != ops_1.re || i__ != ops_1.rd) {
	goto L3150;
    }
    emit_(&ops_1.xchg, &c__0, &c__0);
    goto L3160;
/*     NOT IN D AND E, SO USE TWO BYTE MOVE */
L3150:
    emit_(&ops_1.ld, &ops_1.rl, &l);
/*     NOTE THAT THE FOLLOWING MAY BE A LHI 0 */
    emit_(&ops_1.ld, &ops_1.rh, &i__);
L3160:
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
    goto L3300;

/*     VARIABLE , LITERAL  OR ADDRESS REFERENCE */
L3200:
    if (k > 0) {
	goto L3250;
    }
/*         ADR REF - SET H AND L WITH LITADD */
    litadd_(&regall_1.sp);
    goto L3300;

/*     SIMPLE VARIABLE OR LITERAL REF, MAY USE LHLD */
/*         MAY WANT TO CHECK FOR POSSIBLE INX OR DCX, BUT NOW... */
L3250:
    if (m >= 0) {
	goto L3400;
    }
    m = regall_1.regv[ops_1.rh - 1];
    l = regall_1.regv[ops_1.rl - 1];
    if (m == -3 && -l == k) {
	goto L3260;
    }
    if (m == -4 && -l == k) {
	goto L3255;
    }
    i__1 = code_1.codloc + 1;
    j = chain_(&k, &i__1);
    emit_(&ops_1.lhld, &j, &c__0);
    goto L3260;

L3255:
    emit_(&ops_1.dcx, &ops_1.rh, &c__0);
L3260:
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
    if (regall_1.prec[s - 1] > 1 || typ == 5) {
	goto L3270;
    }
/*         THIS IS A SINGLE BYTE VALUE */
    emit_(&ops_1.ld, &ops_1.rh, &c__0);
    goto L3300;

L3270:
    regall_1.regv[ops_1.rh - 1] = -3;
    regall_1.regv[ops_1.rl - 1] = -k;

L3300:
    if (regall_1.rasn[s - 1] == 0) {
	regall_1.rasn[s - 1] = (ops_1.rh << 4) + ops_1.rl;
    }
    goto L9999;

/*     LITERAL VALUE TO H L */
L3400:
    emit_(&ops_1.lxi, &ops_1.rh, &m);
    regall_1.regv[ops_1.rh - 1] = m / 256;
    regall_1.regv[ops_1.rl - 1] = m % 256;
    goto L9999;

L9990:
    error_(&c__112, &c__5);
L9999:
    return 0;
} /* loadv_ */

/* Subroutine */ int setadr_(integer *val)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;
    extern integer shr_(integer *, integer *), right_(integer *, integer *);
    extern /* Subroutine */ int error_(integer *, integer *);

/*     SET TOP OF STACK TO ADDRESS REFERENCE */
    code_1.alter = 1;

    if (regall_1.sp > regall_1.maxsp) {
	goto L9999;
    }
/*         MARK AS ADDRESS REFERENCE */
    regall_1.st[regall_1.sp - 1] = -(*val);
    i__ = symbl_1.symbol[*val - 1];
    j = (i__1 = symbl_1.symbol[i__ - 2], abs(i__1));
    i__1 = shr_(&j, &c__4);
    regall_1.prec[regall_1.sp - 1] = right_(&i__1, &c__4);
    i__ = symbl_1.symbol[i__ - 1];
/*         *J=SHL(1,16)* */
    j = 65536;
    if (i__ >= 0) {
	goto L4100;
    }
    j = 0;
    i__ = -i__;
L4100:
    i__ = right_(&i__, &c__16);
    regall_1.litv[regall_1.sp - 1] = j + i__;
    regall_1.rasn[regall_1.sp - 1] = 0;
    return 0;
L9999:
    error_(&c__113, &c__5);
    regall_1.sp = 1;
    return 0;
} /* setadr_ */

/* Subroutine */ int ustack_(void)
{
    static integer i__;
    extern /* Subroutine */ int error_(integer *, integer *);

/*     DECREMENT CURDEP AND CHECK FOR UNDERFLOW */
    i__ = pstack_1.curdep[pstack_1.prsp];
    if (i__ > 0) {
	goto L100;
    }
    error_(&c__148, &c__1);
    return 0;
L100:
    pstack_1.curdep[pstack_1.prsp] = i__ - 1;
    return 0;
} /* ustack_ */

integer chain_(integer *sy, integer *loc)
{
    /* System generated locals */
    integer ret_val;

    /* Local variables */
    static integer i__, j;

/*     CHAIN IN DOUBLE-BYTE REFS TO SYMBOL SY, IF NECESSARY */
    i__ = symbl_1.symbol[*sy - 1];
    j = symbl_1.symbol[i__ - 1];
    if (j >= 0) {
	goto L100;
    }
/*         ABSOLUTE ADDRESS ALREADY ASSIGNED */
    ret_val = -j % 65536;
    goto L999;
/*     BACKSTUFF REQUIRED */
L100:
    i__ += -2;
    ret_val = symbl_1.symbol[i__ - 1];
    symbl_1.symbol[i__ - 1] = *loc;
L999:
    return ret_val;
} /* chain_ */

/* Subroutine */ int gensto_(integer *keep)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, i1, i2, iq, jp, lp;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *);
    extern integer chain_(integer *, integer *);
    extern /* Subroutine */ int loadv_(integer *, integer *), litadd_(integer 
	    *), delete_(integer *), ustack_(void);

/*     KEEP = 0 IF STD, KEEP = 1 IF STO (VALUE RETAINED) */
/*     GENERATE A STORE INTO THE ADDRESS AT STACK TOP */
/*     LOAD VALUE IF NOT LITERAL */
    l = regall_1.litv[regall_1.sp - 2];
    if (l >= 0) {
	goto L100;
    }
    iq = 0;
    i__1 = regall_1.sp - 1;
    loadv_(&i__1, &iq);
L100:
    i1 = regall_1.rasn[regall_1.sp - 2];
    i2 = i1 % 16;
    i1 /= 16;
/*     CHECK FOR PENDING REGISTER STORE */
    jp = regall_1.regs[0];
    if (jp == 0) {
	goto L150;
    }
    if (jp == i1) {
	i1 = 1;
    }
    if (jp == i2) {
	i2 = 1;
    }
L150:
/*     ** NOTE THAT THIS ASSUMES 'STACKPTR' IS AT 6 IN SYM TAB */
    if (-regall_1.st[regall_1.sp - 1] == 6) {
	goto L700;
    }
    if (regall_1.litv[regall_1.sp - 1] < 0) {
	goto L1000;
    }
/*     OTHERWISE THIS IS A LITERAL ADDRESS */
/*         IF POSSIBLE, GENERATE A SHLD */
    if (i1 != ops_1.rd || i2 != ops_1.re || peep_1.lastex != code_1.codloc - 
	    1 || regall_1.prec[regall_1.sp - 1] != 2) {
	goto L155;
    }
    emit_(&ops_1.xchg, &c__0, &c__0);
    i__ = (i__1 = regall_1.st[regall_1.sp - 1], abs(i__1));
    i__1 = code_1.codloc + 1;
    j = chain_(&i__, &i__1);
    emit_(&ops_1.shld, &j, &c__0);
    regall_1.regv[ops_1.rh - 1] = -3;
    regall_1.regv[ops_1.rl - 1] = -i__;
    if (*keep != 0) {
	emit_(&ops_1.xchg, &c__0, &c__0);
    }
    goto L600;
L155:
    litadd_(&regall_1.sp);
L160:
/*     WE MAY CHANGE MOV R,M INR R MOV M,R TO INR M. */
/*     IF SO, AND THIS IS A NON-DESTRUCTIVE STORE, THE REGISTER */
/*     ASSIGNMENT MUST BE RELEASED. */
    iq = peep_1.lastir;
/*     GENERATE LOW ORDER BYTE STORE */
    if (i2 == 0) {
	goto L200;
    }
    emit_(&ops_1.ld, &ops_1.me, &i2);
    goto L300;
/*     IMMEDIATE STORE */
L200:
    i__1 = -(abs(l) % 256);
    emit_(&ops_1.ld, &ops_1.me, &i__1);
L300:

/*     NOW STORE HIGH ORDER BYTE (IF ANY) */
    if (regall_1.prec[regall_1.sp - 1] == 1) {
	goto L600;
    }
/*     A DOUBLE BYTE STORE */
    i__ = 0;
/*     STORE SECOND BYTE */
    emit_(&ops_1.incx, &ops_1.rh, &c__0);
/*     REGV(RH) = -3 THEN LHLD HAS OCCURRED ON SYMBOL -REGV(RL) */
/*     REGV(RH) = -4 THEN LHLD AND INCX H HAS OCCURRED */
    j = regall_1.regv[ops_1.rh - 1];
    if (j < 0) {
	goto L310;
    }
    ++regall_1.regv[6];
    goto L320;
L310:
    regall_1.regv[ops_1.rh - 1] = -4;
    if (j == -3) {
	goto L320;
    }
/*         RH AND RL HAVE UNKNOWN VALUES */
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
L320:
    if (regall_1.prec[regall_1.sp - 2] < 2) {
	goto L400;
    }
    if (i1 != 0) {
	goto L500;
    }
/*     SECOND BYTE IS LITERAL */
    i__ = l / 256;
/*     ENTER HERE IF LITERAL */
L400:
    i__1 = -abs(i__);
    emit_(&ops_1.ld, &ops_1.me, &i__1);
    goto L600;
/*     LD MEMORY FROM REGISTER */
L500:
    emit_(&ops_1.ld, &ops_1.me, &i1);
L600:

/*     NOW RELEASE REGISTER CONTAINING ADDRESS */
/*     RELEASE REGISTER ASSIGNMENT FOR VALUE */
/*     IF MOV R,M INR R MOV M,R WAS CHANGED TO INR M. */
    if (iq != code_1.codloc) {
	goto L650;
    }
    i__ = -regall_1.st[regall_1.sp - 1];
    delete_(&c__2);
    ++regall_1.sp;
    regall_1.st[regall_1.sp - 1] = i__;
    regall_1.rasn[regall_1.sp - 1] = 0;
    regall_1.prec[regall_1.sp - 1] = 1;
    regall_1.litv[regall_1.sp - 1] = -1;
    goto L9999;
L650:
    delete_(&c__1);
    goto L9999;

/*     STORE INTO STACKPTR */
L700:
    if (i2 == 0) {
	goto L750;
    }
    emit_(&ops_1.ld, &ops_1.rl, &i2);
    regall_1.regv[ops_1.rl - 1] = -1;
    emit_(&ops_1.ld, &ops_1.rh, &i1);
    regall_1.regv[ops_1.rh - 1] = -1;
    emit_(&ops_1.sphl, &c__0, &c__0);
    goto L600;
L750:
/*     LOAD SP IMMEDIATE */
    emit_(&ops_1.lxi, &ops_1.rsp, &l);
    goto L600;

/*     WE HAVE TO LOAD THE ADDRESS BEFORE THE STORE */
L1000:
    i__ = regall_1.rasn[regall_1.sp - 1];
    if (i__ > 0) {
	goto L1100;
    }
/*     REGISTERS NOT ALLOCATED - CHECK FOR STACKED VALUE */
    if (regall_1.st[regall_1.sp - 1] != 0) {
	goto L1010;
    }
/*     ADDRESS IS STACKED SO POP TO H AND L */
    emit_(&ops_1.pop, &ops_1.rh, &c__0);
    ustack_();
    goto L1110;
L1010:
/*     CHECK FOR REF TO SIMPLE BASED VARIABLE */
    i__ = regall_1.st[regall_1.sp - 1];
    if (i__ <= regall_1.intbas) {
	goto L1020;
    }

/*     MAY BE ABLE TO SIMPLIFY (OR ELIMINATE) THE LHLD */
    k = regall_1.regv[ops_1.rh - 1];
    lp = regall_1.regv[ops_1.rl - 1];
    if (k == -3 && -lp == i__) {
	goto L160;
    }
    if (k == -4 && -lp == i__) {
	goto L1012;
    }
    i__1 = code_1.codloc + 1;
    j = chain_(&i__, &i__1);
    emit_(&ops_1.lhld, &j, &c__0);
    regall_1.regv[ops_1.rh - 1] = -3;
    regall_1.regv[ops_1.rl - 1] = -i__;
    goto L160;
L1012:
    emit_(&ops_1.dcx, &ops_1.rh, &c__0);
    regall_1.regv[ops_1.rh - 1] = -3;
    goto L160;
L1020:
    if (i2 != 0) {
	regall_1.lock[i2 - 1] = 1;
    }
    if (i1 != 0) {
	regall_1.lock[i1 - 1] = 1;
    }
/*         FORCE A DOUBLE BYTE FETCH INTO GPRS */
    loadv_(&regall_1.sp, &c__3);
    i__ = regall_1.rasn[regall_1.sp - 1];

L1100:
    jp = regall_1.regs[0];
    j = i__ % 16;
    i__ /= 16;
    if (i2 == 0 || i__ != j - 1) {
	goto L1105;
    }
/*     IF PREVOUS SYLLABLE IS XCHG THEN DO ANOTHER - PEEP WILL FIX IT */
    if (i__ == ops_1.rd && peep_1.lastex == code_1.codloc - 1) {
	goto L1107;
    }
/*         USE STAX - SET UP ACCUMULATOR */

    if (i2 == 1) {
	goto L2215;
    }
    if (jp != 0) {
	emit_(&ops_1.ld, &jp, &ops_1.ra);
    }
    if (i1 == 1) {
	i1 = jp;
    }
    emit_(&ops_1.ld, &ops_1.ra, &i2);
    regall_1.regs[ops_1.ra - 1] = 0;
L2215:
    emit_(&ops_1.stax, &i__, &c__0);
/*     ***** */
/*     IF BYTE DEST WE ARE DONE */
    if (regall_1.prec[regall_1.sp - 1] < 2) {
	goto L1104;
    }
/*     ***** */
    emit_(&ops_1.incx, &i__, &c__0);
    if (i1 != 0) {
	goto L1102;
    }
/*     ***** */
/*     STORE HIGH ORDER ZERO */
    if (i2 != 1 || *keep != 0) {
	goto L1101;
    }
    i__1 = regall_1.rasn[regall_1.sp - 2] % 16;
    emit_(&ops_1.ld, &i__1, &ops_1.ra);
L1101:
    regall_1.regs[ops_1.ra - 1] = 0;
    emit_(&ops_1.xr, &ops_1.ra, &c__0);
    emit_(&ops_1.stax, &i__, &c__0);
    goto L1104;
/*     ***** */
/*     STORE HIGH ORDER BYTE */
L1102:
    if (i2 != 1 || *keep == 0) {
	goto L1103;
    }
    i__1 = regall_1.rasn[regall_1.sp - 2] % 16;
    emit_(&ops_1.ld, &i__1, &ops_1.ra);
    regall_1.regs[ops_1.ra - 1] = 0;
L1103:
    emit_(&ops_1.ld, &ops_1.ra, &i1);
    emit_(&ops_1.stax, &i__, &c__0);
/*     ***** */
L1104:
    delete_(&c__1);
    goto L9999;
/*     ***** */
/*     ADDRESS IN GPRS BUT CANNOT USE STAX */
L1105:
    if (j == jp) {
	j = 1;
    }
    if (i__ == jp) {
	i__ = 1;
    }
    if (i__ == ops_1.rd && j == ops_1.re) {
	goto L1107;
    }
    emit_(&ops_1.ld, &ops_1.rl, &j);
    emit_(&ops_1.ld, &ops_1.rh, &i__);
    goto L1110;
L1107:
    emit_(&ops_1.xchg, &c__0, &c__0);
/*     XCHG MAY BE REMOVED BY PEEPHOLE OPTIMIZATION */
L1110:
    if (i1 != 0) {
	regall_1.lock[i1 - 1] = 0;
    }
    if (i2 != 0) {
	regall_1.lock[i2 - 1] = 0;
    }
    regall_1.regv[5] = -1;
    regall_1.regv[6] = -1;
    goto L160;

L9999:
    return 0;
} /* gensto_ */

/* Subroutine */ int litadd_(integer *s)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, ih, il, ir, jp, kp, lp, it;
    extern integer shl_(integer *, integer *), shr_(integer *, integer *);
    extern /* Subroutine */ int emit_(integer *, integer *, integer *);
    extern integer right_(integer *, integer *);
    extern /* Subroutine */ int error_(integer *, integer *);

/*     LOAD H AND L WITH THE ADDRESS OF THE VARIABLE AT S IN */
/*     THE STACK */
    ih = regall_1.litv[*s - 1];
    il = ih % 256;
    ih /= 256;
    ir = ops_1.rh;
    l = ih;
    if (ih >= 0) {
	goto L10;
    }
    error_(&c__114, &c__1);
    goto L99999;
L10:

/*     DEASSIGN REGISTERS */
    i__ = regall_1.rasn[*s - 1];
    if (i__ == 103) {
	goto L99999;
    }
/*     6*16+7 = 103 */
    jp = regall_1.regs[0];
    for (j = 1; j <= 2; ++j) {
	k = i__ % 16;
	i__ /= 16;
	if (k == 0) {
	    goto L50;
	}
	if (k == jp) {
	    regall_1.regs[0] = 0;
	}
	regall_1.regs[k - 1] = 0;
	regall_1.lock[k - 1] = 0;
	regall_1.regv[k - 1] = -1;
L50:
	;
    }

    regall_1.rasn[*s - 1] = 0;

    for (i__ = 6; i__ <= 7; ++i__) {
	j = regall_1.regs[i__ - 1];
	if (j == 0) {
	    goto L100;
	}
	k = regall_1.rasn[j - 1];
	kp = k % 16;
	k /= 16;
	if (k == i__) {
	    k = 0;
	}
	if (kp == i__) {
	    kp = 0;
	}
	regall_1.rasn[j - 1] = (k << 4) + kp;

L100:
	lp = regall_1.regv[i__ - 1];
	if (lp == l) {
	    goto L700;
	}
	if (lp != l + 1) {
	    goto L200;
	}
	emit_(&ops_1.dc, &ir, &c__0);
	goto L700;
L200:
	if (lp != l - 1) {
	    goto L300;
	}
	if (l == 0) {
	    goto L300;
	}
	emit_(&ops_1.in, &ir, &c__0);
	goto L700;
L300:
	if (i__ != 6) {
	    goto L350;
	}
/*     NO INC/DEC POSSIBLE, SEE IF L DOES NOT MATCH */
	if (il == regall_1.regv[6]) {
	    goto L350;
	}
	regall_1.regv[6] = il;
	if (l > 255) {
	    goto L310;
	}
/*     OTHERWISE THIS IS A REAL ADDRESS */
	i__1 = il + (ih << 8);
	emit_(&ops_1.lxi, &ops_1.rh, &i__1);
	goto L700;
L310:
/*     THE LXI MUST BE BACKSTUFFED LATER */
	it = regall_1.st[*s - 1];
	if (it >= 0) {
	    goto L410;
	}
	it = -it;
	it = symbl_1.symbol[it - 1];
	j = symbl_1.symbol[it - 3];
/*     PLACE REFERENCE INTO CHAIN */
	emit_(&ops_1.lxi, &ops_1.rh, &j);
	symbl_1.symbol[it - 3] = code_1.codloc - 2;
	goto L700;
L350:
	if (l > 255) {
	    goto L400;
	}
	i__1 = -l;
	emit_(&ops_1.ld, &ir, &i__1);
	goto L700;
/*     THE ADDRESS MUST BE BACKSTUFFED LATER */
L400:
	it = regall_1.st[*s - 1];
	if (it < 0) {
	    goto L500;
	}
L410:
	error_(&c__115, &c__1);
	goto L99999;
L500:
	it = abs(it);
	it = symbl_1.symbol[it - 1];
	j = symbl_1.symbol[it - 1];
	if (j > 0) {
	    goto L600;
	}
	error_(&c__116, &c__1);
	goto L99999;
/*     PLACE LINK INTO CODE */
L600:
	k = shr_(&j, &c__16);
	i__1 = code_1.codloc + 1;
	symbl_1.symbol[it - 1] = shl_(&i__1, &c__16) + right_(&j, &c__16);
	kp = k % 256;
	k /= 256;
	emit_(&c__0, &k, &c__0);
	emit_(&c__0, &kp, &c__0);
/*     DONE LOADING ADDRESS ELEMENT */
L700:
/*     FIX VALUES IN STACK AND REG */
	if (i__ == 7) {
	    regall_1.rasn[*s - 1] = 103;
	}
/*     103 = 6*16+7 */
	regall_1.regs[i__ - 1] = *s;
	regall_1.regv[i__ - 1] = l;
	l = il;
	ir = ops_1.rl;
/* L1000: */
    }

L99999:
    return 0;
} /* litadd_ */

/* Subroutine */ int dump_(integer *l, integer *u, integer *fa, integer *fe)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer a, b, i__, j, k, w, fr, lp, ls, rr, wr;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer get_(integer *);
    static logical same;
    static integer nsame, opcnt;
    extern /* Subroutine */ int error_(integer *, integer *);
    extern integer decode_(integer *, integer *, integer *);
    extern /* Subroutine */ int writel_(integer *), conout_(integer *, 
	    integer *, integer *, integer *);

    lp = *l;
    w = cntrl_1.contrl[33];
    a = 5;
    b = 3;
    if (*fa == 8) {
	a = 6;
    }
    if (*fe != 1) {
	goto L10;
    }
/*         SYMBOLIC DUMP */
    b = 6;
    fr = base_1.debase;
    if (fr == 2) {
	fr = 16;
    }
    wr = 2;
    if (fr == 10) {
	wr = 3;
    }
    rr = 6 - wr;
    if (fr != 10) {
	--rr;
    }
/*         FR IS FORMAT OF NUMBERS AFTER OP CODES */
/*         WR IS THE WIDTH OF THE NUMBER FIELD */
/*         RR IS THE NUMBER OF BLANKS AFTER THE NUMBER FIELD */
    goto L20;
L10:
    if (*fe == 2) {
	b = 9;
    }
    if (*fe == 8) {
	b = 4;
    }
L20:
    w = (w - a) / (b + 1);
/*     W IS NUMBER OF ENTRIES ON EACH LINE */
    if (w == 0) {
	goto L8025;
    }
    if (*fa != 10) {
	--a;
    }
    if (*fe != 10) {
	--b;
    }
/*     A IS THE WIDTH OF THE ADDRESS FIELD */
/*     B IS THE WIDTH OF EACH ENTRY */

    for (i__ = 1; i__ <= 29; ++i__) {
/* L100: */
	scanc_1.accum[i__ - 1] = 256;
    }
    nsame = 0;
    opcnt = 0;

L110:
    same = TRUE_;
    ls = lp;
    i__ = 0;

L200:
    if (lp > *u) {
	goto L500;
    }
    ++i__;
    j = get_(&lp);
    ++lp;
    j %= 256;
    if (j != scanc_1.accum[i__ - 1]) {
	same = FALSE_;
    }
    scanc_1.accum[i__ - 1] = j;
    if (i__ < w) {
	goto L200;
    }

L300:
    if (same) {
	goto L400;
    }
    if (i__ == 0) {
	goto L9999;
    }
    conout_(&c__0, &a, &ls, fa);

    i__1 = i__;
    for (j = 1; j <= i__1; ++j) {
	pad_(&c__1, &c__1, &c__1);
	k = scanc_1.accum[j - 1];
	if (opcnt > 0) {
	    goto L315;
	}
	if (*fe != 1) {
	    goto L310;
	}
	opcnt = decode_(&c__1, &k, &c__6);
	goto L320;

L315:
	--opcnt;
	conout_(&c__1, &wr, &k, &fr);
	pad_(&c__1, &c__1, &rr);
	goto L320;
L310:
	conout_(&c__1, &b, &k, fe);
L320:
	;
    }

    if (lp <= *u) {
	goto L110;
    }
    goto L600;

L400:
    ++nsame;
    if (nsame > 1) {
	goto L110;
    }
    pad_(&c__0, &c__1, &c__1);
    writel_(&c__0);
    goto L110;

L500:
    same = FALSE_;
    goto L300;

L600:
    writel_(&c__0);
    goto L9999;
L8025:
    error_(&c__117, &c__1);
L9999:
    return 0;
} /* dump_ */

integer decode_(integer *cc, integer *i__, integer *w)
{
    /* System generated locals */
    integer ret_val, i__1;

    /* Local variables */
    static integer j, k, l, m, x, y, ip;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *), form_(
	    integer *, integer *, integer *, integer *, integer *);
    static integer insize;
    extern /* Subroutine */ int conout_(integer *, integer *, integer *, 
	    integer *);

/*            ***************************************** */
/*            *INSTRUCTION * DECODING * USING * CTRAN * */
/*            ***************************************** */
/*     THE ELEMENTS OF CTRAN REPRESENT THE 8080 OPERATION CODES IN A */
/*     FORM WHICH IS MORE USABLE FOR INSTRUCTION DECODING IN BOTH THE */
/*     DECODE AND INTERP SUBROUTINES.  GIVEN AN INSTRUCTION I (BETWEEN 0 */
/*     AND 255), CTRAN(I+1) PROVIDES AN ALTERNATE REPRESENTATION OF THE */
/*     INSTRUCTION, AS SHOWN BELOW... */
/*         5B    5B     5B       OR     5B     3B  2B    5B */
/*        ------------------          ----------------------- */
/*        /    /     /     /          /      /    /    /    / */
/*        / X  /  Y  /   I /          / X    / Y1 /Y2  /  I / */
/*        /    /     /     /          /      /    /    /    / */
/*        ------------------          ----------------------- */
/*     WHERE FIELD I SPECIFIES A 'CATEGORY' AND THE X AND Y FIELDS */
/*     QUALIFY INSTRUCTIONS WITHIN THE CATEGORY. */
/*    FIELD I       CATEGORY         VALUE OF X AND Y FIELDS */
/*    ------   ----------------- ---------------------------------------- */
/*       0           MOV         THE FIELDS INDICATE THE VALID OPERANDS */
/*                              INVOLVED... */
/*                              ACC=0, B = 1, C = 2, D = 3, E = 4, H = 5, */
/*                               L = 6, M = 7, I = 8, SP= 9 (M IS MEMORY */
/*                              REFERENCING INSTRUCTION, AND I IS IMMED) */
/*                              THUS, /3/5/0/ IS A MOV D,H INSTRUCTION. */

/*       1     INCREMENT, DECRE- THE VALUE OF X DETERMINES THE INSTRUC- */
/*             MENT, ARITHMETIC, TION WITHIN THE CATEGORY.. */
/*             OR LOGICAL        INR = 1, CDR = 2, ADD = 3, ADC = 4, */
/*                               SUB = 5, SBC = 6, ANA = 7, XRA = 8, */
/*                               ORA = 9, CMP = 10 */
/*                               THE VALUE OF Y DETERMINES THE VALID */
/*                               REGISTER INVOLVED, AS ABOVE.  THUS, */
/*                               /3/4/1/ IS AN ADD E INSTRUCTION. */
/*    ------   ----------------- ---------------------------------------- */
/*       2     JUMP, CALL, OR    THE VALUE OF X DETERMINES THE EXACT IN- */
/*                RETURN         STRUCTION.. JUMP=1, CALL=2, RETURN=3 */
/*                               THE SUBFIELD Y1 DETERMINES THE ORIENTA- */
/*                               TION OF THE CONDITION.. T=1, F=0 */
/*                               THE VALUE OF SUBFIELD Y2 GIVES THE CON- */
/*                               DITION.. CY=0, Z=1, S=2, P=3. */
/*                               THUS, /3/0/1/2/ IS AN RFZ (RETURN FALSE */
/*                               ZERO) INSTRUCTION. */
/*    ------   - --------------  ---------------------------------------- */
/*       3     MISCELLANEOUS     THE VALUE OF THE Y FIELD DETERMINES THE */
/*                               INSTRUCTION (THE X FIELD GIVES THE VALUE */
/*                               OF AAA IN THE RST INSTRUCTION) */
/*                               RLC  = 1  RRC  = 2  RAL  = 3  RAR  = 4 */
/*                               JMP  = 5  CALL = 6  RET  = 7  RST  = 8 */
/*                               IN   = 9  OUT  = 10 HLT  = 11 STA  = 12 */
/*                               LDA  = 13 XCHG = 14 XTHL = 15 SPHL = 16 */
/*                               PCHL = 17 CMA  = 18 STC  = 19 CMC  = 20 */
/*                               DAA  = 21 SHLD = 22 LHLD = 23 EI   = 24 */
/*                               DI   = 25 NOP  = 26 27 --- 31 UNDEFINED */
/*                               (IBYTES GIVES NUMBER OF BYTES FOLLOWING */
/*                               THE FIRST 23 INSTRUCTIONS OF THIS GROUP) */
/*    -------   ---------------- --------------------------------------- */
/*     4 - 11   INSTRUCTIONS RE  THE Y FIELD GIVES A REGISTER PAIR NUM- */
/*              QUIRING A REGIS  BER A = 0, B = 1, D = 3, H = 5, SP = 9 */
/*              TER PAIR */
/*                               THE INSTRUCTIONS IN EACH CATEGORY ARE */
/*                               DETERMINED BY THE I FIELD.. */
/*                               LXI  = 4  PUSH = 5  POP  = 6 */
/*                               DAD  = 7  STAX = 8  LDAX = 9 */
/*                               INX  = 10 DCX  = 11 */
/*    -------   ---------------- --------------------------------------- */

    insize = 284;
    ip = inst_1.ctran[*i__];
    x = ip / 1024;
    y = ip / 32 % 32;
    ip = ip % 32 + 1;
    ret_val = 0;
/*      POINT TO THE PROPER CATEGORY */
/*     (THE FIRST TWO ARE FOR CONDITION CODES AND REGISTER DESIGNATIONS) */
    j = inst_1.insym[ip + 1];
/*     SELECT THE PROPER INSTRUCTION CODE WITHIN THE CATEGORY */
    if (ip > 4) {
	goto L500;
    }
    switch (ip) {
	case 1:  goto L100;
	case 2:  goto L200;
	case 3:  goto L300;
	case 4:  goto L400;
    }
/*     MOV */
L100:
    k = 1;
    goto L210;
/*     INR ... CMP */
L200:
    k = x;
/*     MAY BE AN IMMEDIATE OPERATION */
L210:
    if (y == 8) {
	ret_val = 1;
    }
    goto L1000;
/*     JUMP CALL OR RETURN CONDITIONALLY */
L300:
    k = x;
    if (x != 3) {
	ret_val = 2;
    }
    goto L1000;
/*     RLC ... NOP */
L400:
    k = y;
/*     CHECK FOR JMP */
    if (y > 23) {
	goto L1000;
    }
/*     RLC ... LDA */
    ret_val = inst_1.ibytes[y - 1];
    goto L1000;
/*     LXI ... DCX */
L500:
    k = 1;
    if (ip == 5) {
	ret_val = 2;
    }
L1000:
    j += k;
    l = inst_1.insym[j - 1];
    j = inst_1.insym[j];
    i__1 = j - 1;
    form_(cc, inst_1.insym, &l, &i__1, &insize);
    l = j - l;

    if (ip != 4) {
	goto L1050;
    }
/*     CHECK FOR RST (IF FOUND ADD DECIMAL NUMBER) */
    if (y != 8) {
	goto L1100;
    }
/*     FOUND RST INSTRUCTION */
    pad_(&c__1, &c__1, &c__1);
    conout_(&c__1, &c__1, &x, &c__10);
    l += 2;
L1050:
    if (ip != 3) {
	goto L1100;
    }
/*     CONDITIONAL */
    j = inst_1.insym[1] + 1 + y;
    k = inst_1.insym[j - 1];
    j = inst_1.insym[j];
    i__1 = j - 1;
    form_(&c__1, inst_1.insym, &k, &i__1, &insize);
    l = l + j - k;
L1100:
/*     OPCODE IS WRITTEN.  L CHARACTERS ARE IN BUFFER, CHECK FOR MORE */
    if (ip <= 4 && ip >= 3) {
	goto L1200;
    }
/*     WRITE REGISTER REFERENCE */
    pad_(&c__1, &c__1, &c__1);
L1110:
    m = y;
    if (ip == 1) {
	m = x;
    }
    j = inst_1.insym[0] + 1 + m;
    k = inst_1.insym[j - 1];
    j = inst_1.insym[j];
    i__1 = j - 1;
    form_(&c__1, inst_1.insym, &k, &i__1, &insize);
    l = l + j - k + 1;
    if (ip != 1) {
	goto L1200;
    }
    ip = 0;
    goto L1110;
L1200:
    if (l >= *w) {
	goto L1300;
    }
    i__1 = *w - l;
    pad_(&c__1, &c__1, &i__1);
L1300:
    return ret_val;
} /* decode_ */

/* Subroutine */ int emit_(integer *opr, integer *opa, integer *opb)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, j, n;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer get_(integer *);
    extern /* Subroutine */ int put_(integer *, integer *);
    static integer bits[3];
    extern integer alloc_(integer *), right_(integer *, integer *);
    extern /* Subroutine */ int writel_(integer *), conout_(integer *, 
	    integer *, integer *, integer *);


/*     THE FOLLOWING COMMENTS ARE SAMPLE CALLS TO THE EMIT */
/*     ROUTINE.  NOTE THAT EMIT REQUIRES THREE ARGUMENT AT ALL TIMES */
/*     (THE UNUSED ARGUMENTS ARE ZERO). */

/*             CALL EMIT(LD,RA,RB) */
/*             CALL EMIT(LD,RC,-34) */
/*             CALL EMIT(LD,RD,ME) */
/*             CALL EMIT(LD,ME,RE) */
/*             CALL EMIT(IN,RH,0) */
/*             CALL EMIT(DC,RL,0) */
/*             CALL EMIT(AD,RB,0) */
/*             CALL EMIT(AD,ME,0) */
/*             CALL EMIT(AD,-5,0) */
/*             CALL EMIT(SU,RB,0) */
/*             CALL EMIT(SB,ME,0) */
/*             CALL EMIT(ND,-5,0) */
/*             CALL EMIT(XR,0,0) */
/*             CALL EMIT(OR,RB,0) */
/*             CALL EMIT(CP,RH,0) */
/*             CALL EMIT(ROT,ACC,LFT) */
/*             CALL EMIT(ROT,CY,LFT) */
/*             CALL EMIT(ROT,CY,RGT) */
/*             CALL EMIT(JMP,148,0) */
/*             CALL EMIT(JMC,TRU*32+ZERO,148) */
/*             CALL EMIT(CAL,1048,0) */
/*             CALL EMIT(CLC,FAL*32+PARITY,148) */
/*             CALL EMIT(RTN,0,0) */
/*             CALL EMIT(RTC,FAL*32+CARRY,255) */
/*             CALL EMIT(RST,3,0) */
/*             CALL EMIT(INP,6,0) */
/*             CALL EMIT(OUT,10,0) */
/*             CALL EMIT(HALT,0,0) */
/*             EMIT A LITERAL BETWEEN 0 AND 255 */
/*             CALL EMIT(0,44,0) */

/*             CALL EMIT(STA,300,0) */
/*             CALL EMIT(LDA,300,0) */
/*             CALL EMIT(XCHG,0,0) */
/*             CALL EMIT(SPHL,0,0) */
/*             CALL EMIT(PCHL,0,0) */
/*             CALL EMIT(CMA,0,0) */
/*             CALL EMIT(STC,0,0) */
/*             CALL EMIT(CMC,0,0) */
/*             CALL EMIT(DAA,0,0) */
/*             CALL EMIT(SHLD,300,0) */
/*             CALL EMIT(LHLD,300,0) */
/*             CALL EMIT(EI,0,0) */
/*             CALL EMIT(DI,0,0) */

/*             CALL EMIT(LXI,(RB,RD,RH,RSP),300) */
/*             CALL EMIT(PUSH,(RB,RD,RH,RA),0) */
/*             CALL EMIT(POP,(RB,RD,RH,RA),0) */
/*             CALL EMIT(DAD,(RB,RD,RH,RSP),0) */
/*             CALL EMIT(STAX,(RB,RD),0) */
/*             CALL EMIT(LDAX,(RB,RD),0) */
/*             CALL EMIT(INX,(RB,RD,RH,RSP),0) */
/*             CALL EMIT(DCX,(RB,RD,RH,RSP),0) */

    n = 1;

    if (cntrl_1.contrl[24] == 0) {
	goto L100;
    }
/*      WRITE EMITTER TRACE */
    pad_(&c__0, &c__16, &c__1);
    pad_(&c__1, &c__42, &c__1);
    conout_(&c__2, &c_n6, opr, &c__10);
    pad_(&c__1, &c__48, &c__1);
    if (*opa < 0) {
	pad_(&c__1, &c__45, &c__1);
    }
    i__1 = abs(*opa);
    conout_(&c__2, &c_n6, &i__1, &c__10);
    pad_(&c__1, &c__48, &c__1);
    if (*opb < 0) {
	pad_(&c__1, &c__45, &c__1);
    }
    i__1 = abs(*opb);
    conout_(&c__2, &c_n6, &i__1, &c__10);
    pad_(&c__1, &c__43, &c__1);
    writel_(&c__0);
L100:
    if (*opr <= 0) {
	goto L9000;
    }
    bits[0] = code_1.cbits[*opr - 1];
    switch (*opr) {
	case 1:  goto L1000;
	case 2:  goto L1500;
	case 3:  goto L1500;
	case 4:  goto L2000;
	case 5:  goto L2000;
	case 6:  goto L2000;
	case 7:  goto L2000;
	case 8:  goto L2000;
	case 9:  goto L2000;
	case 10:  goto L2000;
	case 11:  goto L2000;
	case 12:  goto L3000;
	case 13:  goto L4000;
	case 14:  goto L5000;
	case 15:  goto L4000;
	case 16:  goto L5000;
	case 17:  goto L10000;
	case 18:  goto L5100;
	case 19:  goto L7000;
	case 20:  goto L8000;
	case 21:  goto L8000;
	case 22:  goto L10000;
	case 23:  goto L9100;
	case 24:  goto L9100;
	case 25:  goto L9400;
	case 26:  goto L9999;
	case 27:  goto L9999;
	case 28:  goto L9999;
	case 29:  goto L9999;
	case 30:  goto L9999;
	case 31:  goto L9999;
	case 32:  goto L9100;
	case 33:  goto L9100;
	case 34:  goto L9999;
	case 35:  goto L9999;
	case 36:  goto L9200;
	case 37:  goto L9500;
	case 38:  goto L9300;
	case 39:  goto L9300;
	case 40:  goto L9300;
	case 41:  goto L9300;
	case 42:  goto L9300;
	case 43:  goto L9300;
    }

L1000:
/*     LOAD OPERATION */
    if (*opb > 0) {
	goto L1200;
    }
/*         LRI OPERATION */
    n = 2;
    bits[0] = (rgmapp_1.regmap[*opa - 1] << 3) + 6;
    bits[1] = -(*opb);
    goto L10000;
L1200:
/*     CHECK FOR POSSIBLE LOAD REGISTER ELIMINATION */
/*     IS THIS A LMR OR LRM INSTRUCTION... */
    if (*opa != ops_1.me) {
	goto L1210;
    }
/*         MAY CHANGE A MOV R,M INR R MOV M,R TO INR M */
    if (peep_1.lastir != code_1.codloc - 1) {
	goto L1205;
    }
    i__2 = code_1.codloc - 1;
    i__1 = get_(&i__2);
    i__ = right_(&i__1, &c__3) + 48;
/*             THE REGISTER LOAD MAY HAVE BEEN ELIMINATED... */
    if (peep_1.lastld == code_1.codloc - 2 && *opb == peep_1.lastrg) {
	goto L1202;
    }
    --code_1.codloc;
    --memory_1.membot;
L1202:
    i__1 = code_1.codloc - 1;
    put_(&i__1, &i__);
    peep_1.lastir = 0;
    peep_1.lastrg = 0;
    peep_1.lastld = 0;
    if (peep_1.lastin == code_1.codloc || peep_1.lastin == code_1.codloc + 1) 
	    {
	peep_1.lastin = code_1.codloc - 1;
    }
    goto L11000;
L1205:
/*         THIS IS A LOAD MEMORY FROM REGISTER OPERATION - SAVE */
    peep_1.lastld = code_1.codloc;
    peep_1.lastrg = *opb;
    goto L1220;
L1210:
    if (*opb != ops_1.me) {
	goto L1220;
    }
/*         THIS IS A LOAD REGISTER FROM MEMORY - MAYBE ELIMINATE */
    if (peep_1.lastld != code_1.codloc - 1) {
	goto L1220;
    }
    if (peep_1.lastrg == *opa) {
	goto L11000;
    }
L1220:
    bits[0] = bits[0] + (rgmapp_1.regmap[*opa - 1] << 3) + rgmapp_1.regmap[*
	    opb - 1];
    goto L10000;

/*     IN OR DC */
L1500:
    bits[0] += rgmapp_1.regmap[*opa - 1] << 3;
    goto L10000;

L2000:
/*     AD AC SU SB ND XR OR CP */
    if (*opa > 0) {
	goto L2200;
    }
/*         IMMEDIATE OPERAND */
    n = 2;
    bits[0] += 70;
    bits[1] = -(*opa);
    goto L10000;

L2200:
    bits[0] += rgmapp_1.regmap[*opa - 1];
    goto L10000;

L3000:
/*     ROT */
    i__ = (*opa - ops_1.cy << 1) + (*opb - ops_1.lft);
    bits[0] += i__ << 3;
    goto L10000;

/*      JMP CAL */
L4000:
    n = 3;
    i__ = *opa;
L4100:
    bits[2] = i__ / 256;
    bits[1] = i__ % 256;
    goto L10000;

/*     JFC JTC CFC CTC */
L5000:
    n = 3;
L5100:
    i__ = *opa % 32 - ops_1.carry;
    i__ = (i__ / 2 << 1) + (i__ + 1) % 2;
    j = *opa / 32 - ops_1.fal;
    j = (i__ << 1) + j;
    bits[0] += j << 3;
    i__ = *opb;
    goto L4100;

/*     RET HLT */
/*     GO TO 10000 */

/*     RST */
L7000:
    bits[0] += *opa % 8 << 3;
    goto L10000;

/*     INP OUT */
L8000:
    n = 2;
    bits[1] = *opa;
    goto L10000;

/*     LITERAL VALUE */
L9000:
    bits[0] = *opa;
    goto L10000;
/*     STA LDA SHLD LHLD (GET ADDRESS PART) */
L9100:
    n = 3;
    bits[2] = *opa / 256;
    bits[1] = *opa % 256;
    goto L10000;

/*     LXI (GET IMMEDIATE PART) */
L9200:
    n = 3;
    bits[2] = *opb / 256;
    bits[1] = *opb % 256;
/*     AND DROP THROUGH... */
/*     LXI PUSH POP DAD STAX LDAX INX DCX */
L9300:
    i__ = rgmapp_1.regmap[*opa - 1];
/*     CHECK FOR ACC */
    if (i__ == 7) {
	i__ = 6;
    }
L9310:
    bits[0] = (i__ << 3) + bits[0];
    goto L10000;
/*     XCHG - CHECK FOR PREVIOUS XCHG AND ELIMINATE IF FOUND */
L9400:
    if (peep_1.lastex != code_1.codloc - 1) {
	goto L9410;
    }
    --memory_1.membot;
    --code_1.codloc;
    peep_1.lastex = 0;
    goto L11000;
L9410:
    peep_1.lastex = code_1.codloc;
    goto L10000;
/*     PUSH R - CHECK FOR XCHG PUSH D COMBINATION. CHANGE TO PUSH H */
L9500:
    if (peep_1.lastex != code_1.codloc - 1) {
	goto L9300;
    }
    if (*opa != ops_1.rd) {
	goto L9300;
    }
    --memory_1.membot;
    --code_1.codloc;
    peep_1.lastex = 0;
    i__ = rgmapp_1.regmap[ops_1.rh - 1];
    goto L9310;
/*     XCHG SPHL PCHL CMA STC CMC DAA EI DI (NO ADDRESS PART) */
L9999:

L10000:
    i__ = alloc_(&n) - 1;
    code_1.codloc += n;
    i__1 = n;
    for (j = 1; j <= i__1; ++j) {
/* L10100: */
	i__2 = i__ + j;
	put_(&i__2, &bits[j - 1]);
    }

L11000:
    return 0;
} /* emit_ */

/* Subroutine */ int puncod_(integer *lb, integer *ub, integer *mode)
{
    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    /* Local variables */
    static integer i__, j, k, l, t[4], kp, lp, up;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer get_(integer *), shr_(integer *, integer *), imin_(integer 
	    *, integer *);
    extern /* Subroutine */ int form_(integer *, integer *, integer *, 
	    integer *, integer *);
    static integer isum;
    extern integer right_(integer *, integer *);
    extern /* Subroutine */ int writel_(integer *), conout_(integer *, 
	    integer *, integer *, integer *);

/*     PUNCH CODE FROM LOWER BOUND (LB) TO UPPER BOUND (UB) */
/*     MODE = 1  - - PUNCH HEADER ONLY */
/*     MODE = 2 - - PUNCH TRAILER ONLY */
/*     MODE = 3 - - PUNCH HEADER AND TRAILER */

    up = *ub;
    lp = *lb;
    writel_(&c__0);
    if (cntrl_1.contrl[27] != 0) {
	goto L400;
    }
    t[0] = 25;
    t[1] = 27;
    t[2] = 13;
    t[3] = 17;

    for (i__ = 1; i__ <= 4; ++i__) {
/* L10: */
	pad_(&c__1, &c__47, &c__20);
    }
    writel_(&c__0);

    if (lp % 8 != 0) {
	conout_(&c__0, &c_n8, &lp, &c__10);
    }
L100:
    if (lp > up) {
	goto L300;
    }
    if (lp % 4 != 0) {
	goto L200;
    }
    if (lp % 8 != 0) {
	goto L130;
    }
    if (lp % 256 != 0) {
	goto L120;
    }
/*     ********* */
    writel_(&c__0);
    for (i__ = 1; i__ <= 4; ++i__) {
/* L110: */
	pad_(&c__1, &c__47, &c__20);
    }

L120:
    conout_(&c__0, &c_n8, &lp, &c__10);
    goto L200;

L130:
    pad_(&c__0, &c__1, &c__8);
/*     DECODE A MEMORY LOCATION */
L200:
    pad_(&c__1, &c__1, &c__1);
    form_(&c__1, t, &c__3, &c__3, &c__4);
    k = get_(&lp);

    for (i__ = 1; i__ <= 8; ++i__) {
	i__1 = 8 - i__;
	kp = k / pow_ii(&c__2, &i__1);
	kp = kp % 2 + 1;
/* L210: */
	form_(&c__1, t, &kp, &kp, &c__4);
    }

    form_(&c__1, t, &c__4, &c__4, &c__4);
    ++lp;
    goto L100;

L300:
    writel_(&c__0);
    for (i__ = 1; i__ <= 4; ++i__) {
/* L310: */
	pad_(&c__1, &c__47, &c__20);
    }
    writel_(&c__0);
    goto L9999;
L400:
/*     WRITE ******** */
    if (*mode % 2 == 0) {
	goto L402;
    }
    pad_(&c__0, &c__47, &c__20);
    pad_(&c__1, &c__47, &c__20);
L402:
    writel_(&c__0);
    l = cntrl_1.contrl[27];
    if (l < 16) {
	l = 16;
    }
L405:
    if (lp > up) {
	goto L500;
    }
    kp = up - lp + 1;
    k = imin_(&kp, &l);
    if (k == 0) {
	goto L500;
    }
    pad_(&c__1, &c__51, &c__1);
    conout_(&c__1, &c__2, &k, &c__16);
    --files_1.obp;
    conout_(&c__1, &c__4, &lp, &c__16);
    --files_1.obp;
    isum = k + right_(&lp, &c__8) + shr_(&lp, &c__8);
    conout_(&c__1, &c__2, &c__0, &c__16);
    --files_1.obp;
    i__1 = k;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = get_(&lp);
	isum += j;
	++lp;
	conout_(&c__1, &c__2, &j, &c__16);
	--files_1.obp;
/* L410: */
    }
    isum = right_(&isum, &c__8);
    isum = (256 - isum) % 256;
    conout_(&c__1, &c__2, &isum, &c__16);
    --files_1.obp;
    writel_(&c__0);
    goto L405;
L500:
    if (*mode / 2 == 0) {
	goto L510;
    }
/*     ***** */
/*     WRITE END OF FILE RECORD */
    pad_(&c__1, &c__51, &c__1);
    pad_(&c__1, &c__2, &c__10);

/*     WRITE ***** AGAIN */
    pad_(&c__0, &c__47, &c__20);
    pad_(&c__1, &c__47, &c__20);
L510:
    writel_(&c__0);
L9999:
    return 0;
} /* puncod_ */

/* Subroutine */ int cvcond_(integer *s)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, ia, jp;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *), 
	    error_(integer *, integer *), genreg_(integer *, integer *, 
	    integer *);

/*     CONVERT THE CONDITION CODE AT S IN THE STACK TO A BOOLEAN VALUE */
    i__ = regall_1.rasn[*s - 1];
    j = i__ / 256;
    k = j % 16;
    j /= 16;
    ia = i__ % 16;
/*     J = 1 IF TRUE , J = 0 IF FALSE */

/*     K = 1 IF CARRY, 2 IF ZERO, 3 IF SIGN, AND 4 IF PARITY */

/*     WE MAY GENERATE A SHORT SEQUENCE */
    if (k > 2 || ia == 0) {
	goto L40;
    }
    if (regall_1.regs[0] != ia) {
	goto L40;
    }
    if (k == 2) {
	goto L10;
    }
/*     SHORT CONVERSION FOR TRUE OR FALSE CARRY */
    emit_(&ops_1.sb, &ops_1.ra, &c__0);
    if (j == 0) {
	emit_(&ops_1.cma, &c__0, &c__0);
    }
    goto L300;
/*     SHORT CONVERSION FOR TRUE OR FALSE ZERO */
L10:
    if (j == 0) {
	emit_(&ops_1.ad, &c_n255, &c__0);
    }
    if (j == 1) {
	emit_(&ops_1.su, &c_n1, &c__0);
    }
    emit_(&ops_1.sb, &ops_1.ra, &c__0);
    goto L300;
/*     DO WE HAVE TO ASSIGN A REGISTER */
L40:
    if (ia != 0) {
	goto L50;
    }
    genreg_(&c__1, &ia, &jp);
    if (ia != 0) {
	goto L60;
    }
    error_(&c__118, &c__5);
    goto L9999;
L60:
    regall_1.regs[ia - 1] = regall_1.sp;
    i__ = ia;

/*     CHECK PENDING REGISTER STORE */
L50:
    jp = regall_1.regs[0];
    if (jp == 0) {
	goto L100;
    }
    if (jp == ia) {
	goto L100;
    }
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    regall_1.regs[0] = 0;

L100:
    emit_(&ops_1.ld, &ops_1.ra, &c_n255);
    j = (ops_1.fal + j << 5) + (ops_1.carry + k - 1);
    i__1 = code_1.codloc + 4;
    emit_(&ops_1.jmc, &j, &i__1);
    emit_(&ops_1.xr, &ops_1.ra, &c__0);
    goto L300;

/*     ACCUMULATOR CONTAINS THE BOOLEAN VALUE (0 OR 1) */
L300:
/*     SET UP PENDING REGISTER STORE */
    regall_1.regs[0] = ia;
    regall_1.rasn[*s - 1] = i__ % 256;
L9999:
    return 0;
} /* cvcond_ */

/* Subroutine */ int saver_(void)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, i1, i2, ic1, ic2;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *), 
	    error_(integer *, integer *), litadd_(integer *), delete_(integer 
	    *), cvcond_(integer *), setadr_(integer *);

/*     SAVE THE ACTIVE REGISTERS AND RESET TABLES */
/*     FIRST DETERMINE THE STACK ELEMENTS WHICH MUST BE SAVED */
    ic1 = 0;
    ic2 = 0;
    i1 = 0;
    i2 = 0;

    if (regall_1.sp == 0) {
	goto L3000;
    }
    i__1 = regall_1.sp;
    for (j = 1; j <= i__1; ++j) {
	k = regall_1.rasn[j - 1];
	if (k > 255) {
	    cvcond_(&j);
	}
	if (k <= 0) {
	    goto L1000;
	}
	k = regall_1.rasn[j - 1];
	if (k >= 16) {
	    goto L800;
	}
/*         SINGLE BYTE */
	if (regall_1.lock[k - 1] == 1) {
	    goto L1000;
	}
	regall_1.st[j - 1] = i1;
	++ic1;
	i1 = j;
	goto L1000;

/*         DOUBLE BYTE */
L800:
	l = k % 16;
	k /= 16;
	if (regall_1.lock[l - 1] + regall_1.lock[k - 1] > 0) {
	    goto L1000;
	}
	regall_1.st[j - 1] = i2;
	i2 = j;
	++ic2;
L1000:
	;
    }

    symbl_1.lmem = symbl_1.lmem - ic1 - (ic2 << 1);
    if (symbl_1.lmem % 2 * ic2 > 0 && ic1 == 0) {
	--symbl_1.lmem;
    }
/*     LMEM IS NOW PROPERLY ALIGNED. */
    if (symbl_1.lmem >= 0) {
	goto L1100;
    }
    error_(&c__119, &c__1);
    goto L99999;
L1100:
    k = symbl_1.lmem;

L2000:
    if (i1 + i2 == 0) {
	goto L3000;
    }
    if (k % 2 == 1 || i2 == 0) {
	goto L2100;
    }
/*     EVEN BYTE BOUNDARY WITH DOUBLE BYTES TO STORE */
    i__ = i2;
    i2 = regall_1.st[i__ - 1];
    goto L2200;

/*      SINGLE BYTE */
L2100:
    i__ = i1;
    i1 = regall_1.st[i__ - 1];
L2200:
    if (i__ > 0) {
	goto L2300;
    }
    error_(&c__120, &c__1);
    goto L99999;

/*     PLACE TEMPORARY INTO SYMBOL TABLE */
L2300:
    ++symbl_1.sytop;
    regall_1.st[i__ - 1] = symbl_1.sytop;
    symbl_1.symbol[symbl_1.sytop - 1] = symbl_1.syinfo;
    j = regall_1.rasn[i__ - 1];
    l = 1;
    if (j >= 16) {
	l = 2;
    }
    symbl_1.symbol[symbl_1.syinfo - 1] = k;
    k += l;
    --symbl_1.syinfo;
    symbl_1.symbol[symbl_1.syinfo - 1] = (l << 4) + 256 + types_1.varb;
/*     LENGTH IS 1*256 */
    --symbl_1.syinfo;
/*     LEAVE ROOM FOR LXI CHAIN */
    symbl_1.symbol[symbl_1.syinfo - 1] = 0;
    --symbl_1.syinfo;
    if (symbl_1.sytop <= symbl_1.syinfo) {
	goto L2400;
    }
    error_(&c__121, &c__5);
    goto L99999;

L2400:
/*     STORE INTO MEMORY */
    l = regall_1.rasn[i__ - 1];
    regall_1.rasn[i__ - 1] = 0;
    ++regall_1.sp;
    setadr_(&symbl_1.sytop);
    litadd_(&regall_1.sp);
L2450:
    i__ = l % 16;
    if (i__ != regall_1.regs[0]) {
	goto L2500;
    }
    i__ = 1;
    regall_1.regs[ops_1.ra - 1] = 0;
    regall_1.regv[ops_1.ra - 1] = -1;
L2500:
    emit_(&ops_1.ld, &ops_1.me, &i__);
    l /= 16;
    if (l == 0) {
	goto L2700;
    }
/*     DOUBLE BYTE STORE */
    emit_(&ops_1.in, &ops_1.rl, &c__0);
    ++regall_1.regv[6];
    goto L2450;

L2700:
    delete_(&c__1);
    goto L2000;

/*     END OF REGISTER STORES */
L3000:
    for (i__ = 2; i__ <= 7; ++i__) {
	if (regall_1.lock[i__ - 1] == 1) {
	    goto L4000;
	}
	regall_1.regs[i__ - 1] = 0;
	regall_1.regv[i__ - 1] = -1;
L4000:
	;
    }
L99999:
    return 0;
} /* saver_ */

/* Subroutine */ int reloc_(void)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, l, m, n, ip, iw;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer get_(integer *), shl_(integer *, integer *), shr_(integer *
	    , integer *);
    extern /* Subroutine */ int put_(integer *, integer *), form_(integer *, 
	    integer *, integer *, integer *, integer *);
    extern integer right_(integer *, integer *);
    static integer stloc;
    extern /* Subroutine */ int error_(integer *, integer *), conout_(integer 
	    *, integer *, integer *, integer *), writel_(integer *);
    static integer stsize;


    if (cntrl_1.contrl[29] < 2) {
	goto L18;
    }
    i__1 = symbl_1.sytop;
    for (i__ = 1; i__ <= i__1; ++i__) {
	conout_(&c__0, &c_n4, &i__, &c__10);
	pad_(&c__1, &c__39, &c__1);
	conout_(&c__1, &c_n6, &symbl_1.symbol[i__ - 1], &c__10);
/* L12: */
    }

    i__1 = symbl_1.symax;
    for (i__ = symbl_1.syinfo; i__ <= i__1; ++i__) {
	conout_(&c__0, &c_n5, &i__, &c__10);
	pad_(&c__1, &c__39, &c__1);
	j = symbl_1.symbol[i__ - 1];
	k = 45;
	if (j >= 0) {
	    k = 1;
	}
	pad_(&c__1, &k, &c__1);
	i__2 = abs(j);
	conout_(&c__1, &c__8, &i__2, &c__16);
/* L14: */
    }

L18:
/*     COMPUTE MAX STACK DEPTH REQUIRED FOR CORRECT EXECUTION */
    stsize = pstack_1.maxdep[0];
    for (n = 1; n <= 8; ++n) {
	i__ = inter_1.intpro[n - 1];
	if (i__ == 0) {
	    goto L20;
	}
/*         GET INTERRUPT PROCEDURE DEPTH */
	i__ = symbl_1.symbol[i__ - 1] - 3;
	i__ = symbl_1.symbol[i__ - 1] + 1;
/*         NOTE THAT I EXCEEDS DEPTH BY 1 SINCE RET MAY BE PENDING */
	stsize += i__;
L20:
	;
    }
    stsize <<= 1;

    n = stsize;
    if (cntrl_1.contrl[46] != 0) {
	n = 0;
    }
/*     ALIGN TO EVEN BOUNDARY, IF NECESSARY */
    if (n != 0 && symbl_1.lmem % 2 == 1) {
	--symbl_1.lmem;
    }
    stloc = symbl_1.lmem;
    symbl_1.lmem -= n;
/*     STSIZE IS NUMBER OF BYTES REQD FOR STACK, STLOC IS ADDR */

    iw = cntrl_1.contrl[33] / 14;
    n = 0;
/*     COMPUTE PAGE TO START VARIABLES */
    i__ = 0;
    if (code_1.codloc % 256 > symbl_1.lmem % 256) {
	i__ = 1;
    }
    i__ += code_1.codloc / 256;
    if (cntrl_1.contrl[32] > i__) {
	i__ = cntrl_1.contrl[32];
    }

/*     COMPUTE FIRST RELATIVE ADDRESS PAGE */
    j = symbl_1.lmem / 256 - i__;
    if (j >= 0) {
	goto L50;
    }
    error_(&c__122, &c__1);
    goto L9999;
L50:
    i__1 = symbl_1.sytop;
    for (i__ = 1; i__ <= i__1; ++i__) {
	m = symbl_1.symbol[i__ - 1];
	k = symbl_1.symbol[m - 1];
	if (k < 0) {
	    goto L300;
	}

/*     NOW FIX PAGE NUMBER */

	i__2 = shr_(&k, &c__8);
	l = right_(&i__2, &c__8) - j;
/*     L IS RELOCATED PAGE NUMBER */
	symbl_1.symbol[m - 1] = shl_(&l, &c__8) + right_(&k, &c__8);
	k = shr_(&k, &c__16);
L100:
	if (k == 0) {
	    goto L150;
	}
/*     BACKSTUFF LHI L INTO LOCATION K-1 */
	i__2 = k - 1;
	ip = (get_(&i__2) << 8) + get_(&k);
	i__2 = k - 1;
	put_(&i__2, &c__38);
	put_(&k, &l);
	k = ip;
	goto L100;
L150:
/*     BACKSTUFF LXI REFERENCES TO THIS VARIABLE */
	k = symbl_1.symbol[m - 3];
	m = symbl_1.symbol[m - 1];
/*     K IS LXI CHAIN HEADER, M IS REAL ADDRESS */
L160:
	if (k == 0) {
	    goto L300;
	}
	i__2 = k + 1;
	l = get_(&k) + (get_(&i__2) << 8);
	i__2 = m % 256;
	put_(&k, &i__2);
	i__2 = k + 1;
	i__3 = m / 256;
	put_(&i__2, &i__3);
	k = l;
	goto L160;
L300:
	;
    }
    if (cntrl_1.contrl[23] != 0) {
	writel_(&c__0);
    }

/*     RELOCATE AND BACKSTUFF THE STACK TOP REFERENCES */
    stloc -= j << 8;
L310:
    if (pstack_1.lxis == 0) {
	goto L320;
    }
    i__ = pstack_1.lxis;
    i__1 = i__ + 1;
    pstack_1.lxis = get_(&i__) + (get_(&i__1) << 8);
    i__1 = stloc % 256;
    put_(&i__, &i__1);
    i__1 = i__ + 1;
    i__2 = stloc / 256;
    put_(&i__1, &i__2);
    goto L310;
L320:
    form_(&c__0, smessg_1.smssg, &c__1, &c__11, &c__29);
    if (cntrl_1.contrl[46] == 1) {
	goto L330;
    }
    form_(&c__1, smessg_1.smssg, &c__12, &c__13, &c__29);
    conout_(&c__2, &c_n10, &stsize, &c__10);
    form_(&c__1, smessg_1.smssg, &c__24, &c__29, &c__29);
    goto L340;
L330:
    form_(&c__1, smessg_1.smssg, &c__14, &c__23, &c__29);
L340:
    writel_(&c__0);

/*     NOW BACKSTUFF ALL OTHER TRC, TRA, AND PRO ADDRESSES */

    i__1 = symbl_1.sytop;
    for (i__ = 1; i__ <= i__1; ++i__) {
	j = symbl_1.symbol[i__ - 1];
	k = -symbl_1.symbol[j - 1];
	l = (i__2 = symbl_1.symbol[j - 2], abs(i__2));
	l = right_(&l, &c__4);
	if (l != types_1.label && l != types_1.proc) {
	    goto L700;
	}
	i__2 = shr_(&k, &c__2);
	l = right_(&i__2, &c__14);
	n = right_(&k, &c__2);
	k = shr_(&k, &c__16);
L600:
	if (l == 0) {
	    goto L650;
	}
	i__2 = l + 1;
	m = get_(&l) + (get_(&i__2) << 8);
	i__2 = k % 256;
	put_(&l, &i__2);
	i__2 = l + 1;
	i__3 = k / 256;
	put_(&i__2, &i__3);
	l = m;
	goto L600;
L650:
	symbl_1.symbol[j - 1] = shl_(&k, &c__16) + n;
L700:
	;
    }
    if (memory_1.preamb <= 0) {
	goto L900;
    }
    for (i__ = 1; i__ <= 8; ++i__) {
	j = inter_1.intpro[i__ - 1];
	if (j == 0) {
	    goto L710;
	}
	j = symbl_1.symbol[j - 1];
	j = (i__1 = symbl_1.symbol[j - 1], abs(i__1)) / 65536;
	inter_1.intpro[i__ - 1] = (j << 8) + 195;
/*             INTPRO CONTAINS INVERTED JUMP TO PROCEDURE */
L710:
	;
    }
    if (inter_1.intpro[0] == 0) {
	inter_1.intpro[0] = (memory_1.offset + memory_1.preamb << 8) + 195;
    }
/*     ** NOTE THAT JUMP INST IS 11000011B = 195D ** */
    k = memory_1.offset;
    memory_1.offset = 0;
    i__ = 0;
    j = 1;
L720:
    l = inter_1.intpro[j - 1];
    ++j;
L730:
    i__1 = l % 256;
    put_(&i__, &i__1);
    l /= 256;
    ++i__;
    if (i__ >= memory_1.preamb) {
	goto L740;
    }
    if (i__ % 8 == 0) {
	goto L720;
    }
    goto L730;

L740:
    memory_1.offset = k;
L900:
L9999:
    return 0;
} /* reloc_ */

/* Subroutine */ int loadin_(void)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, m, n, lp;
    extern integer gnc_(integer *), get_(integer *);
    extern /* Subroutine */ int put_(integer *, integer *), error_(integer *, 
	    integer *);

/*     SAVE THE CURRENT INPUT FILE NUMBER */
    m = cntrl_1.contrl[19];
    cntrl_1.contrl[19] = cntrl_1.contrl[31];
/*     GET RID OF LAST CARD IMAGE */
    files_1.ibp = 99999;
L5:
    i__ = gnc_(&c__0);
    if (i__ == 1) {
	goto L5;
    }
    if (i__ != 41) {
	goto L8000;
    }

/*     PROCESS NEXT SYMBOL TABLE ENTRY */
L100:
    i__ = gnc_(&c__0);
    if (i__ == 41) {
	goto L9999;
    }

    i__ += -2;
/*     BUILD ADDRESS OF INITIALIZED SYMBOL */
    k = 32;
    for (j = 1; j <= 2; ++j) {
	i__ = (gnc_(&c__0) - 2) * k + i__;
/* L200: */
	k <<= 5;
    }

    j = symbl_1.symbol[i__ - 1];
    k = symbl_1.symbol[j - 2];
    k = k / 16 % 16;
    j = symbl_1.symbol[j - 1];
/*     J IS STARTING ADDRESS, AND K IS THE PRECISION OF */
/*     THE BASE VARIABLE */
    if (code_1.codloc <= j) {
	goto L300;
    }
    error_(&c__123, &c__1);
L300:
    if (code_1.codloc >= j) {
	goto L350;
    }
    put_(&code_1.codloc, &c__0);
    ++code_1.codloc;
    goto L300;

/*     READ HEX VALUES UNTIL NEXT '/' IS ENCOUNTERED */
L350:
    lp = -1;
L400:
    ++lp;
    i__ = gnc_(&c__0) - 2;
/*     CHECK FOR ENDING / */
    if (i__ == 39) {
	goto L100;
    }
    l = i__ / 16;
    i__ = (i__ % 16 << 4) + (gnc_(&c__0) - 2);
/*     I IS THE NEXT HEX VALUE, AND L=1 IF BEGINNING OF A NEW BVALUE */
    if (k != 2) {
	goto L1000;
    }
/*         DOUBLE BYTE INITIALIZE */
    if (l != 0) {
	goto L500;
    }
/*         CHECK FOR LONG CONSTANT */
    if (lp < 2) {
	goto L600;
    }
L500:
    lp = 0;
    put_(&code_1.codloc, &i__);
    i__1 = code_1.codloc + 1;
    put_(&i__1, &c__0);
    goto L1100;

/*         EXCHANGE PLACES WITH H.O. AND L.O. BYTES */
L600:
    i__1 = code_1.codloc - 2;
    n = get_(&i__1);
    i__1 = code_1.codloc - 1;
    put_(&i__1, &n);
    i__1 = code_1.codloc - 2;
    put_(&i__1, &i__);
    goto L400;

L1000:
    put_(&code_1.codloc, &i__);
L1100:
    code_1.codloc += k;
    goto L400;


L8000:
    error_(&c__124, &c__1);
L9999:
    cntrl_1.contrl[19] = m;
    return 0;
} /* loadin_ */

/* Subroutine */ int emitbf_(integer *l)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer i__, k, m, n, kp, jp, lp;
    extern integer get_(integer *);
    extern /* Subroutine */ int put_(integer *, integer *), emit_(integer *, 
	    integer *, integer *);
    extern integer alloc_(integer *);

/*     EMIT CODE FOR THE BUILT-IN FUNCTION L.  THE BIFTAB */
/*     ARRAY IS HEADED BY A TABLE WHICH EITHER GIVES THE STARTING */
/*     LOCATION OF THE BIF CODE IN BIFTAB (IF NEGATIVE) OR THE */
/*     ABSOLUTE CODE LOCATION OF THE FUNCTION IF ALREADY */
/*     EMITTED. */
    i__ = bifcod_1.biftab[*l - 1];
    if (i__ >= 0) {
	goto L1000;
    }
/*     CODE NOT YET EMITTED */
    i__ = -i__;
    emit_(&ops_1.jmp, &c__0, &c__0);
/*     BACKSTUFF ADDRESS LATER */
    bifcod_1.biftab[*l - 1] = code_1.codloc;
/*     GET NUMBER OF BYTES TO EMIT */
    k = bifcod_1.biftab[i__ - 1];
    ++i__;
/*     THEN THE NUMBER OF RELATIVE ADDRESS STUFFS */
    kp = bifcod_1.biftab[i__ - 1];
    ++i__;
/*     START EMITTING CODE */
    m = i__ + kp;
    jp = 0;
L100:
    if (jp >= k) {
	goto L200;
    }
    if (jp % 3 != 0) {
	goto L110;
    }
    n = bifcod_1.biftab[m - 1];
    ++m;
L110:
    lp = alloc_(&c__1);
    i__1 = n % 256;
    put_(&code_1.codloc, &i__1);
    n /= 256;
    ++code_1.codloc;
    ++jp;
    goto L100;

/*     NOW GO BACK AND REPLACE RELATIVE ADDRESSES WITH */
/*     ABSOLUTE ADDRESSES. */

L200:
    jp = 0;
    n = bifcod_1.biftab[*l - 1];
L300:
    if (jp >= kp) {
	goto L400;
    }
    m = bifcod_1.biftab[i__ - 1];
    ++i__;
    i__1 = n + m;
    i__2 = m + n + 1;
    k = get_(&i__1) + (get_(&i__2) << 8) + n;
    i__1 = n + m;
    i__2 = k % 256;
    put_(&i__1, &i__2);
    i__1 = n + m + 1;
    i__2 = k / 256;
    put_(&i__1, &i__2);
    ++jp;
    goto L300;

L400:
    i__ = bifcod_1.biftab[*l - 1];
/*     BACKSTUFF BRANCH AROUND FUNCTION */
    i__1 = i__ - 2;
    i__2 = code_1.codloc % 256;
    put_(&i__1, &i__2);
    i__1 = i__ - 1;
    i__2 = code_1.codloc / 256;
    put_(&i__1, &i__2);

/*     EMIT CALL ON THE FUNCTION */
L1000:
    emit_(&ops_1.cal, &i__, &c__0);
    return 0;
} /* emitbf_ */

/* Subroutine */ int inldat_(void)
{
    static integer i__, j, k, l, ic, iq, kp;
    extern integer gnc_(integer *);
    extern /* Subroutine */ int emit_(integer *, integer *, integer *), 
	    error_(integer *, integer *);

/*     EMIT DATA INLINE */
    iq = code_1.codloc;
    l = 0;
L100:
    k = 0;
    if (peep_1.lapol == 0) {
	goto L600;
    }
    for (j = 1; j <= 3; ++j) {
L150:
	i__ = gnc_(&c__0);
	if (i__ == 1) {
	    goto L150;
	}
	if (i__ < 2 || i__ > 33) {
	    goto L600;
	}
/* L200: */
	k = (k << 5) + i__ - 2;
    }

    i__ = k;
    k = peep_1.lapol;
    peep_1.lapol = i__;

    kp = k % 8;
    k /= 8;
/*     KP IS TYP AND K IS DATA */
    if (l > 0) {
	goto L300;
    }

/*     DEFINE INLINE DATA SYMBOL */
    if (kp != ilcod_1.def) {
	goto L600;
    }
    ic = k;
    if (k > 0) {
	goto L400;
    }
/*     INLINE CONSTANT -- SET UP SYMBOL ENTRY */
    ++symbl_1.sytop;
    ic = -symbl_1.sytop;
    symbl_1.symbol[symbl_1.sytop - 1] = symbl_1.syinfo;
    symbl_1.syinfo += -2;
/*     WILL BE FILLED LATER */
    if (symbl_1.syinfo < symbl_1.sytop) {
	goto L600;
    }
    goto L400;

/*     READ DATA AND STORE INTO ROM */
L300:
    if (kp == ilcod_1.opr) {
	goto L500;
    }
    if (kp != ilcod_1.lit) {
	goto L600;
    }
    emit_(&c__0, &k, &c__0);
L400:
    ++l;
    goto L100;

/*     END OF DATA */
L500:
    if (k != ilcod_1.dat) {
	goto L600;
    }
/*     BACKSTUFF JUMP ADDRESS */
/*     NOW FIX SYMBOL TABLE ENTRIES */
    k = abs(ic);
    --l;
    k = symbl_1.symbol[k - 1];
    symbl_1.symbol[k - 1] = -iq;
    --k;
    j = symbl_1.symbol[k - 1];
/*     CHECK SYMBOL LENGTH AGAINST COUNT */
    j /= 256;
    symbl_1.symbol[k - 1] = (l << 8) + 16 + types_1.varb;
    if (ic < 0) {
	goto L550;
    }
/*     CHECK SIZE DECLARED AGAINST SIZE READ */
    if (j == l) {
	goto L1000;
    }

L600:
    if (kp != ilcod_1.lin) {
	goto L700;
    }
    cntrl_1.contrl[13] = k;
    goto L100;
L700:
    error_(&c__125, &c__1);
    goto L1000;

/*     THIS IS AN ADDRESS REFERENCE TO A CONSTANT, SO.. */
L550:
    ++regall_1.sp;
    regall_1.st[regall_1.sp - 1] = ic;
    regall_1.rasn[regall_1.sp - 1] = 0;
    regall_1.litv[regall_1.sp - 1] = iq;
    regall_1.prec[regall_1.sp - 1] = 2;


L1000:
/* L2000: */
    return 0;
} /* inldat_ */

/* Subroutine */ int unary_(integer *ival)
{
    static integer i__, j, k, ia, ib, ip, jp, iq, val;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *), 
	    loadv_(integer *, integer *), error_(integer *, integer *), 
	    cvcond_(integer *);

/*     'VAL' IS AN INTEGER CORRESPONDING TO THE OPERATIONS-- */
/*     RTL(1) RTR(2) SFL(3) SFR(4) SCL(5) SCR(6) HIV(7) LOV(8) */
/*     ** NOTE THAT THE FOLLOWING CODE ASSUMES THE VALUE OF RTL = 37 */
    val = *ival - 36;
    if (regall_1.rasn[regall_1.sp - 1] > 255) {
	cvcond_(&regall_1.sp);
    }
    ip = regall_1.prec[regall_1.sp - 1];
    switch (val) {
	case 1:  goto L1000;
	case 2:  goto L1000;
	case 3:  goto L3000;
	case 4:  goto L3000;
	case 5:  goto L3000;
	case 6:  goto L3000;
	case 7:  goto L9990;
	case 8:  goto L5000;
	case 9:  goto L6000;
    }
/*     RTL RTR */
L1000:
    if (ip > 1) {
	goto L9990;
    }
    if (regall_1.rasn[regall_1.sp - 1] != 0) {
	goto L1100;
    }
    loadv_(&regall_1.sp, &c__1);
    regall_1.regs[0] = regall_1.rasn[regall_1.sp - 1] % 16;
L1100:
    i__ = regall_1.rasn[regall_1.sp - 1] % 16;
    k = regall_1.regs[0];
    if (k == 0) {
	goto L1200;
    }
    if (k == i__) {
	goto L1300;
    }
    emit_(&ops_1.ld, &k, &ops_1.ra);
L1200:
    emit_(&ops_1.ld, &ops_1.ra, &i__);
    regall_1.regs[0] = i__;
L1300:
    i__ = ops_1.lft;
    if (val == 2) {
	i__ = ops_1.rgt;
    }
    emit_(&ops_1.rot, &ops_1.cy, &i__);
    goto L9999;

/*     SFL SFR  SCL SCR */
L3000:
    j = 1;
    if ((val == 4 || val == 6) && ip > 1) {
	j = 0;
    }
    i__ = regall_1.rasn[regall_1.sp - 1];
    if (i__ > 0) {
	goto L3100;
    }

/*         LOAD FROM MEMORY */
    loadv_(&regall_1.sp, &j);
    i__ = regall_1.rasn[regall_1.sp - 1];
    if (j == 1) {
	regall_1.regs[0] = i__ % 16;
    }

/*     MAY HAVE TO STORE THE ACCUMULATOR */
L3100:
    ia = i__ % 16;
    ib = i__ / 16;
    k = ia;
    if (j != 1) {
	k = ib;
    }
    jp = regall_1.regs[0];
/*     WE WANT REGISTER K TO BE IN THE ACCUMULATOR */
    if (jp == k) {
	goto L3200;
    }
    if (jp == 0) {
	goto L3150;
    }
    emit_(&ops_1.ld, &jp, &ops_1.ra);
L3150:
    emit_(&ops_1.ld, &ops_1.ra, &k);
L3200:
    regall_1.regs[0] = k;

/*     SFL AND SFR TAKE SEPARATE PATHS NOW... */
    if (val == 4 || val == 6) {
	goto L4000;
    }

/*     SFL - CLEAR CARRY AND SHIFT */
    if (val == 3) {
	emit_(&ops_1.ad, &ops_1.ra, &ops_1.ra);
    }
    if (val == 5) {
	emit_(&ops_1.rot, &ops_1.acc, &ops_1.lft);
    }
    if (ip < 2) {
	goto L9999;
    }
    emit_(&ops_1.ld, &ia, &ops_1.ra);
    emit_(&ops_1.ld, &ops_1.ra, &ib);
    emit_(&ops_1.rot, &ops_1.acc, &ops_1.lft);
    regall_1.regs[0] = ib;
    goto L9999;

/*     SFR - ACCUMULATOR CONTAINS VALUE TO SHIFT FIRST */
L4000:
    if (val == 4) {
	emit_(&ops_1.or, &ops_1.ra, &c__0);
    }
    emit_(&ops_1.rot, &ops_1.acc, &ops_1.rgt);
    if (ip < 2) {
	goto L9999;
    }
    emit_(&ops_1.ld, &ib, &ops_1.ra);
    emit_(&ops_1.ld, &ops_1.ra, &ia);
    emit_(&ops_1.rot, &ops_1.acc, &ops_1.rgt);
    regall_1.regs[0] = ia;
    goto L9999;

/*     HIV */
L5000:
    if (ip < 2) {
	goto L9990;
    }
    if (regall_1.rasn[regall_1.sp - 1] > 0) {
	goto L5100;
    }
    loadv_(&regall_1.sp, &c__0);
L5100:
    i__ = regall_1.rasn[regall_1.sp - 1];
    ip = i__ / 16 % 16;
    iq = i__ % 16;
    if (regall_1.regs[0] == iq) {
	regall_1.regs[0] = 0;
    }
    regall_1.regs[ip - 1] = 0;
    regall_1.regv[ip - 1] = -1;
    regall_1.rasn[regall_1.sp - 1] = iq;
    regall_1.prec[regall_1.sp - 1] = 1;
    if (regall_1.regs[0] != ip) {
	goto L5200;
    }
    regall_1.regs[0] = iq;
    goto L9999;
L5200:
    emit_(&ops_1.ld, &iq, &ip);
    goto L9999;

/*     LOV */
L6000:
    regall_1.prec[regall_1.sp - 1] = 1;
/*     MAY HAVE TO RELEASE REGISTER */
    i__ = regall_1.rasn[regall_1.sp - 1];
    regall_1.rasn[regall_1.sp - 1] = i__ % 16;
    i__ /= 16;
    if (i__ == 0) {
	goto L9999;
    }
    regall_1.regs[i__ - 1] = 0;
    regall_1.regv[i__ - 1] = -1;
    if (regall_1.regs[0] == i__) {
	regall_1.regs[0] = 0;
    }
    goto L9999;

L9990:
    error_(&c__126, &c__1);
L9999:
    return 0;
} /* unary_ */

/* Subroutine */ int exch_(void)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, ia, ib;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *), 
	    error_(integer *, integer *), genreg_(integer *, integer *, 
	    integer *), ustack_(void);

/*     EXCHANGE THE TOP TWO ELEMENTS OF THE STACK */
    j = regall_1.sp - 1;
    if (regall_1.st[j - 1] != 0 || regall_1.rasn[j - 1] != 0 || regall_1.litv[
	    j - 1] >= 0) {
	goto L40;
    }
/*     SECOND ELEMENT IS PUSHED - CHECK TOP ELT */
    if (regall_1.rasn[regall_1.sp - 1] == 0 && regall_1.litv[regall_1.sp - 1] 
	    < 0) {
	goto L30;
    }
/*     TOP ELT IS IN CPU REGS */

/*     ASSUME THERE WILL BE AN IMMEDIATE OPERATION, SO ALLOW */
/*          REG/PUSH TO BE CHANGED TO PUSH/REG */
    goto L40;

/*     POP ELEMENT (SECOND IF DROP THRU, TOP IF FROM 30) */
L20:
    genreg_(&c_n1, &ia, &ib);
    if (ia != 0) {
	goto L25;
    }
    error_(&c__107, &c__5);
    goto L40;
L25:
    if (regall_1.prec[j - 1] > 1) {
	ib = ia - 1;
    }
    i__1 = ia - 1;
    emit_(&ops_1.pop, &i__1, &c__0);
    ustack_();
    regall_1.regs[ia - 1] = j;
    if (ib != 0) {
	regall_1.regs[ib - 1] = j;
    }
    regall_1.rasn[j - 1] = (ib << 4) + ia;
    if (j != regall_1.sp) {
	goto L40;
    }
    j = regall_1.sp - 1;
    goto L20;
/*     SECOND ELT IS PUSHED, TOP ELT IS NOT IN CPU */
L30:
    if (regall_1.st[regall_1.sp - 1] != 0) {
	goto L40;
    }
/*     BOTH ARE PUSHED, SO GO THRU 20 TWICE */
    j = regall_1.sp;
    goto L20;

L40:
    j = regall_1.sp - 1;
    for (i__ = 2; i__ <= 7; ++i__) {
	if (regall_1.regs[i__ - 1] != regall_1.sp) {
	    goto L50;
	}
	regall_1.regs[i__ - 1] = j;
	goto L100;
L50:
	if (regall_1.regs[i__ - 1] == j) {
	    regall_1.regs[i__ - 1] = regall_1.sp;
	}
L100:
	;
    }
    i__ = regall_1.prec[regall_1.sp - 1];
    regall_1.prec[regall_1.sp - 1] = regall_1.prec[j - 1];
    regall_1.prec[j - 1] = i__;

    i__ = regall_1.rasn[regall_1.sp - 1];
    regall_1.rasn[regall_1.sp - 1] = regall_1.rasn[j - 1];
    regall_1.rasn[j - 1] = i__;

    i__ = regall_1.st[regall_1.sp - 1];
    regall_1.st[regall_1.sp - 1] = regall_1.st[j - 1];
    regall_1.st[j - 1] = i__;

    i__ = regall_1.litv[regall_1.sp - 1];
    regall_1.litv[regall_1.sp - 1] = regall_1.litv[j - 1];
    regall_1.litv[j - 1] = i__;

    return 0;
} /* exch_ */

/* Subroutine */ int stack_(integer *n)
{
    static integer j, k;

/*     ADD N TO CURRENT DEPTH, TEST FOR STACKSIZE EXC MAXDEPTH */
    k = pstack_1.prsp + 1;
    j = pstack_1.curdep[k - 1] + *n;
    if (j > pstack_1.maxdep[k - 1]) {
	pstack_1.maxdep[k - 1] = j;
    }
    pstack_1.curdep[k - 1] = j;
    return 0;
} /* stack_ */

/* Subroutine */ int readcd_(void)
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer i__, j, k, l, m, ia, ib, ii, ip, kp, lp;
    extern integer gnc_(integer *), get_(integer *);
    static integer val;
    extern integer shl_(integer *, integer *), shr_(integer *, integer *);
    static integer typ;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *), put_(
	    integer *, integer *);
    static integer lloc, lcnt;
    extern /* Subroutine */ int emit_(integer *, integer *, integer *), form_(
	    integer *, integer *, integer *, integer *, integer *);
    static integer ibase;
    extern integer alloc_(integer *);
    static integer lline;
    extern /* Subroutine */ int stack_(integer *);
    extern integer right_(integer *, integer *);
    extern /* Subroutine */ int saver_(void), error_(integer *, integer *), 
	    genreg_(integer *, integer *, integer *), cvcond_(integer *), 
	    operat_(integer *);
    static integer polcnt;
    extern /* Subroutine */ int setadr_(integer *), writel_(integer *), 
	    conout_(integer *, integer *, integer *, integer *);

    cntrl_1.contrl[13] = 1;
    lline = 0;
    lloc = 0;
    lcnt = cntrl_1.contrl[33] / 12;
    code_1.alter = 0;
    m = cntrl_1.contrl[19];
    cntrl_1.contrl[19] = cntrl_1.contrl[20];
    polcnt = 0;
/*     RESERVE SPACE FOR INTERRUPT LOCATIONS */
    for (i__ = 1; i__ <= 8; ++i__) {
	ii = 9 - i__;
	if (inter_1.intpro[ii - 1] != 0) {
	    goto L20;
	}
/* L10: */
    }
    memory_1.preamb = 0;
    goto L22;
L20:
    memory_1.preamb = (ii - 1 << 3) + 3;
/*     ADJUST CODLOC TO ACCOUNT FOR PREAMBLE */
L22:
    if (code_1.codloc < memory_1.preamb) {
	code_1.codloc = memory_1.preamb;
    }
/*     ALLOCATE 'PREAMBLE' CELLS AT START OF CODE */
    i__ = alloc_(&memory_1.preamb);
    memory_1.offset = code_1.codloc - memory_1.preamb;
/*     SET STACK POINTER UPON PROGRAM ENTRY */
    j = cntrl_1.contrl[46];
    if (j == 1) {
	goto L100;
    }
    if (j != 0) {
	goto L90;
    }
/*     START CHAIN OF LXIS */
    pstack_1.lxis = code_1.codloc + 1;
L90:
    emit_(&ops_1.lxi, &ops_1.rsp, &j);
L100:
    if (terrr_1.errflg) {
	goto L9000;
    }
    ibase = 0;
/*     MAY HAVE BEEN STACK OVERFLOW SO... */
    if (regall_1.sp < 0) {
	regall_1.sp = 0;
    }
    if (cntrl_1.contrl[11] == 0) {
	goto L10700;
    }
    if (code_1.alter == 0 || regall_1.sp <= 0) {
	goto L10700;
    }
/*     WRITE STACK */
    pad_(&c__0, &c__1, &c__1);
    pad_(&c__0, &c__1, &c__2);
    form_(&c__1, sthed_1.sthead, &c__1, &c__2, &c__12);
    pad_(&c__1, &c__1, &c__3);
    form_(&c__1, sthed_1.sthead, &c__3, &c__4, &c__12);
    pad_(&c__1, &c__1, &c__3);
    form_(&c__1, sthed_1.sthead, &c__5, &c__8, &c__12);
    pad_(&c__1, &c__1, &c__2);
    form_(&c__1, sthed_1.sthead, &c__9, &c__12, &c__12);
    writel_(&c__0);
    i__1 = regall_1.sp;
    for (i__ = 1; i__ <= i__1; ++i__) {
	ip = regall_1.sp - i__ + 1;
	k = regall_1.prec[ip - 1];
	conout_(&c__0, &c__2, &ip, &c__10);
	conout_(&c__1, &c_n2, &k, &c__10);
	pad_(&c__1, &c__1, &c__1);
	j = regall_1.st[ip - 1];
	if (j == 0) {
	    goto L10200;
	}
	k = 30;
	if (j >= 0) {
	    goto L10100;
	}
	k = 12;
	j = -j;
L10100:
	pad_(&c__1, &k, &c__1);
	conout_(&c__1, &c__5, &j, &c__10);
	goto L10300;

L10200:
	pad_(&c__1, &c__1, &c__6);
L10300:
	pad_(&c__1, &c__1, &c__1);
	k = regall_1.rasn[ip - 1];
	for (j = 1; j <= 2; ++j) {
	    i__3 = 2 - j << 2;
	    i__2 = shr_(&k, &i__3);
	    l = right_(&i__2, &c__4) + 11;
	    if (l == 11) {
		l = 45;
	    }
	    pad_(&c__1, &c__1, &c__1);
/* L10400: */
	    pad_(&c__1, &l, &c__1);
	}

	k = regall_1.litv[ip - 1];
	if (k < 0) {
	    goto L10600;
	}
	l = 1;
	if (shr_(&k, &c__16) == 0) {
	    goto L10500;
	}
	l = 29;
	k = right_(&k, &c__16);
L10500:
	pad_(&c__1, &c__1, &c__1);
	pad_(&c__1, &l, &c__1);
	conout_(&c__1, &c__5, &k, &c__10);
L10600:
	writel_(&c__0);
    }
/*     WRITE REGISTERS */
    if (cntrl_1.contrl[11] < 2) {
	goto L10700;
    }
    for (i__ = 1; i__ <= 7; ++i__) {
	ip = regall_1.regs[i__ - 1];
	kp = regall_1.lock[i__ - 1];
	lp = regall_1.regv[i__ - 1];
	if (kp + ip + lp < 0) {
	    goto L10650;
	}
	pad_(&c__1, &c__1, &c__1);
	i__1 = i__ + 11;
	pad_(&c__1, &i__1, &c__1);
	pad_(&c__1, &c__42, &c__1);
	k = 32;
	if (kp == 1) {
	    k = 23;
	}
	pad_(&c__1, &k, &c__1);
	pad_(&c__1, &c__48, &c__1);
	if (ip == 0) {
	    goto L10610;
	}
	conout_(&c__1, &c__2, &ip, &c__10);
	goto L10620;
L10610:
	pad_(&c__1, &c__47, &c__1);
L10620:
	pad_(&c__1, &c__48, &c__1);
	if (lp < 0) {
	    goto L10630;
	}
	conout_(&c__2, &c_n10, &lp, &c__16);
	goto L10640;
L10630:
	pad_(&c__1, &c__47, &c__1);
L10640:
	pad_(&c__1, &c__43, &c__1);
L10650:
	;
    }
    writel_(&c__0);

L10700:
    k = 0;
    if (peep_1.lapol == 0) {
	goto L250;
    }
    for (j = 1; j <= 3; ++j) {
L110:
	i__ = gnc_(&c__0);
	if (i__ == 1) {
	    goto L110;
	}
	if (i__ >= 2 && i__ <= 33) {
	    goto L150;
	}
	error_(&c__127, &c__5);
	goto L99999;
L150:
	k = (k << 5) + (i__ - 2);
/* L200: */
    }

/*     COPY THE ELT JUST READ TO THE POLISH LOOK-AHEAD, AND */
/*     INTERPRET THE PREVIOUS ELT */

L250:
    i__ = k;
    k = peep_1.lapol;
    peep_1.lapol = i__;
/*     READ AGAIN (ONLY ON FIRST ARRIVAL HERE) IF ELT IS NULL */
    if (k < 0) {
	goto L10700;
    }

/*     CHECK FOR END OF CODE */
    if (k == 0) {
	goto L9000;
    }
    ++polcnt;
    typ = right_(&k, &c__3);
    val = shr_(&k, &c__3);
/*     $G=0 FOR NO TRACE, $G=1 GIVES LINES VS LOCS, */
/*     $G=2 YIELDS FULL INTERLIST OF I.L. */
    i__ = cntrl_1.contrl[17];
    if (i__ == 0) {
	goto L2000;
    }
    if (i__ > 1) {
	goto L900;
    }

/*     PRINT LINE NUMBER = CODE LOCATION, IF ALTERED */
    if (lline == cntrl_1.contrl[13] || lloc == code_1.codloc) {
	goto L2000;
    }
/*     CHANGED COMPLETELY, SO PRINT IT */
    lline = cntrl_1.contrl[13];
    lloc = code_1.codloc;
    i__ = 1;
    if (lcnt > 0) {
	goto L300;
    }
    lcnt = cntrl_1.contrl[33] / 12;
    i__ = 0;
L300:
    --lcnt;
    pad_(&i__, &c__1, &c__1);
    conout_(&c__1, &c_n4, &lline, &c__10);
    pad_(&c__1, &c__39, &c__1);
    conout_(&c__1, &c__4, &lloc, &c__16);
    goto L2000;

/*     OTHERWISE INTERLIST THE I.L. */
L900:
    conout_(&c__0, &c__5, &code_1.codloc, &c__10);
    pad_(&c__1, &c__1, &c__1);
    conout_(&c__1, &c__4, &code_1.codloc, &c__16);
    pad_(&c__1, &c__1, &c__1);
    conout_(&c__1, &c_n5, &polcnt, &c__10);
    pad_(&c__1, &c__1, &c__1);
    i__ = typ * 3 + 1;
    i__1 = i__ + 2;
    form_(&c__1, opcod_1.polchr, &i__, &i__1, &c__18);
    pad_(&c__1, &c__1, &c__1);
    i__ = typ + 1;
    j = 1;
    switch (i__) {
	case 1:  goto L1000;
	case 2:  goto L1001;
	case 3:  goto L1001;
	case 4:  goto L1001;
	case 5:  goto L1004;
	case 6:  goto L1004;
    }
L1000:
    j = opcod_1.opcval[val];
    for (i__ = 1; i__ <= 3; ++i__) {
	i__1 = (3 - i__) * 6;
	kp = shr_(&j, &i__1);
	i__1 = right_(&kp, &c__6);
	pad_(&c__1, &i__1, &c__1);
/* L400: */
    }

    goto L1100;

L1001:
    j = 30;
L1004:
    pad_(&c__1, &j, &c__1);
    conout_(&c__1, &c__5, &val, &c__10);
L1100:
    writel_(&c__0);

L2000:
    ++typ;
    ++regall_1.sp;
    if (regall_1.sp <= regall_1.maxsp) {
	goto L2100;
    }
/*         STACK OVERFLOW */
    error_(&c__128, &c__5);
    regall_1.sp = 1;
L2100:
    regall_1.prec[regall_1.sp - 1] = 0;
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.rasn[regall_1.sp - 1] = 0;
    regall_1.litv[regall_1.sp - 1] = -1;
    code_1.alter = 0;
    switch (typ) {
	case 1:  goto L3000;
	case 2:  goto L4000;
	case 3:  goto L5000;
	case 4:  goto L6000;
	case 5:  goto L7000;
	case 6:  goto L8000;
    }
/*     OPERATOR */
L3000:
    --regall_1.sp;
    operat_(&val);
    goto L100;
/*     LOAD ADDRESS */
L4000:
    if (regall_1.sp <= 1) {
	goto L4010;
    }
/*     CHECK FOR ACTIVE CONDITION CODE WHICH MUST BE CHANGED TO BOOLEAN */
    if (regall_1.rasn[regall_1.sp - 2] > 255) {
	i__1 = regall_1.sp - 1;
	cvcond_(&i__1);
    }
L4010:
    i__ = symbl_1.symbol[val - 1];
    j = symbl_1.symbol[i__ - 2];
    if (j >= 0) {
	goto L4500;
    }
/*     LOAD ADDRESS OF BASED VARIABLE.  CHANGE TO */
/*     LOAD VALUE OF THE BASE, USING THE VARIABLE'S PRECISION */
    i__2 = -j;
    i__1 = shr_(&i__2, &c__4);
    ibase = right_(&i__1, &c__4);
    val = symbl_1.symbol[i__ - 3];
    goto L5000;
L4500:
    setadr_(&val);
    goto L100;
/*     LOAD VALUE */
L5000:
    i__ = symbl_1.symbol[val - 1];
    j = symbl_1.symbol[i__ - 2];
    if (regall_1.sp <= 1) {
	goto L5010;
    }
/*     ALLOW ONLY A LABEL VARIABLE TO BE STACKED */
    if (abs(j) % 16 == types_1.label) {
	goto L5010;
    }
/*     CHECK FOR ACTIVE CONDITION CODE WHICH MUST BE CHANGED TO BOOLEAN */
    if (regall_1.rasn[regall_1.sp - 2] > 255) {
	i__1 = regall_1.sp - 1;
	cvcond_(&i__1);
    }
L5010:
/*     CHECK FOR CONDITION CODES */
    if (val > regall_1.intbas) {
	goto L5400;
    }
    if (val <= 4) {
	goto L5100;
    }
/*     MAY BE A CALL TO INPUT OR OUTPUT */
    if (val >= bifloc_2.firsti && val <= regall_1.intbas) {
	goto L5400;
    }
/*     CHECK FOR REFERENCE TO 'MEMORY' */
/*     ** NOTE THAT 'MEMORY' MUST BE AT LOCATION 5 IN THE SYMBOL TABLE ** */
    if (val == 5) {
	goto L5400;
    }
/*     ** NOTE THAT 'STACKPTR' MUST BE AT 6 IN SYM TAB */
    if (val == 6) {
	goto L5300;
    }
    error_(&c__129, &c__1);
    goto L100;
/*     CARRY ZERO MINUS PARITY */
/*     SET TO TRUE/CONDITION (1*16+VAL) */
L5100:
    regall_1.rasn[regall_1.sp - 1] = val + 16 << 8;
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.prec[regall_1.sp - 1] = 1;
    code_1.alter = 1;
    goto L100;
L5300:
/*     LOAD VALUE OF STACKPOINTER TO REGISTERS IMMEDIATELY */
    genreg_(&c__2, &ia, &ib);
    if (ib != 0) {
	goto L5310;
    }
    error_(&c__107, &c__5);
    goto L100;
L5310:
    regall_1.rasn[regall_1.sp - 1] = (ib << 4) + ia;
    regall_1.litv[regall_1.sp - 1] = -1;
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.regs[ia - 1] = regall_1.sp;
    regall_1.regs[ib - 1] = regall_1.sp;
    regall_1.prec[regall_1.sp - 1] = 2;
    emit_(&ops_1.lxi, &ops_1.rh, &c__0);
    emit_(&ops_1.dad, &ops_1.rsp, &c__0);
    emit_(&ops_1.ld, &ia, &ops_1.rl);
    emit_(&ops_1.ld, &ib, &ops_1.rh);
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
    code_1.alter = 1;
    goto L100;
L5400:
    if (j >= 0) {
	goto L5500;
    }

/*     VALUE REFERENCE TO BASED VARIABLE. FIRST INSURE THAT THIS */
/*     IS NOT A LENGTH ATTRIBUTE REFERENCE, (I.E., THE VARIABLE IS */
/*     NOT AN ACTUAL PARAMETER FOR A CALL ON LENGTH OR LAST) BY */
/*     INSURING THAT THE NEXT POLISH ELT IS NOT AN ADDRESS */
/*     REFERENCE TO SYMBOL (LENGTH+1) OR (LAST+1) */
/*     NOTE THAT THIS ASSUMES LENGTH AND LAST ARE SYMBOL NUMBERS */
/*     18 AND 19 */

    if (peep_1.lapol == 153 || peep_1.lapol == 161) {
	goto L5500;
    }
/*     LOAD VALUE OF BASE VARIABLE.  CHANGE TO LOAD */
/*     VALUE OF BASE, FOLLOWED BY A LOD OP. */
    i__2 = -j;
    i__1 = shr_(&i__2, &c__4);
    ibase = right_(&i__1, &c__4) + 16;
    val = symbl_1.symbol[i__ - 3];
    i__ = symbl_1.symbol[val - 1];
    j = symbl_1.symbol[i__ - 2];
L5500:
    code_1.alter = 1;
/*         EXAMINE ATTRIBUTES */
    regall_1.st[regall_1.sp - 1] = val;
    i__ = right_(&j, &c__4);
    j = shr_(&j, &c__4);
    k = right_(&j, &c__4);
    if (ibase > 0) {
	k = ibase % 16;
    }
    regall_1.prec[regall_1.sp - 1] = k;
    if (i__ < types_1.liter - 1) {
	goto L5800;
    }
    if (k > 0 && k < 3) {
	goto L5900;
    }
    error_(&c__130, &c__1);
    goto L100;
L5900:
    i__1 = shr_(&j, &c__4);
    regall_1.litv[regall_1.sp - 1] = right_(&i__1, &c__16);
L5800:
/*     CHECK FOR BASE ADDRESS WHICH MUST BE LOADED */
    if (ibase < 16) {
	goto L100;
    }
/*     MUST BE A BASED VARIABLE VALUE REFERENCE. */
/*     LOAD THE VALUE OF THE BASE AND FOLLOW IT BY */
/*     A LOAD OPERATION. */
    k = regall_1.prec[regall_1.sp - 1];
/*     MARK AS A BYTE LOAD FOR THE LOD OPERATION IN OPERAT */
/*     LEAVES 2 IF DOUBLE BYTE RESULT AND 6 (=2 MOD 4) IF SINGLE BYTE */
    regall_1.prec[regall_1.sp - 1] = 10 - (k << 2);
    operat_(&ilcod_1.lod);
    goto L100;

/*     DEFINE LOCATION */
L6000:
/*     MARK LAST REGISTER LOAD NIL */
    peep_1.lastrg = 0;
    peep_1.lastex = 0;
    peep_1.lastin = 0;
    peep_1.lastir = 0;
    --regall_1.sp;
/*     SAVE REGISTERS IF THIS IS A PROC OR A LABEL WHICH WAS */
/*     REFERENCED IN A GO-TO STATEMENT OR WAS COMPILER-GENERATED. */
    ip = symbl_1.symbol[val - 1];
    i__ = (i__1 = symbl_1.symbol[ip - 2], abs(i__1));

/*     SAVE THIS DEF SYMBOL NUMBER AND THE LITERAL VALUES OF THE */
/*     H AND L REGISTERS FOR POSSIBLE TRA CHAIN STRAIGHTENING. */

    if (right_(&i__, &c__4) != types_1.label) {
	goto L6001;
    }
    xfropt_1.defsym = val;
    xfropt_1.defrh = regall_1.regv[ops_1.rh - 1];
    xfropt_1.defrl = regall_1.regv[ops_1.rl - 1];

/*     WE MAY CONVERT THE SEQUENCE */

/*          TRC L, TRA/PRO/RET, DEF L */

/*     TO AN EQUIVALENT CONDITIONAL TRA/PRO/RET... */

L6001:
    if (i__ / 256 != 1) {
	goto L6004;
    }
    if (xfropt_1.tstloc != code_1.codloc) {
	goto L6004;
    }
    if (xfropt_1.conloc != xfropt_1.xfrloc - 3) {
	goto L6004;
    }
    j = -symbl_1.symbol[ip - 1];
    i__1 = shr_(&j, &c__2);
    k = right_(&i__1, &c__14);
    if (k != xfropt_1.conloc + 1) {
	goto L6004;
    }


/*     ADJUST BACKSTUFFING CHAIN FOR JMP OR CALL */

    if (xfropt_1.xfrsym <= 0) {
	goto L6002;
    }
    k = symbl_1.symbol[xfropt_1.xfrsym - 1];
/*     DECREMENT BACKSTUFF LOCATION BY 3 */
    symbl_1.symbol[k - 1] += 12;
L6002:
/*     ARRIVE HERE WITH THE CONFIGURATION TRC...DEF */

    i__1 = shr_(&j, &c__16);
    symbl_1.symbol[ip - 1] = -(shl_(&i__1, &c__16) + right_(&j, &c__2));
    k = (i__1 = symbl_1.symbol[ip - 2], abs(i__1)) % 256;
    if (symbl_1.symbol[ip - 2] < 0) {
	k = -k;
    }
    symbl_1.symbol[ip - 2] = k;
    j = get_(&xfropt_1.conloc);
    j = get_(&xfropt_1.conloc);
    j = shr_(&j, &c__3);
    k = (j % 2 + 1) % 2;
    i__1 = shr_(&j, &c__1);
    k = shl_(&i__1, &c__1) + k;
    j = get_(&xfropt_1.xfrloc);
    i__1 = shr_(&j, &c__1);
    l = right_(&i__1, &c__2);
    j = shl_(&k, &c__3) + shl_(&l, &c__1);
L6003:
    put_(&xfropt_1.conloc, &j);
    ++xfropt_1.conloc;
    ++xfropt_1.xfrloc;
    j = get_(&xfropt_1.xfrloc);
    if (xfropt_1.xfrloc != code_1.codloc) {
	goto L6003;
    }
    code_1.codloc = xfropt_1.conloc;
    memory_1.membot += -3;
    xfropt_1.conloc = -1;
    xfropt_1.xfrloc = -1;
    xfropt_1.tstloc = -1;

/*     NOTICE THAT DEFRH AND DEFRL ARE NOW INCORRECT */
/*     DEFSYM=0 PREVENTS USE OF THESE VARIABLES... */
/*     ... IF A TRA IMMEDIATELY FOLLOWS */

    xfropt_1.defsym = 0;
L6004:
    j = right_(&i__, &c__4);
    if (j != types_1.label) {
	goto L6005;
    }
/*         LABEL FOUND.  CHECK FOR REFERENCE TO LABEL */
    i__ /= 256;
    if (i__ == 0) {
	goto L6020;
    }
/*     CHECK FOR SINGLE REFERENCE, NO CONFLICT WITH H AND L */
    if (i__ != 1) {
	goto L6010;
    }
    i__ = symbl_1.symbol[ip - 3];
/*     CHECK FOR PREVIOUS REFERENCE  FORWARD */
    if (i__ == 0) {
	goto L6010;
    }
    l = i__ % 256;
    i__ /= 256;
    j = i__ % 512;
    i__ /= 512;
    if (i__ % 2 != 1) {
	l = -1;
    }
    if (i__ / 2 % 2 != 1) {
	j = -1;
    }
/*     J IS H REG, L IS L REG */
    regall_1.lock[5] = 1;
    regall_1.lock[6] = 1;
    saver_();
/*     COMPARE OLD HL WITH NEW HL */
    regall_1.lock[5] = 0;
    regall_1.lock[6] = 0;
    k = regall_1.regv[5];
    regall_1.regv[5] = -1;
    if (k == -255 || k == j) {
	regall_1.regv[5] = j;
    }
    k = regall_1.regv[6];
    regall_1.regv[6] = -1;
    if (k == -255 || k == l) {
	regall_1.regv[6] = l;
    }
    goto L6020;

/*     OTHERWISE NOT A LABEL, CHECK FOR PROCEDURE ENTRY */
L6005:
    if (j != types_1.proc) {
	goto L6010;
    }
/*         SET UP PROCEDURE STACK FOR PROCEDURE ENTRY */
    ++pstack_1.prsp;
    if (pstack_1.prsp <= pstack_1.prsmax) {
	goto L6008;
    }
    error_(&c__145, &c__5);
    goto L6010;
L6008:
    j = ip - 2;
    pstack_1.prstk[pstack_1.prsp - 1] = j;
/*         MARK H AND L AS UNALTERED INITIALLY */
/*          /  1B  /  1B  /  1B  /  1B  /  9B  /  8B  / */
/*          /H UNAL/L UNAL/H VALD/L VALD/H VALU/L VALU/ */
/*          ------------------------------------------- */
    symbl_1.symbol[j - 1] = shl_(&c__3, &c__19);
    saver_();
    regall_1.regv[5] = -254;
    regall_1.regv[6] = -254;
    k = code_1.codloc;
/*         SET UP STACK DEPTH COUNTERS */
    pstack_1.maxdep[pstack_1.prsp] = 0;
    pstack_1.curdep[pstack_1.prsp] = 0;
    for (i__ = 1; i__ <= 8; ++i__) {
	if (val != inter_1.intpro[i__ - 1]) {
	    goto L6009;
	}
/*         INTERRUPT PROCEDURE IS MARKED WITH HO 1 */
	pstack_1.prstk[pstack_1.prsp - 1] = j + 65536;
	emit_(&ops_1.push, &ops_1.rh, &c__0);
	emit_(&ops_1.push, &ops_1.rd, &c__0);
	emit_(&ops_1.push, &ops_1.rb, &c__0);
	emit_(&ops_1.push, &ops_1.ra, &c__0);
	stack_(&c__4);
L6009:
	;
    }
    goto L6025;

L6010:
    saver_();

L6020:
/*         LABEL IS RESOLVED.  LAST TWO BITS OF ENTRY MUST BE 01 */
    k = code_1.codloc;
L6025:
    i__ = -symbl_1.symbol[ip - 1];
    j = i__ % 4;
    i__ /= 4;
    if (j == 1) {
	goto L6200;
    }
    error_(&c__131, &c__1);
L6200:
    symbl_1.symbol[ip - 1] = -(shl_(&k, &c__16) + shl_(&i__, &c__2) + 3);

/*     NOW CHECK FOR PROCEDURE ENTRY POINT */

    i__ = symbl_1.symbol[ip - 2];
    if (right_(&i__, &c__4) != types_1.proc) {
	goto L100;
    }
    i__ = shr_(&i__, &c__8);

/*     BUILD RECEIVING SEQUENCE FOR REGISTER PARAMETERS */

    if (i__ < 1) {
	goto L100;
    }
    k = i__ - 2;
    if (k < 0) {
	k = 0;
    }
    if (i__ > 2) {
	i__ = 2;
    }
    i__1 = i__;
    for (j = 1; j <= i__1; ++j) {
	++regall_1.sp;
	if (regall_1.sp <= regall_1.maxsp) {
	    goto L6310;
	}
	error_(&c__113, &c__5);
	regall_1.sp = 1;
/*     (RD,RE) = 69    (RB,RC) = 35 */
L6310:
	if (j == 1) {
	    l = 35;
	}
	if (j == 2) {
	    l = 69;
	}
	regall_1.rasn[regall_1.sp - 1] = l;
	regall_1.st[regall_1.sp - 1] = 0;
	regall_1.litv[regall_1.sp - 1] = -1;
	regall_1.prec[regall_1.sp - 1] = 2;
	++regall_1.sp;
	if (regall_1.sp <= regall_1.maxsp) {
	    goto L6320;
	}
	error_(&c__113, &c__5);
	regall_1.sp = 1;
L6320:
	regall_1.rasn[regall_1.sp - 1] = 0;
	regall_1.litv[regall_1.sp - 1] = -1;
	i__2 = val + k + j;
	setadr_(&i__2);
	operat_(&ilcod_1.std);
/* L6300: */
    }
    goto L100;
/*     LITERAL VALUE */
L7000:
    if (regall_1.sp <= 1) {
	goto L7010;
    }
/*     CHECK FOR ACTIVE CONDITION CODE WHICH MUST BE CHANGED TO BOOLEAN */
    if (regall_1.rasn[regall_1.sp - 2] > 255) {
	i__1 = regall_1.sp - 1;
	cvcond_(&i__1);
    }
L7010:
    code_1.alter = 1;
    regall_1.litv[regall_1.sp - 1] = val;
    regall_1.prec[regall_1.sp - 1] = 1;
    if (regall_1.litv[regall_1.sp - 1] > 255) {
	regall_1.prec[regall_1.sp - 1] = 2;
    }
    goto L100;
/*     LINE NUMBER */
L8000:
    cntrl_1.contrl[13] = val;
    --regall_1.sp;
    goto L100;
L9000:
    emit_(&ops_1.ei, &c__0, &c__0);
    emit_(&ops_1.halt, &c__0, &c__0);

/*     MAY BE LINE/LOC'S LEFT IN OUTPUT BUFFER */
    if (cntrl_1.contrl[17] != 0) {
	writel_(&c__0);
    }

L99999:
    cntrl_1.contrl[19] = m;
    return 0;
} /* readcd_ */

/* Subroutine */ int operat_(integer *val)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    integer pow_ii(integer *, integer *);

    /* Local variables */
    static integer i__, j, k, l, m, ia, ib, ic, id, ih, iq, jp, ip, il, jh, 
	    jl, it, kp, ml, mh, lp;
    extern integer get_(integer *);
    static integer jph, icy;
    extern integer shl_(integer *, integer *), shr_(integer *, integer *);
    static integer iop;
    extern /* Subroutine */ int put_(integer *, integer *);
    static integer iop2, icom;
    extern /* Subroutine */ int exch_(void), emit_(integer *, integer *, 
	    integer *);
    extern integer imin_(integer *, integer *), chain_(integer *, integer *);
    extern /* Subroutine */ int loadv_(integer *, integer *), stack_(integer *
	    );
    extern integer right_(integer *, integer *);
    extern /* Subroutine */ int saver_(void), apply_(integer *, integer *, 
	    integer *, integer *), error_(integer *, integer *), unary_(
	    integer *), delete_(integer *), emitbf_(integer *), genreg_(
	    integer *, integer *, integer *), inldat_(void), cvcond_(integer *
	    ), ustack_(void), gensto_(integer *);

/*     ADD ADC SUB SBC MUL DIV MOD NEG AND IOR */
/*     XOR NOT EQL LSS GTR NEQ LEQ GEQ INX TRA */
/*     TRC PRO RET STO STD XCH DEL CAT LOD BIF */
/*     INC CSE END ENB ENP HAL RTL RTR SFL SFR */
/*     HIV LOV CVA ORG AX1 AX2 AX3 */
    icy = 0;
    icom = 0;
    iq = 0;
    switch (*val) {
	case 1:  goto L1000;
	case 2:  goto L2000;
	case 3:  goto L3000;
	case 4:  goto L3500;
	case 5:  goto L4000;
	case 6:  goto L5000;
	case 7:  goto L6000;
	case 8:  goto L99999;
	case 9:  goto L9000;
	case 10:  goto L10000;
	case 11:  goto L11000;
	case 12:  goto L12000;
	case 13:  goto L13000;
	case 14:  goto L14000;
	case 15:  goto L15000;
	case 16:  goto L16000;
	case 17:  goto L17000;
	case 18:  goto L18000;
	case 19:  goto L19000;
	case 20:  goto L20000;
	case 21:  goto L21000;
	case 22:  goto L22000;
	case 23:  goto L23000;
	case 24:  goto L24000;
	case 25:  goto L24000;
	case 26:  goto L26000;
	case 27:  goto L27000;
	case 28:  goto L28000;
	case 29:  goto L29000;
	case 30:  goto L99999;
	case 31:  goto L31000;
	case 32:  goto L32000;
	case 33:  goto L99999;
	case 34:  goto L99999;
	case 35:  goto L99999;
	case 36:  goto L36000;
	case 37:  goto L37000;
	case 38:  goto L37000;
	case 39:  goto L37000;
	case 40:  goto L37000;
	case 41:  goto L37000;
	case 42:  goto L37000;
	case 43:  goto L43000;
	case 44:  goto L44000;
	case 45:  goto L45000;
	case 46:  goto L45100;
	case 47:  goto L45200;
	case 48:  goto L45500;
	case 49:  goto L46000;
	case 50:  goto L99999;
    }

/*     ADD */
L1000:
/*     MAY DO THE ADD IN H AND L (USING INX OPERATOR) */
    if (regall_1.prec[regall_1.sp - 1] != 1) {
	exch_();
    }
    if (regall_1.prec[regall_1.sp - 2] != 1) {
	goto L1100;
    }
    exch_();
    icy = 1;
    iop = ops_1.ad;
    iop2 = ops_1.ac;
    icom = 1;
    goto L88888;
L1100:
/*     SET PREC = 1 FOR INX */
    jp = 1;
    goto L19001;

/*     ADC */
L2000:
    icy = 1;
    iop = ops_1.ac;
    iop2 = ops_1.ac;
    icom = 1;
    goto L88888;

/*     SUB */
L3000:
/*     CHANGE ADDRESS VALUE - 1 TO ADDRESS VALUE + 65535 AND APPLY ADD */
    if (regall_1.prec[regall_1.sp - 2] == 1 || regall_1.litv[regall_1.sp - 1] 
	    != 1) {
	goto L3100;
    }
    regall_1.litv[regall_1.sp - 1] = 65535;
    regall_1.prec[regall_1.sp - 1] = 2;
    goto L1100;
L3100:
    icy = 1;
    iop = ops_1.su;
    iop2 = ops_1.sb;
    goto L88888;

/*     SBC */
L3500:
    icy = 1;
    iop = ops_1.sb;
    iop2 = ops_1.sb;
    goto L88888;

/*     MUL */
L4000:
    i__ = 1;
    j = 2;
    goto L6100;
/*     DIV */
L5000:
    i__ = 2;
    j = 1;
    goto L6100;
/*     MOD */
L6000:
    i__ = 2;
    j = 2;
L6100:
/*     CLEAR CONDITION CODE */
    if (regall_1.rasn[regall_1.sp - 1] > 255) {
	cvcond_(&regall_1.sp);
    }
/*     CLEAR PENDING STORE */
    if (regall_1.regs[ops_1.ra - 1] != 0) {
	emit_(&ops_1.ld, &regall_1.regs[ops_1.ra - 1], &ops_1.ra);
    }
    regall_1.regs[ops_1.ra - 1] = 0;
/*     LOCK ANY CORRECTLY ASSIGNED REGISTERS */
/*     ....AND STORE THE REMAINING REGISTERS. */
    if (regall_1.rasn[regall_1.sp - 1] % 16 == ops_1.re) {
	regall_1.lock[ops_1.re - 1] = 1;
    }
    if (regall_1.rasn[regall_1.sp - 1] / 16 == ops_1.rd) {
	regall_1.lock[ops_1.rd - 1] = 1;
    }
    if (regall_1.rasn[regall_1.sp - 2] % 16 == ops_1.rc) {
	regall_1.lock[ops_1.rc - 1] = 1;
    }
    if (regall_1.rasn[regall_1.sp - 2] / 16 == ops_1.rb) {
	regall_1.lock[ops_1.rb - 1] = 1;
    }
    saver_();
/*     MARK REGISTER C USED. */
    if (regall_1.regs[ops_1.rc - 1] == 0) {
	regall_1.regs[ops_1.rc - 1] = -1;
    }
/*     LOAD TOP OF STACK INTO REGISTERS D AND E. */
    loadv_(&regall_1.sp, &c__0);
    if (regall_1.prec[regall_1.sp - 1] == 1) {
	emit_(&ops_1.ld, &ops_1.rd, &c__0);
    }
/*     NOW DEASSIGN REGISTER C UNLESS CORRECTLY LOADED. */
    if (regall_1.regs[ops_1.rc - 1] == -1) {
	regall_1.regs[ops_1.rc - 1] = 0;
    }
/*     LOAD T.O.S. - 1 INTO REGISTERS B AND C. */
    i__1 = regall_1.sp - 1;
    loadv_(&i__1, &c__0);
    if (regall_1.prec[regall_1.sp - 2] == 1) {
	emit_(&ops_1.ld, &ops_1.rb, &c__0);
    }
    delete_(&c__2);

/*     CALL THE BUILT-IN FUNCTION */
    emitbf_(&i__);
/*     REQUIRES 2 LEVELS IN STACK FOR BIF (CALL AND TEMP.) */
    stack_(&c__2);
    ustack_();
    ustack_();
/*    AND THEN RETRIEVE RESULTS */
    for (k = 1; k <= 7; ++k) {
/* L6500: */
	regall_1.lock[k - 1] = 0;
    }
/*     CANNOT PREDICT WHERE REGISTERS H AND L WILL END UP */
    regall_1.regv[ops_1.rl - 1] = -1;
    regall_1.regv[ops_1.rh - 1] = -1;
    ++regall_1.sp;
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.prec[regall_1.sp - 1] = 2;
    regall_1.litv[regall_1.sp - 1] = -1;
    if (j == 2) {
	goto L6600;
    }
    regall_1.rasn[regall_1.sp - 1] = (ops_1.rb << 4) + ops_1.rc;
    regall_1.regs[ops_1.rb - 1] = regall_1.sp;
    regall_1.regs[ops_1.rc - 1] = regall_1.sp;
    goto L99991;
L6600:
    regall_1.rasn[regall_1.sp - 1] = (ops_1.rd << 4) + ops_1.re;
    regall_1.regs[ops_1.rd - 1] = regall_1.sp;
    regall_1.regs[ops_1.re - 1] = regall_1.sp;
    goto L99991;

/*     AND */
L9000:
    iop = ops_1.nd;
L9100:
    icom = 1;
    goto L88887;

/*     IOR */
L10000:
    iop = ops_1.or;
    goto L9100;

/*     XOR */
L11000:
    iop = ops_1.xr;
    goto L9100;

/*     NEGATE (COMPLEMENT THE ENTIRE NUMBER) */
L12000:
    i__ = regall_1.rasn[regall_1.sp - 1];
    if (i__ <= 255) {
	goto L12100;
    }

/*     CONDITION CODE - CHANGE PARITY */
    j = 1 - i__ / 4096;
    regall_1.rasn[regall_1.sp - 1] = (j << 12) + i__ % 4096;
    goto L99991;

L12100:
/*     PERFORM XOR WITH 255 OR 65535 (BYTE OR ADDRESS) */
    i__ = regall_1.prec[regall_1.sp - 1];
    j = pow_ii(&c__256, &i__);
    ++regall_1.sp;
    regall_1.litv[regall_1.sp - 1] = j - 1;
    regall_1.prec[regall_1.sp - 1] = i__;
    goto L11000;

L13000:
/*     EQUAL TEST */
    if (regall_1.prec[regall_1.sp - 1] + regall_1.prec[regall_1.sp - 2] > 2) {
	goto L13200;
    }

/*     MARK AS TRUE/ZERO (1*16+2) */
    j = 18;
L13050:
    icom = 1;
L13080:
    iop = ops_1.su;
L13090:
    iop2 = 0;
/* L13100: */
    apply_(&iop, &iop2, &icom, &icy);
/*         MARK AS CONDITION CODE */
    regall_1.rasn[regall_1.sp - 1] = (j << 8) + regall_1.rasn[regall_1.sp - 1]
	    ;
    goto L99991;

/*     DOUBLE BYTE EQUAL */
L13200:
    iq = 1;
/*     MARK AS TRUE/ZERO (1*16 + 2) */
    j = 18;
L13300:
    icom = 1;
L13400:
    iop = ops_1.su;
    iop2 = ops_1.sb;
    icy = 1;
    apply_(&iop, &iop2, &icom, &icy);
/*     CHANGE TO CONDITION CODE */
    i__ = regall_1.rasn[regall_1.sp - 1];
    ip = i__ % 16;
    if (iq == 1) {
	emit_(&ops_1.or, &ip, &c__0);
    }

/*     GET RID OF HIGH ORDER REGISTER IN THE RESULT */
    regall_1.regs[0] = ip;
    regall_1.rasn[regall_1.sp - 1] = (j << 8) + ip;
    regall_1.prec[regall_1.sp - 1] = 1;
    regall_1.litv[regall_1.sp - 1] = -1;
    regall_1.st[regall_1.sp - 1] = 0;
    j = i__ / 16 % 16;
    if (j == 0) {
	goto L99991;
    }
    regall_1.lock[j - 1] = 0;
    regall_1.regs[j - 1] = 0;
    regall_1.regv[j - 1] = -1;
    goto L99991;

L14000:
/*     LSS - SET TO TRUE/CARRY (1*16+1) */
    j = 17;
    if (regall_1.prec[regall_1.sp - 1] + regall_1.prec[regall_1.sp - 2] > 2) {
	goto L13400;
    }
L14010:
    if (regall_1.litv[regall_1.sp - 1] != 1) {
	goto L13080;
    }
    iop = ops_1.cp;
    goto L13090;

L15000:
/*     GTR - CHANGE TO LSS */
    exch_();
    goto L14000;

L16000:
/*     NEQ */
/*     MARK AS FALSE/ZERO (0*16+2) */
    j = 2;
    iq = 1;
    if (regall_1.prec[regall_1.sp - 1] + regall_1.prec[regall_1.sp - 2] > 2) {
	goto L13300;
    }
    goto L13050;

L17000:
/*     LEQ - CHANGE TO GEQ */
    exch_();

L18000:
/*     GEQ - SET TO FALSE/CARRY (0*16+1) */
    j = 1;
    if (regall_1.prec[regall_1.sp - 1] + regall_1.prec[regall_1.sp - 2] > 2) {
	goto L13400;
    }
    goto L14010;

/*     INX */
L19000:
    jp = regall_1.prec[regall_1.sp - 2];
/*     INX IS ALSO USED FOR ADDING ADDRESS VALUES, ENTERING FROM ADD */
L19001:
/*     BASE MAY BE INDEXED BY ZERO... */
    if (regall_1.litv[regall_1.sp - 1] != 0) {
	goto L19002;
    }
/*         JUST DELETE THE INDEX AND IGNORE THE INX OPERATOR */
    delete_(&c__1);
    goto L99991;
L19002:
    if (regall_1.rasn[regall_1.sp - 1] > 255) {
	cvcond_(&regall_1.sp);
    }
    j = regall_1.regs[0];
    ih = regall_1.rasn[regall_1.sp - 1];
    il = ih % 16;
    ih /= 16;
    jh = regall_1.rasn[regall_1.sp - 2];
    jl = jh % 16;
    jh /= 16;
/*     CHECK FOR PENDING STORE TO BASE OR INDEX */
    if (j == 0 || j != jh && j != jl && j != ih && j != il) {
	goto L19010;
    }
    emit_(&ops_1.ld, &j, &ops_1.ra);
    regall_1.regs[0] = 0;
L19010:
/*     MAKE SURE THAT D AND E ARE AVAILABLE */
    if (regall_1.regs[ops_1.re - 1] == 0 && regall_1.regs[ops_1.rd - 1] == 0) 
	    {
	goto L19020;
    }
    if (il == ops_1.re || jl == ops_1.re) {
	goto L19020;
    }
/*     MARK ALL REGISTERS FREE */
    if (il != 0) {
	regall_1.regs[il - 1] = 0;
    }
    if (jl != 0) {
	regall_1.regs[jl - 1] = 0;
    }
    genreg_(&c__2, &ia, &ib);
    regall_1.regs[ia - 1] = 1;
    genreg_(&c__2, &ic, &ib);
    regall_1.regs[ia - 1] = 0;
/*     ALL REGS ARE CLEARED EXCEPT BASE AND INDEX, IF ALLOCATED. */
    if (il != 0) {
	regall_1.regs[il - 1] = regall_1.sp;
    }
    if (jl != 0) {
	regall_1.regs[jl - 1] = regall_1.sp - 1;
    }
/*     GET INDEX FROM MEMORY, IF NECESSARY */
L19020:
/*     IF LITERAL 1 OR -1, USE INX OR DCX */
    if (regall_1.litv[regall_1.sp - 1] == 1 || regall_1.litv[regall_1.sp - 1] 
	    == 65535) {
	goto L19040;
    }
/*     IF THE INDEX IS CONSTANT, AND THE BASE AN ADDRESS VARIABLE, */
/*     DOUBLE THE LITERAL VALUE AT COMPILE TIME */
    if (regall_1.litv[regall_1.sp - 1] < 0 || jp == 1) {
	goto L19030;
    }
    regall_1.litv[regall_1.sp - 1] += regall_1.litv[regall_1.sp - 1];
    jp = 1;
L19030:
    i__ = 0;
    if (regall_1.litv[regall_1.sp - 1] >= 0) {
	i__ = 3;
    }
    loadv_(&regall_1.sp, &i__);
L19040:
/*     IF THE INDEX WAS ALREADY IN THE REGISTERS, MAY */
/*     HAVE TO EXTEND PRECISION TO ADDRESS. */
    ih = regall_1.rasn[regall_1.sp - 1];
    il = ih % 16;
    ih /= 16;
    if (il == 0 || ih != 0) {
	goto L19050;
    }
    ih = il - 1;
    emit_(&ops_1.ld, &ih, &c__0);
L19050:
    i__ = ops_1.dad;
    if (regall_1.litv[regall_1.sp - 1] == 1) {
	i__ = ops_1.incx;
    }
    if (regall_1.litv[regall_1.sp - 1] == 65535) {
	i__ = ops_1.dcx;
    }
    if (ih == 0) {
	ih = ops_1.rh;
    }
/*     DELETE THE INDEX.  (NOTE THAT SP WILL THEN POINT TO THE BASE) */
    delete_(&c__1);
/*     LOAD THE BASE INTO THE H AND L REGISTERS */
    loadv_(&regall_1.sp, &c__5);
/*     ADD THE BASE AND INDEX */
    emit_(&i__, &ih, &c__0);
/*     AND ADD INDEX AGAIN IF BASE IS AN ADDRESS VARIABLE. */
    if (jp != 1) {
	emit_(&i__, &ih, &c__0);
    }
    emit_(&ops_1.xchg, &c__0, &c__0);
/*     NOTE XCHG HERE AND REMOVE WITH PEEPHOLE OPTIMIZATION LATER */

    i__ = regall_1.prec[regall_1.sp - 1];
    delete_(&c__1);
    ++regall_1.sp;
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.prec[regall_1.sp - 1] = i__;
    regall_1.litv[regall_1.sp - 1] = -1;
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
    regall_1.rasn[regall_1.sp - 1] = (ops_1.rd << 4) + ops_1.re;
    regall_1.regs[ops_1.rd - 1] = regall_1.sp;
    regall_1.regs[ops_1.re - 1] = regall_1.sp;
    goto L99991;

/*     TRA -   CHECK STACK FOR SIMPLE LABEL VARIABLE */
L20000:
    iop = 1;
/*     IN CASE THERE ARE ANY PENDING VALUES ... */
    regall_1.lock[5] = 1;
    regall_1.lock[6] = 1;
    saver_();
    regall_1.lock[5] = 0;
    regall_1.lock[6] = 0;
/*     THIS MAY BE A JUMP TO AN ABSOLUTE ADDRESS */
    m = regall_1.litv[regall_1.sp - 1];
    if (m < 0) {
	goto L20050;
    }
/*     ABSOLUTE JUMP - PROBABLY TO ASSEMBLY LANGUAGE SUBRTNE... */
/*     ...SO MAKE H AND L REGISTERS UNKNOWN */
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
    emit_(&ops_1.jmp, &m, &c__0);
    delete_(&c__1);
    goto L99991;
L20050:
    i__ = regall_1.st[regall_1.sp - 1];
    if (i__ > 0) {
	goto L20100;
    }
    if (iop == 1 && i__ == 0) {
	goto L20700;
    }
/*         COULD BE A COMPUTED ADDRESS */
    error_(&c__134, &c__1);
    goto L99990;
L20100:
    i__ = symbl_1.symbol[i__ - 1];
    j = symbl_1.symbol[i__ - 2];
    j = right_(&j, &c__4);
/*         MAY BE A SIMPLE VARIABLE */
    if (iop == 1 && j == types_1.varb) {
	goto L20700;
    }
    if (iop == 3 && j == types_1.proc || j == types_1.label) {
	goto L20200;
    }
    error_(&c__135, &c__1);
    goto L99990;
L20200:
    j = -symbl_1.symbol[i__ - 1];
    m = shr_(&j, &c__16);
    if (iop != 1) {
	goto L20206;
    }
    it = (i__1 = symbl_1.symbol[i__ - 2], abs(i__1));
    i__1 = shr_(&it, &c__4);
    it = right_(&i__1, &c__4);
/*          IT IS TYPE OF LABEL... */
/*          3 IS USER-DEFINED OUTER BLOCK, 4 IS USER DEFINED */
/*          NOT OUTER BLOCK, 5 IS COMPILER DEFINED */
    if (it != 5) {
	goto L20206;
    }

/*         THIS TRA IS ONE OF A CHAIN OF COMPILER GENERATED */
/*         TRA'S - STRAIGHTEN THE CHAIN IF NO CODE HAS BEEN */
/*         GENERATED SINCE THE PREVIOUS DEF. */

    if (xfropt_1.defsym <= 0) {
	goto L20206;
    }
    k = symbl_1.symbol[xfropt_1.defsym - 1];
    i__1 = shr_(&symbl_1.symbol[k - 2], &c__4);
    if (right_(&i__1, &c__4) != 5) {
	goto L20206;
    }
    l = -symbl_1.symbol[k - 1];
    jp = shr_(&l, &c__16);
    if (jp != code_1.codloc) {
	goto L20205;
    }

/*         ADJUST THE REFERENCE COUNTS AND OPTIMIZATION */
/*         INFORMATION FOR BOTH DEF'S. */

    i__2 = (i__1 = symbl_1.symbol[k - 2], abs(i__1));
    ia = shr_(&i__2, &c__8);
    ib = 0;
    if (ia == 1) {
	ib = symbl_1.symbol[k - 3];
    }
    if (xfropt_1.defrh == -255) {
	--ia;
    }
    symbl_1.symbol[k - 2] = 84;
/*         I.E., ZERO REFERENCES TO COMPILER GENERATED LABEL */
    i__2 = (i__1 = symbl_1.symbol[i__ - 2], abs(i__1));
    if (shr_(&i__2, &c__8) == 1) {
	symbl_1.symbol[i__ - 3] = ib;
    }
    symbl_1.symbol[i__ - 2] += ia << 8;
/*         CORRECTED REFERENCE COUNT FOR OBJECT OF THE DEF */

/*         MERGE THE BACKSTUFFING CHAINS */

L20201:
    i__1 = shr_(&l, &c__2);
    ia = right_(&i__1, &c__14);
    if (ia == 0) {
	goto L20203;
    }
    i__1 = ia + 1;
    ib = get_(&ia) + (get_(&i__1) << 8);
    l = shl_(&jp, &c__16) + shl_(&ib, &c__2) + right_(&l, &c__2);
    symbl_1.symbol[k - 1] = -l;
    i__1 = shr_(&j, &c__2);
    ip = right_(&i__1, &c__14);
    i__1 = ip % 256;
    put_(&ia, &i__1);
    i__1 = ia + 1;
    i__2 = ip / 256;
    put_(&i__1, &i__2);
    j = shl_(&m, &c__16) + shl_(&ia, &c__2) + right_(&j, &c__2);
    symbl_1.symbol[i__ - 1] = -j;
    goto L20201;
L20203:

/*         EQUATE THE DEFS */

    i__1 = symbl_1.sytop;
    for (ia = 1; ia <= i__1; ++ia) {
	if (symbl_1.symbol[ia - 1] == k) {
	    symbl_1.symbol[ia - 1] = i__;
	}
/* L20202: */
    }

/*         OMIT THE TRA IF NO PATH TO IT */

/* L20204: */
    regall_1.regv[ops_1.rh - 1] = xfropt_1.defrh;
    regall_1.regv[ops_1.rl - 1] = xfropt_1.defrl;
L20205:
    if (regall_1.regv[ops_1.rh - 1] != -255) {
	goto L20206;
    }
    delete_(&c__1);
    goto L99991;
L20206:
    if (it != 3 || iop != 1) {
	goto L20208;
    }
/*         WE HAVE A TRA TO THE OUTER BLOCK... */
    j = cntrl_1.contrl[46];
    if (pstack_1.prsp == 0 || j == 1) {
	goto L20208;
    }
    if (j != 0) {
	goto L20207;
    }
    j = pstack_1.lxis;
    pstack_1.lxis = code_1.codloc + 1;
L20207:
    i__1 = j % 65536;
    emit_(&ops_1.lxi, &ops_1.rsp, &i__1);

L20208:
    j = -symbl_1.symbol[i__ - 1];
    i__1 = shr_(&j, &c__2);
    m = right_(&i__1, &c__14);
/*             CONNECT ENTRY INTO CHAIN */
    k = code_1.codloc + 1;
    if (iop == 4) {
	k = code_1.codloc;
    }
/*             IOP = 4 IF WE ARRIVED HERE FROM CASE TABLE JMP */
    i__1 = shr_(&j, &c__16);
    symbl_1.symbol[i__ - 1] = -(shl_(&i__1, &c__16) + shl_(&k, &c__2) + 
	    right_(&j, &c__2));

/*     CHECK FOR SINGLE REFERENCE */
    j = symbl_1.symbol[i__ - 2];
    k = abs(j) / 256;
    if (k != 1) {
	goto L20300;
    }
/*     MAKE SURE THIS IS THE FIRST FWD REFERENCE */
    l = symbl_1.symbol[i__ - 3];
    if (l != 0) {
	goto L20220;
    }
/*     SAVE H AND L, MARK AS A FORWARD REFERENCE */
/*     /  1B   /  1B   /  9B   /  8B   / */
/*     /H VALID/L VALID/H VALUE/L VALUE/ */
    k = 0;
    l = regall_1.regv[6];
    if (l < 0 || l > 255) {
	goto L20210;
    }
    k = l + 131072;
L20210:
    l = regall_1.regv[5];
    if (l < 0 || l > 511) {
	goto L20220;
    }
    k = (l + 1024 << 8) + k;
L20220:
    symbl_1.symbol[i__ - 3] = k;

/*         TRA, TRC, PRO, AX2 (CASE TRA) */
L20300:
    switch (iop) {
	case 1:  goto L20400;
	case 2:  goto L20500;
	case 3:  goto L20600;
	case 4:  goto L20650;
    }

L20400:
/*             MAY BE INC TRA COMBINATION IN DO-LOOP */
    if (peep_1.lastin + 1 != code_1.codloc) {
	goto L20410;
    }
/*                 CHANGE TO JFZ TO TOP OF LOOP */
    i__1 = (ops_1.fal << 5) + ops_1.zero;
    emit_(&ops_1.jmc, &i__1, &m);
    delete_(&c__1);
    goto L99991;
L20410:
    xfropt_1.xfrloc = code_1.codloc;
    xfropt_1.xfrsym = regall_1.st[regall_1.sp - 1];
    xfropt_1.tstloc = code_1.codloc + 3;
    emit_(&ops_1.jmp, &m, &c__0);
    delete_(&c__1);
/*             MARK H AND L NIL (= - 255) */
L20550:
    regall_1.regv[5] = -255;
    regall_1.regv[6] = -255;
    goto L99991;

L20500:
    xfropt_1.conloc = code_1.codloc;
    emit_(&ops_1.jmc, &iop2, &m);
    delete_(&c__2);
    goto L99991;

L20600:
    xfropt_1.xfrloc = code_1.codloc;
    xfropt_1.xfrsym = regall_1.st[regall_1.sp - 1];
    xfropt_1.tstloc = code_1.codloc + 3;
    emit_(&ops_1.cal, &m, &c__0);
/*              ADJUST THE MAXDEPTH, IF NECESSARY */
    j = symbl_1.symbol[i__ - 4] + 1;
/*             J IS NUMBER OF DOUBLE-BYTE STACK ELEMENTS REQD */
    stack_(&j);
/*             NOW RETURNED FROM CALL SO... */
    pstack_1.curdep[pstack_1.prsp] -= j;

/*             NOW FIX THE H AND L VALUES UPON RETURN */
    j = symbl_1.symbol[i__ - 3];
    k = shr_(&j, &c__19);
/*             MAY BE UNCHANGED FROM CALL */
    if (k == 3) {
	goto L20610;
    }
/*                 COMPARE VALUES */
    j = right_(&j, &c__19);
    l = j % 256;
    j /= 256;
    k = j % 512;
    j /= 512;
    if (j % 2 != 1) {
	l = -1;
    }
    if (j / 2 % 2 != 1) {
	k = -1;
    }
    regall_1.regv[5] = k;
    regall_1.regv[6] = l;
L20610:
    delete_(&c__1);
/*             MAY HAVE TO CONSTRUCT A RETURNED */
/*             VALUE AT THE STACK TOP */
    j = symbl_1.symbol[i__ - 2];
    j = j / 16 % 16;
    if (j <= 0) {
	goto L99991;
    }
/*             SET STACK TOP TO PRECISION OF PROCEDURE */
    ++regall_1.sp;
    regall_1.prec[regall_1.sp - 1] = j;
    regall_1.st[regall_1.sp - 1] = 0;
    i__ = ops_1.rc;
    if (j > 1) {
	i__ = (ops_1.rb << 4) + i__;
    }
    regall_1.rasn[regall_1.sp - 1] = i__;
    regall_1.regs[ops_1.ra - 1] = ops_1.rc;
    regall_1.regs[ops_1.rc - 1] = regall_1.sp;
    if (j > 1) {
	regall_1.regs[ops_1.rb - 1] = regall_1.sp;
    }
    regall_1.litv[regall_1.sp - 1] = -1;
    goto L99991;
/*         CAME FROM A CASE VECTOR */
L20650:
    i__1 = m % 256;
    emit_(&c__0, &i__1, &c__0);
    i__1 = m / 256;
    emit_(&c__0, &i__1, &c__0);
    delete_(&c__1);
    goto L99991;

/*     JUMP TO COMPUTED LOCATION */
L20700:
    loadv_(&regall_1.sp, &c__4);
    delete_(&c__1);
    emit_(&ops_1.pchl, &c__0, &c__0);
/*     PC HAS BEEN MOVED, SO MARK H AND L UNKNOWN */
    regall_1.regv[ops_1.rh - 1] = -255;
    regall_1.regv[ops_1.rl - 1] = -255;
    goto L99991;
/*     TRC */
L21000:
    j = regall_1.sp - 1;
    i__ = regall_1.litv[j - 1];
    if (right_(&i__, &c__1) != 1) {
	goto L21100;
    }
/*     THIS IS A DO FOREVER (OR SOMETHING SIMILAR) SO IGNORE THE JUMP */
    delete_(&c__2);
    goto L99991;

/*     NOT A LITERAL '1' */
L21100:
    iop = 2;
/*     CHECK FOR CONDITION CODE */
    i__ = regall_1.rasn[j - 1];
    if (i__ <= 255) {
	goto L21200;
    }
/*     ACTIVE CONDITION CODE, CONSTRUCT MASK FOR JMC */
    i__ /= 256;
    j = i__ / 16;
    i__ %= 16;
    iop2 = (ops_1.fal + 1 - j << 5) + (ops_1.carry + i__ - 1);
    goto L20050;

/*     OTHERWISE NOT A CONDITION CODE, CONVERT TO CARRY */
L21200:
    if (i__ != 0) {
	goto L21300;
    }
/*     LOAD VALUE TO ACCUMULATOR */
    regall_1.prec[j - 1] = 1;
    loadv_(&j, &c__1);
    goto L21400;

/*     VALUE ALREADY LOADED */
L21300:
    i__ %= 16;
    j = regall_1.regs[0];
    if (j == i__) {
	goto L21400;
    }
    if (j != 0) {
	emit_(&ops_1.ld, &j, &ops_1.ra);
    }
    emit_(&ops_1.ld, &ops_1.ra, &i__);

L21400:
    regall_1.regs[0] = 0;
    emit_(&ops_1.rot, &ops_1.cy, &ops_1.rgt);
    iop2 = (ops_1.fal << 5) + ops_1.carry;
    goto L20050;

/*     PRO */

/*     ROL     ROR     SHL     SHR */
/*     SCL     SCR */
/*     TIME    HIGH    LOW     INPUT */
/*     OUTPUT  LENGTH  LAST    MOVE */
/*     DOUBLE  DEC */

L22000:
    i__ = regall_1.st[regall_1.sp - 1];
    if (i__ > regall_1.intbas) {
	goto L22500;
    }
/*     THIS IS A BUILT-IN FUNCTION. */
    delete_(&c__1);
    if (i__ < bifloc_2.firsti) {
	goto L22499;
    }
    i__ = i__ - bifloc_2.firsti + 1;

    switch (i__) {
	case 1:  goto L22300;
	case 2:  goto L22300;
	case 3:  goto L22300;
	case 4:  goto L22300;
	case 5:  goto L22300;
	case 6:  goto L22300;
	case 7:  goto L22200;
	case 8:  goto L22300;
	case 9:  goto L22300;
	case 10:  goto L22050;
	case 11:  goto L22100;
	case 12:  goto L22310;
	case 13:  goto L22310;
	case 14:  goto L22499;
	case 15:  goto L22320;
	case 16:  goto L22350;
    }
/*      INPUT(X) */
L22050:
/*         INPUT FUNCTION. GET INPUT PORT NUMBER */
    i__ = regall_1.litv[regall_1.sp - 1];
    if (i__ < 0 || i__ > 255) {
	goto L22499;
    }
    delete_(&c__1);
    ++regall_1.sp;
    genreg_(&c__1, &j, &k);
    if (j == 0) {
	goto L22499;
    }
    k = regall_1.regs[0];
    if (k != 0) {
	emit_(&ops_1.ld, &k, &ops_1.ra);
    }
    regall_1.regs[0] = j;
    regall_1.rasn[regall_1.sp - 1] = j;
    regall_1.litv[regall_1.sp - 1] = -1;
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.prec[regall_1.sp - 1] = 1;
    regall_1.regs[j - 1] = regall_1.sp;
    emit_(&ops_1.inp, &i__, &c__0);
    goto L99991;

/*     OUTPUT(X) */
L22100:
/*         CHECK FOR PROPER OUTPUT PORT NUMBER */
    i__ = regall_1.litv[regall_1.sp - 1];
    if (i__ < 0 || i__ > 255) {
	goto L22499;
    }
    delete_(&c__1);
    ++regall_1.sp;
/*         NOW BUILD AN ENTRY WHICH CAN BE RECOGNIZED BY */
/*         OPERAT. */
    regall_1.litv[regall_1.sp - 1] = i__;
    regall_1.rasn[regall_1.sp - 1] = 0;
    regall_1.prec[regall_1.sp - 1] = 1;
    regall_1.st[regall_1.sp - 1] = bifloc_2.outloc;
    goto L99991;
/*     TIME(X) */
L22200:
    if (regall_1.rasn[regall_1.sp - 1] > 255) {
	cvcond_(&regall_1.sp);
    }

/*    EMIT THE FOLLOWING CODE SEQUENCE FOR 100 USEC PER LOOP */
/*       8080 CPU ONLY */
/*     (GET TIME PARAMETER INTO THE ACCUMULATOR) */
/*              MVI   B,12   (7 CY OVERHEAD) */
/*     START    MOV   C,B    (5 CY * .5 USEC = 2.5 USEC) */
/*     -------------------- */
/*     TIM180   DCR   C      (5 CY * .5 USEC = 2.5 USEC) */
/*              JNZ   TIM180 (10 CY* .5 USEC = 5.0 USEC) */
/*     -------------------- */
/*                  12 *     (15 CY* .5 USEC = 7.5 USEC) */
/*                   =       (180 CY* .5 USEC = 90 USEC) */
/*              DCR   A      (5 CY * .5 USEC = 2.5 USEC) */
/*              JNZ   START  (10 CY* .5 USEC = 5.0 USEC) */

/*              TOTAL TIME   (200 CY*.5 USEC = 100 USEC/LOOP) */

    j = regall_1.regs[ops_1.ra - 1];
    i__ = regall_1.rasn[regall_1.sp - 1];
    ip = i__ / 16;
    i__ %= 16;
    if (j != 0 && j == i__) {
	goto L22210;
    }
/*         GET TIME PARAMETER INTO THE ACCUMULATOR */
    if (j != 0 && j != ip) {
	emit_(&ops_1.ld, &j, &ops_1.ra);
    }
    regall_1.regs[ops_1.ra - 1] = 0;
    if (i__ == 0) {
	loadv_(&regall_1.sp, &c__1);
    }
    i__ = regall_1.rasn[regall_1.sp - 1] % 16;
    if (j != 0) {
	emit_(&ops_1.ld, &ops_1.ra, &i__);
    }
L22210:
    regall_1.regs[ops_1.ra - 1] = 0;
    i__1 = i__ - 1;
    emit_(&ops_1.ld, &i__1, &c_n12);
    i__1 = i__ - 1;
    emit_(&ops_1.ld, &i__, &i__1);
    emit_(&ops_1.dc, &i__, &c__0);
    i__1 = (ops_1.fal << 5) + ops_1.zero;
    i__2 = code_1.codloc - 1;
    emit_(&ops_1.jmc, &i__1, &i__2);
    emit_(&ops_1.dc, &ops_1.ra, &c__0);
    i__1 = (ops_1.fal << 5) + ops_1.zero;
    i__2 = code_1.codloc - 6;
    emit_(&ops_1.jmc, &i__1, &i__2);

    delete_(&c__1);
    goto L99991;
/*     STOP HERE BEFORE GOING TO THE UNARY OPERATORS */
/*     ** NOTE THAT THIS DEPENDS UPON FIXED RTL = 37 ** */
L22300:
    *val = i__ + 36;
    if (*val <= 42) {
	goto L22307;
    }
/*     ** NOTE THAT THIS ALSO ASSUMES ONLY 6 SUCH BIFS */
/* L22305: */
    unary_(val);
    goto L99991;

/*     MAY HAVE TO ITERATE */
L22307:
    i__ = regall_1.litv[regall_1.sp - 1];
    if (i__ <= 0) {
	goto L22308;
    }
/*     GENERATE IN-LINE CODE FOR SHIFT COUNTS OF */
/*         1 OR 2 FOR ADDRESS VALUES */
/*         1 TO 3 FOR SHR OF BYTE VALUES */
/*         1 TO 6 FOR ALL OTHER SHIFT FUNCTIONS ON BYTE VALUES */
    j = 6;
    if (*val == 40) {
	j = 3;
    }
    if (regall_1.prec[regall_1.sp - 2] != 1) {
	j = 2;
    }
    if (i__ > j) {
	goto L22308;
    }
    delete_(&c__1);
    i__1 = i__;
    for (j = 1; j <= i__1; ++j) {
	unary_(val);
/* L22306: */
    }
    goto L99991;
/*     BUILD A SMALL LOOP AND COUNT DOWN TO ZERO */
L22308:
    exch_();
/*     LOAD THE VALUE TO DECREMENT */
    i__1 = regall_1.sp - 1;
    loadv_(&i__1, &c__0);
    j = regall_1.rasn[regall_1.sp - 2];
    j %= 16;
    if (regall_1.regs[ops_1.ra - 1] != j) {
	goto L22311;
    }
    emit_(&ops_1.ld, &j, &ops_1.ra);
    regall_1.regs[ops_1.ra - 1] = 0;
L22311:
    regall_1.lock[j - 1] = 1;
/*     LOAD THE VALUE WHICH IS TO BE OPERATED UPON */
    kp = regall_1.prec[regall_1.sp - 1];
    i__ = 1;
    if (kp > 1) {
	i__ = 0;
    }
    if (regall_1.rasn[regall_1.sp - 1] != 0) {
	goto L22312;
    }
    loadv_(&regall_1.sp, &i__);
    if (i__ == 1) {
	regall_1.regs[0] = regall_1.rasn[regall_1.sp - 1] % 16;
    }
L22312:
    k = regall_1.rasn[regall_1.sp - 1];
    m = k % 16;
    k /= 16;
    jp = regall_1.regs[ops_1.ra - 1];
    if (i__ == 1 && jp == m) {
	goto L22314;
    }
    if (jp == 0) {
	goto L22313;
    }
    emit_(&ops_1.ld, &jp, &ops_1.ra);
    regall_1.regs[ops_1.ra - 1] = 0;
L22313:
    if (i__ == 0) {
	goto L22314;
    }
    emit_(&ops_1.ld, &ops_1.ra, &m);
    regall_1.regs[ops_1.ra - 1] = m;
L22314:
    i__ = code_1.codloc;
    unary_(val);
    if (kp == 1) {
	goto L22309;
    }
    k = regall_1.regs[0];
    if (k != 0) {
	emit_(&ops_1.ld, &k, &ops_1.ra);
    }
    regall_1.regs[0] = 0;
L22309:
    emit_(&ops_1.dc, &j, &c__0);
    i__1 = (ops_1.fal << 5) + ops_1.zero;
    emit_(&ops_1.jmc, &i__1, &i__);
/*     END UP HERE AFTER OPERATION COMPLETED */
    exch_();
    regall_1.lock[j - 1] = 0;
    delete_(&c__1);
    goto L99991;

/*     LENGTH AND LAST */
/*     ** NOTE THAT THIS ASSUMES THAT LENGTH AND LAST ARE */
/*     BUILT-IN FUNCTIONS 10 AND 11 ** */
L22310:
    j = regall_1.st[regall_1.sp - 1];
    if (j <= 0) {
	goto L22499;
    }
    j = symbl_1.symbol[j - 1] - 1;
    j = (i__1 = symbl_1.symbol[j - 1], abs(i__1)) / 256 + 12 - i__;
    delete_(&c__1);
    ++regall_1.sp;
    regall_1.st[regall_1.sp - 1] = 0;
    i__ = 1;
    if (j > 255) {
	i__ = 2;
    }
    regall_1.prec[regall_1.sp - 1] = i__;
    regall_1.rasn[regall_1.sp - 1] = 0;
    regall_1.litv[regall_1.sp - 1] = j;
    if (j < 0) {
	goto L22499;
    }
    goto L99991;

/*     DOUBLE */
L22320:
    if (regall_1.prec[regall_1.sp - 1] > 1) {
	goto L99999;
    }
    if (regall_1.rasn[regall_1.sp - 1] != 0) {
	goto L22330;
    }
    if (regall_1.litv[regall_1.sp - 1] < 0) {
	goto L22332;
    }
    regall_1.prec[regall_1.sp - 1] = 2;
    regall_1.st[regall_1.sp - 1] = 0;
    goto L99991;
/*         LOAD VALUE TO ACCUMULATOR AND GET A REGISTER */
L22332:
    loadv_(&regall_1.sp, &c__1);
    regall_1.regs[0] = regall_1.rasn[regall_1.sp - 1] % 16;

L22330:
    ia = regall_1.rasn[regall_1.sp - 1];
    regall_1.prec[regall_1.sp - 1] = 2;
    regall_1.st[regall_1.sp - 1] = 0;
    if (ia > 15) {
	goto L99991;
    }
    regall_1.lock[ia - 1] = 1;
    ib = ia - 1;
    regall_1.regs[ib - 1] = regall_1.sp;
    regall_1.lock[ia - 1] = 0;
    regall_1.rasn[regall_1.sp - 1] = (ib << 4) + ia;
/*     ZERO THE REGISTER */
    emit_(&ops_1.ld, &ib, &c__0);
    if (ib != 0) {
	goto L99991;
    }
    error_(&c__133, &c__5);
    goto L99991;


/*     DEC */
L22350:
    j = regall_1.rasn[regall_1.sp - 1] % 16;
    if (j == 0) {
	goto L22499;
    }
    if (regall_1.prec[regall_1.sp - 1] != 1) {
	goto L22499;
    }
    i__ = regall_1.regs[ops_1.ra - 1];
    if (i__ == j) {
	goto L22370;
    }
/*     MAY BE A PENDING REGISTER STORE */
    if (i__ != 0) {
	emit_(&ops_1.ld, &i__, &ops_1.ra);
    }
    emit_(&ops_1.ld, &ops_1.ra, &j);
    regall_1.regs[ops_1.ra - 1] = j;
L22370:
    emit_(&ops_1.daa, &c__0, &c__0);
    goto L99991;

/*     BUILT IN FUNCTION ERROR */
L22499:
    error_(&c__136, &c__1);
    goto L99999;

/*     PASS THE LAST TWO (AT MOST) PARAMETERS IN THE REGISTERS */

L22500:
    i__ = right_(&regall_1.st[regall_1.sp - 1], &c__16);
    i__ = symbl_1.symbol[i__ - 1];
    i__ = shr_(&symbl_1.symbol[i__ - 2], &c__8);
    i__ = imin_(&i__, &c__2);
    if (i__ < 1) {
	goto L22630;
    }
    j = regall_1.sp - i__ - i__;
    i__1 = i__;
    for (k = 1; k <= i__1; ++k) {
	ip = regall_1.rasn[j - 1];
	jp = ip / 16 % 16;
	ip %= 16;
	if (ip != 0) {
	    regall_1.lock[ip - 1] = 1;
	}
	if (jp != 0) {
	    regall_1.lock[jp - 1] = 1;
	}
	regall_1.prec[j - 1] = imin_(&regall_1.prec[j - 1], &regall_1.prec[j])
		;
	if (regall_1.prec[j - 1] > 1 || jp == 0) {
	    goto L22510;
	}
	regall_1.regs[jp - 1] = 0;
	regall_1.lock[jp - 1] = 0;
	jp = 0;
	if (regall_1.regs[0] == ip) {
	    regall_1.lock[0] = 1;
	}
	if (regall_1.regs[0] == jp) {
	    regall_1.lock[0] = 1;
	}
L22510:
	regall_1.rasn[j - 1] = (jp << 4) + ip;
	j += 2;
/* L22520: */
    }
    j = regall_1.sp - 1 - i__ - i__;
    it = 0;
/*     STACK ANY STUFF WHICH DOES NOT GO TO THE PROCEDURE */
    i__1 = regall_1.sp;
    for (k = 1; k <= i__1; ++k) {
/*         CHECK FOR VALUE TO PUSH */
	jp = regall_1.rasn[k - 1];
	if (jp == 0) {
	    goto L22524;
	}
/*         POSSIBLE PUSH IF NOT A PARAMETER */
	if (k > j) {
	    goto L22530;
	}
/*             REGISTERS MUST BE PUSHED */
	jph = jp / 16;
	kp = regall_1.regs[ops_1.ra - 1];
	jp %= 16;
	if (kp == 0) {
	    goto L22522;
	}
/*            PENDING ACC STORE, CHECK HO AND LO REGISTERS */
	if (kp != jph) {
	    goto L22521;
	}
/*            PENDING HO BYTE STORE */
	emit_(&ops_1.ld, &jph, &ops_1.ra);
	regall_1.regs[ops_1.ra - 1] = 0;
	goto L22522;
/*            CHECK LO BYTE */
L22521:
	if (kp != jp) {
	    goto L22522;
	}
	emit_(&ops_1.ld, &jp, &ops_1.ra);
	regall_1.regs[ops_1.ra - 1] = 0;
L22522:
	i__2 = jp - 1;
	emit_(&ops_1.push, &i__2, &c__0);
	stack_(&c__1);
	regall_1.st[k - 1] = 0;
	it = regall_1.rasn[k - 1];
	jp = it % 16;
	if (jp != 0) {
	    regall_1.regs[jp - 1] = 0;
	}
	jp = it / 16;
	if (jp != 0) {
	    regall_1.regs[jp - 1] = 0;
	}
	regall_1.rasn[k - 1] = 0;
	regall_1.litv[k - 1] = -1;
	it = k;
	goto L22530;
/*     REGISTERS NOT ASSIGNED - CHECK FOR STACKED VALUE */
L22524:
	if (regall_1.st[k - 1] != 0 || regall_1.litv[k - 1] >= 0) {
	    goto L22530;
	}
	if (it == 0) {
	    goto L22530;
	}
	error_(&c__150, &c__1);
L22530:
	;
    }
/* L22550: */
    it = ops_1.rh;
    j = regall_1.sp - i__ - i__;
    i__1 = i__;
    for (k = 1; k <= i__1; ++k) {
	id = k + k + 2;
	ip = regall_1.rasn[j - 1];
	jp = ip / 16 % 16;
	ip %= 16;
L22560:
	--id;
	if (ip == 0) {
	    goto L22590;
	}
	if (ip == id) {
	    goto L22580;
	}
	if (regall_1.regs[id - 1] == 0) {
	    goto L22570;
	}
	m = regall_1.regs[id - 1];
	ml = regall_1.rasn[m - 1];
	mh = ml / 16 % 16;
	ml %= 16;
	if (ml == id) {
	    ml = it;
	}
	if (mh == id) {
	    mh = it;
	}
	emit_(&ops_1.ld, &it, &id);
	regall_1.regs[it - 1] = m;
	regall_1.rasn[m - 1] = (mh << 4) + ml;
	++it;
L22570:
	regall_1.regs[ip - 1] = 0;
	regall_1.lock[ip - 1] = 0;
	if (regall_1.regs[0] != ip) {
	    goto L22575;
	}
	ip = 1;
	regall_1.regs[0] = 0;
	regall_1.lock[0] = 0;
L22575:
	emit_(&ops_1.ld, &id, &ip);
	regall_1.regs[id - 1] = j;
L22580:
	regall_1.lock[id - 1] = 1;
	ip = jp;
	if (ip == -1) {
	    goto L22590;
	}
	jp = -1;
	goto L22560;
L22590:
	j += 2;
    }
    j = regall_1.sp - i__ - i__;
    i__1 = i__;
    for (k = 1; k <= i__1; ++k) {
	if (regall_1.rasn[j - 1] == 0) {
	    loadv_(&j, &c__0);
	}
	ip = k + k;
	regall_1.regs[ip - 1] = j;
	regall_1.lock[ip - 1] = 1;
	if (regall_1.prec[j] == 2 && regall_1.prec[j - 1] == 1) {
	    emit_(&ops_1.ld, &ip, &c__0);
	}
	j += 2;
/* L22600: */
    }
    if (regall_1.regs[0] != 0) {
	emit_(&ops_1.ld, regall_1.regs, &ops_1.ra);
    }
    for (k = 1; k <= 7; ++k) {
	regall_1.regs[k - 1] = 0;
	regall_1.regv[k - 1] = -1;
	regall_1.lock[k - 1] = 0;
/* L22610: */
    }
    j = i__ + i__;
    i__1 = j;
    for (k = 1; k <= i__1; ++k) {
	exch_();
	if (regall_1.st[regall_1.sp - 1] != 0 || regall_1.rasn[regall_1.sp - 
		1] != 0 || regall_1.litv[regall_1.sp - 1] >= 0) {
	    goto L22615;
	}
	emit_(&ops_1.pop, &ops_1.rh, &c__0);
	ustack_();
	regall_1.regv[ops_1.rh - 1] = -1;
	regall_1.regv[ops_1.rl - 1] = -1;
L22615:
	delete_(&c__1);
/* L22620: */
    }
    iop = 3;
    goto L20050;
L22630:
    regall_1.lock[5] = 1;
    regall_1.lock[6] = 1;
    saver_();
    regall_1.lock[5] = 0;
    regall_1.lock[6] = 0;
    iop = 3;
    goto L20050;

/*     RET */
L23000:
    jp = pstack_1.prsp;
    if (jp > 0) {
	goto L23050;
    }
    error_(&c__146, &c__1);
    goto L20550;
L23050:
/*     CHECK FOR TYPE AND PRECISION OF PROCEDURE */
    l = pstack_1.prstk[jp - 1] % 65536 + 1;
    l = symbl_1.symbol[l - 1] / 16;
    l %= 16;
/*     L IS THE PRECISION OF THE PROCEDURE */
    if (l == 0) {
	goto L23310;
    }
    i__ = regall_1.rasn[regall_1.sp - 1];
    if (i__ == 0) {
	loadv_(&regall_1.sp, &c__1);
    }
    if (i__ >= 256) {
	cvcond_(&regall_1.sp);
    }
    k = regall_1.rasn[regall_1.sp - 1];
    jp = regall_1.regs[0];
    j = k % 16;
    k /= 16;
    if (i__ == 0 || j == jp) {
	goto L23200;
    }
/*         HAVE TO LOAD THE ACCUMULATOR.  MAY HAVE H.O. BYTE. */
    if (jp == 0 || jp != k) {
	goto L23150;
    }
    emit_(&ops_1.ld, &k, &ops_1.ra);
L23150:
    emit_(&ops_1.ld, &ops_1.ra, &j);

L23200:
    if (k == 0) {
	goto L23300;
    }
    if (k != ops_1.rb) {
	emit_(&ops_1.ld, &ops_1.rb, &k);
    }
L23300:
/*     COMPARE PRECISION OF PROCEDURE WITH STACK */
    if (l > regall_1.prec[regall_1.sp - 1]) {
	emit_(&ops_1.ld, &ops_1.rb, &c__0);
    }
L23310:
    delete_(&c__1);
    if (pstack_1.prstk[pstack_1.prsp - 1] <= 65535) {
	goto L23320;
    }
/*         INTERRUPT PROCEDURE - USE THE DRT CODE BELOW */
    jp = pstack_1.prsp;
    k = 0;
    goto L45020;
L23320:
    emit_(&ops_1.rtn, &c__0, &c__0);
/*     MERGE VALUES OF H AND L FOR THIS PROCEDURE */
/*         CAN ALSO ENTER WITH JP SET FROM END OF PROCEDURE */
    jp = pstack_1.prsp;
L23350:
    xfropt_1.xfrloc = code_1.codloc - 1;
    xfropt_1.xfrsym = 0;
    xfropt_1.tstloc = code_1.codloc;
    i__ = pstack_1.prstk[jp - 1] % 65536;
    jp = symbl_1.symbol[i__ - 1];
    k = regall_1.regv[5];
    l = regall_1.regv[6];
    j = right_(&jp, &c__19);
    jp = shr_(&jp, &c__19);
    if (jp != 3) {
	goto L23360;
    }
    if (k == -254 && l == -254) {
	goto L99991;
    }
/*     H AND L HAVE BEEN ALTERED IN THE PROCEDURE */
    kp = k;
    lp = l;
    goto L23370;
/*     OTHERWISE MERGE VALUES OF H AND L */

L23360:
    lp = j % 256;
    j /= 256;
    kp = j % 512;
    j /= 512;
    if (j % 2 == 0) {
	lp = -1;
    }
    if (j / 2 % 2 == 0) {
	kp = -1;
    }
/*     COMPARE K WITH KP AND L WITH LP */
L23370:
    j = 0;
    if (l >= 0 && lp == l) {
	j = l + 131072;
    }
    if (k >= 0 && kp == k) {
	j = (k + 1024 << 8) + j;
    }
    symbl_1.symbol[i__ - 1] = j;
/*     MARK H AND L NIL BEFORE RETURNING FROM SUBR */
    goto L20550;

/*     STO AND STD */
L24000:
    i__ = regall_1.st[regall_1.sp - 1];
/*     CHECK FOR OUTPUT FUNCTION */
    if (i__ == bifloc_2.outloc) {
	goto L24050;
    }
/*     CHECK FOR COMPUTED ADDRESS OR SAVED ADDRESS */
    if (i__ >= 0) {
	goto L24100;
    }
/*     CHECK FOR ADDRESS REFERENCE OUTSIDE INTRINSIC RANGE */
    i__ = -i__;
    if (i__ > regall_1.intbas) {
	goto L24100;
    }
/*     CHECK FOR 'MEMORY' ADDRESS REFERENCE */
/*    ** NOTE THAT STACKTOP MUST BE AT 6 ** */
    if (i__ <= 6) {
	goto L24100;
    }
    if (i__ == 5) {
	goto L24100;
    }
/*     IGNORE THE STORE FOR INTRINSIC PARAMETERS */
    goto L24200;
/*     OUTPUT FUNCTION */
L24050:
    j = regall_1.litv[regall_1.sp - 1];
    i__ = regall_1.rasn[regall_1.sp - 2];
    if (i__ > 0 && i__ < 256) {
	goto L24060;
    }
/*         LOAD VALUE TO ACC */
    i__ = regall_1.regs[ops_1.ra - 1];
    if (i__ > 0) {
	emit_(&ops_1.ld, &i__, &ops_1.ra);
    }
    i__1 = regall_1.sp - 1;
    loadv_(&i__1, &c__1);
    i__ = regall_1.rasn[regall_1.sp - 2];
    goto L24070;
/*     OPERAND IS IN THE GPRS */
L24060:
    i__ %= 16;
    k = regall_1.regs[ops_1.ra - 1];
    if (k > 0 && k != i__) {
	emit_(&ops_1.ld, &k, &ops_1.ra);
    }
    if (k != i__) {
	emit_(&ops_1.ld, &ops_1.ra, &i__);
    }
/*     NOW MARK ACC ACTIVE IN CASE SUBSEQUENT STO OPERATOR */
L24070:
    regall_1.regs[ops_1.ra - 1] = i__ % 16;
    emit_(&ops_1.out, &j, &c__0);
    delete_(&c__1);
    goto L24200;
L24100:
    i__ = 1;
/*     CHECK FOR STD */
    if (*val == 25) {
	i__ = 0;
    }
    gensto_(&i__);
/*     * CHECK FOR STD * */
L24200:
    if (*val == 25) {
	delete_(&c__1);
    }
    goto L99991;
/*     XCH */
L26000:
    exch_();
    goto L99991;
/*     DEL */
L27000:
    if (regall_1.st[regall_1.sp - 1] != 0 || regall_1.rasn[regall_1.sp - 1] !=
	     0 || regall_1.litv[regall_1.sp - 1] >= 0) {
	goto L27100;
    }
/*     VALUE IS STACKED, SO GET RID OF IT */
    emit_(&ops_1.pop, &ops_1.rh, &c__0);
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
    ustack_();
L27100:
    delete_(&c__1);
    goto L99991;

/*     CAT (INLINE DATA FOLLOWS) */
L28000:
    inldat_();
    goto L99999;

/*    LOD */
L29000:
    il = 0;
    k = regall_1.prec[regall_1.sp - 1];
/*     MAY BE A LOD FROM A BASE FOR A BASED VARIABLE */
    regall_1.prec[regall_1.sp - 1] = k % 4;
    ia = regall_1.rasn[regall_1.sp - 1];
    if (ia > 0) {
	goto L29050;
    }
/*     CHECK FOR SIMPLE BASED VARIABLE CASE */
    i__ = regall_1.st[regall_1.sp - 1];
    if (i__ <= 0) {
	goto L29010;
    }
/*         RESERVE REGISTERS FOR THE RESULT */
    genreg_(&c__2, &ia, &ib);
    regall_1.regs[ia - 1] = regall_1.sp;
    regall_1.regs[ib - 1] = regall_1.sp;
    regall_1.rasn[regall_1.sp - 1] = (ib << 4) + ia;
/*         MAY BE ABLE TO SIMPLIFY LHLD */
    lp = regall_1.regv[ops_1.rh - 1];
    l = regall_1.regv[ops_1.rl - 1];
    if (lp == -3 && -l == i__) {
	goto L29110;
    }
    if (lp == -4 && -l == i__) {
	goto L29007;
    }
    i__1 = code_1.codloc + 1;
    j = chain_(&i__, &i__1);
    emit_(&ops_1.lhld, &j, &c__0);
    regall_1.regv[ops_1.rh - 1] = -3;
    regall_1.regv[ops_1.rl - 1] = -i__;
    goto L29110;
L29007:
    emit_(&ops_1.dcx, &ops_1.rh, &c__0);
    regall_1.regv[ops_1.rh - 1] = -3;
    goto L29110;

L29010:
/*     FIRST CHECK FOR AN ADDRESS REFERENCE */
    if (regall_1.st[regall_1.sp - 1] == 0) {
	goto L29011;
    }
/*         CHANGE THE ADDRESS REFERENCE TO A VALUE REFERENCE */
    regall_1.st[regall_1.sp - 1] = -regall_1.st[regall_1.sp - 1];
    regall_1.litv[regall_1.sp - 1] = -1;
    goto L99991;
/*     LOAD THE ADDRESS */
L29011:
    loadv_(&regall_1.sp, &c__0);
    ia = regall_1.rasn[regall_1.sp - 1];
L29050:
    ib = ia / 16;
    ia %= 16;
    i__ = regall_1.regs[0];
    if (ia == i__) {
	ia = 1;
    }
    if (ib == i__) {
	ib = 1;
    }
    if (ib == ia - 1) {
	il = ib;
    }
    if (ia * ib != 0) {
	goto L29100;
    }
    error_(&c__138, &c__5);
    goto L99991;
L29100:
/*     MAY BE POSSIBLE TO USE LDAX OR XCHG */
    if (il != ops_1.rd) {
	goto L29105;
    }
/*     POSSIBLE XCHG OR LDAX */
    if (peep_1.lastex == code_1.codloc - 1) {
	goto L29102;
    }
/*     LAST INSTRUCTION NOT AN XCHG */
    if (regall_1.prec[regall_1.sp - 1] % 2 == 1) {
	goto L29110;
    }
/*     DOUBLE XCHG OR DOUBLE BYTE LOAD WITH ADDR IN D AND E */
L29102:
    emit_(&ops_1.xchg, &c__0, &c__0);
    goto L29107;

L29105:
    emit_(&ops_1.ld, &ops_1.rl, &ia);
    emit_(&ops_1.ld, &ops_1.rh, &ib);
L29107:
    il = 0;
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
L29110:
    i__ = regall_1.prec[regall_1.sp - 1] - k / 4;
    regall_1.prec[regall_1.sp - 1] = i__;
/*     RECOVER THE REGISTER ASSIGNMENT FROM RASN */
    ib = regall_1.rasn[regall_1.sp - 1];
    ia = ib % 16;
    ib /= 16;
    j = regall_1.regs[0];
    k = j * (j - ia) * (j - ib);
/*     JUMP IF J=0, IA, OR IB */
    if (k == 0) {
	goto L29150;
    }
    emit_(&ops_1.ld, &j, &ops_1.ra);
/*     SET PENDING STORE OPERATION IN REGS(1) */
L29150:
/*     MAY BE ABLE TO CHANGE REGISTER ASSIGNMENT TO BC */
    if (ia != ops_1.re) {
	goto L29160;
    }
    if (regall_1.regs[ops_1.rb - 1] != 0 || regall_1.regs[ops_1.rc - 1] != 0) 
	    {
	goto L29160;
    }
/*         BC AVAILABLE, SO RE-ASSIGN */
    regall_1.regs[ia - 1] = 0;
    regall_1.regs[ib - 1] = 0;
    regall_1.regs[ops_1.rb - 1] = regall_1.sp;
    regall_1.regs[ops_1.rc - 1] = regall_1.sp;
    ia = ops_1.rc;
    ib = ops_1.rb;
    regall_1.rasn[regall_1.sp - 1] = (ops_1.rb << 4) + ops_1.rc;
L29160:
    regall_1.regs[ops_1.ra - 1] = ia;
    if (il == 0) {
	emit_(&ops_1.ld, &ops_1.ra, &ops_1.me);
    }
    if (il != 0) {
	emit_(&ops_1.ldax, &il, &c__0);
    }
    if (i__ > 1) {
	goto L29200;
    }
/*     SINGLE BYTE LOAD - RELEASE H.O. REGISTER */
    ib = regall_1.rasn[regall_1.sp - 1];
    regall_1.rasn[regall_1.sp - 1] = ib % 16;
    ib /= 16;
    if (ib == regall_1.regs[0]) {
	regall_1.regs[0] = 0;
    }
    regall_1.regs[ib - 1] = 0;
    regall_1.regv[ib - 1] = -1;
    goto L29300;

L29200:
    emit_(&ops_1.incx, &ops_1.rh, &c__0);
/*     MAY HAVE DONE A PREVOUS LHLD, IF SO MARK INCX H */
    if (regall_1.regv[ops_1.rh - 1] == -3) {
	regall_1.regv[ops_1.rh - 1] = -4;
    }
    emit_(&ops_1.ld, &ib, &ops_1.me);
L29300:
    regall_1.regs[5] = 0;
    regall_1.regs[6] = 0;
    regall_1.st[regall_1.sp - 1] = 0;
    goto L99991;

/*     INC */
L31000:
/*     PLACE A LITERAL 1 AT STACK TOP AND APPLY ADD OPERATOR */
    ++regall_1.sp;
    regall_1.litv[regall_1.sp - 1] = 1;
/*     CHECK FOR SINGLE BYTE INCREMENT, MAY BE COMPARING WITH 255 */
    if (regall_1.prec[regall_1.sp - 2] != 1) {
	goto L1000;
    }
    apply_(&ops_1.ad, &ops_1.ac, &c__1, &c__1);
    peep_1.lastin = code_1.codloc;
/*     TRA WILL NOTICE LASTIN = CODLOC AND SUBSTITUTE JFZ */
    goto L99991;

/*     CSE (CASE STATEMENT INDEX) */
L32000:
/*     LET X BE THE VALUE OF THE STACK TOP */
/*     COMPUTE 2*X + CODLOC, FETCH TO HL, AND JUMP WITH PCHL */
/*     RESERVE REGISTERS FOR THE JUMP TABLE BASE */
    genreg_(&c__2, &ia, &ib);
    regall_1.lock[ia - 1] = 1;
    regall_1.lock[ib - 1] = 1;
/*     INDEX IS IN H AND L, SO DOUBLE IT */
    emit_(&ops_1.dad, &ops_1.rh, &c__0);
/*     NOW LOAD THE VALUE OF TABLE BASE, DEPENDING UPON 9 BYTES */
/*     LXI R X Y, DAD R, MOV EM, INX H, MOV DM XCHG PCHL */
    i__1 = code_1.codloc + 9;
    emit_(&ops_1.lxi, &ib, &i__1);
    emit_(&ops_1.dad, &ib, &c__0);
    emit_(&ops_1.ld, &ops_1.re, &ops_1.me);
    emit_(&ops_1.incx, &ops_1.rh, &c__0);
    emit_(&ops_1.ld, &ops_1.rd, &ops_1.me);
    emit_(&ops_1.xchg, &c__0, &c__0);
    emit_(&ops_1.pchl, &c__0, &c__0);
/*     PHONEY ENTRY IN SYMBOL TABLE TO KEEP CODE DUMP CLEAN */
    ++symbl_1.sytop;
    symbl_1.symbol[symbl_1.sytop - 1] = symbl_1.syinfo;
    symbl_1.symbol[symbl_1.syinfo - 1] = -code_1.codloc;
    --symbl_1.syinfo;
/*     SET ENTRY TO LEN=0/PREC=2/TYPE=VARB/ */
    symbl_1.symbol[symbl_1.syinfo - 1] = types_1.varb + 32;
    bifloc_2.casjmp = symbl_1.syinfo;
/*     CASJMP WILL BE USED TO UPDATE THE LENGTH FIELD */
    --symbl_1.syinfo;
    if (symbl_1.syinfo <= symbl_1.sytop) {
	error_(&c__108, &c__5);
    }

    regall_1.lock[ib - 1] = 0;
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
/*     MARK H AND L NIL AT CASE OR COMPUTED JUMP BEFORE RETURNING */
    goto L20550;
/*     HAL (HALT) */
L36000:
    emit_(&ops_1.ei, &c__0, &c__0);
    emit_(&ops_1.halt, &c__0, &c__0);
    goto L99991;

/*     RTL RTR SFL SFR */
L37000:
    unary_(val);
    goto L99991;

/*     CVA (CONVERT ADDRESS TO DOUBLE PRECISION VARIABLE) */
L43000:
/*     CVA MUST BE IMMEDIATELY PRECEDED BY AN INX OR ADR REF */
    regall_1.prec[regall_1.sp - 1] = 2;
/*     IF THE ADDRESS IS ALREADY IN THE GPR'S THEN NOTHING TO DO */
    if (regall_1.rasn[regall_1.sp - 1] > 0) {
	goto L99991;
    }
    if (regall_1.st[regall_1.sp - 1] < 0) {
	goto L43100;
    }
    if (regall_1.st[regall_1.sp - 1] > 0) {
	goto L43050;
    }
    error_(&c__139, &c__1);
    goto L99999;

/*     LOAD VALUE OF BASE FOR ADDRESS REF TO A BASED VARIABLE */
L43050:
    loadv_(&regall_1.sp, &c__3);
    goto L99991;

/*     CHECK FOR ADDRESS REF TO DATA IN ROM. */
L43100:
    jp = regall_1.litv[regall_1.sp - 1];
    if (jp > 65535) {
	goto L43190;
    }
    if (jp < 0) {
	error_(&c__149, &c__1);
    }
/*         LEAVE LITERAL VALUE */
    regall_1.st[regall_1.sp - 1] = 0;
    goto L99991;

/*     DO LXI R WITH THE ADDRESS */
L43190:
    genreg_(&c__2, &ia, &ib);
    if (ia > 0) {
	goto L43200;
    }
    error_(&c__140, &c__5);
    goto L99999;

L43200:
    i__1 = -regall_1.st[regall_1.sp - 1];
    i__2 = code_1.codloc + 1;
    j = chain_(&i__1, &i__2);
    emit_(&ops_1.lxi, &ib, &j);
    regall_1.st[regall_1.sp - 1] = 0;
    regall_1.rasn[regall_1.sp - 1] = (ib << 4) + ia;
    regall_1.regs[ia - 1] = regall_1.sp;
    regall_1.regs[ib - 1] = regall_1.sp;
    goto L99991;


/*     ORG */
L44000:
    i__ = regall_1.litv[regall_1.sp - 1];
    if (code_1.codloc <= i__) {
	goto L44100;
    }
    error_(&c__141, &c__1);

L44100:
    j = cntrl_1.contrl[46];
    k = 3;
    if (j == 1) {
	k = 0;
    }
    if (code_1.codloc != memory_1.offset + memory_1.preamb + k) {
	goto L44200;
    }
/*         THIS IS THE START OF PROGRAM, CHANGE OFFSET */
    memory_1.offset = i__ - memory_1.preamb;
    code_1.codloc = i__ + k;
    if (pstack_1.lxis > 0) {
	pstack_1.lxis = code_1.codloc - 2;
    }
/*         WE HAVE ALREADY GENERATED LXI SP (IF ANY) */
    goto L99990;
/*     SOME CODE HAS BEEN GENERATED, SO LXI IF NECESSARY */
L44200:
    if (code_1.codloc >= i__) {
	goto L44300;
    }
    emit_(&c__0, &c__0, &c__0);
    goto L44200;

L44300:
    if (j == 1) {
	goto L99990;
    }
    if (j > 1) {
	goto L44400;
    }
    j = pstack_1.lxis;
    pstack_1.lxis = code_1.codloc + 1;
L44400:
    emit_(&ops_1.lxi, &ops_1.rsp, &j);
    goto L99990;

/*     DRT (DEFAULT RETURN FROM SUBROUTINE) */
/*     MERGE H AND L VALUES USING RET OPERATION ABOVE */
L45000:
    jp = pstack_1.prsp;
    if (pstack_1.prstk[jp - 1] <= 65535) {
	goto L45005;
    }
/*     THIS IS THE END OF AN INTERRUPT PROCEDURE */
    pstack_1.curdep[jp] += -4;
L45005:
    if (pstack_1.prsp > 0) {
	--pstack_1.prsp;
    }
/*         GET STACK DEPTH FOR SYMBOL TABLE */
    if (jp <= 0) {
	goto L45010;
    }
    if (pstack_1.curdep[jp] != 0) {
	error_(&c__150, &c__1);
    }
    k = pstack_1.maxdep[jp];
    l = pstack_1.prstk[jp - 1] % 65536 - 1;
/*         K IS MAX STACK DEPTH, L IS SYMBOL TABLE COUNT ENTRY */
    symbl_1.symbol[l - 1] = k;
L45010:
    k = regall_1.regv[5];
    l = regall_1.regv[6];
    if (k == -255 && l == -255) {
	goto L99999;
    }
    if (pstack_1.prstk[jp - 1] <= 65535) {
	goto L45030;
    }
L45020:
/*         POP INTERRUPTED REGISTERS AND ENABLE INTERRUPTS */
    emit_(&ops_1.pop, &ops_1.ra, &c__0);
    emit_(&ops_1.pop, &ops_1.rb, &c__0);
    emit_(&ops_1.pop, &ops_1.rd, &c__0);
    emit_(&ops_1.pop, &ops_1.rh, &c__0);
    emit_(&ops_1.ei, &c__0, &c__0);
L45030:
    emit_(&ops_1.rtn, &c__0, &c__0);
    if (k == -254 && l == -254) {
	goto L20550;
    }
    if (jp > 0) {
	goto L23350;
    }
    error_(&c__146, &c__1);
    goto L20550;

/*     ENA  -  ENABLE INTERRUPTS */
L45100:
    emit_(&ops_1.ei, &c__0, &c__0);
    goto L99999;
/*     DIS  - DISABLE INTERRUPTS */
L45200:
    emit_(&ops_1.di, &c__0, &c__0);
    goto L99999;

/*     AX1 - CASE BRANCH TO CASE SELECTOR */
L45500:
/*     LOAD CASE NUMBER TO H AND L */
    exch_();
    loadv_(&regall_1.sp, &c__4);
    delete_(&c__1);
    regall_1.regv[ops_1.rh - 1] = -1;
    regall_1.regv[ops_1.rl - 1] = -1;
/*     USE TRA CODE */
    goto L20000;

/*     MAY NOT BE OMITTED EVEN THOUGH NO OBVIOUS PATH EXISTS). */
L46000:
    iop = 4;
/*     CASJMP POINTS TO SYMBOL TABLE ATTRIBUTES - INC LEN FIELD */
    symbl_1.symbol[bifloc_2.casjmp - 1] += 256;
    goto L20050;
L88887:
    iop2 = iop;
L88888:
    apply_(&iop, &iop2, &icom, &icy);
    goto L99991;
L99990:
    --regall_1.sp;
L99991:
    code_1.alter = 1;
L99999:
    return 0;
} /* operat_ */

/* Subroutine */ int sydump_(void)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, k, l, m, n;
    extern /* Subroutine */ int pad_(integer *, integer *, integer *);
    extern integer gnc_(integer *);
    static integer addr__, char__[32];
    extern /* Subroutine */ int form_(integer *, integer *, integer *, 
	    integer *, integer *);
    static integer ichar;
    extern /* Subroutine */ int error_(integer *, integer *), writel_(integer 
	    *), conout_(integer *, integer *, integer *, integer *);

/*     DUMP THE SYMBOL TABLE FOR THE SIMULATOR */
/*     CLEAR THE OUTPUT BUFFER */
    writel_(&c__0);
    l = 0;
/*     SAVE THE CURRENT INPUT FILE NUMBER, POINT INPUT */
/*     AT SYMBOL FILE. */
    m = cntrl_1.contrl[19];
    cntrl_1.contrl[19] = cntrl_1.contrl[31];
/*     GET RID OF LAST CARD IMAGE */
    files_1.ibp = 99999;
L50:
    i__ = gnc_(&c__0);
    if (i__ == 1) {
	goto L50;
    }
    if (i__ != 41) {
	goto L8000;
    }

/*     PROCESS NEXT SYMBOL TABLE ENTRY */
L100:
    i__ = gnc_(&c__0);
    if (i__ == 41) {
	goto L9000;
    }
/*     PROCESS THE NEXT SYMBOL */
/* L110: */
    i__ += -2;
/*     BUILD ADDRESS OF INITIALIZED SYMBOL */
    k = 32;
    for (j = 1; j <= 2; ++j) {
	i__ = (gnc_(&c__0) - 2) * k + i__;
/* L200: */
	k <<= 5;
    }

    if (i__ > 4 && i__ != 6) {
	goto L260;
    }
L250:
    j = gnc_(&c__0);
    if (j == 41) {
	goto L100;
    }
    goto L250;
L260:
/*     WRITE SYMBOL NUMBER, SYMBOL, AND ABSOLUTE ADDRESS (OCTAL) */
    conout_(&c__1, &c_n5, &i__, &c__10);
    pad_(&c__1, &c__1, &c__1);
    ichar = 1;
    for (k = 1; k <= 32; ++k) {
	char__[k - 1] = 40;
/* L290: */
    }
/*     READ UNTIL NEXT / SYMBOL */
L300:
    j = gnc_(&c__0);
    if (j == 41) {
	goto L400;
    }
    char__[ichar - 1] = j;
    ++ichar;
/*     WRITE NEXT CHARACTER IN STRING */
    pad_(&c__1, &j, &c__1);
    goto L300;

/*     END OF SYMBOL */
L400:
    pad_(&c__1, &c__1, &c__1);
/*     WRITE OCTAL ADDRESS */
    j = symbl_1.symbol[i__ - 1];
    i__ = (i__1 = symbl_1.symbol[j - 1], abs(i__1));
    j = symbl_1.symbol[j - 2];
    if (j % 16 == types_1.varb) {
	goto L410;
    }
/*     SYMBOL IS A LABEL, SO SHIFT RIGHT TO GET ADDR */
    i__ /= 65536;
L410:
    conout_(&c__1, &c__5, &i__, &c__16);
    addr__ = i__;
    pad_(&c__1, &c__1, &c__3);
    if (cntrl_1.contrl[12] == 0) {
	goto L430;
    }
    n = cntrl_1.contrl[25];
    cntrl_1.contrl[25] = cntrl_1.contrl[12];
    writel_(&c__0);
    l = 1;
    cntrl_1.contrl[25] = n;
L430:
    files_1.obp = cntrl_1.contrl[35] - 1;
    if (cntrl_1.contrl[23] == 0) {
	goto L440;
    }
    form_(&c__1, char__, &c__1, &c__32, &c__32);
    conout_(&c__1, &c__4, &addr__, &c__16);
    writel_(&c__0);
L440:
    goto L100;

L8000:
    error_(&c__143, &c__1);

L9000:
    if (l == 0) {
	goto L9999;
    }
    if (cntrl_1.contrl[12] == 0) {
	goto L9999;
    }
    pad_(&c__1, &c__1, &c__1);
    pad_(&c__1, &c__38, &c__1);
    n = cntrl_1.contrl[25];
    cntrl_1.contrl[25] = cntrl_1.contrl[12];
    writel_(&c__0);
    cntrl_1.contrl[25] = n;

L9999:
    cntrl_1.contrl[19] = m;
    return 0;
} /* sydump_ */

/*     PSTACK IS THE PROCEDURE STACK USED IN HL OPTIMIZATION */
/*     XFROPT IS USED IN BRANCH OPTIMIZTION */
/*     BUILT-IN FUNCTION CODE (MULTIPLICATION AND DIVISION) */

/*     OPRADRVALDEFLITLIN */
/*     ... PLM2 VERS ... */
/*     COMPILATION TERMINATED */
/*     STACK SIZE = OVERRIDDEN BYTES */
/*     PEEP IS USED IN PEEPHOLE OPTIMIZATION (SEE EMIT) */
/*         LAPOL IS A ONE ELEMENT POLISH LOOK-AHEAD */
/*         LASTLD IS CODLOC OF LAST REGISTER TO MEMORY STORE */
/*         LASTRG IS THE EFFECTED REGISTER */
/*         LASTIN IS THE CODLOC OF THE LAST INCREMENT */
/*         (USED IN DO-LOOP INDEX INCREMENT) */
/*         LASTEX IS LOCATION OF LAST XCHG OPERATOR */
/*         LASTIR IS THE CODLOC OF THE LAST REGISTER INCREMENT */
/*         (USED IN APPLY AND GENSTO TO GEN INR MEMORY) */
/*     BUILT-IN FUNCTION VECTOR -- */
/*     MULTIPLY AND DIVIDE OR MOD */
/*     +  FIRST TWO GIVE BASE LOCATIONS OF BIF CODE SEGMENTS */
/*     +  NEXT COMES NUMBER OF BYTES, NUMBER OF RELOCATIONS, AND */
/*     +  A VECTOR OF ABSOLUTE LOCATIONS WHERE STUFFS OCCUR */

/*     THE CODE SEGMENTS ARE ABSOLUTE, PACKED THREE PER ENTRY */


/* MULTIPLY */

/* 121 147 120 154 242 012 000 096 105 235 068 077 033 000 000 235 */
/* 120 177 200 235 120 031 071 121 031 079 210 030 000 025 235 041 */
/* 195 016 000 */

/* DIVIDE */

/* 122 047 087 123 047 095 019 033 000 000 062 017 229 025 210 018 */
/* 000 227 225 245 121 023 079 120 023 071 125 023 111 124 023 103 */
/* 241 061 194 012 000 183 124 031 087 125 031 095 201 */

/*     PASS-NOPROGRAM */
/*     ERROR */
/*     ()NEARAT */
/*     PARSE STACK */
/*     SYMBOL  ADDR WDS CHRS   LENGTH PR TY */
/*     NUMBER OF BYTES FOLLOWING FIRST 13 INSTRUCTIONS IN CATEGORY 3 */

/*  STA    011    000    LDA    011    000    XCHG   SPHL   PCHL */
/*  CMA    STC    CMC    DAA    SHLD   011    000    LHLD   011 */
/*  000    EI     DI     LXI B  011    000    PUSH B POP B  DAD B */
/*  STAX B LDAX B INX B  DCX B  NOP    NOP    NOP    NOP    NOP */
/*  050 011 000 058 011 000 235 249 233 047 055 063 039 034 011 000 */
/*  042 011 000 251 243 001 011 000 197 193 009 002 010 003 011 000 */
/*     INTBAS IS THE LARGEST INTRINSIC SYMBOL NUMBER */
/*     PRSTRASNLITV */

