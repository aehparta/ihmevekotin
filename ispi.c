/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "ispi.h"
#include "slogon.h"

#include <avr/io.h>
#include <avr/delay.h>


/******************************************************************************/
/* DEFINES */
#define VS1001_WRITE	0x02
#define VS1001_READ		0x03


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Reset SPI.
*/
void spi_reset(void)
{
	/* Variables. */
	
	/* Set up port B. */
	DDRB = 0xe7;
	PORTB = 0xfd;

	/* Port G as outputs. */
	DDRG |= 0xfc;
	PORTG |= 0xfc;
	
	/* Reset chips in SPI. */
	PORTB &= ~0x01;
	_delay_loop_2(60000);
	PORTB |= 0x01;
	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);

	/* Set some vs1001 registers. */
	spi_wcmd_vs1001(VS1001_MODE, 0x00, 0x00);
	spi_wcmd_vs1001(VS1001_CLOCKF, 0xe0, 0x2e);
	spi_wcmd_vs1001(VS1001_VOLUME, 0x0, 0x0);
	spi_sreset_vs1001();
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Software reset SPI.
*/
void spi_sreset_vs1001(void)
{
	/* Software reset VS1001. */
	spi_wcmd_vs1001(VS1001_MODE, 0x04, 0x00);
	_delay_loop_2(60000);
	
	while (bit_is_clear(PINB, PB4));
	
	sdi_w_vs1001(0x00);
	sdi_w_vs1001(0x00);
	sdi_w_vs1001(0x00);
	sdi_w_vs1001(0x00);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Send command to vs1001 trough SPI.
*/
void spi_wcmd_vs1001(uint8_t a, uint8_t d1, uint8_t d2)
{
	PORTB &= ~(1 << 6);
	spi_w_vs1001(VS1001_WRITE);
	spi_w_vs1001(a);
	spi_w_vs1001(d2);
	spi_w_vs1001(d1);
	PORTB |= (1 << 6);
}
/* END OF FUNCTION */



/******************************************************************************/
/**
	Write to VS1001 trough SPI.
*/
void spi_w_vs1001(uint8_t d)
{
	/* Select VS1001. */
//	PORTB = PORTB & 0xbf;
	if (d & 0x80)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;

	if (d & 0x40)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;

	if (d & 0x20)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;

	if (d & 0x10)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;

	if (d & 0x08)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;

	if (d & 0x04)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;

	if (d & 0x02)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;

	if (d & 0x01)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB |= 0x02;
	PORTB &= ~0x02;
	PORTB &= ~0x02;
		
	/* Unselect VS1001. */
//	PORTB = PORTB | 0x40;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Write to VS1001 trough SPI.
*/
void sdi_w_vs1001(uint8_t d)
{
	PORTB |= (1 << 5);
	
	if (d & 0x80)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);

	PORTB &= ~(1 << 5);

	if (d & 0x40)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);

	if (d & 0x20)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);

	if (d & 0x10)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);

	if (d & 0x08)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);

	if (d & 0x04)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);

	if (d & 0x02)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);

	if (d & 0x01)
	{
		 PORTB |= 0x04;
	}
	else
	{
		 PORTB &= ~0x04;
	}
	
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG |= (1 << 3);
	PORTG &= ~(1 << 3);
	PORTG &= ~(1 << 3);
	
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Beeb (use vs1001 sine test to create sine wave).
*/
void vs1001_beeb(void)
{
	int i;
	
	for (i = 0; i < 32; i++)
	{
		sdi_w_vs1001(0x00);
	}

	sdi_w_vs1001(0x53);
	sdi_w_vs1001(0xef);
	sdi_w_vs1001(0x6e);
	sdi_w_vs1001(0x30);

	for (i = 0; i < 32; i++)
	{
		sdi_w_vs1001(0x00);
	}
	
	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);

	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);

	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);

	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);
	_delay_loop_2(60000);
	
	sdi_w_vs1001(0x45);
	sdi_w_vs1001(0x78);
	sdi_w_vs1001(0x69);
	sdi_w_vs1001(0x74);

	for (i = 0; i < 32; i++)
	{
		sdi_w_vs1001(0x00);
	}
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/

