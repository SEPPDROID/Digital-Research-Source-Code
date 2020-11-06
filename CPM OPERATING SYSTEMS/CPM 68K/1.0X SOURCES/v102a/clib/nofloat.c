/* NOFLOAT: provides 'stubroutines' to satisfy floating point library	    */
/*	references from 'printf' et al.					    */


nofloat() { ; }					/* stubroutine 'tag'	    */

char * __nofloat_msg = "floating point";

_petoa() { _optoff(__nofloat_msg); }
_pftoa() { _optoff(__nofloat_msg); }
_pgtoa() { _optoff(__nofloat_msg); }
_atof()   { _optoff(__nofloat_mst); }
