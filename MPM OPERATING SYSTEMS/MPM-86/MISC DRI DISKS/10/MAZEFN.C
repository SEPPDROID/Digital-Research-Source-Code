#include	"cpyrt.h"			/* DRI copyright message    */
/****************************************************************************/
/*									    */
/*	The  source code contained in this listing is a proprietary trade   */
/*	secret of Digital Research Inc., Pacific Grove, California and is   */
/*	copyrighted  as  an  unpublished  work pursuant to Section 408 of   */
/*	Title  17  of  the  United  States  Code.  Unauthorized  copying,   */
/*	adaptation, distribution, use or display is prohibited by law.	    */
/*									    */
/*	Author:	 xxxxxxxxxxx						    */
/*	PRODUCT: MAZE VidLink Program					    */
/*	Module:  mazefn							    */
/*									    */
/****************************************************************************/

/****************************************************************************/
/*									    */
/*	MAZE Module: MAZEFN, Maze Function				    */
/*	-----------------------------					    */
/*									    */
/*		This is the MAZE function module.  This module		    */
/*	provides the VidLink Maze Program functions.		  	    */
/*									    */
/*	The following routines are present:				    */
/*									    */
/*		maze		MAZE function				    */
/*									    */
/*	The following routines are called:				    */
/*									    */
/*		xxxxxxxxx	xxxxxxxxxxxxxxxx			    */
/*									    */
/*	Revision History:						    */
/*									    */
/*		06/20/84  by  xxxxxxxxxxxx				    */
/*									    */
/****************************************************************************/

/*
 *	Include Files
 */						/****************************/
#include	"portab.h"			/* DRI portable coding conv */
						/****************************/
#include	"ascii.h"			/* ASCII character defines  */
						/****************************/
#include	"color.h"			/* color defines	    */
						/****************************/
#include	"graph.h"			/* graphics chars	    */
						/****************************/

/*#page*/
/*
 *	External Data/Functions:
 */

#include	"vcrt.d"

#include	"mazex.d"			/* maze function extrnl data*/

EXTERN	WORD	vdpiof();

EXTERN	WORD	VIOSTAT();


/*#page*/
/*
 *	Local Data/Functions:
 */
#define	MAX_PLAYERS	6
#define QMAX		4000		/* size of question buffer */
#define AMAX		40		/* size of answer buffer */
#define RETURN		0x0d		/* carriage return */
#define BACKSP		0x09		/* backspace ?? */

struct scene =
    {
    UWORD	start;
    UWORD	finish;
    BYTE	right;
    BYTE	wrong;
    };

struct scene intro [] =
    {
    /* 01 */ 00150, 01482, 02, 00,
    /* 02 */ 01800, 02120, 00, 00
    };

struct scene nightmare_castle [] =
    {
    / * 01 */ 02138, 02202, 02, 01
    };

struct scene blast_off [] =
    {
    /* 01 */  00000, 00000, 00, 00
    };

struct scene first_and_ten [] =
    {
    /* 01 */  00000, 00000, 00, 00
    };

struct scene shoot_out [] =
    {
    /* 01 */  00000, 00000, 00, 00
    };

struct player_state =
    {
    BYTE	score;
    UBYTE	adventure;
    UBYTE	scene;
    UBYTE	group;
    BOOLEAN	bonus;
    };

struct player_state player [MAX_PLAYERS];

BYTE	c_scene;		/* current scene */
BYTE	c_player;		/* current player */
BYTE	last_player;		/* last player number */

UBYTE	row;			/* current row */

UBYTE	q_buffer[QMAX];		/* question buffer */
UBYTE	eoq;			/* TRUE if end of question file */
UWORD	q_size;			/* size of q_buffer (normally QMAX) */
UWORD	q_next;			/* next q_buffer to fill */
UWORD	g_next;			/* next group number */
UWORD	g_count;		/* group count */
UWORD	g_base;			/* group base */
UBYTE	g_max;			/* used to find next question */

