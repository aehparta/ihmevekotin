/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "timer.h"
#include "display.h"
#include "itwi.h"
#include "tetris.h"

#include <stdlib.h>
#include <inttypes.h>
#include <avr/io.h>


/******************************************************************************/
/* DEFINES */
enum
{
	MODE_CLOCK = 0,
	MODE_COOL1,
	MODE_TETRIS,
	MODE_N
};


/******************************************************************************/
/* VARIABLES */
static uint8_t mode = MODE_CLOCK;
static uint8_t w = 0;
static uint8_t kbuf;


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void clock(void);
void cool1(void);


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Should be called about 20 times per second...
*/
void timing(void)
{
	/* Variables. */
	uint8_t n;
	
	/* Read input. */
	if (w < 1)
	{
		kbuf = 0;
		
		if (!(PINE & 0x80))
		{
			mode++;

			if (mode >= MODE_N)
			{
				mode = 0;
			}
			
			w = 10;
		}
		if (!(PINE & 0x40))
		{
			kbuf |= 0x1;
			w = 2;
		}
		if (!(PINE & 0x20))
		{
			kbuf |= 0x2;
			w = 1;
		}
		if (!(PINE & 0x10))
		{
			kbuf |= 0x4;
			w = 1;
		}
		if (!(PINE & 0x08))
		{
			kbuf |= 0x8;
			w = 1;
		}
		if (!(PINE & 0x04))
		{
			mode++;

			if (mode >= MODE_N)
			{
				mode = 0;
			}
			
			w = 10;
		}
	}
	else
	{
		kbuf = 0;
		w--;
	}

	/* Run selected mode. */
	switch (mode)
	{
	case MODE_CLOCK:
		clock();
		break;
		
	case MODE_COOL1:
		cool1();
		break;
		
	case MODE_TETRIS:
		return (tetris());
	}
	
	/* Return redraw. */
	return (0);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Get key buffer value.
*/
uint8_t get_keys(void)
{
	return (kbuf);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Show clock.
*/
void clock(void)
{
	/* Variables. */
	char mins = 0;
	char hours = 0;
	char n1 = 0;
	char n2 = 0;
	char n3 = 0;
	char n4 = 0;

	/* Just show clock for now. */
	twi_read(0x82, &mins);
	twi_read(0x84, &hours);
	
	n1 = mins & 0x0f;
	n2 = mins >> 4;
	n3 = hours & 0x0f;
	n4 = hours >> 4;
	
	display_nput(n4, 0);
	display_nput(n3, 1);
	display_nput(n2, 2);
	display_nput(n1, 3);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Show cool demo 1.
*/
void cool1(void)
{
	/* Variables. */
	static uint8_t cnt = 0;
	int i;
	int j;
	int r;
	
	if (cnt < 10)
	{
		cnt++;
		return;
	}
	
	/* Draw. */
	for (i = 0; i < 10; i++)
	{
		r = rand() % 14;
		
		for (j = 13; j >= r; j--)
		{
			put_pixel(i, j, 0);
		}
		
		r--;
		
		for (j = r; j >= 0; j--)
		{
			put_pixel(i, j, 1);
		}
	}
	
	cnt = 0;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Show countdown.
*/
void countdown(void)
{
	/* Variables. */
	static uint8_t cnt = 0;
	int i;
	int j;
	int r;
	
	if (cnt < 10)
	{
		cnt++;
		return;
	}
	
	/* Draw. */
	for (i = 0; i < 10; i++)
	{
		r = rand() % 14;
		
		for (j = 13; j >= r; j--)
		{
			put_pixel(i, j, 0);
		}
		
		r--;
		
		for (j = r; j >= 0; j--)
		{
			put_pixel(i, j, 1);
		}
	}
	
	cnt = 0;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Set mode.
*/
void set_mode(int m)
{
	if (m < 0 || m >= MODE_N)
	{
		return;
	}

	mode = m;
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/


