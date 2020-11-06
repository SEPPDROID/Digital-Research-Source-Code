/* literal symbol table for PL/M.

$Id: scope.c,v 1.1 1994/02/28 20:55:56 hays Exp $

Copyright 1994 by Kirk Hays (hays@ichips.intel.com)

USE AT YOUR OWN RISK. NO WARRANTY EXPRESSED OR IMPLIED.
This code is distributed in the hope that it will be useful,
but without any warranty.  Further, there is no implied warranty of
merchantability or fitness for a particular purpose.

This code implements a scoped symbol table.  It depends on Paul
Vixie's PD AVL Tree code, distributed in comp.sources.unix, Volume 27,
Issue 34.

*/

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "scope.h"

typedef struct lit_table_stack_t_elem {
  struct lit_table_stack_t_elem *next;
  tree * literal_table;
} lit_table_stack_t;

/* innermost scope for scope list */
static lit_table_stack_t  *lit_table_stack;

typedef struct {
  char * identifier;
  char * string;
} literal_t;

/* static forward references */
static void new_scope (lit_table_stack_t *);
static int literal_compar (literal_t *, literal_t *);
static void literal_error (literal_t *);
static void literal_t_delete (literal_t *);

/* lookup an identifier in the scoped symbol table */
char *
lookup_literal (char * identifier)
{
  literal_t * datum;
  literal_t * node;
  lit_table_stack_t * scope;
  
  node = alloca (sizeof (literal_t));
  node->identifier = identifier;
  
  for (scope = lit_table_stack; scope; scope=scope->next)
    {
      datum = tree_srch(&(scope->literal_table), literal_compar, node);
      if (datum)
        {
          return datum->string;
        }
    }
  
  return 0;
  
}

/* enter an identifier in the current scoping level */
void
enter_literal (char * identifier, char * string)
{  
  literal_t * datum;
  
  datum = malloc (sizeof (literal_t));
  datum->identifier = identifier;
  datum->string = string;
  tree_add (&(lit_table_stack->literal_table),
            literal_compar,
            datum,
            literal_error);
}

/* increase scope depth by one level - creates a new symbol table
   for the current scope */
void 
push_scope (void)
{
  new_scope (lit_table_stack);
}

/* remove the innermost scope of the symbol table - releases all
   allocated storage for that scope */
void 
pop_scope (void)
{
  lit_table_stack_t * p;
  
  p = lit_table_stack;
  lit_table_stack = lit_table_stack->next;
  tree_mung (&(p->literal_table), literal_t_delete);
  if (p->literal_table)
    {
      free(p->literal_table);
    }
  free(p);
}

/* initialize this module, creating the outermost scope */
void 
init_scope (void) 
{
  new_scope (0);
}

/* work procedure to create a new scope */
static void 
new_scope (lit_table_stack_t * next)
{
  lit_table_stack = malloc (sizeof (lit_table_stack_t));
  lit_table_stack->next = next;
  tree_init (&(lit_table_stack->literal_table));
}

/* internal procedure to free storage when a symbol table entry is
   deleted */
static 
void literal_t_delete (literal_t * datum)
{
  free (datum->string);
  free (datum->identifier);
}

/* internal procedure to determine the match order for two symbol
   table entries */

static int 
literal_compar (literal_t * left, literal_t * right)
{
  return strcmp (left->identifier, right->identifier);
}

/* assertion procedure to assure that duplicate literals are never
   added at a single scoping level */

static void 
literal_error (literal_t * in_error)
{
  fprintf (stderr, 
	   "literal table error - attempt to enter same identifier twice\n");
  exit (255);
}


