/* YACC parser for PL/M syntax.

$Id: plm-parse.y,v 1.1 1994/02/28 20:54:56 hays Exp $

Copyright 1994 by Kirk Hays (hays@ichips.intel.com) and Gary Funck
(gary@intrepid.com)

USE AT YOUR OWN RISK. NO WARRANTY EXPRESSED OR IMPLIED.
This code is distributed in the hope that it will be useful,
but without any warranty.  Further, there is no implied warranty of
merchantability or fitness for a particular purpose.

*/

/* This file defines the grammar of PL/M. */

%{

/*
 * YACC grammar of PL/M
 */
#include <stdio.h>
#include <stdlib.h>
#include "plm-manifest.h"
#include "scope.h"

void yyerror ();

/* Cause the `yydebug' variable to be defined.  */
#define YYDEBUG (1)

%}

%token ADDRESS AND ASSIGN_OP AT AT_SIGN BASED
%token BINARY_NUMBER BY BYTE CALL CASE
%token CAUSE_INTERRUPT CHARINT COLON COMMA DATA
%token DECIMAL_NUMBER DECLARE DISABLE DO DOT
%token DWORD ELSE ENABLE END EOF_KW EQ
%token EXTERNAL FLOATING_POINT_NUMBER GE GO
%token GOTO GT HALT HEX_NUMBER HWORD
%token IDENTIFIER
%token IF INITIAL_KW INTEGER INTERRUPT
%token LABEL LE LITERALLY LONGINT LPAREN LT
%token MINUS MINUS_SIGN MOD NE NOT
%token OCTAL_NUMBER OFFSET OR PLUS PLUS_SIGN
%token POINTER PROCEDURE PUBLIC QWORD
%token REAL REENTRANT RETURN RPAREN SELECTOR
%token SEMI SHORTINT SLASH STAR STRING
%token STRUCTURE THEN TO WHILE WORD XOR


%start compilation

%%
actual_parameters:
        LPAREN expression_list RPAREN
        ;
adding_operator:
        MINUS
        | MINUS_SIGN
        | PLUS
        | PLUS_SIGN
        ;
and_operator:
        AND
        ;
arithmetic_expression:
        term
        | arithmetic_expression adding_operator term
        ;
array_specifier:
        explicit_dimension
        | implicit_dimension
        ;
assignment_statement:
        left_part EQ expression SEMI
        ;
base_specifier:
        IDENTIFIER
        | IDENTIFIER DOT IDENTIFIER
        ;
basic_statement:
        assignment_statement
        | call_statement
        | goto_statement
        | microprocessor_dependent_statement
        | null_statement
        | return_statement
        ;
basic_type:
        ADDRESS
        | BYTE
        | CHARINT
        | DWORD
        | HWORD
        | INTEGER
        | LONGINT
        | OFFSET
        | POINTER
        | QWORD
        | REAL
        | SELECTOR
        | SHORTINT
        | WORD
        ;
bound_expression:
        expression
        ;
by_part:
        BY step_expression
        ;
call_statement:
        CALL simple_variable SEMI
        | CALL simple_variable actual_parameters SEMI
        ;
cast_type:
        basic_type
        ;
cause_interrupt_statement:
        CAUSE_INTERRUPT LPAREN integer_constant RPAREN SEMI
        ;
compilation:
        module
        | module EOF_KW
        ;
conditional_clause:
        if_condition true_unit
        | if_condition true_element ELSE false_element
        ;
constant:
        STRING
        | numeric_constant
        ;
constant_attribute:
        DATA
        ;
constant_list:
        constant
        | constant_list COMMA constant
        ;
declaration:
        declare_statement
        | procedure_definition
        ;
declaration_sequence:
        declaration
        | declaration_sequence declaration
        ;
declare_element:
        factored_element
        | unfactored_element
        ;
declare_element_list:
        declare_element
        | declare_element_list COMMA declare_element
        ;
declare_statement:
        DECLARE declare_element_list SEMI
        ;
disable_statement:
        DISABLE SEMI
        ;
do_block:
        do_case_block
        | do_while_block
        | iterative_do_block
        | simple_do_block
        ;
do_case_block:
        do_case_statement ending
        | do_case_statement unit_sequence ending
        ;
do_case_statement:
        DO CASE expression SEMI {
                                 push_scope();
                                }
        ;
do_while_block:
        do_while_statement ending
        | do_while_statement unit_sequence ending
        ;
do_while_statement:
        DO WHILE expression SEMI {
                                  push_scope();
                                 }
        ;
embedded_assignment:
        variable_reference ASSIGN_OP logical_expression
        ;
enable_statement:
        ENABLE SEMI
        ;
end_statement:
        END opt_identifier SEMI {
                                 pop_scope();
                                }
        ;
