/****************************************************************************/
/*                                                                          */
/*                         W r i t e   F u n c t i o n                      */
/*                         ---------------------------                      */
/*      Copyright 1982 by Digital Research Inc.  All rights reserved.       */
/*                                                                          */
/*      Function "write" simulates the UNIX write system call.  Any         */
/*      arbitrary number of bytes are written to the file specified by      */
/*      file descriptor.  No special alignment of file or buffer is         */
/*      required.                                                           */
/*                                                                          */
/*      Calling Sequence:                                                   */
/*                                                                          */
/*              ret = write(fd,buffer,bytes);                               */
/*                                                                          */
/*      Where:                                                              */
/*                                                                          */
/*              fd      is an open file descriptor                          */
/*              buffer  is the buffer address                               */
/*              bytes   is the number of bytes to be written                */
/*                                                                          */
/*              ret     is the number of bytes actually written             */
/*                                                                          */
/****************************************************************************/
#include <portab.h>
#include <cpm.h>
#include <errno.h>

#define  OFFSECT  (fp->offset    & ~(SECSIZ-1))
#define  HIWSECT  (fp->hiwater-1 & ~(SECSIZ-1))

WORD    write(fd,buff,pbytes)                   /****************************/
                                                /*                          */
        WORD            fd;                     /* File descriptor          */
        REG     BYTE    *buff;                  /* Buffer address           */
        UWORD           pbytes;                 /* Number of bytes to xfer  */
                                                /*                          */
{                                               /****************************/
        REG     FD      *fp;                    /* File (ccb) pointer       */
        EXTERN  FD      *_chkc();               /* fd -> fp Convertor       */
        LOCAL   LONG    ofs;                    /* Buffer offset            */
        LOCAL   LONG    written;                /* Number of bytes written  */
        LOCAL   LONG    xsector;                /* Sector number temporary  */
        REG     BYTE    *p1;                    /* Move loop pointers       */
        REG     BYTE    c;                      /* Character temp           */
        REG     LONG    xbytes;                 /* Saved bytes              */
        REG     LONG    bytes;                  /* Bytes for everyone else  */
        EXTERN  LONG    _wrtbin();              /* Binary write routine     */
        EXTERN  LONG    _chrout();              /* Character device output  */
                                                /****************************/
                                                /*                          */
        bytes = (LONG)pbytes;                   /* Convert to long          */
                                                /****************************/
        if((fp=_chkc(fd)) == NULL)              /* Get CCB address          */
                return(FAILURE);                /*      Can't               */
                                                /*                          */
        if(bytes <= 0)                          /* Trying to write 0 or less*/
                return(0);                      /* Yes, a wise guy!         */
                                                /*                          */
        if((fp->flags & ISREAD) != 0)           /* Check for readonly file  */
                RETERR(FAILURE,EBADF);          /* Barf if so               */
                                                /*                          */
        if((fp->flags & ISTTY) != 0)            /* TTY File?                */
                return(_chrout(buff,bytes,CONOUT));/*   Yes, out to TTY     */
                                                /*                          */
        if((fp->flags & ISLPT) != 0)            /* LIST file?               */
                return(_chrout(buff,bytes,LSTOUT));/*   Yes, out to LST:    */
                                                /****************************/
        if((fp->flags & ISASCII) == 0)          /* If binary file           */
                return(_wrtbin(fp,buff,bytes)); /*      do binary style     */
                                                /****************************/
        written = bytes;                        /* Remember original request*/
                                                /****************************/
                                                /*                          */
/****************************************************************************/
/*      Here begins the actual write loop                                   */
/****************************************************************************/
                                                /*                          */
        c = 0;                                  /* Init to garbage value    */
                                                /*                          */
while(bytes >0)                                 /* While more bytes to write*/
{                                               /*                          */
        xsector = fp->offset >> 7;              /* Compute beginning sector */
        ofs = 0;                                /* start on sector boundary */
                                                /*                          */
        if(xsector != fp -> sector)             /* Have to read it first    */
        {                                       /*                          */
          if((fp->flags & DIRTY) != 0)          /* Buffer has data?         */
          {                                     /* Yes                      */
            if(_blkio(fp,fp->sector,            /*                          */
                fp->buffer,1L,B_WRITE) != 1)    /* Try to write             */
                        RETERR(FAILURE,EIO);    /* Couldn't                 */
            fp->flags &= ~DIRTY;                /* Nice clean buffer now    */
          }                                     /****************************/
        }                                       /*                          */
                                                /*                          */
        if( OFFSECT <= HIWSECT )                /* Starting below hiwater   */
                                                /* mark (adjusted to sector */
        {                                       /* bounds)                  */
            _blkio(fp,xsector,fp->buffer,       /*                          */
                1L,B_READ);	                /* Now read the correct sec */
        }                                       /****************************/
        fp->sector = xsector;                   /* Mark buffer correctly    */
        ofs = fp->offset & (SECSIZ-1);          /* Compute 1st char offset  */
                                                /*                          */
        p1 = &(fp->buffer[ofs]);                /* first char to write      */
                                                /*                          */
        xbytes = bytes;                         /* Save byte count remaining*/
        while (p1 < &(fp->buffer[SECSIZ])       /* Until buffer is full     */
             && (bytes > 0))                    /* or until request done    */
        {                                       /****************************/
          if(*buff == '\n' && c != '\r')        /* need to insert C/R       */
            c = '\r';                           /*                          */
          else                                  /* Don't need to            */
          {                                     /*                          */
            c = *buff++;                        /*                          */
            bytes--;                            /*                          */
          }                                     /****************************/
            *p1++ = c;                          /* Put char in buffer       */
            fp->offset++;                       /* Increment file offset    */
        }                                       /****************************/
        if(p1 >= &(fp->buffer[SECSIZ]))         /* Need to write buffer     */
        {                                       /*                          */
          if(_blkio(fp,xsector,fp->buffer,      /*                          */
                1L,B_WRITE) !=1)                /* Try the write            */
            return(written-xbytes);             /* return # actually written*/
        }                                       /****************************/
        else                                    /*                          */
          fp -> flags |= DIRTY;                 /* Buffer dirty again       */
        if(bytes == 0)                          /* If done,                 */
        {                                       /* Check offset here        */
            if(fp->offset > fp->hiwater)        /*                          */
              fp->hiwater = fp->offset;         /* Fix hiwater if necessary */
            return(written);                    /*              return      */
        }                                       /*                          */
}                                               /****************************/
}

