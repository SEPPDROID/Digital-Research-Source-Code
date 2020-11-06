/*--------------------------------------------------------------*\
 |	ccp.c	      CONSOLE COMMAND PROCESSOR          v1.0   |
 |		      =========================			|
 |								|
 |      P- CP/M:  A CP/M derived operating system		|
 |								|
 |      Description:					        |
 |	-----------						|
 |			The Console Command Processor is a      |
 |			distinct program which references       |
 |			the BDOS to provide a human-oriented    |
 |			interface for the console user to the   |
 |			information maintained by the BDOS on   |
 |			disk storage.				|
 |								|
 |	created by :    Tom Saulpaugh	Date created: 7/13/82	|
 |	----------			------------		|
 |      last modified:  12/20/82				|
 |      -------------						|
 |     								|
 |	(c) COPYRIGHT  Digital Research 1982			|
 |	               all rights reserved			|
 |								|
\*--------------------------------------------------------------*/


/*--------------------------------------------------------------*\
 |		     CCP Macro Definitions			|
\*--------------------------------------------------------------*/
#include	"ccpdef.h"        /* include CCP defines	*/

			
/*--------------------------------------------------------------*\
 |		    CP/M Builtin Command Table			|
\*--------------------------------------------------------------*/
struct _cmd_tbl
{
	BYTE	*ident;		/* command identifer field	*/
	UWORD	cmd_code;	/* command code field		*/
}
cmd_tbl[8] = 			/* declare CP/M built-in table	*/
{
	"DIR",DIRCMD,
       "DIRS",DIRSCMD,
       "TYPE",TYPECMD,
	"REN",RENCMD,
	"ERA",ERACMD,
       "USER",UCMD,
     "SUBMIT",SUBCMD,
	 NULL,-1
};		


/*--------------------------------------------------------------*\
 |		 Table of User Prompts and Messages		|
\*--------------------------------------------------------------*/
BYTE	msg[]  = "NON-SYSTEM FILE(S) EXIST$";
BYTE	msg2[] = "Enter Filename: $";
BYTE	msg3[] = "Enter Old Name: $";
BYTE	msg4[] = "Enter New Name: $";
BYTE	msg5[] = "File already exists$";
BYTE	msg6[] = "No file$";
BYTE	msg7[] = "No wildcard filenames$";
BYTE	msg8[] = "Syntax: REN Newfile=Oldfile$";
BYTE	msg9[] = "Confirm(Y/N)? $";
BYTE   msg10[] = "Enter User No: $";
BYTE   msg11[] = ".SUB file not found$";
BYTE   msg12[] = "User # range is [0-15]$";
BYTE   msg13[] = "Too many arguments: $";
/*--------------------------------------------------------------*\
 |		    Global Arrays & Variables			|
\*--------------------------------------------------------------*/

				/********************************/
BYTE load_try = FALSE;		/* flag to mark a load try	*/
BYTE first_sub = TRUE;		/* flag to save current cmd ptr */
BYTE chain_sub = FALSE;		/* submit chaining flag		*/
extern BYTE submit;		/* submit file flag		*/
BYTE end_of_file = FALSE;	/* submit end of file flag	*/
BYTE dirflag = TRUE;		/* used by fill_fcb(? or blanks)*/
BYTE subprompt = FALSE;		/* submit file was prompted for */
extern BYTE morecmds;		/* command after warmboot flag  */
UWORD sub_index = ZERO;		/* index for subdma buffer	*/
UWORD index = ZERO;		/* index into cmd argument array*/ 
UWORD user = USER_ZERO;		/* current default user number  */
UWORD cur_disk = DISK_A;	/* current default disk drive   */
BYTE subcom[CMD_LEN+1];		/* submit command buffer	*/
BYTE subdma[CMD_LEN];		/* buffer to fill from sub file */
extern BYTE usercmd[CMD_LEN+2];	/* user command buffer		*/
BYTE *user_ptr = usercmd;	/* next user command to execute */
BYTE *glb_index = usercmd;	/* points to current command	*/
BYTE save_sub[CMD_LEN+1];	/* saves cur cmd line for submit*/
BYTE subfcb[FCB_LEN];		/* global fcb for sub files	*/
BYTE cmdfcb[FCB_LEN];		/* global fcb for Z8k files	*/
BYTE *tail = usercmd;		/* pointer to command tail      */
extern BYTE autost;		/* autostart flag		*/
BYTE dma[DMA_LEN+3];		/* 128 byte dma buffer		*/
BYTE parm[MAX_ARGS][ARG_LEN];	/* cmd argument array		*/
BYTE del[] =	 		/* CP/M-Z8K set of delimeters   */
{'>','<','.',',','=','[',']',';','|','&','/','(',')','+','-','\\'};

/*--------------------------------------------------------------*\
 |		      Function Definitions			|
\*--------------------------------------------------------------*/

				/********************************/
extern UWORD bdos();		/* this returns a word          */
BYTE *scan_cmd();		/* this returns a ptr to a byte */
UWORD strcmp();			/* this returns a word		*/
UWORD decode();			/* this returns a word		*/
UWORD delim();			/* this returns a word		*/
BYTE true_char();		/* this returns a byte		*/
UWORD fill_fcb();		/* this returns a word		*/
UWORD user_cmd();		/* this returns a word		*/
UWORD cmd_file();		/* this returns a word		*/
UWORD dollar();			/* this returns a word		*/
UWORD comments();		/* this returns a word		*/
UWORD submit_cmd();		/* this returns a word		*/
				/********************************/

LONG map_adr();			/* map a memory address		*/

#define cbdos(c,a) bdos(c,map_adr((long)a, 0))
				/* call BDOS with address mapped*/
				/*  in caller's memory space	*/


				/********************************/
