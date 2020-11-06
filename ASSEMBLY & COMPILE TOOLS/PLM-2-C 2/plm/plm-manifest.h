/* manifest constants/types for PL/M lexer/parser.

$Id: plm-manifest.h,v 1.1 1994/02/28 20:54:26 hays Exp $

Copyright 1994 by Kirk Hays (hays@ichips.intel.com)

USE AT YOUR OWN RISK. NO WARRANTY EXPRESSED OR IMPLIED.
This code is distributed in the hope that it will be useful,
but without any warranty.  Further, there is no implied warranty of
merchantability or fitness for a particular purpose.

*/

extern int lineno;

/* brute force type the value stack - this would not be appropriate in
   a real compiler, but suits our purposes just fine */

typedef char * CHAR_PTR;
#define YYSTYPE CHAR_PTR

