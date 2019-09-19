/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

#ifndef SLOGON_H
#define SLOGON_H


/******************************************************************************/
/* INCLUDES */
#include <avr/pgmspace.h>
#include <stdint.h>


/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* EXTERNS */
extern const uint8_t logon_mp3[] PROGMEM;


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void play_logon(void);


#endif /* END OF HEADER FILE */
/******************************************************************************/

