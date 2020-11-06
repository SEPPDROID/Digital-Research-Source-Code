%{
/* lexer for PL/M syntax.

$Id: plm-lex.l,v 1.2 1994/02/28 22:24:34 hays Exp $

Copyright 1994 by Kirk Hays (hays@ichips.intel.com) and Gary Funck
(gary@intrepid.com)

USE AT YOUR OWN RISK. NO WARRANTY EXPRESSED OR IMPLIED.
This code is distributed in the hope that it will be useful,
but without any warranty.  Further, there is no implied warranty of
merchantability or fitness for a particular purpose.

*/

/* This file defines the syntax of PL/M. */

#include <ctype.h>
#include <string.h>
#include "plm-manifest.h"
#include "plm-parse.h"
#include "scope.h"

int lineno = 1;

/* forward references */
char * canonical_string (char *);
char * canonical_identifier (char *);
static void error (char *);

%}

DIGIT_STRING [0-9][0-9$]*
A       [Aa]($)*
B       [Bb]($)*
C       [Cc]($)*
D       [Dd]($)*
E       [Ee]($)*
F       [Ff]($)*
G       [Gg]($)*
H       [Hh]($)*
I       [Ii]($)*
L       [Ll]($)*
M       [Mm]($)*
N       [Nn]($)*
O       [Oo]($)*
P       [Pp]($)*
Q       [Qq]($)*
R       [Rr]($)*
S       [Ss]($)*
T       [Tt]($)*
U       [Uu]($)*
W       [Ww]($)*
X       [Xx]($)*
Y       [Yy]($)*

%%

{A}{D}{D}{R}{E}{S}{S}           return ADDRESS;
{A}{N}{D}                       return AND;
{A}{T}                          return AT;
{B}{A}{S}{E}{D}                 return BASED;
{B}{Y}                          return BY;
{B}{Y}{T}{E}                    return BYTE;
{C}{A}{L}{L}                    return CALL;
{C}{A}{S}{E}                    return CASE;
{C}{A}{U}{S}{E}{I}{N}{T}{E}{R}{R}{U}{P}{T}      return CAUSE_INTERRUPT;
{C}{H}{A}{R}{I}{N}{T}           return CHARINT;
{D}{A}{T}{A}                    return DATA;
{D}{E}{C}{L}{A}{R}{E}           return DECLARE;
{D}{I}{S}{A}{B}{L}{E}           return DISABLE;
{D}{O}                          return DO;
{D}{W}{O}{R}{D}                 return DWORD;
{E}{L}{S}{E}                    return ELSE;
{E}{N}{A}{B}{L}{E}              return ENABLE;
{E}{N}{D}                       return END;
{E}{O}{F}                       return EOF_KW;
{E}{X}{T}{E}{R}{N}{A}{L}        return EXTERNAL;
{G}{O}                          return GO;
{G}{O}{T}{O}                    return GOTO;
{H}{A}{L}{T}                    return HALT;
{H}{W}{O}{R}{D}                 return HWORD;
{I}{F}                          return IF;
{I}{N}{I}{T}{I}{A}{L}           return INITIAL_KW;
{I}{N}{T}{E}{G}{E}{R}           return INTEGER;
{I}{N}{T}{E}{R}{R}{U}{P}{T}     return INTERRUPT;
{L}{A}{B}{E}{L}                 return LABEL;
{L}{I}{T}{E}{R}{A}{L}{L}{Y}     return LITERALLY;
{L}{O}{N}{G}{I}{N}{T}           return LONGINT;
{M}{I}{N}{U}{S}                 return MINUS;
{M}{O}{D}                       return MOD;
{N}{O}{T}                       return NOT;
{O}{F}{F}{S}{E}{T}              return OFFSET;
{O}{R}                          return OR;
{P}{L}{U}{S}                    return PLUS;
{P}{O}{I}{N}{T}{E}{R}           return POINTER;
{P}{R}{O}{C}{E}{D}{U}{R}{E}     return PROCEDURE;
{P}{U}{B}{L}{I}{C}              return PUBLIC;
{Q}{W}{O}{R}{D}                 return QWORD;
{R}{E}{A}{L}                    return REAL;
{R}{E}{E}{N}{T}{R}{A}{N}{T}     return REENTRANT;
{R}{E}{T}{U}{R}{N}              return RETURN;
{S}{E}{L}{E}{C}{T}{O}{R}        return SELECTOR;
{S}{H}{O}{R}{T}{I}{N}{T}        return SHORTINT;
{S}{T}{R}{U}{C}{T}{U}{R}{E}     return STRUCTURE;
{T}{H}{E}{N}                    return THEN;
{T}{O}                          return TO;
{W}{H}{I}{L}{E}                 return WHILE;
{W}{O}{R}{D}                    return WORD;
{X}{O}{R}                       return XOR;

