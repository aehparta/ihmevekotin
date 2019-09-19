/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

#ifndef DISPLAY_H
#define DISPLAY_H


/******************************************************************************/
/* INCLUDES */
#include <inttypes.h>


/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void display_reset(void);
void display_put(char, char);
void display_nput(char, char);
void put_pixel(uint8_t, uint8_t, uint8_t);
void draw_tetris_screen(uint32_t *);


#endif /* END OF HEADER FILE */
/******************************************************************************/