VOID cr_lf()			/*   print a CR and a Linefeed	*/
				/********************************/
{
	bdos(CONSOLE_OUTPUT,LF);
	bdos(CONSOLE_OUTPUT,CR);
}


				/********************************/
UWORD strcmp(s1,s2)	  	/*    compare 2 char strings	*/
				/********************************/
REG BYTE *s1,*s2;
{		
	while(*s1)
	{
		if(*s1 > *s2)
			return(1);
		if(*s1 < *s2)
			return(-1);
		s1++; s2++;
	}
	return((*s2 == NULL) ? 0 : -1);
}
				/********************************/
VOID copy_cmd(com_index)	/*  Save the command which	*/
				/*  started a submit file       */
				/*  Parameter substituion will  */
				/*  need this command tail.	*/
				/*  The buffer save_sub is used */
				/*  to store the command.	*/
				/********************************/ 
REG BYTE *com_index;
{
	REG BYTE *t1,*temp;

	temp = save_sub;
	if(subprompt)
	{
		t1 = &parm[0][0];
		while(*t1 != NULL)
			*temp++ = *t1++;
		*temp++ = ' ';
		subprompt = FALSE;
	}
	while(*com_index != NULL && *com_index != EXLIMPT)
		*temp++ = *com_index++;
	*temp = NULL;
}



				/********************************/
VOID prompt()  			/*   print the CCP prompt       */
				/********************************/
{
	REG UWORD cur_drive,cur_user_no;
	BYTE      buffer[3];

	cur_user_no = bdos(GET_USER_NO,(long)255);
	cur_drive  = bdos(RET_CUR_DISK,(long)0);		
	cur_drive += 'A';
	cr_lf();
	if (cur_user_no != 0)
	{
		if(cur_user_no >= 10)
		{
			buffer[0] = '1';
			cur_user_no -= 10;
			buffer[1] = (cur_user_no + '0');
			buffer[2] = '$';
		}
		else
		{
			buffer[0] = (cur_user_no + '0');
			buffer[1] = '$';
		}
		cbdos(PRINT_STRING,&buffer[0]);
	}
	bdos(CONSOLE_OUTPUT,(long)cur_drive);
	bdos(CONSOLE_OUTPUT,ARROW);
}



				/********************************/
VOID echo_cmd(cmd,mode)		/* echo any multiple commands   */
				/* or any illegal commands	*/
				/********************************/
REG BYTE *cmd;
REG UWORD mode;
{
	if(mode == GOOD && !(autost))
		prompt();
	while(*cmd != NULL && *cmd != EXLIMPT)
		bdos(CONSOLE_OUTPUT,(long)*cmd++);
	if(mode == BAD)
		bdos(CONSOLE_OUTPUT,(long)'?');
	else
		cr_lf();
}


				/********************************/	
UWORD decode(cmd)		/* Recognize the command as:	*/
				/* ---------			*/
				/* 1. Builtin			*/
				/* 2. File			*/
				/********************************/
REG BYTE *cmd;
{
	REG UWORD i,n;


	/****************************************/
	/* Check for a CP/M builtin command	*/
	/****************************************/
	for(i = 0; i < 7;i++)
		if (strcmp(cmd,cmd_tbl[i].ident) == MATCH)
			return(cmd_tbl[i].cmd_code);
	/********************************************************/
	/*	Check for a change of disk drive command	*/
	/********************************************************/
	i = 0;
	while(i < (ARG_LEN-1) && parm[0][i] != ':')
		i++;
	if(i == 1 && parm[0][2] == NULL && parm[1][0] == NULL)
		if((parm[0][0] - 'A' >= 0) && (parm[0][0] - 'A' <= 15)) 
				return(CH_DISK);
	if(i == 1 && ((parm[0][0] - 'A' < 0) || (parm[0][0] - 'A' > 15)))
		return(-1);
	if(i != 1 && parm[0][i] == ':')
		return(-1);
	/*****************************************************/
	/* Check for Wildcard Filenames			     */
	/* Check First Character of Filename for a Delimeter */
	/*****************************************************/
	if(fill_fcb(0,cmdfcb) > 0)
		return(-1);
	if(i == 1)
		i = 2;
	else
		i = 0;
	for(n = 0; n < sizeof del;n++)
		if(parm[0][i] == del[n])
			return(-1);
	for(n = 0;n < ARG_LEN-1;n++)
		if(parm[0][n] > NULL && parm[0][n] < ' ')
			return(-1);
	return(FILE);
}

					/************************/
VOID check_cmd(tcmd)			/*  Check end of cmd	*/
					/*  for an '!' which    */
					/*  starts another cmd  */
REG BYTE *tcmd;				/************************/
{
	while(*tcmd != NULL && *tcmd != EXLIMPT)
		tcmd++;
	/*----------------------------*/
	/* check for multiple command */
	/*   in case of a warmboot    */	
	/*----------------------------*/
	if(*tcmd++ == EXLIMPT && *tcmd != NULL)
	{
		morecmds = TRUE;
		while(*tcmd == ' ')
			tcmd++;
		user_ptr = tcmd;
	}
	else
		if(submit)	/* check original cmd line */
		{
			if(!(end_of_file))
				morecmds = TRUE;
				/*--------------------------*/
			else	/* restore cmd to where user*/
				/* ptr points to. User_ptr  */
				/* always points to next    */
				/* console command to exec  */
			{	/*--------------------------*/
				submit = FALSE;
				if(*user_ptr != NULL)
					morecmds = TRUE;
			}
		}
		else
			morecmds = FALSE;		
}

					/************************/
VOID get_cmd(cmd,max_chars)	        /*   Read in a command  */
					/*Strip off extra blanks*/
					/************************/
