/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "itwi.h"

#include <avr/io.h>
#include <compat/twi.h>


/******************************************************************************/
/* DEFINES */
#define MAX6900_SLAVE_ID 0xa0 /* 1010 0000 */

/*
	Maximal number of iterations to wait for a device to respond for a
	selection.  Should be large enough to allow for a pending write to
	complete, but low enough to properly abort an infinite loop in case
	a slave is broken or not present at all.  With 100 kHz TWI clock,
	transfering the start condition and SLA+R/W packet takes about 10
	µs.  The longest write period is supposed to not exceed ~ 10 ms.
	Thus, normal operation should not require more than 100 iterations
	to get the device to respond to a selection.
*/
#define MAX_ITER        20


/******************************************************************************/
/* VARIABLES */

/*
	Saved TWI status register, for error messages only.  We need to
	save it in a variable, since the datasheet only guarantees the TWSR
	register to have valid contents while the TWINT bit in TWCR is set.
*/
uint8_t twst;


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Initialize TWI interface for MAX6900.
*/
void twi_init(void)
{
	/* Initialize TWI clock. */
	TWSR = 0;
	TWBR = 12;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Write data trough TWI to MAX6900.

	@return 0 on success, -1 on errors.
*/
int twi_write(uint8_t cmd,
              uint8_t d)
{
	/* Variables. */
	uint8_t sla = MAX6900_SLAVE_ID;
	int i;
	
	/* All write commands are even. */
	cmd &= 0xfe;
	
	/* Try to send. */
	for (i = 0; i < MAX_ITER; i++)
	{
		/* Send start condition. */
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);
		
		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_REP_START: /* OK, but should not happen. */
			case TW_START:
				break;
		
			case TW_MT_ARB_LOST:
				i--;
				continue;
		
			default:
				return (-1); /* Error: not in start condition. */
				             /* NB: do !not! send stop condition */
		}

		/* Send SLA+W. */
		TWDR = sla | TW_WRITE;
		
		/* Clear interrupt to start transmission. */
		TWCR = _BV(TWINT) | _BV(TWEN);
		
		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);
		
		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_MT_SLA_ACK:
				break;
		
			case TW_MT_SLA_NACK: /* Nack during select: device busy writing. */
				continue;
		
			case TW_MT_ARB_LOST: /* re-arbitrate */
				i--;
				continue;
		
			default:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-2);
		}
  
		/* Send command. */
		TWDR = cmd;
		
		/* Clear interrupt to start transmission. */
		TWCR = _BV(TWINT) | _BV(TWEN);
		
		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);
		
		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_MT_DATA_ACK:
				break;
		
			case TW_MT_DATA_NACK:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-3);
		
			case TW_MT_ARB_LOST:
				i--;
				continue;
		
			default:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-4);
		}
		
		break;
	}

	/* Send data byte. */
	TWDR = d;
	
	/* Start transmission. */
	TWCR = _BV(TWINT) | _BV(TWEN);
	
	/* Wait for transmission. */
	while ((TWCR & _BV(TWINT)) == 0);
	
	/* Check status. */
	switch ((twst = TW_STATUS))
	{
		case TW_MT_DATA_NACK:
			/* Must send stop condition. */
			TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
			return (-5);
	
		case TW_MT_DATA_ACK:
			break;
	
		default:
			/* Must send stop condition. */
			TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
			return (-6);
	}

	/* Send stop condition. */
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
	
	/* Return. */
	return (0);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Read data trough TWI from MAX6900.

	@return 0 on success, -1 on errors.
*/
int twi_read(uint8_t cmd,
             uint8_t *d)
{
	/* Variables. */
	uint8_t sla = MAX6900_SLAVE_ID;
	uint8_t twcr;
	int i;

	/* All read commands are odd. */
	cmd |= 1;

	/* Try to read. */
	for (i = 0; i < MAX_ITER; i++)
	{
		/* Send start condition. */
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);
		
		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_REP_START: /* OK, but should not happen. */
			case TW_START:
				break;
		
			case TW_MT_ARB_LOST:
				i--;
				continue;
		
			default:
				return (-1); /* Error: not in start condition. */
				             /* NB: do !not! send stop condition */
		}
		
		/* Send SLA+W. */
		TWDR = sla | TW_WRITE;
		
		/* Clear interrupt to start transmission. */
		TWCR = _BV(TWINT) | _BV(TWEN);
		
		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);
		
		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_MT_SLA_ACK:
				break;
		
			case TW_MT_SLA_NACK: /* Nack during select: device busy writing. */
				continue;
		
			case TW_MT_ARB_LOST: /* re-arbitrate */
				i--;
				continue;
		
			default:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-2);
		}

 		/* Send command. */
		TWDR = cmd;
		
		/* Clear interrupt to start transmission. */
		TWCR = _BV(TWINT) | _BV(TWEN);
		
		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);
		
		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_MT_DATA_ACK:
				break;
		
			case TW_MT_DATA_NACK:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-3);
		
			case TW_MT_ARB_LOST:
				i--;
				continue;
		
			default:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-4);
		}

		/* Next cycle: master receiver mode. */
		/* Send (rep.) start condition. */
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
		
		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);
	
		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_START: /* OK, but should not happen. */
			case TW_REP_START:
				break;
		
			case TW_MT_ARB_LOST:
				i--;
				continue;
		
			default:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-5);
		}

		/* Send SLA+R. */
		TWDR = sla | TW_READ;
		
		/* Clear interrupt to start transmission. */
		TWCR = _BV(TWINT) | _BV(TWEN);

		/* Wait for transmission. */
		while ((TWCR & _BV(TWINT)) == 0);

		/* Check status. */
		switch ((twst = TW_STATUS))
		{
			case TW_MR_SLA_ACK:
				break;
		
			case TW_MR_SLA_NACK:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-6);
	
			case TW_MR_ARB_LOST:
				i--;
				continue;
		
			default:
				/* Must send stop condition. */
				TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
				return (-7);
		}
		
		break;
	}
	
	/* Read data byte. */
	twcr = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);

	/* Clear int to start transmission. */
	TWCR = twcr;

	/* Wait for transmission. */
	while ((TWCR & _BV(TWINT)) == 0);

	/* Check status. */
	switch ((twst = TW_STATUS))
	{
		case TW_MR_DATA_NACK:
			/* Send stop condition. */
			TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
			return (-8);
			
		case TW_MR_DATA_ACK:
			*d = TWDR;
			break;
		
		default:
			/* Send stop condition. */
			TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
			return (-9);
	}

	/* Send stop condition. */
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
	
	/* Return. */
	return (0);
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/