/****************************************************************************/
/*                                                                          */
/*                      W r t b i n    F u n c t i o n                      */
/*                      ------------------------------                      */
/*                                                                          */
/*      Function "wrtbin" is called from "write" to take advantage of a     */
/*      potential multi-sector transfer for binary files.                   */
/*                                                                          */
/*      Calling sequence:                                                   */
/*                                                                          */
/*              ret = _wrtbin(fp,buffer,bytes);                             */
/*                                                                          */
/*      Where:                                                              */
/*                                                                          */
/*              fp      Points to the affected ccb.                         */
/*              buffer  Is the buffer address                               */
/*              bytes   Is the number of bytes to write                     */
/*                                                                          */
/*              ret     Is the number of bytes actually written             */
/*                                                                          */
/****************************************************************************/

LONG    _wrtbin(fp,buff,bytes)                  /****************************/
                                                /*                          */
        REG     FD      *fp;                    /* -> CCB                   */
        REG     BYTE    *buff;                  /* -> User's buffer         */
        REG     LONG    bytes;                  /* # bytes to write         */
{                                               /****************************/
        LOCAL   LONG    xsector;                /* Sector temp              */
        LOCAL   LONG    nsector;                /* Multi-sector count       */
        LOCAL   LONG    written;                /* # bytes to write         */
        REG     BYTE    *p1;                    /* Temp buffer pointer      */
                                                /****************************/
        written = bytes;                        /* Remember request length  */
        xsector = fp->offset >> 7;              /* Calculate present sector */
                                                /****************************/
        if((fp -> offset & (SECSIZ-1)) != 0)    /* Are we at a boundary?    */
        {                                       /*                          */
          if(xsector != fp->sector)             /* No, do sectors match?    */
          {                                     /*                          */
            if((fp->flags & DIRTY) != 0)        /* No, is buffer dirty?     */
            {                                   /*                          */
              if(_blkio(fp,fp->sector,          /*                          */
                fp->buffer,1L,B_WRITE) != 1)    /* Yes, write it            */
                  RETERR(FAILURE,EIO);          /* Couldn't write buffer    */
              fp->flags &= ~DIRTY;              /* Clear dirty bit          */
            }                                   /****************************/
                                                /*                          */
            if ( OFFSECT <= HIWSECT )           /* Within hiwater area?     */
                                                /*                          */
            {                                   /* If yes, then read it     */
              if(_blkio(fp,xsector,fp->buffer,  /*                          */
                        1L,B_READ) != 1)        /* Try to read the correct  */
                   RETERR(FAILURE,EIO);         /* Can't                    */
            }                                   /****************************/
            else _clear(fp->buffer,SECSIZ);     /* Zero out the buffer      */
                                                /*                          */
            fp->sector = xsector;               /* Label buffer             */
                                                /*                          */
          }                                     /****************************/
          p1 = &fp->buffer[fp->offset&(SECSIZ-1)];/* p1->1st loc in buffer  */
                                                /*                          */
          while (p1 < &(fp->buffer[SECSIZ]) &&  /* while still in buffer    */
                 bytes > 0)                     /* And still writing        */
          {                                     /*                          */
                *p1++ = *buff++;                /* Move a byte              */
                bytes--;                        /* Decrement counter        */
          }                                     /****************************/
          if (bytes <= 0)                       /* Byte count exhausted??   */
          {                                     /*                          */
            fp->flags |= DIRTY;                 /* Yes, buffer is now dirty */
            fp->offset += written;              /* fix offset               */
            if(fp->offset > fp->hiwater)        /* See if above hiwater mark*/
                fp->hiwater = fp->offset;       /* Fix if necessary         */
            return(written);                    /* Return original byte cnt */
          }                                     /****************************/
          if(_blkio(fp,xsector,fp->buffer,1L,   /* Write full buffer        */
                    B_WRITE) != 1)              /*                          */
                    RETERR(FAILURE,EIO);        /* Couldn't                 */
          xsector++;                            /* Bump sector counter      */
        }                                       /* End boundary problem code*/
                                                /****************************/
                                                /*                          */
/*************************************************                          */
/* The observant reader will note that after the above malarkey, we are now */
/* aligned on a sector boundary.  The following code exploits the oppor-    */
/* tunity to do a multi-sector write.                                       */
/*************************************************                          */
                                                /*                          */
        nsector = bytes >> 7;                   /* divide by 128 for sectors*/
        if(nsector > 0)                         /* Check for no more left   */
        {                                       /*                          */
           if( (xsector <= fp->sector) &&       /* Is current sector buffer */
               (fp->sector < xsector+nsector) ) /* in range of this write?  */
           {                                    /*                          */
                fp->flags &=  ~DIRTY; 		/* If so, invalidate sector */
                fp->sector = -1;                /* buffer.                  */
           }                                    /*                          */
           if(_blkio(fp,xsector,buff,           /*                          */
              nsector,B_WRITE) != nsector)      /* Multi-sector xfer        */
             RETERR(FAILURE,EIO);               /* Just quit on error       */
        }                                       /*                          */
                                                /****************************/
        bytes -= (nsector << 7);                /* Subtract multi-sector    */
                                                /* Byte count               */
        buff  += (nsector << 7);                /* Update address           */
        fp->offset += written;                  /* Update offset now        */
                                                /* (All I/O is complete)    */
        if(fp->offset > fp->hiwater)            /* Fix up hiwater mark      */
                fp->hiwater = fp->offset;       /*                          */
        if(bytes == 0)                          /* If done,                 */
                return(written);                /*          return success  */
                                                /****************************/
        if((fp->flags & DIRTY) != 0)            /* Is buffer dirty?         */
        {                                       /* Test (again) here in case*/
              if(_blkio(fp,fp->sector,          /*  of boundary condition   */
                fp->buffer,1L,B_WRITE) != 1)    /* Yes, write it            */
                  RETERR(FAILURE,EIO);          /* Couldn't write buffer    */
        }                                       /****************************/
        fp -> flags |= DIRTY;                   /* Let's dirty the buffer   */
        fp -> sector = fp -> offset >> 7;       /* Mark sector number       */
                                                /****************************/
        if ( OFFSECT <= HIWSECT )               /* Sector in hiwater area?  */
                                                /****************************/
          _blkio(fp,fp->sector,fp->buffer,1L,   /* Read sector              */
                B_READ);                        /*                          */
        else _clear(fp->buffer,SECSIZ);         /* Or clear it              */
                                                /****************************/
        p1 = &(fp->buffer[0]);                  /* p1 -> address            */
        while(bytes > 0)                        /* Move the bytes           */
        {                                       /*                          */
                *p1++ = *buff++;                /* One at a time            */
                bytes--;                        /* Decrement count          */
        }                                       /****************************/
                                                /*                          */
        return(written);                        /* Return requested #       */
}                                               /****************************/

