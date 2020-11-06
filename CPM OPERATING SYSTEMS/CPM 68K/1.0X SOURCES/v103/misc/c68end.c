/* @(#)end.c	2.3 */
/* if you need more space for string constants, increase size of __buffer */
char	__buffer[200];
char	_bufMax;	/* used only to size the buffer */

_end_()
{
    int	x;

    x = 1;	/* just something to hang a breakpoint on - see proc.c */

/* If you want to force certain routines from the standard libraries to be
 * available during debugging, place a reference to them here.  It is not
 * necessary for the call to be a 'complete' one, just enough to force the
 * loader to bring it in. For example, saying:
	printf();
 * is sufficient to load printf (assuming that it wasn't already
 * referenced in your normal program modules.
 *
 * If you do this, do it to YOUR OWN COPY(!!) of end.c, not the main one!
 */
}
