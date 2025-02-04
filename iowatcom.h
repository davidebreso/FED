/*
 *      FED - Folding Editor
 *
 *      By Shawn Hargreaves, 1994
 *
 *      See README.TXT for copyright conditions
 *
 *      watcom-specific IO routines for screen output, reading from the
 *      keyboard and mouse, and simple (faster than stdio) buffered file IO
 */


#ifndef IOWATCOM_H
#define IOWATCOM_H


#ifndef TARGET_WATCOM
   #error This file should only be compiled as part of the watcom target
#endif


#define TN     "watcom"


#include <stdio.h>
#include <bios.h>
#include <io.h>
#include <unistd.h>
#include <fcntl.h>


#define _USE_LFN        0


#define SCRN_ADDR(x,y)  (video_base+((x)*2)+((y)*screen_w*2))


#define TL_CHAR         0xDA
#define TOP_CHAR        0xC4
#define TR_CHAR         0xBF
#define SIDE_CHAR       0xB3
#define BL_CHAR         0xC0
#define BOTTOM_CHAR     0xC4
#define BR_CHAR         0xD9
#define LJOIN_CHAR      0xC3
#define RJOIN_CHAR      0xB4
#define BJOIN_CHAR      0xC1
#define UP_CHAR         0x18
#define DOWN_CHAR       0x19
#define BAR_CHAR        0xB2


#define _NORMALCURSOR   0x0607          /* normal underline cursor */
#define _FULLCURSOR     0x0007          /* full block cursor */
#define _HALFCURSOR     0x0407          /* half-height block cursor */
#define _NOCURSOR       0x2000          /* no cursor */


extern int screen_w;
extern int screen_h;
extern int x_pos, y_pos;
extern int attrib;
extern int norm_attrib;
extern int video_base;

#define gch()           _bios_keybrd(_KEYBRD_READ)                               /* 'raw' character input */
#define ascii(c)        ((c) & 0x00ff)                                           /* convert raw to ascii */
#define keypressed()    (_bios_keybrd(_KEYBRD_READY)!=0)                         /* are characters waiting? */
#define modifiers()     (_bios_keybrd(_KEYBRD_SHIFTSTATUS))                      /* read shift, ctrl + alt */
#define ctrl_pressed()  (modifiers() & 4)                                        /* is the ctrl key pressed? */
#define alt_pressed()   ((modifiers() & 8) && !(*(char *)(1174) & 8))            /* is the alt key pressed? */
#define print(c)        _bios_printer(0,0,c)                                     /* print a character */
#define printer_ready() (TRUE)                                                   /* is there a printer? */
void cls();
#define home()          goto1(0,0)
#define newline()       { cr(); linefeed(); }
#define goto1(x,y)      { x_pos = x; y_pos = y; }
void goto2(int x, int y);
#define backspace()     { if (x_pos > 0) { x_pos--; pch(' '); x_pos--; } }
#define linefeed()      { if (y_pos < screen_h-1) y_pos++; }
#define cr()            x_pos=0;
#define tattr(col)      { attrib = col; }
#define sel_vid()       attrib = config.sel_col
#define fold_vid()      attrib = config.fold_col
#define hi_vid()        attrib = config.hi_col
#define n_vid()         attrib = config.norm_col
#define keyword_vid()   attrib = config.keyword_col
#define comment_vid()   attrib = config.comment_col
#define string_vid()    attrib = config.string_col
#define number_vid()    attrib = config.number_col
#define symbol_vid()    attrib = config.symbol_col
#define sf_vid()        attrib = (((config.sel_col) & 0x0f) |  \
					   ((config.fold_col) & 0xf0))
#define hide_c()        my_setcursor(0x2000)
#define show_c()        my_setcursor(config.big_cursor ? 0x0007 : 0x0D0E);
#define show_c2(ovr)    my_setcursor(((ovr) ?                  \
				      !config.big_cursor :     \
				      config.big_cursor) ?     \
				     0x0007 : 0x0D0E )

#define refresh_screen()

#define term_suspend(newline)    my_setcursor(_NORMALCURSOR)


/* file IO functions */

#define FMODE_READ      1       /* file mode flags */
#define FMODE_WRITE     2

#define BUFFER_SIZE     2048    /* size of the buffer used for file IO */

#ifdef EOF
#undef EOF
#endif

#define EOF             1       /* EOF returned by get_char() in errno */


typedef struct MYFILE     /* we implement our own files: faster than stdio */
{
   int f_hndl;                  /* DOS file handle */
   int f_mode;                  /* read or write flag */
   unsigned int f_size;         /* number of bytes yet to be read */
   unsigned char *f_buf_pos;    /* current position in buffer */
   unsigned char *f_buf_end;    /* end of the buffer */
   unsigned char f_buf[BUFFER_SIZE];
} MYFILE;


/* these macros access the buffers directly, for speed. Sod being readable */

#define get_char(f)     ((f->f_buf_pos < f->f_buf_end) ?       \
			 *(f->f_buf_pos++) :                   \
			 refill_buffer(f,FALSE))

#define peek_char(f)    ((f->f_buf_pos < f->f_buf_end) ?       \
			 *(f->f_buf_pos) :                     \
			 refill_buffer(f,TRUE))

#define put_char(f,c)   {                                      \
			   if (f->f_buf_pos>=f->f_buf_end)     \
			      flush_buffer(f);                 \
			   *(f->f_buf_pos++)=c;                \
			}


#define delete_file(p)      unlink(p)

int file_size(char *p);

#define file_exists(p)      (file_size(p) >= 0)



#endif          /* IOWATCOM_H */
