/*
    Copyright 1982, 1983
    Alcyon Corporation
    8716 Production Ave.
    San Diego, Ca. 92121

    @(#)fscanf.c	1.1	11/9/83
*/

/*
**  formatted read routine
**
**      functionally equivalent to scanf in portable C library
*/

#include <stdio.h>
#include <math.h>

/* Delimiters */

#define NEWLINE '\n'
#define TAB     '\t'
#define SPACE   ' '
#define NULL    '\0'

/* returns from __next() */

#define CHAR        0
#define NOT_WHT     -1
#define NOT_WHT_NL  1

/* returns from __scan() */

#define NORETURN    0
#define VALID       1
#define NOMATCH     -1
#define AT_EOF      -2
#define ERROR       -3

FILE *__stream;
char **_p, *__sstr, __holdch;
int __smode, __hold;

scanf(parlist)
char *parlist;
{
    __smode = 0;
    _p = &parlist;
    return(__doscanf());
}

fscanf(stream,parlist)
FILE *stream;
char *parlist;
{
    if( (stream->_flag&(_RMODE|_UPDATE)) == 0 || feof(stream) ) 
        return(EOF);
        
    __smode = 1;
    __stream = stream;
    _p = &parlist;
    return(__doscanf());
}

sscanf(s,parlist)
char *s, *parlist;
{
    __smode = 2;
    __sstr = s;
    _p = &parlist;
    return(__doscanf());
}

__doscanf()
{
    register int    nmatch;
    register char   ch;
    char *format;
    register char match_ch;

    nmatch = __hold = 0;
    format = *_p++;
    while( 1 ) {
        switch (ch = *format++) {

         case NULL:
            return(nmatch);

         case '%':
            if( *format != '%' ) {
                switch (__scan(&format, *_p)) {
    
                 case VALID:    /* good return*/
                    _p++;
                    nmatch++;
                 case NORETURN: /* no return*/
                    break;
    
                 case NOMATCH:  /* no match */
                    return(nmatch);
    
                 case AT_EOF:   /* end of file */
                    return(nmatch ? nmatch : NOMATCH);
    
                 default:   /* syntax error */
                    return(NOMATCH);
    
                }
                break;
            }
            format++;

         default:
            match_ch = __next(CHAR);
            if( ch != match_ch ) {
                __unget(match_ch);
                return(nmatch ? nmatch : AT_EOF);
            }
            break;

        }
    }
}

/*
 * main scan routine -- look at characters in the conversion string
 * and do their bidding
 */
__scan(spec, result)
char    **spec;
char    *result;
{
    register int    longf, length;
    register char   ch;
    extern int  __strend(), __splend();

    longf = length = 0;
    while( 1 ) {
        switch (ch = *(*spec)++) {
         case '*':
            result = 0;
            break;
    
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            length = length * 10 + ch - '0';
            break;
    
         case 'l':
            if( longf )
                return(ERROR);
            longf = 1;
            break;
    
         case 'h':  /* short */
            if( longf )
                return(ERROR);
            longf = NOMATCH;
            break;
    
         case 'o':
         case 'O':
            return(__dec(result, length ? length : 100, 8, longf));
    
         case 'd':
         case 'D':
            return(__dec(result, length ? length : 100, 10, longf));
    
         case 'x':
         case 'X':
            return(__dec(result, length ? length : 100, 16, longf));
    
         case 'c':
         case 'C':
            if( longf )
                return(ERROR);
            return(__char(result, length ? length : 1));
    
         case 's':
         case 'S':
            if( longf )
                return(ERROR);
            return(__strx(result, length ? length : 100, __strend));
    
         case 'e':
         case 'E':
         case 'f':
         case 'F':
            if( longf )
                return(ERROR);
            return(__float(result, length ? length : 100));
            /*return(ERROR);        /* not yet implemented */
    
         case '[':
            if( longf )
                return(ERROR);
            if( __inits(spec) )
                return(ERROR);
            return(__strx(result, length ? length : 100, __splend));
    
         default:
            return(ERROR);
    
        }
    }
}

/*
 * get a constant -- octal, decimal, or hex depending on base
 */
__dec(result, length, base, longf)
register int    *result;
int length;
int base;
int longf;
{
    register char   ch;
    register int    val;
    register int    ndigit;
    register long   *lresult;
    register long   lres;
    register int    ires;
    register int    minus, ok;

    ires = 0;
    lres = 0;
    ndigit = minus = 0;
    switch (ch = __next(NOT_WHT_NL)) {

         case NULL:
         case EOF:
            return(AT_EOF);

         case '-':
            minus = 1;
         case '+':
            ndigit++;
            ch = __next(NOT_WHT);
    }
    ok = 0;
    while( (val = __digit(ch, base)) >= 0 && ndigit++ < length ) {
        ok++;
        if( longf )
            lres = lres * base + val;
        else
            ires = ires * base + val;
        ch = __next(CHAR);
    }
    __unget(ch);
    if( !ok )
        return(NOMATCH);
    if( !result )
        return(NORETURN);
    if( minus )
        if( longf )
            lres = -lres;
        else
            ires = -ires;
    if( longf ) {
        lresult = result;
        *lresult = lres;
    }
    else
        *result = ires;
    return(VALID);
}

