/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

#ifndef ITWI_H
#define ITWI_H


/******************************************************************************/
/* INCLUDES */
#include <inttypes.h>


/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void twi_init(void);
int twi_write(uint8_t, uint8_t);
int twi_read(uint8_t, uint8_t *);


#endif /* END OF HEADER FILE */
/******************************************************************************/

