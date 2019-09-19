/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "bricks.h"
#include "display.h"

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <stdint.h>


/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* STRUCTS */
const uint8_t bricks[] PROGMEM =
{
	/* .....X..	 *
	 * XXXX.X..	 *
	 * .....X..	 *
	 * .....X..	 */
	0xdf, 0xd0, 0xdf, 0xdf,

	/* .....X..	 *
	 * XXXX.X..	 *
	 * .....X..	 *
	 * .....X..	 */
	0xdf, 0xd0, 0xdf, 0xdf,

	/* ..X...X.	 *
	 * .XX..XXX	 *
	 * ..X.....	 *
	 * ........	 */
	0xbb, 0x19, 0xfb, 0xff,

	/* ..X.....	 *
	 * ..XX.XXX	 *
	 * ..X...X.	 *
	 * ........	 */
	0xfb, 0x13, 0xbb, 0xff,

	/* ........	 *
	 * .XX..XX.	 *
	 * .XX..XX.	 *
	 * ........	 */
	0xff, 0x99, 0x99, 0xff,

	/* ........	 *
	 * .XX..XX.	 *
	 * .XX..XX.	 *
	 * ........	 */
	0xff, 0x99, 0x99, 0xff,

	/* XX....X.	 *
	 * .XX..XX.	 *
	 * .....X..	 *
	 * ........	 */
	0xbc, 0x99, 0xdf, 0xff,

	/* XX....X.	 *
	 * .XX..XX.	 *
	 * .....X..	 *
	 * ........	 */
	0xbc, 0x99, 0xdf, 0xff,

	/* .XX..X..	 *
	 * XX...XX.	 *
	 * ......X.	 *
	 * ........	 */
	0xd9, 0x9c, 0xbf, 0xff,

	/* .XX..X..	 *
	 * XX...XX.	 *
	 * ......X.	 *
	 * ........	 */
	0xd9, 0x9c, 0xbf, 0xff,

	/* ........	 *
	 * .X....XX	 *
	 * .XXX..X.	 *
	 * ......X.	 */
	0xff, 0x3d, 0xb1, 0xbf,

	/* ........	 *
	 * ......X.	 *
	 * .XXX..X.	 *
	 * ...X.XX.	 */
	0xff, 0xbf, 0xb1, 0x97,

	/* ........	 *
	 * ...X..X.	 *
	 * .XXX..X.	 *
	 * ......XX	 */
	0xff, 0xb7, 0xb1, 0x3f,

	/* ........	 *
	 * .....XX.	 *
	 * .XXX..X.	 *
	 * .X....X.	 */
	0xff, 0x9f, 0xb1, 0xbd
};