REG BYTE  *cmd;
REG LONG   max_chars;
{
	REG BYTE *c;

	dma[0] = CMD_LEN;	  /* set maximum chars to read  */
	cbdos(READ_CONS_BUF,dma);  /* then read console		*/
	if(dma[1] != 0 && dma[2] != ';')
		cr_lf();
	dma[((UWORD)dma[1] & 0xFF)+2] = '\n'; /* tack on end of line char   */ 
	if(dma[2] == ';')	  /* ';' denotes a comment	*/	
		dma[2] = '\n';
	c = &dma[2];
	while(*c == ' ' || *c == TAB)
		c++;
	while(*c != '\n' && --max_chars > 0)
	{			
		*cmd++ = toupper(*c);
		if(*c == ' ' || *c == TAB)
			while(*++c == ' ' || *c == TAB);
		else
			c++;
	}
	*cmd = NULL;	     /* tack a null character on the end*/
}
					/************************/
BYTE *scan_cmd(com_index)		/* move ptr to next cmd */
					/* in the command line	*/
					/************************/
REG BYTE *com_index;
{
	while((*com_index != EXLIMPT) && 
	      (*com_index != NULL))
		 com_index++;
	while(*com_index == EXLIMPT || *com_index == ' ' ||
	      *com_index == TAB)
		com_index++;
	return(com_index);
}


					/************************/
VOID get_parms(cmd)			/* extract cmd arguments*/
					/* from command line	*/
REG BYTE *cmd;				/************************/
{



	/************************************************/
	/*	This function parses the command line   */
	/* read in by get_cmd().  The expected command  */
	/* from that line is put into parm[0].  All     */
	/* parmeters associated with the command are put*/
	/* in in sequential order in parm[1],parm[2],   */
	/* up to parm[4]. A command ends at a NULL or   */
	/* an exlimation point.				*/
	/************************************************/


	REG BYTE *line;		/* pointer to parm array   */
	REG UWORD    i;		/* Row Index   		   */
	REG UWORD    j;		/* Column Index		   */

	line = parm;
	for(i = 0; i < (MAX_ARGS * ARG_LEN); i++)
		*line++ = NULL;

	i = 0;
	/***************************************************/
	/*  separate command line at blanks,exlimation pts */
	/***************************************************/

        while(*cmd != NULL    &&
	      *cmd != EXLIMPT &&
	      i < MAX_ARGS)
	{
		j = 0;
		while(*cmd != EXLIMPT &&
		      *cmd != ' '     &&
		      *cmd != TAB     &&	
		      *cmd != NULL)
		{
			if(j < (ARG_LEN-1))
				parm[i][j++] = *cmd;
			cmd++;
		}
		parm[i++][j] = NULL;
		if(*cmd == ' ' || *cmd == TAB)
			cmd++;
		if(i == 1)
			tail = cmd; /* mark the beginning of the tail */
	}
}


					/************************/
UWORD delim(ch)				/* check ch to see	*/ 
					/* if it's a delimeter  */
					/************************/
REG BYTE *ch;
{
	if(*ch <= ' ')
		return(TRUE);
	switch(*ch)
	{
		case '>':
		case '<':
		case '.':
		case ',':
		case '=':
		case ':':
		case '+':
		case '-':
		case '&':
		case '/':
		case '\\':
		case '|':
		case '(':
		case ')':
		case '[':
		case ']':
		case ';': return(TRUE);
	}
	return(FALSE);
}



					/************************/
BYTE true_char(ch)			/* return the desired	*/
					/* character for fcb	*/
					/************************/

REG BYTE *ch;
{
	if(*ch == '*') return('?');	/* wildcard		*/

	if(!delim(ch)) 			/* ascii character	*/
	{
		index++;		/* increment cmd index	*/
		return(*ch);
	}

	return(' ');			/* pad field with blank */
}

					/************************/
UWORD fill_fcb(which_parm,fcb)		/* fill the fields of	*/
					/* the file control blk */
					/************************/

REG UWORD which_parm;
REG BYTE  *fcb;
{
	REG BYTE  *ptr;
	REG BYTE  fillch;
	REG UWORD j,k;

	*fcb = 0;
	for(k = 12;k <= 35; k++)        /* fill fcb with zero	*/
		fcb[k] = ZERO;
	for(k = 1;k <= 11;k++)
		fcb[k] = BLANK;	/* blank filename+type  */
	
	/*******************************************/
	/* extract drivecode,filename and filetype */
	/*	 from parmeter blk		   */
	/*******************************************/

	if(dirflag)
		fillch = '?';
	else
		fillch = ' ';

	index = ZERO;
	ptr = fcb;
	if(parm[which_parm][index] == NULL) /* no parmemters  */
	{
		ptr++;
		for(j = 1;j <= 11;j++)
			*ptr++ = fillch; 
		*fcb = (bdos(RET_CUR_DISK,(long)0)+1);
		if(dirflag)
			return(11);
		else
			return(0);
	}
	if(parm[which_parm][index+1] == ':')
	{
		*ptr = parm[which_parm][index] - 'A' + 1;
		index += 2;
		if(parm[which_parm][index] == NULL)
		{
			ptr = &fcb[1];
			for(j = 1;j <= 11;j++)
				*ptr++ = fillch;
			if(dirflag)
				return(11);
			else
				return(0);
		}
	}							
	else	/* fill drivecode with the default disk		*/	
	             *fcb = (bdos(RET_CUR_DISK,(long)0) + 1);

	ptr = fcb;
	ptr++;			/* set pointer to fcb filename */
	for(j = 1;j <= 8;j++)	/* get filename */
	  *ptr++ = true_char(&parm[which_parm][index]);
	while((!(delim(&parm[which_parm][index])))) index++;
	if(parm[which_parm][index] == PERIOD)
	{
	  	index++;
       	  	for(j = 1;j <= 3;j++)	/* get extension */
	  	 	*ptr++ = true_char(&parm[which_parm][index]);
	}
	k = 0;
	for(j = 1;j <= 11;j++)
		if(fcb[j] == '?') k++;

	return(k);	/* return the number of question marks	*/
}





					/************************/
