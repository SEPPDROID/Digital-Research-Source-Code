/* public interfaces for literal symbol table for PL/M.

$Id: scope.h,v 1.1 1994/02/28 20:56:13 hays Exp $

Copyright 1994 by Kirk Hays (hays@ichips.intel.com)

USE AT YOUR OWN RISK. NO WARRANTY EXPRESSED OR IMPLIED.
This code is distributed in the hope that it will be useful,
but without any warranty.  Further, there is no implied warranty of
merchantability or fitness for a particular purpose.

This code implements a scoped symbol table.  It depends on Paul
Vixie's PD AVL Tree code, distributed in comp.sources.unix, Volume 27,
Issue 34.

*/

char * lookup_literal (char *);
void enter_literal (char *, char *);
void push_scope (void);
void pop_scope (void);
void init_scope (void);