UBYTE	ans[AMAX];		/* answer buffer */
UBYTE	a_size;			/* current answer size */

/*#page*/
/****************************************************************************/
/*									    */
/*				xxxxxxxx				    */
/*			        --------				    */
/*									    */
/*		This subroutine performs the xxxxxxxxxxxxxxxxxxxxxxxx.	    */
/*									    */
/****************************************************************************/
GLOBAL	maze(argc,argv)
						/****************************/
WORD	argc;					/* cmd tail argument count  */
CHAR	*argv[];				/* cmd tail string pointers */
    {						/*****	begin maze	*****/
    CHAR c;


#ifdef	CPU_65
    d_ouchar(14);				/* switch to lower case	    */
#endif
    clear();
    sign_on();
    vdpiof("VC");
    p_inchar();

    clear();
    set_row(5);
    l_outstr("     Do you want to see");
    set_row(6);
    l_outstr("     the Introduction (y/n)? ");
    c = p_inchar();
    if ((c == 'Y') || (c == 'y'))
	{
	clear();
	set_row(5);
	l_outstr("     Insert Maze Mania Side #1 ");
	set_row(6);
	l_outstr("     Strike any key to continue ");
	c = p_inchar();
	vdpiof("FR1SE");
	if (VIOSTAT())
	    {		/* not in play mode */
	    while (!VIOSTAT())
		;
	    while (vdpiof("PL"))
		{
		set_row(8);
		l_outstr("     Please insert disc.");
		}
	    set_row(9);
	    l_outstr("     Thank-you.");
	    while (!VIOSTAT())
		;
	    vdpiof("PA");
	    }
	else
	    while (!VIOSTAT())
		;
	clear();
	set_row(5);
	l_outstr("     Introduction Part #1 ");
	vdpiof("VOVP");

	if (!sequence((UWORD)150,(UWORD)1482))
	    {
	    vdpiof("VC");
	    p_move(6,0);
	    l_outstr("     Introduction Part #2 ");
	    vdpiof("VP");
	    sequence((UWORD)1800,(UWORD)2120);
	    }
	vdpiof("VC");
	}

    init_players();
    c_scene = 1;
    c_player = 0;
    g_next   = 0;
    g_count = 0;
    q_size   = QMAX;
    while (select_player())
	ask_question();

    /* saved for reference
    p_clear();
    p_move(9,0);
    l_outstr("     Sequence test.");
    p_move(10,0);
    l_outstr("     Strike any key to abort.");
    vdpiof("VP");
    while (!sequence((UWORD)12450,(UWORD)12500))
	;
    */

#ifdef	CPU_65
    d_ouchar(142);				/* switch to upper case	    */
#endif
    }						/*****	end maze	*****/

/*#page*/
/****************************************************************************/
/*									    */
/*				xxxxxxxx				    */
/*			        --------				    */
/*									    */
/*		This subroutine performs the xxxxxxxxxxxxxxxxxxxxxxxx.	    */
/*									    */
/****************************************************************************/
MLOCAL sign_on()
    {
    UBYTE	i;
    UBYTE	j;
    UBYTE	k;

    p_color(BLUE);
    i = 0;
    j = (p_crt.rows - h_so_maze) >> 1;
    k = (p_crt.cols - w_so_maze) >> 1;

    while (so_maze[i])
	{
	p_move(i+j,k);
	l_outstr(so_maze[i++]);
	}
    }

GLOBAL	l_outstr(s)

CHAR	*s;
    {
#ifdef	CPU_65
    CHAR	c;

    while (c = *s++)
	{
	if ((c >= 'a') && (c <= 'z'))		/* if lower case	    */
	    c = c & 0xdf;			/*	then to upper	    */
	else
	if ((c >= 'A') && (c <= 'Z'))		/* if upper case	    */
	    c = c | 0x20;			/*	then to lower	    */
	d_ouchar(c);
	}
#else
	p_outstr(s);
#endif
    }