VOID dir_cmd(attrib)	       		/*     print out a	*/
					/*  directory listing   */
					/*----------------------*/
					/* attrib->1 (sysfiles) */
					/* attrib->0 (dirfiles) */
					/************************/
REG UWORD attrib;
{
	BYTE		needcr_lf;
	REG UWORD	dir_index,file_cnt;
 	REG UWORD	save,j,k,curdrive,exist;

	j = 0; exist = FALSE; needcr_lf = FALSE;
	while(parm[1][j] != NULL && parm[1][j] != ':')
		j++;
	if(parm[1][j] == ':' && j != 1)
	{
		echo_cmd(&parm[1][0],BAD);
		return(0);
	}	
	if(parm[2][0] != NULL)
	{
		cbdos(PRINT_STRING,&msg13[0]);
		echo_cmd(&parm[2][0],BAD);
		return(0);
	}
	if(parm[1][1] == ':')
		if((parm[1][0] < 'A') ||
		   (parm[1][0] > 'P'))
		{
			echo_cmd(&parm[1][0],BAD);
			return(0);
		}
	fill_fcb(1,cmdfcb);
	curdrive = (cmdfcb[0] + 'A' - 1);
	dir_index = cbdos(SEARCH_FIRST,cmdfcb);
	if(dir_index == 255)
		cbdos(PRINT_STRING,&msg6[0]);
	save = (32 * dir_index) + 1;
	file_cnt = 0;		
	while(dir_index != 255)
	{
      		if(((attrib) && (dma[save+9] & 0x80)) ||
		  (!(attrib) && (!(dma[save+9] & 0x80))))
	      	{
			if(needcr_lf)
			{
				cr_lf();
				needcr_lf = FALSE;
			}	
			if(file_cnt == 0)
				bdos(CONSOLE_OUTPUT,(long)curdrive);
	      	}
		else
			{
				exist = TRUE;
				dir_index = cbdos(SEARCH_NEXT,cmdfcb);
				save = (32 * dir_index) + 1;
				continue;
			}
		dir_index = (32 * dir_index) + 1;
		bdos(CONSOLE_OUTPUT,COLON);
		bdos(CONSOLE_OUTPUT,BLANKS);
		j = 1;
		while(j <= 11)
		{
			if(j == 9)
				bdos(CONSOLE_OUTPUT,BLANKS);
			bdos(CONSOLE_OUTPUT,(long)(dma[dir_index++] & CMASK));
			j++;
		}
		bdos(CONSOLE_OUTPUT,BLANKS);
		dir_index = cbdos(SEARCH_NEXT,cmdfcb);
		if(dir_index == 255)
			break;
		file_cnt++;
		save = (32 * dir_index) + 1;
		if(file_cnt == FILES_PER_LINE)	/* SS 821221 */
		{
			file_cnt = 0;
			if((attrib && (dma[save+9] & 0x80)) ||
			  (!(attrib) && (!(dma[save+9] & 0x80))))
				cr_lf();
			else
				needcr_lf = TRUE;
		}	 

	}		/*----------------------------------------*/
	if(exist)	/* if files exist that were not displayed */
			/* print out a message to the console	  */
	{		/*----------------------------------------*/
		cr_lf();
		if(attrib)
			cbdos(PRINT_STRING,&msg[0]);
		else
			cbdos(PRINT_STRING,&msg[4]);
	}
}





					/************************/
VOID type_cmd()				/*   type out a file	*/
					/*   to the console 	*/
					/************************/
{
	REG 	UWORD i;

	if(parm[1][0] == NULL)		/*prompt user for filename*/
	{
		cbdos(PRINT_STRING,&msg2[0]);
		get_cmd(&parm[1][0],(long)ARG_LEN-1);
	}
	if(parm[2][0] != NULL)
	{
		cbdos(PRINT_STRING,&msg13[0]);
		echo_cmd(&parm[2][0],BAD);
		return(0);
	}
	i = 0;
	while(parm[1][i] != NULL && parm[1][i] != ':')
		i++;
	if(parm[1][i] == ':')
	{
		if(i != 1 || parm[1][0] < 'A' || parm[1][0] > 'P')
		{	
			echo_cmd(&parm[1][0],BAD);
			return(0);
		}
	}
	i = fill_fcb(1,cmdfcb);		/*fill a file control block*/
	if(i == 0 && parm[1][0] != NULL && (cbdos(OPEN_FILE,cmdfcb) <= 3))
	{
		while(cbdos(READ_SEQ,cmdfcb) == 0)
		{
			for(i = 0;i <= 127;i++)
				if(dma[i] != EOF)
				    bdos(CONSOLE_OUTPUT,(long)dma[i]);
				else
					break;
		}
		cbdos(CLOSE_FILE,cmdfcb);
		bdos(RESET_DRIVE,(long)cmdfcb[0]);
	} else
		if(parm[1][0] != NULL)
		{
			if(i > 0)
			{
				if(i == 11)
					echo_cmd(&parm[1][0],BAD);
				else
					cbdos(PRINT_STRING,&msg7[0]);
			}
			else
				cbdos(PRINT_STRING,&msg6[0]);
		}
}



					/************************/						
VOID ren_cmd()				/*    rename a file	*/
					/************************/

