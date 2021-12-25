#ifndef SCREEN_H
#define SCREEN_H

#define BACKGROUND_COLOUR   ((short) 0x1F00)
#define VIDEO_ADDR          (0xB8000)
#define SCREEN_HEIGHT            25
#define SCREEN_WIDTH            80

#define REG_SCREEN_CTRL     0x3D4
#define REG_SCREEN_DATA     0x3D5

/* va list parameter list */
typedef unsigned char *va_list;

/* width of stack == width of int */
#define	STACKITEM	int
 
/* round up width of objects pushed on stack. The expression before the
& ensures that we get 0 for objects of size 0. */
#define	VA_SIZE(TYPE)					\
	((sizeof(TYPE) + sizeof(STACKITEM) - 1)	\
		& ~(sizeof(STACKITEM) - 1))

/* &(LASTARG) points to the LEFTMOST argument of the function call
(before the ...) */
#define	va_start(AP, LASTARG)	\
	(AP=((va_list)&(LASTARG) + VA_SIZE(LASTARG)))

/* nothing for va_end */
#define va_end(AP)

#define va_arg(AP, TYPE)	\
	(AP += VA_SIZE(TYPE), *((TYPE *)(AP - VA_SIZE(TYPE))))

void printf(char *format, ...);
void clear_screen();
int strlen(const char *string);

#endif