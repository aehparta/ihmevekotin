/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "leds.h"

#include <inttypes.h>
#include <avr/io.h>


/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* VARIABLES */


/******************************************************************************/
/* FUNCTION DEFINITIONS */


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Initialize leds.
*/
void leds_init(void)
{
	/* Init leds. */
	DDRG = 0xff;
	PORTG = 0xff;
	DDRD = 0xff;
	PORTD = 0xff;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Set leds to given value.
	
	@param leds Value of leds.
*/
void leds_set(uint8_t leds)
{
	/* Set leds. */
	PORTD = (PORTD & 0x0f) | ((leds & 0x0f) << 4);
	PORTG = (PORTG & 0xfc) | ((leds >> 4) & 0x03);
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/