{
	BYTE    	new_fcb[FCB_LEN];
	REG UWORD	i,j,k,bad_cmd;

	bad_cmd = FALSE;	       /*-------------------------*/	
	if(parm[1][0] == NULL)	       /*prompt user for filenames*/
	{			       /*-------------------------*/	
		cbdos(PRINT_STRING,&msg3[0]); 
		get_cmd(&parm[3][0],(long)ARG_LEN-1);
		if(parm[3][0] == NULL)
			return(0);
		cbdos(PRINT_STRING,&msg4[0]);  
		get_cmd(&parm[1][0],(long)ARG_LEN-1);
		parm[2][0] = '=';
	}			/*--------------------------------*/
	 else		        /*check for correct command syntax*/
	 {			/*--------------------------------*/
		i = 0;
		while(parm[1][i] != '=' && parm[1][i] != NULL) i++;
		if(parm[1][i] == '=')
		{
			if(!(i > 0 && parm[1][i+1] != NULL &&
			     parm[2][0] == NULL))
				bad_cmd = TRUE;
		}
		else
			if(!(parm[2][0] == '='  &&
			     parm[2][1] == NULL &&
			     parm[3][0] != NULL))
				bad_cmd = TRUE;
		if(!bad_cmd && parm[1][i] == '=')
		{
			parm[1][i] = NULL;
			i++;
			j = 0;
			while((parm[3][j++] = parm[1][i++]) != NULL);
			parm[2][0] = '=';
		}
	}
	for(j = 1;j < 4;j += 2)
	{
		k = 0;
		while(parm[j][k] != ':' && parm[j][k] != NULL)
			k++;
		if(k > 1 && parm[j][k] == ':')
			bad_cmd = TRUE;
		for(i = 0;i < sizeof del;i++)
			if(parm[j][0] == del[i])
			{
				echo_cmd(&parm[j][0],BAD);
				return(0);
			}
	}
	if(!bad_cmd && parm[1][0] != NULL && parm[3][0] != NULL)
	{
		i = fill_fcb(1,new_fcb);
		j = fill_fcb(3,cmdfcb);
		if(i == 0 && j == 0)
		{
			if(new_fcb[0] != cmdfcb[0])
			{
				if(parm[1][1] == ':' && parm[3][1] != ':')
					cmdfcb[0] = new_fcb[0];
				else
				if(parm[1][1] != ':' && parm[3][1] == ':')
					new_fcb[0] = cmdfcb[0];
				else
				bad_cmd = TRUE;
			}
			if(new_fcb[0] < 1 || new_fcb[0] > 16)
				bad_cmd = TRUE;
			if(!(bad_cmd) && cbdos(SEARCH_FIRST,new_fcb) != 255)
				cbdos(PRINT_STRING,&msg5[0]);
			else{
				k = 0;
				for(i = 16;i <= 35;i++)
					cmdfcb[i] = new_fcb[k++];
				if(cmdfcb[0] < 0 || cmdfcb[0] > 15)
					bad_cmd = TRUE;
				if(!(bad_cmd) &&
				cbdos(RENAME_FILE,cmdfcb) > 0)
					cbdos(PRINT_STRING,&msg6[0]);
			    }
		}
		else
		 cbdos(PRINT_STRING,&msg7[0]);
	}
	if(bad_cmd)
	         cbdos(PRINT_STRING,&msg8[0]);
}


					/************************/
VOID era_cmd()				/*  erase a file from	*/
					/*  the directory       */
					/************************/
{
	REG 	UWORD i;
				        /*----------------------*/
	if(parm[1][0] == NULL)		/* prompt for a file	*/
	{				/*----------------------*/
		cbdos(PRINT_STRING,&msg2[0]);
		get_cmd(&parm[1][0],(long)ARG_LEN-1);
	}
	if(parm[1][0] == NULL)
		return(0);
	if(parm[2][0] != NULL)
	{
		cbdos(PRINT_STRING,&msg13[0]);
		echo_cmd(&parm[2][0],BAD);
		return(0);
	}
	i = 0;
	while(parm[1][i] != ':' && parm[1][i] != NULL)
		i++;
	if(parm[1][i] == ':')
	{
		if(i != 1 || parm[1][0] < 'A' || parm[1][0] > 'P' ||
		   parm[1][2] == NULL)
		{
			echo_cmd(&parm[1][0],BAD);
			return(0);
		}
	}
	i = fill_fcb(1,cmdfcb);		/* fill an fcb	   */
	if(i > 0 && !(submit))		/* no confirmation */
	{				/* if submit file  */
		cbdos(PRINT_STRING,&msg9[0]);
		parm[2][0] = bdos(CONIN,(long)0);
		parm[2][0] = toupper(parm[2][0]);
		cr_lf();
		if(parm[2][0] != 'N' && parm[2][0] != 'Y')
			return(0);
	}
	if(parm[2][0] != 'N')
		if(cbdos(DELETE_FILE,cmdfcb) > 0)
			cbdos(PRINT_STRING,&msg6[0]);
}

					/************************/
UWORD user_cmd()			/* change user number	*/
					/*----------------------*/
					/* update global user   */
					/************************/

{
	REG UWORD i;

	if(parm[1][0] == NULL)		/* prompt for a number	*/
	{
		cbdos(PRINT_STRING,&msg10[0]);
		get_cmd(&parm[1][0],(long)ARG_LEN-1);
	}
	if(parm[1][0] == NULL)
		return(TRUE);
	if(parm[2][0] != NULL)
	{
		cbdos(PRINT_STRING,&msg13[0]);
		echo_cmd(&parm[2][0],BAD);
		return(TRUE);
	}
	if((parm[1][0] < '0') ||
	   (parm[1][0] > '9'))
		return(FALSE);
	i = (parm[1][0] - '0');
	if(i > 9)
		return(FALSE);
	if(parm[1][1] != NULL)
		i = ((i * 10) + (parm[1][1] - '0'));
	if(i < 16 && parm[1][2] == NULL)
		bdos(GET_USER_NO,(long)i);
	else
		return(FALSE);
	user = i;
	return(TRUE);
}




					/************************/
