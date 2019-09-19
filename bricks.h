/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

#ifndef BRICKS_H
#define BRICKS_H


/******************************************************************************/
/* INCLUDES */
#include <inttypes.h>


/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void put_brick(uint32_t *, int, int, int, int);
void remove_brick(uint32_t *, int, int, int, int);
int test_brick(uint32_t *, int, int, int, int);
int remove_full_lines(uint32_t *);


#endif /* END OF HEADER FILE */
/******************************************************************************/

