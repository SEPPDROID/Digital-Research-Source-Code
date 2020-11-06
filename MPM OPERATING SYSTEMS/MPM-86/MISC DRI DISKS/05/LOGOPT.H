#define CARDEF		/* Use inline macro version of elt *car(p) */
#define CDRDEF		/* Use inline macro version of elt *cdr(p) */

        /*STACK_CHK*/	/* No checks for null ptr in stack routines*/

#define STCOPYDEF	/* macro version of elt *stcopy()	   */
#define STREPDEF	/* macro version of elt *strep(p)	   */
#define STEMPTYDEF	/* macro version of elt *stempty	   */

#define NULLPDEF
#define SET_NOVALUEDEF

#ifdef CARDEF
#define car(p) ((p)->cell1.carf)
#else
elt *carf();
#endif

#ifdef CDRDEF
#define cdr(p) ((p)->cell1.cdrf)
#else
elt *cdrf();
#endif

#ifdef STCOPYDEF
#define stcopy() (elist->cell1.cdrf)
#else
elt *stcopy();
#endif

#ifdef STREPDEF
#define strep(p) replacd(elist,(p))
#else
elt *strep();
#endif

#ifdef STEMPTYDEF
#define stempty() (elist == NULL)
#else
int stempty();
#endif

#ifdef NULLPDEF
#define nullp(p) ((p) == NULL)
#else
int nullp();
#endif

#ifdef SET_NOVALUEDEF
#define set_novalue() retval = NOVALUE
#endif
