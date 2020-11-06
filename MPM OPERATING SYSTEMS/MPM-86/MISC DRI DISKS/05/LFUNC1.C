#include <logo.h>
/*****************************************************************/
/*                                                               */
/*                  LOGO Primitive Functions-1                   */
/*                                                               */
/*****************************************************************/
double tan();
double sin();
double cos();
double sqrt();
double atan();
double exp();
double log();
double log10();
double fabs();

lnum rnval();
lnum nval();

sub_f ();
add_f ();
div_f ();
lss_f ();
gtr_f ();
mul_f ();
pow_f ();

nextcom()
	{
	editon   = FALSE;		/* not in editor */
	defining = FALSE;		/* not in TO */
	getcom('?');
	if (acceq("to"))
		makdef();
	else
		{
		if (!(initial || inpath == CONSOLE))
			{
			if (!((acceq("make")) ||
			      (acceq("package")) ||
			      (acceq("pprop"))))
				putcom();
			}
		if (acceq("bye"))
			{
			full_scr();
			exit(0);
			}
		else
		if (acceq("collect"))
			debcoll = !debcoll;
		else
			{
			interp();
			}
		}
	}