_clear(ptr,bytes)                               /****************************/
                                                /* Clear a memory area      */
REG     BYTE    *ptr;                           /* -> Area to be cleared    */
REG     WORD    bytes;                          /* # bytes to zero          */
{                                               /****************************/
        while (bytes-- > 0)                     /* Until all exhausted      */
                *ptr++ = 0;                     /* Clear 1 byte             */
}                                               /****************************/

/****************************************************************************/
/*                                                                          */
/*                      _ c h r o u t   F u n c t i o n                     */
/*                      -------------------------------                     */
/*                                                                          */
/*      Function "_chrout" is called to perform "write" operations to the   */
/*      console or list device.                                             */
/*                                                                          */
/*      Calling Sequence:                                                   */
/*                                                                          */
/*              ret = _chrout(buffer,count,func);                           */
/*                                                                          */
/*      Where:                                                              */
/*                                                                          */
/*              buffer  -> the output buffer                                */
/*              count   =  the number of bytes to output                    */
/*              func    =  BDOS function to use                             */
/*                                                                          */
/*              ret     =  count on return                                  */
/*                                                                          */
/****************************************************************************/

LONG _chrout(buffer,count,func)                 /***************************/
                                                /*                         */
REG     BYTE    *buffer;                        /* -> 1st char output      */
REG     LONG    count;                          /* =  # bytes to xfer      */
REG     WORD    func;                           /* BDOS Function           */
                                                /*                         */
{                                               /***************************/
        LONG    xcount;                         /* save area for count     */
                                                /*                         */
        xcount = count;                         /* Copy for later          */
                                                /*                         */
        while(count > 0)                        /* Until all written       */
        {                                       /*                         */
                if(*buffer == '\n')             /* New line char??         */
                {                               /*                         */
                  __BDOS(func,(LONG)'\r');      /* Output carriage return  */
                }                               /*                         */
                __BDOS(func,(LONG)*buffer++);   /* Output next character   */
                count--;                        /* Decrement count         */
        }                                       /*                         */
        return(xcount);                         /* return original count   */
}                                               /***************************/