newline()
    {
    set_row(row+1);
    }

set_row(r)

UBYTE	r;
    {
    p_move(row = r, 0);
    }

clear()
    {
    row = 0;
    p_clear();
    }

stop_display()
    {
    UWORD i;
    for (i = 0; i < 30000; i++);
    }

BOOLEAN	read_group()
    {
    UWORD q, k, k_loc;
    UBYTE n, n1, n2;
    if (eoq || (eoq = !(n = gn_num())))
	return;
    q_next = g_next;
    put_q(n);
    while (n--)
	{
	n1 = gn_num();
	n2 = gn_num();
	k_loc = q_next;
	put_q(0);
	put_q(0);
	k = 0;
	while (n1--)
	    k += gn_nstr();
	put_q('\0');
	while (n2--)
	    k += gn_nstr();
	put_q('\0');
	k += 2;
	put_nq(k_loc, k & 0xff);
	put_nq(k_loc+1, k >> 16);
	if (q_next >= q_size - 2)
	    return (FALSE);
	g_next = q_next;
	g_count++;
	return (TRUE);
	}
    }

UBYTE gn_num()
    {
    UBYTE x, d;
    x = 0;
    while ((d = gnc()) == ' ' || d == ',')
	;
    while (d = d - '0') <= 9)
	x = x * 10 + d;
    return (x);
    }

UWORD gn_nstr()
    {
    UWORD d;
    UBYTE c;
    d = 1;
    while (gnc() != '"')
	;
    while ((c = gnc()) != '"')
	{
	put_q(c);
	d++;
	}
    put_q('\n');
    return (d);
    }

UBYTE gnc()
    {
    if (eoq)
        return ('"');
    /* note: read next character */
    return ('#');
    }

put_q(x)

UBYTE	x;
    {
    put_nq(q_next,x);
    }

put_nq(loc,x)

UWORD	loc;
UBYTE	x;
    {
    if (loc >= q_size)
	return;
    q_buffer[loc] = x;
    if (loc >= q_next)
	q_next = loc + 1;
    }
    
UBYTE rand_q(g)

UBYTE	g;
    {
    /* return random number between 1 and g */
    return (1);
    }

next_question()
    {
    UWORD q;
    q = rand_q(g_max = get_q(g_base));
    q_next = 1;
    while (q--)
	gn_qpos();
    for (q = 1; q < g_max; q++)
	{
	if (!(get_q(q_base+1) & 0x80))
	    break;
	gn_qpos();
	}
    if (q >= g_max)
	{
	for (q = 1; q < g_count; q++)
	    {
	    put_q(q_base+1,get_q(q_base+1) | 0x80);
	    gn_qpos();
	    }
	}
    }

gn_qpos()
    {
    q_base +=
	gnq(q_base) | ((gnq(q_base+1) & 0x7f) << 8);
    if (++q_next > g_max)
	{
	q_base = g_base + 1;
	q_next = 1;
	}
    }

display_question()
    {
    set_row(4);
    l_outstr("    Here's the question:");
    newline();
    write_q(&q_buffer[q_base]);
    }

write_q(q)

UBYTE	*q;
    {
    while (*q)
	{
	if (*q == '\n')
	    {
	    newline();
	    l_outstr("    ");
	    }
	else
	    d_outchar(*q);
	q++;
	}
    }

BOOLEAN get_answer()
    {
    UBYTE c;
    newline();
    newline();
    l_outstr("    What's your answer?");
    a_size = 0;
    while ((c = p_inchar()) != RETURN)
    if (c == BACKSP)
	{
	if (a_size > 0)
	    {
	    d_outchar(BACKSP);
	    d_outchar(' ');
	    d_outchar(BACKSP);
	    }
	}
    else
    if (a_size < AMAX)
	d_outchar(ans[a_size++] = c);
    while (get_q(q_base))
	{
	if match_q()
	    return (TRUE);
	while (get_q(q_base++) != '\n')
	    ;
	}
    return (FALSE);
    }

