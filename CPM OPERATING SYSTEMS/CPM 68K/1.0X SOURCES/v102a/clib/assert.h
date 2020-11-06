#ifdef  IN_SCCS_ID
static char S_assert.h [] = "@(#)assert.h        3.1      11:30:01 - 82/01/26 ";
#endif  IN_SCCS_ID
# ifndef NDEBUG
# define _assert(ex) {if (!(ex)){fprintf(stderr,"Assertion failed: file %s, line %d\n", __FILE__, __LINE__);exit(1);}}
# def
ine assert(ex) {if (!(ex)){fprintf(stderr,"Assertion failed: file %s, line %d\n", __FILE__, __LINE__);exit(1);}}
# else
# define _assert(ex) ;
# define assert(ex) ;
