/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "tetris.h"
#include "display.h"
#include "bricks.h"
#include "timer.h"
#include "iee.h"
#include "leds.h"

#include <inttypes.h>


/******************************************************************************/
/* DEFINES */
enum
{
	TMODE_START = 0,
	TMODE_PLAY,
	TMODE_END,
	TMODE_N
};


/******************************************************************************/
/* VARIABLES */
static int tmode = TMODE_START;
static uint16_t tcount = 0;
static uint32_t tscreen[10];
static int brick = 0;
static int nbrick = 0;
static int rot = 0;
static int bx = 3;
static int by = 14;
static int level = 0;
static uint16_t points = 0;


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void t_start(void);
void t_play(void);
void t_end(void);


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Run tetris.
*/
void tetris(void)
{
	switch (tmode)
	{
	case TMODE_START:
		t_start();
		break;

	case TMODE_PLAY:
		t_play();
		break;

	case TMODE_END:
		t_end();
		break;
	}
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Tetris is starting.
*/
void t_start(void)
{
	/* Variables. */
	uint8_t x;
	
	/* Check user action. */
	switch (get_keys())
	{
	case 0x01:
		tcount = 60;
		break;
	}

	/* Check count down. */
	if (tcount > 59)
	{
		tcount = 0;
		tmode = TMODE_PLAY;
		
		for (x = 0; x < 10; x++)
		{
			tscreen[x] = 0xffffffffc;
		}
		
		brick = rand() % 7;
		nbrick = rand() % 7;
		rot = 0;//rand() & 3;
		points = 0;
		level = 0;
		
		/* Set next brick leds. */
		switch (nbrick)
		{
		case 0:
			leds_set(0x3f);
			break;

		case 1:
			leds_set(0x1c);
			break;

		case 2:
			leds_set(0x3c);
			break;

		case 3:
			leds_set(0x2d);
			break;

		case 4:
			leds_set(0x36);
			break;

		case 5:
			leds_set(0x26);
			break;

		case 6:
			leds_set(0x25);
			break;
		}

		return;
		
	}
	
	/* Show count down. */
	x = 5 - (uint8_t)(tcount / 10);
	display_nput(x, 0);
	display_nput(x, 1);
	display_nput(x, 2);
	display_nput(x, 3);
	
	tcount++;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Play!
*/
void t_play(void)
{
	/* Variables. */
	int bbx;
	int bby;
	int rrot;
	int x;
	
	/* Remove brick from old position. */
	remove_brick(tscreen, bx, by, brick, rot);

	bbx = bx;
	bby = by;
	rrot = rot;

	/* Check user action. */
	x = get_keys();
	
	if (x & 0x01)
	{
		rrot++;
	}

	if (x & 0x02)
	{
		bbx++;
	}
	else if (x & 0x04)
	{
		bbx--;
	}
	else if (x & 0x08)
	{
		bby--;
	}
	
	if (test_brick(tscreen, bbx, by, brick, rot) == 0)
	{
		bx = bbx;
	}

	if (test_brick(tscreen, bx, bby, brick, rot) == 0)
	{
		by = bby;
	}

	if (test_brick(tscreen, bx, by, brick, rrot) == 0)
	{
		rot = rrot;
	}
	
	/* Check count. */
	if (tcount < (13 - level))
	{
		tcount++;
	}
	else
	{
		tcount = 0;
		by--;
	}
	
	if (test_brick(tscreen, bx, by, brick, rot) < 0 || by < -2)
	{
		put_brick(tscreen, bx, by + 1, brick, rot);
		
		switch (remove_full_lines(tscreen))
		{
		case 8:
			points += 32;
		case 7:
			points += 28;
		case 6:
			points += 24;
		case 5:
			points += 20;
		case 4:
			points += 16;
		case 3:
			points += 12;
		case 2:
			points += 8;
		case 1:
			points += 4;
		}
			
		points += 1;

		if (points < 50)
		{
			level = 0;
		}
		else if (points < 100)
		{
			level = 1;
		}
		else if (points < 150)
		{
			level = 2;
		}
		else if (points < 200)
		{
			level = 3;
		}
		else if (points < 250)
		{
			level = 4;
		}
		else if (points < 310)
		{
			level = 5;
		}
		else if (points < 370)
		{
			level = 6;
		}
		else if (points < 430)
		{
			level = 8;
		}
		else if (points < 500)
		{
			level = 10;
		}

		if (by > 12)
		{
			tmode = TMODE_END;
			tcount = 0;
			ee_add_tscore(points);
		}
		
		bx = 3;
		by = 14;
		brick = nbrick;
		nbrick = rand() % 7;
		rot = 0; //rand() & 3;

		/* Set next brick leds. */
		switch (nbrick)
		{
		case 0:
			leds_set(0x3f);
			break;

		case 1:
			leds_set(0x1c);
			break;

		case 2:
			leds_set(0x3c);
			break;

		case 3:
			leds_set(0x2d);
			break;

		case 4:
			leds_set(0x36);
			break;

		case 5:
			leds_set(0x26);
			break;

		case 6:
			leds_set(0x25);
			break;
		}
		
		return;
	}

	/* Draw brick to screen. */
	put_brick(tscreen, bx, by, brick, rot);
	
	/* Draw the screen. */
	draw_tetris_screen(tscreen);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Tetris is ending.
*/
void t_end(void)
{
	/* Variables. */
	uint8_t x;
	
	/* Check user action. */
	switch (get_keys())
	{
	case 0x01:
		tcount = 60;
		break;
	}

	/* Check count down. */
	if (tcount > 59)
	{
		tcount = 0;
		tmode = TMODE_START;
		
		return;
	}

	/* Show points down. */
	display_nput((points / 1000) % 10, 0);
	display_nput((points / 100) % 10, 1);
	display_nput((points / 10) % 10, 2);
	display_nput(points % 10, 3);

/*	if (((points >> 12) & 0x0f) < 10)
	{
		display_put('0' + ((points >> 12) & 0x0f), 0);
	}
	else
	{
		display_put('A' - 10 + ((points >> 12) & 0x0f), 0);
	}
	
	if (((points >> 8) & 0x0f) < 10)
	{
		display_put('0' + ((points >> 8) & 0x0f), 1);
	}
	else
	{
		display_put('A' - 10 + ((points >> 8) & 0x0f), 1);
	}

	if (((points >> 4) & 0x0f) < 10)
	{
		display_put('0' + ((points >> 4) & 0x0f), 2);
	}
	else
	{
		display_put('A' - 10 + ((points >> 4) & 0x0f), 2);
	}

	if (((points >> 0) & 0x0f) < 10)
	{
		display_put('0' + ((points >> 0) & 0x0f), 3);
	}
	else
	{
		display_put('A' - 10 + ((points >> 0) & 0x0f), 3);
	}*/
	
	tcount++;
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/


