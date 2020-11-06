/* macro's and define's useful to write test programs and to build test
   suites */
/* NOV 83 - Bill Fitler */
/* FEB 84 - Ken Chai */

/*********************** From "C Puzzle Book": ****************************/
#define PR(format,value) printf("\n\tvalue = %format\t",(value))
#define NL printf("\n")

#define PRINT1(f,x1)   PR(f,x1); NL
#define PRINT2(f,x1,x2)   PR(f,x1); PRINT1(f,x2)
#define PRINT3(f,x1,x2,x3)   PR(f,x1); PRINT2(f,x2,x3)
#define PRINT4(f,x1,x2,x3,x4)   PR(f,x1); PRINT3(f,x2,x3,x4)
/********************** end of C Puzzle's "defs.h" ***********************/

#define PRINT_EXPR(format,expr) printf("\n\texpr = format\n",(expr))
#define PRE(fmt,expr,val) printf("\n\texpected value of expr = %fmt",val)
#define PRA(fmt,expr) printf("\n\tactual value of expr = %fmt\n",(expr))
#define MODEL_NAME ((sizeof(char *) == 2)   ?  \
               (sizeof(char (*)())==2  ?  "small"  :  "medium")  :  \
               (sizeof(char (*)())==2  ?  "compact"  :  "big")  )
#define TRUE_COND(expr) printf("\n\tassertion succeeded: expr\n") 
#define FALSE_COND(expr) printf("\n\tassertion failed: expr\n")
#define TEST_OK(expr) if (expr) TRUE_COND(expr)
#define TEST_NOK(expr) if (expr) FALSE_COND(expr)
#define PRS(b,c) printf("\n\t%s: succeeded for %s model", c, b)
#define PRF(b,c) printf("\n\t%s: failed for %s model", c, b)
#define ASSER(a,b) {if (a) PRS(model,b) ; else PRF(model,b); }
#define PRI(format,val) printf("\n\twe expect to see %format", val)
#define ASSERT(cond) {{model=MODEL_NAME;} ; ASSER(cond,bugnum); }
#define ASSRT(cond) ASSER(cond,bugnum)


/* #define PRS68(c) printf("\n\t%s: succeeded", c)
#define PRF68(b) printf("\n\t%s: failed", b)
#define ASSER68(a,b) {if (a) PRS68(b) ; else PRF68(b); }
#define PRI(format,val) printf("\n\twe expect to see %format", val)
#define ASSERT68(cond) ASSER68(cond,bugnum)
#define ASSRT68(cond) ASSER68(cond,bugnum) */
*/
*/