/******************************************************************************/
/* VARIABLES */


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Draws brick into tetris screen.
*/
void put_brick(uint32_t *screen, int x, int y, int b, int r)
{
	/* Variables. */
	uint8_t br[4];
	uint32_t bc;
	
	r = r & 3;

	memcpy_P(br, bricks + (b << 3) + ((r & 2) << 1), 4);

	if (x > -1)
	{
		bc = ((uint32_t)(~(br[0] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x] &= ~bc;
	}

	if (x > -2)
	{
		bc = ((uint32_t)(~(br[1] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x + 1] &= ~bc;
	}
	
	if (x < 8)
	{
		bc = ((uint32_t)(~(br[2] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x + 2] &= ~bc;
	}

	if (x < 7)
	{
		bc = ((uint32_t)(~(br[3] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x + 3] &= ~bc;
	}
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Removes brick from tetris screen.
*/
void remove_brick(uint32_t *screen, int x, int y, int b, int r)
{
	/* Variables. */
	uint8_t br[4];
	uint32_t bc;
	
	r = r & 3;

	memcpy_P(br, bricks + (b << 3) + ((r & 2) << 1), 4);
	
	if (x > -1)
	{
		bc = ((uint32_t)(~(br[0] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x] |= bc;
	}

	if (x > -2)
	{
		bc = ((uint32_t)(~(br[1] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x + 1] |= bc;
	}

	if (x < 8)
	{
		bc = ((uint32_t)(~(br[2] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x + 2] |= bc;
	}

	if (x < 7)
	{
		bc = ((uint32_t)(~(br[3] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
		screen[x + 3] |= bc;
	}
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Test brick whether it fits to given place in tetris screen.
	
	@return 0 if fits, -1 if not.
*/
int test_brick(uint32_t *screen, int x, int y, int b, int r)
{
	/* Variables. */
	uint8_t br[4];
	uint32_t bc;
	
	if (x < -2 || x > 8)
	{
		return (-1);
	}

	r = r & 3;

	memcpy_P(br, bricks + (b << 3) + ((r & 2) << 1), 4);
	
	bc = ((uint32_t)(~(br[0] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);

	if (x < 0)
	{
		if (bc)
		{
			return (-1);
		}
	}
	else if ((~screen[x]) & bc)
	{
		return (-1);
	}

	bc = ((uint32_t)(~(br[1] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
	
	if (x < -1)
	{
		if (bc)
		{
			return (-1);
		}
	}
	else if ((~screen[x + 1]) & bc)
	{
		return (-1);
	}

	bc = ((uint32_t)(~(br[2] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
	
	if (x > 7)
	{
		if (bc)
		{
			return (-1);
		}
	}
	else if ((~screen[x + 2]) & bc)
	{
		return (-1);
	}

	bc = ((uint32_t)(~(br[3] >> ((r & 1) ? 4 : 0)) & 0x0f)) << (y + 2);
	
	if (x > 6)
	{
		if (bc)
		{
			return (-1);
		}
	}
	else if ((~screen[x + 3]) & bc)
	{
		return (-1);
	}
	
	return (0);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Removes full lines from tetris screen.
	
	@return Number of lines removed.
*/
int remove_full_lines(uint32_t *screen)
{
	/* Variables. */
	uint32_t r = 0xffff00003;
	int lines = 0;
	int i;
	
	/* Check lines. */
	for (i = 0; i < 10; i++)
	{
		r |= screen[i];
	}
	
	/* If no full lines. */
	if (r == 0xffffffff)
	{
		return (0);
	}

	/* Remove full lines. */
	for (i = 2; i < 16; i++)
	{
		if ((r & (1 << i)) != 0)
		{
			continue;
		}
		
		screen[0] = 0xfffffffc & (0xfff000003 | (screen[0] & ((1 << i) - 1)) |
		            ((screen[0] >> 1) & ~((1 << i) - 1)));
		screen[1] = 0xfffffffc & (0xfff000003 | (screen[1] & ((1 << i) - 1)) |
		            ((screen[1] >> 1) & ~((1 << i) - 1)));
		screen[2] = 0xfffffffc & (0xfff000003 | (screen[2] & ((1 << i) - 1)) |
		            ((screen[2] >> 1) & ~((1 << i) - 1)));
		screen[3] = 0xfffffffc & (0xfff000003 | (screen[3] & ((1 << i) - 1)) |
		            ((screen[3] >> 1) & ~((1 << i) - 1)));
		screen[4] = 0xfffffffc & (0xfff000003 | (screen[4] & ((1 << i) - 1)) |
		            ((screen[4] >> 1) & ~((1 << i) - 1)));
		screen[5] = 0xfffffffc & (0xfff000003 | (screen[5] & ((1 << i) - 1)) |
		            ((screen[5] >> 1) & ~((1 << i) - 1)));
		screen[6] = 0xfffffffc & (0xfff000003 | (screen[6] & ((1 << i) - 1)) |
		            ((screen[6] >> 1) & ~((1 << i) - 1)));
		screen[7] = 0xfffffffc & (0xfff000003 | (screen[7] & ((1 << i) - 1)) |
		            ((screen[7] >> 1) & ~((1 << i) - 1)));
		screen[8] = 0xfffffffc & (0xfff000003 | (screen[8] & ((1 << i) - 1)) |
		            ((screen[8] >> 1) & ~((1 << i) - 1)));
		screen[9] = 0xfffffffc & (0xfff000003 | (screen[9] & ((1 << i) - 1)) |
		            ((screen[9] >> 1) & ~((1 << i) - 1)));
		
		lines++;
		
		r = (r >> 1) | 0x80000000;
		i--;
	}

	/* Check for empty screen lines. */
	r = 0xffffffff;
	for (i = 0; i < 10; i++)
	{
		r &= screen[i];
	}
	
	if (r & 0x4)
	{
		lines *= 2;
	}
	
	/* Return number of lines. */
	return (lines);
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/

