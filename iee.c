/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "iee.h"

#include <inttypes.h>
#include <avr/eeprom.h>


/******************************************************************************/
/* DEFINES */
#define EE_ID 0x7747
#define EE_A_ID 0x0000
#define EE_A_TSCORE (EE_A_ID + 2)
#define EE_A_END 0x0100


/******************************************************************************/
/* VARIABLES */


/******************************************************************************/
/* FUNCTION DEFINITIONS */


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Init eeprom.
*/
void ee_init(void)
{
	/* Variables. */
	uint16_t t;
	
	/* Check ID. */
	eeprom_busy_wait();
	if (eeprom_read_word(EE_A_ID) != EE_ID)
	{
		/* ID failed, clear eeprom contents. */
		eeprom_busy_wait();
		eeprom_write_word(EE_A_ID, EE_ID);
		
		for (t = (EE_A_ID + 2); t < EE_A_END; t += 2)
		{
			eeprom_busy_wait();
			eeprom_write_word(t, 0x0000);
		}
	}
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Add tetris eeprom.
*/
void ee_add_tscore(uint16_t score)
{
	/* Variables. */
	uint16_t t;
	uint16_t k;
	
	/* Wait and then check new score. */
	eeprom_busy_wait();
	
	for (t = EE_A_TSCORE; t < (EE_A_TSCORE + 20); t += 2)
	{
		eeprom_busy_wait();
		k = eeprom_read_word(t);
		
		if (k < score)
		{
			break;
		}
	}
	
	if (t >= (EE_A_TSCORE + 20))
	{
		return;
	}
	
	for ( ; t < (EE_A_TSCORE + 20); t += 2)
	{
		eeprom_busy_wait();
		eeprom_write_word(t, score);
		score = k;
		eeprom_busy_wait();
		k = eeprom_read_word(t + 2);
	}
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Read tetris eeprom.
*/
uint16_t ee_read_tscore(uint16_t score)
{
	/* Variables. */
	uint16_t t;
	
	/* Wait and then return score. */
	eeprom_busy_wait();
	return (eeprom_read_word(EE_A_TSCORE + 2 * score));
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/