ending:
        end_statement
        | label_definition_sequence end_statement
        ;
explicit_dimension:
        LPAREN numeric_constant RPAREN
        ;
expression:
        embedded_assignment
        | logical_expression
        ;
expression_list:
        expression
        | expression_list COMMA expression
        ;
factored_element:
        factored_label_element
        | factored_variable_element
        ;
/*
 * factored_label_element doesn't permit based variables,
 * yet factored_variable_element does.  This can't be disambiguated
 * syntactically.  Thus, the factored_label element will have to
 * make the proper semantic checks to make the sure that the
 * variable_name_specifier_list is in fact an identifier_list.
 */ 
factored_label_element:
        LPAREN variable_name_specifier_list RPAREN LABEL opt_public_or_external
        ;
factored_member:
        LPAREN member_name_list RPAREN opt_explicit_dimension variable_type
        ;
factored_variable_element:
        LPAREN variable_name_specifier_list RPAREN opt_explicit_dimension variable_type opt_variable_attributes
        ;
false_element:
        unit
        ;
formal_parameter:
        IDENTIFIER
        ;
formal_parameter_list:
        formal_parameter
        | formal_parameter_list COMMA formal_parameter
        ;
formal_parameter_specifier:
        LPAREN formal_parameter_list RPAREN
        ;
go_to:
        GOTO
        | GO TO
        ;
goto_statement:
        go_to IDENTIFIER SEMI
        ;
halt_statement:
        HALT SEMI
        ;
id_colon:
        IDENTIFIER COLON
        ;
if_condition:
        IF expression THEN
        ;
implicit_dimension:
        LPAREN STAR RPAREN
        ;
index_part:
        index_variable EQ start_expression
        ;
index_variable:
        simple_variable
        ;
initial_value:
        expression
        ;
initial_value_list:
        initial_value
        | initial_value_list COMMA initial_value
        ;
initialization:
        DATA LPAREN initial_value_list RPAREN
        | INITIAL_KW LPAREN initial_value_list RPAREN
        ;
integer_constant:
        BINARY_NUMBER
        | DECIMAL_NUMBER
        | HEX_NUMBER
        | OCTAL_NUMBER
        ;
interrupt:
        INTERRUPT opt_interrupt_number
        ;
interrupt_number:
        integer_constant
        ;
iterative_do_block:
        iterative_do_statement ending
        | iterative_do_statement unit_sequence ending
        ;
iterative_do_statement:
        DO index_part to_part opt_by_part SEMI {
                                                push_scope();
                                               }
        ;
label_definition:
        id_colon
        ;
label_definition_sequence:
        label_definition
        | label_definition_sequence label_definition
        ;
label_element:
        IDENTIFIER LABEL opt_public_or_external
        ;
left_part:
        variable_reference_list
        ;
literal_element:
        IDENTIFIER LITERALLY STRING     {
                                          enter_literal ($1, $3);
                                        }
        ;
location_reference:
        AT_SIGN variable_reference
        | AT_SIGN LPAREN constant_list RPAREN
        | DOT variable_reference
        | DOT LPAREN constant_list RPAREN
        ;
locator:
        AT LPAREN expression RPAREN
        ;
locator_initialization:
        locator
        | initialization
        | locator initialization
        ;
logical_expression:
        logical_factor
        | logical_expression or_operator logical_factor
        ;
logical_factor:
        logical_secondary
        | logical_factor and_operator logical_secondary
        ;
logical_primary:
        arithmetic_expression
        | arithmetic_expression relation_operator arithmetic_expression
        ;
logical_secondary:
        logical_primary
        | NOT logical_primary
        ;
member_element:
        structure_type
        | factored_member
        | unfactored_member
        ;
member_element_list:
        member_element
        | member_element_list COMMA member_element
        ;
member_name:
        IDENTIFIER
        ;
member_name_list:
        member_name
        | member_name_list COMMA member_name
        ;
member_specifier:
        DOT member_name 
        | DOT member_name subscript
        ;
member_specifier_sequence:
        member_specifier_sequence member_specifier
        | member_specifier
        ;
microprocessor_dependent_statement:
        cause_interrupt_statement
        | disable_statement
        | enable_statement
        | halt_statement
        ;
module:
        module_name COLON simple_do_block
        ;
module_name:
        IDENTIFIER
        ;
multiplying_operator:
        MOD
        | SLASH
        | STAR
        ;
null_statement:
        SEMI
        ;
numeric_constant:
        FLOATING_POINT_NUMBER
        | integer_constant
        ;
opt_array_specifier:
        /* empty */
        | array_specifier
        ;
opt_by_part:
        /* empty */
        | by_part
        ;
opt_explicit_dimension:
        /* empty */
        | explicit_dimension
        ;