VOID change_cmd()			/*    change default	*/
					/*    disk drive	*/
					/*----------------------*/
					/*  update global disk  */
					/************************/
{
	cur_disk = (parm[0][0] - 'A');
	bdos(SELECT_DISK,(long)cur_disk);
}

UWORD cmd_file(mode)
					/************************/
					/*			*/
					/*      SEARCH ORDER	*/
					/*	============	*/
					/*			*/
					/* 1. Z8K type on the   */
					/*    current user #	*/
					/* 2. BLANK type on     */
					/*    current user #	*/
					/* 3. SUB type on the   */
					/*    current user #	*/
					/* 4. Z8K type on	*/
					/*    user 0		*/
					/* 5. BLANK type on the */
					/*    user 0	        */
					/* 6. SUB type on 	*/
					/*    user 0		*/
					/*			*/
					/*----------------------*/
					/* 			*/
					/*   If a filetype is   */
					/*   specified then I   */
					/*   search the current */
					/*   user # then user 0 */
					/*			*/
					/************************/
UWORD mode;
{
	BYTE search_flag[6],submitfile;
	REG UWORD i,n,open,sub_open,ok;

	open = FALSE;
	sub_open = FALSE;
	dirflag = FALSE;
	ok = TRUE;
	load_try = TRUE; /* if a warmboot occurs reset drive & user # */
	submitfile = FALSE;
	user = bdos(GET_USER_NO,(long)255);
	cur_disk = bdos(RET_CUR_DISK,(long)0);
	if(mode == SEARCH)
		i = fill_fcb(0,cmdfcb);
	else
		i = fill_fcb(1,cmdfcb);
	if(i > 0)
	{
		cbdos(PRINT_STRING,&msg7[0]);	
		return(FALSE);
	}
	if(cmdfcb[9] == ' ')
	{
		for(i = 0;i < 6;i++)
			search_flag[i] = 0;
		bdos(SELECT_DISK,(long)(cmdfcb[0]-1));
		cmdfcb[0] = '?';
		cmdfcb[12] = NULL;
		i = cbdos(SEARCH_FIRST,cmdfcb);
		while(i != 255)
		{
			i *= 32;
			if(dma[i] == 0 || dma[i] == user)
			{
				for(n = 9;n <= 11;n++)
					dma[i+n] &= CMASK;
				dma[i+12]  = NULL;
				cmdfcb[9]  = 'Z';
				cmdfcb[10] = '8';
				cmdfcb[11] = 'K';
				if(strcmp(&cmdfcb[1],&dma[i+1]) == MATCH)
				{
					if(dma[i] == user)
						search_flag[0] = 1;
					else
						search_flag[3] = 1;
				}
				else
				{
				    cmdfcb[9]  = ' ';
				    cmdfcb[10] = ' ';
				    cmdfcb[11] = ' ';
				    if(strcmp(&cmdfcb[1],&dma[i+1]) == MATCH)
				    {
					if(dma[i] == user)
						search_flag[1] = 1;
					else
						search_flag[4] = 1;
				    }
				    else
				    {
				      cmdfcb[9]  = 'S';
				      cmdfcb[10] = 'U';
				      cmdfcb[11] = 'B';
				      if(strcmp(&cmdfcb[1],&dma[i+1]) == MATCH)
				      {
					if(dma[i] == user)
						search_flag[2] = 1;
					else
						search_flag[5] = 1;
				      }
				    }
				}
			}
			if((mode == SEARCH && search_flag[0] == 1) ||
			   (mode == SUB_FILE && search_flag[2] == 1))
				i = 255;
			else
				i = cbdos(SEARCH_NEXT,cmdfcb);  
		}
		if(mode == SEARCH)
			fill_fcb(0,cmdfcb);
		else
			fill_fcb(1,cmdfcb);
		bdos(SELECT_DISK,(long)cur_disk);
		i = 0;
		if(mode == SUB_FILE)
			if(search_flag[2] == 1)
				i = 2;
			else
				i = 5;
		while(i < 6 && search_flag[i] == 0)
			i++;
		switch(i)
		{
			case 3:	bdos(GET_USER_NO,(long)0);
			case 0: cmdfcb[9]  = 'Z';
				cmdfcb[10] = '8';
				cmdfcb[11] = 'K';
				break;
			case 4: bdos(GET_USER_NO,(long)0);
			case 1:	break;
			case 5:	bdos(GET_USER_NO,(long)0);
			case 2: cmdfcb[9]  = 'S';
				cmdfcb[10] = 'U';
				cmdfcb[11] = 'B';
				break;
		       default: ok = FALSE;
		}
	}
	if(cmdfcb[9] == 'S' && cmdfcb[10] == 'U' && cmdfcb[11] == 'B')
		submitfile = TRUE;
	while(!(open) && !(sub_open) && (ok))
	{
		if(cbdos(OPEN_FILE,cmdfcb) <= 3)
		{
			if(submitfile)
			{
				sub_open = TRUE;
				if(submit)
					chain_sub = TRUE;
				else
					first_sub = TRUE;
				for(i = 0;i < FCB_LEN;i++)
					subfcb[i] = cmdfcb[i];
				submit = TRUE;
				end_of_file = FALSE;
			}
			else
				open = TRUE;
		}
		else
			if(bdos(GET_USER_NO,(long)255) == 0)
				break;
			else
				bdos(GET_USER_NO,(long)0);
	}
	if(open)
	{
		check_cmd(glb_index);
		__LOAD();
	}
	if(!(sub_open) && mode == SUB_FILE)
		cbdos(PRINT_STRING,&msg11[0]);
	bdos(GET_USER_NO,(long)user);
	dirflag = TRUE;
	load_try = FALSE;
	return(sub_open);
}

					/************************/	
