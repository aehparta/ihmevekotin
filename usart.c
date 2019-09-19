/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "usart.h"
#include "ispi.h"

#include <ctype.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>


/******************************************************************************/
/* VARIABLES */
unsigned char echo = 1;


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Init USART.
*/
void USART_Init(uint16_t ubrr)
{
	/* Set baud rate. */
	UBRR1H = (ubrr >> 8);
	UBRR1L = ubrr;
	
	/* Enable receiver and transmitter. */
	UCSR1B = (1 << RXEN) | (1 << TXEN);
	
	/* Set fram format: 8 data, 1 stop, no parity. */
	UCSR1C = (0x3 << UCSZ0);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Disable USART.
*/
void USART_Disable(void)
{
	/* Enable receiver and transmitter. */
	UCSR1B = 0;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Enable USART.
*/
void USART_Enable(void)
{
	/* Enable receiver and transmitter. */
	UCSR1B = (1 << RXEN) | (1 << TXEN);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Transmit byte trough USART.
*/
void USART_Transmit(char data)
{
	/* Check data. */
	if (data == '\n')
	{
		USART_Transmit('\r');
	}

	/* Wait for empty transmit buffer. */
	while (!(UCSR1A & (1 << UDRE)));
	
	/* Transmit data. */
	UDR1 = data;

	/* Return. */
	return;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Receive byte trough USART.
*/
uint16_t USART_Receive(void)
{
	/* Wait for data to be received. */
	while (!(UCSR1A & (1 << RXC)));
	
	/* Receive data. */
	return (UDR1 & 0xff);
}


/******************************************************************************/
/**
	Receive byte trough USART.
*/
uint16_t USART_ReceiveD(uint16_t loops)
{
	/* Variables. */
	uint16_t i;
	
	/* Wait for data to be received. */
	for (i = 0; i < loops; i++)
	{
		if (UCSR1A & (1 << RXC))
		{
			break;
		}
		
		/* ~0.1ms delay. */
		_delay_loop_2(800);
	}

	if (!(UCSR1A & (1 << RXC)))
	{
		return (0xffff);
	}
	
	/* Receive data. */
	return (UDR1 & 0xff);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Print given string trough USART.
*/
void USART_Print(char *string)
{
	/* Variables. */
	int i;
	
	/* Print line. */
	for (i = 0; string[i] != '\0'; i++)
	{
		USART_Transmit(string[i]);
	}
	
	/* Return. */
	return;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Print given line trough USART.
*/
void USART_PrintLine(char *line)
{
	/* Variables. */
	int i;
	
	/* Print line. */
	for (i = 0; line[i] != '\0'; i++)
	{
		USART_Transmit(line[i]);
	}
	
	/* Transmit CR-LF. */
	USART_Transmit(0x0d);
	USART_Transmit(0x0a);
	
	/* Return. */
	return;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Output carriage return and line feed to USART.
*/
void USART_CRLF(void)
{
	/* CR. */
	USART_Transmit(0x0d);
	/* LF. */
	USART_Transmit(0x0a);
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Receive line from USART (Line end is CR).
*/
void receive_line(char *line, int len)
{
	/* Variables. */
	int i = 0;
	unsigned char x;
	
	while (1)
	{
		/* Recieve. */
		x = USART_Receive();

		/* If carriage return, then line feed and break out. */
		if (x == 0x0d)
		{
			/* Transmit CR-LF. */
			if (echo)
			{
				USART_Transmit(0x0d);
				USART_Transmit(0x0a);
			}
						
			/* End line. */
			line[i] = '\0';
			
			/* Break out. */
			break;
		}

		/* If del. */
		if (x == 0x7f)
		{
			/* If counter zero. */
			if (i < 1)
			{
				continue;
			}
			
			/* Decrease counter. */
			i--;
						
			/* Transmit backspace-space-backspace. */
			if (echo)
			{
				USART_Transmit(0x08);
				USART_Transmit(0x20);
				USART_Transmit(0x08);
			}
			
			/* Continue. */
			continue;
		}
		
		/* If not printable char. */
		if (!isprint(x))
		{
			continue;
		}
		
		if (i < (len - 1))
		{
			/* Save char. */
			line[i] = x;
			
			/* Increase counter. */
			i++;

			/* Echo. */
			if (echo)
			{
				USART_Transmit(x);
			}
		}
	}
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Echo off.
*/
void USART_EchoOff(void)
{
	/* Disable echo. */
	echo = 0;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Echo on.
*/
void USART_EchoOn(void)
{
	/* Enable echo. */
	echo = 1;
}
/* END OF FUNCTION */


/******************************************************************************/
/**
	Receive data trough xmodem protocol.
*/
void xmodem_receive(void)
{
	/* Variables. */
	uint8_t i = 0;
	uint16_t x = 0;
	uint8_t chksum = 0;
	uint8_t part = 0;
	uint32_t a = 0;
//	uint8_t buf[128];

	/* Disable interrupts. */
	cli();
	
	for (i = 0; i < 32; i++)
	{
		sdi_w_vs1001(0);
	}
	
	/* Print. */
	printf("Waiting for transmit...\n");

	/* Send NAK once in second and then wait response. */
	for (i = 0; i < 15; i++)
	{
		/* Send NAK. */
		USART_Transmit(0x15);

		/* Try to receive SOH. */
		x = USART_ReceiveD(10000);

		/* Check if nothing received. */		
		if (x == 0xffff)
		{
			continue;
		}
		/* Check if SOH received. */
		else if (x == 0x01)
		{
			break;
		}
		/* Something received but not SOH, exit. */
		else
		{
			x = 15;
			break;
		}
	}
	
	/* Check if no response. */
	if (i >= 15)
	{
		printf("Nothing received.\n");
	
		/* Enable interrupts. */
		sei();
	
		return;
	}
	
	/* Receive. */
	while (1)
	{
		/* Receive Block number and it's complement. */
		part = USART_Receive();
		USART_Receive();

		/* Clear checksum. */
		chksum = 0;
		
		/* Receive data. */
		for (i = 0; i < 128; i++)
		{
			/* Receive data byte. */
			x = USART_Receive();
			
			/* Save data byte to buffer. */
//			buf[i] = (uint8_t)x;

			/* Write buffer. */
			while (PINB & 0x10 == 0);
			
			sdi_w_vs1001((uint8_t)x);
			
			/* Calculate checksum. */
//			chksum += (uint8_t)x;
		}

		/* Receive checksum. */
		x = USART_Receive();
		
		/* Check cheksum. */
/*		if (chksum == (uint8_t)x)
		{
			/* Increase address counter. */
			a += 128;

			/* Send ACK. */
			USART_Transmit(0x06);
//		}
		/* Checksum failed. */
//		else
//		{
			/* Send NAK. */
//			USART_Transmit(0x15);
//		}
		
		/* Try to receive SOH or EOT. */
		x = USART_Receive();

		if (x != 0x01)
		{
			break;
		}
	}

	/* Send ACK. */
	USART_Transmit(0x06);
	
	/* Wait. */
	_delay_loop_2(10000);
	
	/* Print. */
	printf("Transmit complete.\n");
	
	/* Enable interrupts. */
	sei();
	
	/* Return. */
	return;
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/