BOOLEAN match_q()
    {
    UBYTE a;
    for (a = 0; a < a_size; a++)
    if (toupper(get_q(q_base)) != toupper(ans[a]))
    if (get_q(q_base) == '\n')
	return (FALSE);
    return (get_q(q_base) == '\n');
    }

ask_question()
    {
    UBYTE p, g, r, w;
    BOOLEAN a;
    UWORD s;
    if (player.bonus[c_player])
	vdpiof("ST");
    else
	vdpiof("AL");
    clear();
    set_row(3);
    l_outstr("    Player ");
    outBYTE(c_player);
    l_outstr(" has ");
    outBYTE(p = player.score[c_player]);
    l_outstr(" points");
    g = player.group[c_player];
    a = TRUE;
    while (a)
    	{
	/* select next question from group */
	if (g > g_count)
	if (!read_group())
	    g = 1;
	/* note: make g_base absolute q_buffer address from group g */
	next_question();
	display_question();
	if (a = get_answer())
	    {
	    g += 1;
	    if ((p += 5) > 100)
		p = 100;
	    s = nightmare.right[scene];
	    }
	else
	    {
	    if ((p -= 3) < 0)
		p = 0;
	    s = nightmare.wrong[scene];
	    }
 	/* set frame display for bonus questions later */
	play_scene(scene);
	clear();
	set_row(3);
	if ((r = nightmare.right[scene]) + (w = nightmare.wrong[scene]) == 0)
	    {
	    l_outstr("    Congratulations!  You made");
	    newline();
	    l_outstr("    it through the maze");
	    s = 0;
	    a = FALSE;
	    }
	else
	if (r == 0)
	    {
	    l_outstr("    Sorry, DEAD END");
	    s = w;
	    }
	if (a)
	    {
	    l_outstr("    Correct!");
	    /* display bonus question info later */
	    s = r;
	    }
	else
	    {
	    l_outstr("    Oops!  You got the");
	    newline();
	    l_outstr("    question wrong");
	    }
	stop_display();
	}
    player.score[c_player] = p;
    player.scene[c_player] = s;
    player.group[c_player] = g;
    }



play_scene(s)

UBYTE	s;
    {
    /* should test for abort from sequence */
    while (TRUE)
	{
	sequence(nightmare.start[s],nightmare.finish[s]);
	if (nightmare.wrong[s] != 0)
	    return;
	if ((s = nightmare.right[s]) == 0)
	    return;
	}
    }


GLOBAL	BOOLEAN	sequence(start,stop)

UWORD	start;
UWORD	stop;
    {
    UBYTE	str[10];
    UBYTE	stp[10];
    BOOLEAN	key;

    sprintf(str,"%d",start);
    sprintf(stp,"%d",stop-1);

    vdpiof("VO");
    vdpiof("FR");
    vdpiof(str);
    vdpiof("SE");
    while (!VIOSTAT())
	;
    vdpiof("MF");
    vdpiof(stp);
    vdpiof("SE");
/*
    if (VIOSTAT())
	while (VIOSTAT())
	    ;
*/
    while (!VIOSTAT())
	;
    vdpiof("VNPL");
/*
    if (VIOSTAT())
	while (VIOSTAT())
	    ;
*/
    while (!VIOSTAT())
	;
    key = FALSE;
/*
    while (VIOSTAT())
	if (key = p_instat())
	    {
	    p_inchar();
	    break;
	    }
*/
    while (1)
	{
	if (!VIOSTAT())
	    if (!VIOSTAT())
		if (!VIOSTAT())
		    if (!VIOSTAT())
			break;
	if (key = p_instat())
	    {
	    p_inchar();
	    break;
	    }
	}
    vdpiof("PA");
    return(key);
    }