UWORD dollar(k,mode,com_index)		/*    Translate $n to   */ 
					/*  nth argument on the */
					/*  command line	*/
					/************************/
REG UWORD k;
REG UWORD mode;				
REG BYTE *com_index;			
{
	REG UWORD n,j,p_index;
	REG BYTE *p1;

	j = sub_index;
	if(k >= CMD_LEN)
	{
		k = 0;
		if(cbdos(READ_SEQ,subfcb) != 0)
		{
			end_of_file = TRUE;
			return(k);
		}
	}
	if((subdma[k] >= '0') && (subdma[k] <= '9'))
	{
		p_index = (subdma[k] - '0');
		p1 = com_index;
		if(*p1++ == 'S' &&
		   *p1++ == 'U' &&
		   *p1++ == 'B' &&
		   *p1++ == 'M' &&
	  	   *p1++ == 'I' &&
		   *p1++ == 'T' &&
		   *p1   == ' ')
			p_index++;
		p1 = com_index;
		for(n = 1; n <= p_index; n++)
		{
			while(*p1 != ' ' && *p1 != NULL)
				p1++;
			if(*p1 == ' ')
				p1++;
		}
		while(*p1 != ' ' && *p1 != NULL && j < CMD_LEN)
			if(mode == FILL)
				subcom[j++] = *p1++;
			else
				bdos(CONSOLE_OUTPUT,(long)*p1++);
		k++;
	}
	else
	{
		if(mode == FILL)
			subcom[j++] = '$';
		else
			bdos(CONSOLE_OUTPUT,(long)'$');
		if(subdma[k] == '$')
			k++;
	}
	sub_index = j;
	if(k >= CMD_LEN)
	{
		k = 0;
		if(cbdos(READ_SEQ,subfcb) != 0)
			end_of_file = TRUE;
	}
	return(k);
}
					/************************/
UWORD comments(k,com_index)		/* Strip and echo submit*/
					/* file comments	*/
					/************************/
REG UWORD k;
REG BYTE *com_index;
{
	REG UWORD done;

	done = FALSE;
	prompt();
	do
	{
		while(k < CMD_LEN     &&
		     subdma[k] != EOF && 
		     subdma[k] != Cr)
		{
			if(subdma[k] == '$')
			{
				k++;
				k = dollar(k,NOFILL,com_index);
			}
			else
				bdos(CONSOLE_OUTPUT,(long)subdma[k++]);
		}
		if(k == CMD_LEN && subdma[k] != EOF && subdma[k] != Cr)
		{
			k = 0;
			if(cbdos(READ_SEQ,subfcb) != 0)
			{
				end_of_file = TRUE;
				done = TRUE;
			}
		}
		else
		{
			if(subdma[k] == Cr)
			{
				k += 2;
				if(k >= CMD_LEN)
				{
					k = 0;
					if(cbdos(READ_SEQ,subfcb) != 0)
						end_of_file = TRUE;
				}
			}
			else
				end_of_file = TRUE;
			done = TRUE;
		}
	}while(!(done));
	return(k);
}

					/************************/
VOID translate(com_index)		/* TRANSLATE the subfile*/
					/* and fill sub buffer. */
					/************************/
REG BYTE *com_index;
{
	REG BYTE *p1;
	REG UWORD j,n,k,p_index;

	j = 0;
	k = sub_index;
	while(!(end_of_file) && j < CMD_LEN
		&& subdma[k] != Cr && subdma[k] != EXLIMPT)
	{
		switch(subdma[k])
		{
			case ';': k = comments(k,com_index);
				  break; 
			case TAB:
			case ' ': if(j > 0)
				  	subcom[j++] = subdma[k++];
	blankout:		  while(k < CMD_LEN && 
				       (subdma[k] == ' ' || subdma[k] == TAB))
						k++;
				  if(k >= CMD_LEN)
				  {
					k = 0;
					if(cbdos(READ_SEQ,subfcb) != 0)
						end_of_file = TRUE;
					else
						goto blankout;						
				  }
				  break;
			case '$': k++;
				  sub_index = j;
				  k = dollar(k,FILL,com_index);
				  j = sub_index;
				  break;
			case Lf:  k++;
				  if(k >= CMD_LEN)
				  {
					k = 0;
					if(cbdos(READ_SEQ,subfcb) != 0)
						end_of_file = TRUE;
			          }			
				  break;
			case EOF: 
				  end_of_file = TRUE;
				  break;
                         default: subcom[j++] = subdma[k++];
				  if(k >= CMD_LEN)
				  {
					k = 0;
					if(cbdos(READ_SEQ,subfcb) != 0)
						end_of_file = TRUE;
			          }
		}
	}
	/*------------------------------------------------------------------*/
	/*	       TRANSLATION OF A COMMAND IS COMPLETE		    */
	/*	       -Now move sub_index to next command-          	    */
        /*------------------------------------------------------------------*/

	if(subdma[k] == Cr || subdma[k] == EXLIMPT)
	do
	{
		while(k < CMD_LEN && 
		     (subdma[k] == Cr ||
		      subdma[k] == Lf ||
		      subdma[k] == EXLIMPT))
				k++;	
		if(k == CMD_LEN)
		{
			k = 0;
			if(cbdos(READ_SEQ,subfcb) != 0)
			{
				end_of_file = TRUE;
				break;
			}
		}
		else
		{
			if(subdma[k] == EOF)
				end_of_file = TRUE;
			break;
		}
	}while(TRUE);
	sub_index = k;
}

					/************************/
UWORD submit_cmd(com_index)		/* fill up the subcom   */
					/*      buffer 		*/
					/************************/
