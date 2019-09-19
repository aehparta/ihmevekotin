/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

#ifndef IEE_H
#define IEE_H


/******************************************************************************/
/* INCLUDES */
#include <inttypes.h>


/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void ee_init(void);
void ee_add_tscore(uint16_t score);
uint16_t ee_read_tscore(uint16_t score);


#endif /* END OF HEADER FILE */
/******************************************************************************/

