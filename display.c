/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "display.h"
#include "fonts.h"
#include "timer.h"

#include <string.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <inttypes.h>


/******************************************************************************/
/* VARIABLES */

/** Screen buffer. */
static unsigned char screen[4 * 8] =
{
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};


/** Kinda counter. */
static uint8_t h = 0;


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Resets the LED matrix.
*/
void display_reset(void)
{
	/* Set ports. */
	DDRA = 0xff;
	DDRC = 0xff;
	DDRF = 0xff;

	PORTA = 0xff;
	PORTC = 0xff;
	PORTF = 0xff;

	/* Clear screen. */
	memset(screen, 0x00, 4 * 8);
	
   	/* Set interrupt handler. */
   	ASSR = 0x00;
	TCNT0 = 0x00;
	OCR0 = 200;
	TCCR0 = 0x0b;
	TIFR = 0xff;
	
	/* Enable timer 0 interrupts. */
	TIMSK = 0x02;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Put char to display.
*/
void display_put(char c, char d)
{
	/* Variables. */
	uint16_t ch;
	unsigned char buf[5];
	
	ch = (c - 0x20) * 5;
	memcpy_P(buf, ascii + ch, 5);
	
	memcpy(screen + (d << 3), buf, 5);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Put number to display.
*/
void display_nput(char n, char d)
{
	/* Variables. */
	uint16_t ch;
	unsigned char buf[5];
	
	ch = (n + 16) * 5;
	memcpy_P(buf, ascii + ch, 5);
	
	memcpy(screen + (d << 3), buf, 5);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Put pixel to display.
*/
void put_pixel(uint8_t x, uint8_t y, uint8_t state)
{
	/* Variables. */
	char d;
	uint16_t a;
	
	/* Calculate pixel position. */
	if (y < 7)
	{
		a = 0;
	}
	else
	{
		a = 16;
	}
	
	if (x < 5)
	{
		a += x;
	}
	else
	{
		a += 8 + x - 5;
	}
	
	/* Draw pixel. */
	d = screen[a];
	
	if (state)
	{
		d |= (1 << (6 - y % 7));
	}
	else
	{
		d &= 0xff - (1 << (6 - y % 7));
	}
	
	screen[a] = d;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Updates the LED matrix.
*/
ISR(TIMER0_COMP_vect)
{
	/* Variables. */
	unsigned char display = 0;
	unsigned char column = 0;
	char i;

	/* Disable interrupts. */
	cli();
	
	/* Check timing events. */
	if (h < 60)
	{
		h++;
	}
	else
	{
		h = 0;
		
		timing();
	}
	
	/* Update display. */
	for (i = 0; i < (5 * 4); i++)
	{
		if (column < 4)
		{
			column++;
		}
		else
		{
			if (display < 3)
			{
				display++;
			}
			else
			{
				display = 0;
			}
			
			column = 0;
		}
		
		PORTF = 0xff;
		PORTA = 0xff - (4 << display);
	
//		PORTC = (i & 1) ? 0x55 : 0xaa;
		PORTC = screen[column + (display << 3)];
		PORTF = 0xff - (1 << column);
	}
	
	for (i = 0; i < 2; i++);
	PORTF = 0xff;
	
	/* Enable interrupts. */
//	TCNT0 = 0x00;
//	OCR0 = 0x30;
	TIFR = 0xff;
	sei();
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Draw tetris screen to the LED matrix.
*/
void draw_tetris_screen(uint32_t *ts)
{
	/* Variables. */
	int display;
	int column;
	uint8_t c;

	/* Draw. */
	for (display = 0; display < 4; display++)
	{
		for (column = 0; column < 5; column++)
		{
			c = (uint8_t)(ts[column + (display & 1) * 5] >>
			    ((~display & 2) ? 9 : 2));
			screen[column + (display << 3)] =  c;
		}
	}
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/