opt_formal_parameter_specifier:
        /* empty */
        | formal_parameter_specifier
        ;
opt_identifier:
        /* empty */
        | IDENTIFIER
        ;
opt_interrupt_number:
        /* empty */
        | interrupt_number
        ;
opt_procedure_attribute_sequence:
        /* empty */
        | procedure_attribute_sequence
        ;
opt_procedure_type:
        /* empty */
        | procedure_type
        ;
opt_public_or_external:
        /* empty */
        | EXTERNAL
        | PUBLIC
        ;
opt_variable_attributes:
        /* empty */
        | variable_attributes
        ;
or_operator:
        OR
        | XOR
        ;
primary:
        constant
        | location_reference
        | subexpression
        | variable_reference
        ;
procedure_attribute:
        EXTERNAL
        | PUBLIC
        | REENTRANT
        | interrupt PUBLIC
        | interrupt EXTERNAL
        ;
procedure_attribute_sequence:
        procedure_attribute
        | procedure_attribute_sequence procedure_attribute
        ;
procedure_definition:
        procedure_statement ending
        | procedure_statement declaration_sequence ending
        | procedure_statement unit_sequence ending
        | procedure_statement declaration_sequence unit_sequence ending
        ;
procedure_statement:
        id_colon PROCEDURE opt_formal_parameter_specifier opt_procedure_type opt_procedure_attribute_sequence SEMI
                {
                 push_scope();
                }       
        ;
procedure_type:
        basic_type
        ;
relation_operator:
        EQ
        | GE
        | GT
        | LE
        | LT
        | NE
        ;
return_statement:
        typed_return
        | untyped_return
        ;
secondary:
        primary
        | unary_sign primary
        ;
simple_do_block:
        simple_do_statement ending
        | simple_do_statement unit_sequence ending
        | simple_do_statement declaration_sequence ending
        | simple_do_statement declaration_sequence unit_sequence ending
        ;
simple_do_statement:
        DO SEMI {
                 push_scope();
                }
        ;
simple_variable:
        IDENTIFIER
        | IDENTIFIER DOT IDENTIFIER
        ;
start_expression:
        expression
        ;
step_expression:
        expression
        ;
structure_type:
        STRUCTURE LPAREN member_element_list RPAREN
        ;
subexpression:
        LPAREN expression RPAREN
        ;
subscript:
        LPAREN expression RPAREN
        ;
subscript_or_actual_parameters:
        LPAREN expression_list RPAREN
        ;
term:
        secondary
        | term multiplying_operator secondary
        ;
to_part:
        TO bound_expression
        ;
true_element:
        true_statement
        | label_definition_sequence true_statement
        ;
true_statement:
        do_block
        | basic_statement
        ;
true_unit:
        unit
        ;
typed_return:
        RETURN expression SEMI
        ;
unary_minus:
        MINUS_SIGN
        ;
unary_plus:
        PLUS_SIGN
        ;
unary_sign:
        unary_minus
        | unary_plus
        ;
unfactored_element:
        label_element
        | literal_element
        | variable_element
        ;
unfactored_member:
        member_name opt_explicit_dimension variable_type
        ;
unit:
        unit_element
        | label_definition_sequence unit_element
        ;
unit_element:
        basic_statement
        | conditional_clause
        | do_block
        ;
unit_sequence:
        unit
        | unit_sequence unit
        ;
untyped_return:
        RETURN SEMI
        ;
variable_attributes:
        EXTERNAL constant_attribute
        | EXTERNAL
        | PUBLIC locator_initialization
        | PUBLIC
        | locator_initialization
        ;
variable_element:
        variable_name_specifier opt_array_specifier variable_type opt_variable_attributes
        ;
variable_name_specifier:
        IDENTIFIER
        | IDENTIFIER BASED base_specifier
        ;
variable_name_specifier_list:
        variable_name_specifier
        | variable_name_specifier_list COMMA variable_name_specifier
        ;
/*
 * Variable references may be either data references or function
 * references.  Syntactically, they appear to be the same, each
 * is followed by a parenthesized comma separated list of expressions.
 *
 * A function reference, of course, cannot have the trailing list of 
 * member specifiers - semantic checking will catch this.
 */
variable_reference:
        IDENTIFIER
        | IDENTIFIER member_specifier_sequence
        | cast_type subscript
        | IDENTIFIER subscript_or_actual_parameters
        | IDENTIFIER subscript_or_actual_parameters member_specifier_sequence
        ;
variable_reference_list:
        variable_reference
        | variable_reference_list COMMA variable_reference
        ;
variable_type:
        basic_type
        | structure_type
        ;

%%

void
yyerror(char * s)
{
  fprintf (stderr, "error at line %d: %s\n", lineno, s);
}

main() 
{
  init_scope();
  return yyparse();
}