/*--------------------------------------------------------------*\
 |								|
 |	Submit_Cmd is a Procedure that returns exactly		|
 |	one command from the submit file.  Submit_Cmd is	|
 |	called only when the end of file marker has not 	|
 |	been read yet.  Upon leaving submit_cmd,the variable    |
 |	sub_index points to the beginning of the next command   |
 |	to translate and execute.  The buffer subdma is used    |
 |      to hold the UN-translated submit file contents.  	|
 |	The buffer subcom holds a translated command.		|
 |	Comments are echoed to the screen by the procedure      |
 |	"comments".  Parameters are substituted in comments     |
 |      as well as command lines.				|
 |								|
\*--------------------------------------------------------------*/

REG BYTE *com_index;
{
	REG UWORD i;

	for(i = 0;i <= CMD_LEN;i++)
		subcom[i] = NULL;
	cbdos(SET_DMA_ADDR,subdma);
	if(first_sub || chain_sub)
	{
		for(i = 0;i < CMD_LEN;i++)
			subdma[i] = NULL;
		if(cbdos(READ_SEQ,subfcb) != 0)
		{
			end_of_file = TRUE;
			subcom[0] = NULL;
		}
		sub_index = 0;
	}
	if(!(end_of_file))
		translate(com_index);
	for(i = 0;i < CMD_LEN;i++)
		subcom[i] = toupper(subcom[i]);
	cbdos(SET_DMA_ADDR,dma);
}

					/************************/	
VOID execute_cmd(cmd)			/*    branch to		*/
		 			/* appropriate routine	*/
					/************************/

REG BYTE *cmd;
{
	REG UWORD i,flag;

	switch( decode(cmd) )
	{
		case  DIRCMD:	dir_cmd(0);
				break;
		case DIRSCMD:	dir_cmd(1);
				break;
		case TYPECMD:	type_cmd();
				break;
		case  RENCMD:	ren_cmd();
				break;
		case  ERACMD:	era_cmd();
				break;
		case    UCMD:	if(!(user_cmd()))
					cbdos(PRINT_STRING,&msg12[0]);
				break;
		case CH_DISK:   change_cmd();
				break;
		case  SUBCMD:   flag = SUB_FILE;
				if(parm[1][0] == NULL)
				{
					cbdos(PRINT_STRING,&msg2[0]);
					get_cmd(subdma,(long)CMD_LEN-1);
					i = 0;
					while(subdma[i] != ' ' &&
					      subdma[i] != NULL) {
						parm[1][i] = subdma[i];
						i++;
					}
					parm[1][i] = NULL;
					subprompt = TRUE;
				}
				else
					subprompt = FALSE;
				if(subdma[0] == NULL)
					break;
				goto gosub; 
		case    FILE:   flag = SEARCH;
gosub:				if(cmd_file(flag))
					break;
				if(flag == SUB_FILE)
					break;
		default	    :	echo_cmd(&parm[0][0],BAD);
	}
}





main()
{					 /*---------------------*/
	REG BYTE *com_index;		 /* cmd execution ptr   */ 	
					 /*---------------------*/
	dirflag = TRUE;		         /* init fcb fill flag  */
	cbdos(SET_DMA_ADDR,dma);          /* set system dma addr */
					 /*---------------------*/
	if(load_try)
	{
		bdos(SELECT_DISK,(long)cur_disk);
		bdos(GET_USER_NO,(long)user);
		load_try = FALSE;
	}
					 /*---------------------*/
	if(morecmds)			 /* if a warmboot 	*/
	{				 /* occured & there were*/
					 /* more cmds to do	*/
		if(submit)		 /*---------------------*/
		{
			com_index = subcom;
			submit_cmd(save_sub);
		}
		else
			com_index = user_ptr;
		morecmds = FALSE;
		echo_cmd(com_index,GOOD);
	}
	else
	{				/*----------------------*/
               prompt();		/* prompt for command   */
	       com_index = usercmd;	/* set execution pointer*/
	       if(!(autost))		/* check autostart flag */	
	       get_cmd(usercmd,(long)CMD_LEN);/* read a command */
	       else			/* -------------------- */
	       {			/* -------------------- */
		echo_cmd(usercmd,GOOD);	/* echo auto cmd and	*/
		autost = FALSE;		/* turn off flag   	*/
	       }			/*----------------------*/
	}

/*--------------------------------------------------------------*\
 |								|
 |		       MAIN CCP PARSE LOOP			|
 |		       ===================			|
 |								|
\*--------------------------------------------------------------*/
			

	while(*com_index != NULL)
	{				  /*--------------------*/
		glb_index = com_index;	  /* save for use in    */
					  /* check_cmd call	*/
		get_parms(com_index);	  /* parse command line */
		if(parm[0][0] != NULL)	  /* ------------------	*/
		execute_cmd(&parm[0][0]); /* execute command    */
					  /*--------------------*/
		if(!(submit))
		com_index = scan_cmd(com_index);/* inc pointer  */
		else					  
		{			  /*--------------------*/
			if(first_sub) 	  /* save ptr to next	*/
					  /* console command    */
					  /*--------------------*/	
			{
				if(subprompt)
					copy_cmd(subdma);
				else				
					copy_cmd(com_index);
				com_index = subcom;
				user_ptr = scan_cmd(glb_index);
				submit_cmd(save_sub);
				first_sub = FALSE;
			}
			else
			if(chain_sub)
			{
				if(subprompt)
					copy_cmd(subdma);
				else
					copy_cmd(com_index);
				com_index = subcom;
				submit_cmd(save_sub);
				chain_sub = FALSE;
			}
			else
				com_index = NULL;
			while(*com_index == NULL)
			{
				if(end_of_file)
				{
					com_index = user_ptr;
					submit = FALSE;
					break;
				}
				else
				{
					com_index = subcom;
					submit_cmd(save_sub);
				}
			}
		}
		if(*com_index != NULL)		
			echo_cmd(com_index,GOOD);
	}
}