/*
 * get a floating point constant
 */
__float(result, length)
register double *result;
int length;
{
    char buffer[100];
    double val;
    int ret, ch;

    ret = __strx(buffer, 100, __strend);
    val = atof(buffer);
    *result = val;
    return(ret);
}

__next(mode)
int mode;
{
/*
 *  mode -1: get next non-space or non-tab
 *  mode 0:  get next character
 *  mode 1:  get next non-space, non-tab, or non-newline
 */
    register int    ch;

    if( (ch = __getch()) == EOF )
        return(EOF);
    if( mode == 0 )
        return(ch);
    while( ch == SPACE || ch == TAB || ch == NEWLINE ) {
        if( ch == NEWLINE && mode < 0 )
            break;
        ch = __getch();
    }
    return(ch);
}

/*
 * check an input character for a valid constant digit (octal, decimal,
 * or hex) if found, return the proper numeric value.  Negative results
 * indicate error.
 */
__digit(ch, base)
register char   ch;
register int    base;
{
    register int    n;

    if( ch < '0' )
        return(NOMATCH);
    if( ch <= '7' )
        return(ch - '0');
    if( base == 8 )
        return(NOMATCH);
    if( ch <= '9' )
        return(ch - '0');
    if( base == 10 || ch < 'A' )
        return(NOMATCH);
    if( ch <= 'F' )
        return(ch - 'A' + 10);
    if( ch < 'a' || ch > 'f' )
        return(NOMATCH);
    return(ch - 'a' + 10);
}

/*
 * check for an end of string delimiter
 */
__strend(cha)
char    cha;
{
    register char   ch;

    if( (ch = cha) == EOF )
        return(EOF);
    if( ch == SPACE || ch == TAB || ch == NEWLINE || ch == NULL )
        return(VALID);
    return(NORETURN);
}

char    __splset[128];

/*
 * check for the occurrance of any character in the set which
 * the user wants to be end-of-string delimiters
 */
__splend(ch)
char    ch;
{
    if( ch == EOF )
        return(EOF);
    return(__splset[ch]);
}

/*
 * initialize the array which inidcates the special chars which the user
 * wants to be included (or not included) in strings.
 */
__inits(spec)
register char   **spec;
{
    register char   ch;
    register int    i;
    register int    val;

    ch = *(*spec)++;
    if( ch == '^' ) {
        val = 0;
        ch = *(*spec)++;
    }
    else
        val = 1;
    for (i = 1; i < 128; i++)
        __splset[i] = val;
    val = 1 - val;
    while( ch != ']' ) {
        if( ch == 0 )
            return(NOMATCH);
        __splset[ch & 0177] = val;
        ch = *(*spec)++;
    }
    __splset[0] = 1;
    return(NORETURN);
}

/*
 * getting a string
 */
__strx(result, length, endfn)
register char   *result;
register int    length;
register int    (*endfn)();
{
    register char   ch;
    extern int  __splend();
    register int    imode, notok;

    notok = 1;
    imode = (endfn != __splend);
    if (imode) {
		ch = __next(NOT_WHT_NL); 
		__unget(ch);	/* bypass tab or space... */
	}
    while( !(*endfn)( (ch = __next(imode)) ) && length-- > 0 ) {
        if( result )
            *result++ = ch;
        imode = notok = 0;
    }
    __unget(ch);
    if( notok )
        return(ch == EOF ? AT_EOF : NOMATCH);
    if( !result )
        return(NORETURN);
    *result = 0;
    return(VALID);
}

/*
 * getting a character constant
 */
__char(result, length)
register char   *result;
register int    length;
{
    register char   *r, ch;
    register int    l;

    r = result;
    l = length;

    while( l-- ) {
        if( (ch = __next(CHAR)) <= 0 ) {
            if( l + 1 == length )
                return(ch == EOF ? AT_EOF : NOMATCH);
            else
                return(result != 0);
        }
        if( result )
            *result++ = ch;
    }
    return(result != 0);
}

__getch()
{
    switch(__smode) {
        case 0:
            return(__gstdi());
        case 1:
            return(getc(__stream));
        case 2:
            return(__gs());
    }
}

__unget(ch)
char ch;
{
    switch(__smode) {
        case 0:
            __ugstdi(ch);
            break;
        case 1:
            ungetc(ch,__stream);
            break;
        case 2:
            __ungs(ch);
            break;
    }
}

/*
 * return the next char pointed to by *s 
 */
__gs()
{
    register char   c;

    c = *__sstr;
    if( c )
        __sstr++;
    else
        return(EOF);
    return(c);
}

/*
 * put back a char for further scanning
 */
__ungs(c)
char    c;
{
    if( c )
        __sstr--;
}

__gstdi()
{
    if( !__hold)
        return(getchar());
    else {
        __hold = 0;
        return(__holdch);
    }
}

__ugstdi(ch)
char ch;
{
    __hold = 1;
    __holdch = ch;
}
                                                                                                = 1;
    __holdch = ch;
}
                                                                                                = 1;
    __holdch = ch;
}
                                                                                                = 1;
    __holdch = ch;
}
                                                                                                