[_A-Za-z][_$0-9A-Za-z]*         {
                                 char * string;
                                 int i;
                                 yylval = canonical_identifier(yytext);
                                 string = lookup_literal(yylval);
                                 if (!string)
                                   {
                                     return IDENTIFIER;
                                   }
                                 free(yylval); 
                                 yylval=0; /*excessive neatness*/
                                 /* push the string back onto the input
                                    stream - it is necessary to push 
                                    from right to left */
                                 for (i = strlen (string);
                                      i >= 0;
                                      i--) 
                                   {
                                     /* very weak - depends on lexical 
                                        generator allowing sufficient
                                        push-back */
                                     unput (string[i]);
                                   }
                                }

[0-9][0-9$]*(d)?                return DECIMAL_NUMBER;

{DIGIT_STRING}\.({DIGIT_STRING})?([E|e][+|-]?{DIGIT_STRING})?   return FLOATING_POINT_NUMBER;

[01][01$]*[bB]                  return BINARY_NUMBER;

[0-9][0-9a-fA-F$]*[hH]          return HEX_NUMBER;

[0-7][0-7$]*[OoQq]              return OCTAL_NUMBER;

'([^'\n\201]|(''))+'            {
                                 yylval = canonical_string(yytext); 
                                 return STRING;
                                }

:=                              return ASSIGN_OP;

[\@]                            return AT_SIGN;

:                               return COLON;

,                               return COMMA;

[\.]                            return DOT;

=                               return EQ;

>=                              return GE;

>                               return GT;

"<="                            return LE;

\(                              return LPAREN;

"<"                             return LT;

-                               return MINUS_SIGN;

"<>"                            return NE;

"+"                             return PLUS_SIGN;

\)                              return RPAREN;

;                               return SEMI;

"/"                             return SLASH;

\*                              return STAR;

\f

\n                              lineno++;

[\t ]*

"/*"                            {
                                register int c;

                                for ( ; ; )
                                  {
                                    while ( (c = input()) != '*' &&
                                           c != EOF )
                                      if (c == '\n') lineno++;
                                      /* eat up text of comment */

                                    if ( c == '*' )
                                      {
                                        while ( (c = input()) == '*' )
                                          ;
                                        if ( c == '/' )
                                          break;    /* found the end */
                                      }

                                    if (c == '\n') lineno++;

                                    if ( c == EOF )
                                      {
                                        error( "EOF in comment" );
                                        break;
                                      }
                                  }
                              }

.

%%

static void error (char * x) {printf (x);}
 
/* Strip the single quotes from a pl/m string, and
   compress any single quote pairs to one quote.

   Allocates new storage for the string.
*/
char * 
canonical_string (char * s)
{
  int i, i_ret;
  char * ret;
  int len = strlen (s);

  ret = malloc (len+1);
  if (!ret)
    return 0;

  for (i = 1, i_ret = 0; i < (len-1); i++)
    {
      ret[i_ret++] = s[i];
      if (s[i] == '\'')
        {
          i++;
        }
    }

  ret[i_ret] = 0;

  return ret;

}

/* Strip the dollar signs from a pl/m identifier or 
   numeric constant, and force all alphabetic characters 
   to lower case.

   Allocates new storage for the identifier string.

*/
char * 
canonical_identifier (char * s)
{
  int i, i_ret;
  char * ret;
  int len = strlen (s);

  ret = malloc (len+1);
  if (!ret)
    return 0;

  for (i = 0, i_ret = 0; i < len; i++)
    {
      if (s[i] == '$')
        {
          continue;
        }
      ret[i_ret++] = tolower (s[i]);
    }

  ret[i_ret] = 0;

  return ret;

}

#ifdef LEX_ONLY

main() {

  for (;;)
    yylex();

}

#endif
