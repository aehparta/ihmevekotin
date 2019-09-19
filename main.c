/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "display.h"
#include "itwi.h"
#include "usart.h"
#include "leds.h"
#include "slogon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/delay.h>


/******************************************************************************/
/* VARIABLES */

/******************************************************************************/
/* FUNCTION DEFINES */
void help(char *);
void date(char *);
void spireset(char *);
void smode(char *);
void tscore(char *);


/******************************************************************************/
/* DEFINES */

/* Command struct. */
typedef struct
{
	char *cmd;
	void (*func)(char *);
} COMMAND;

/* Command list. */
COMMAND cmds[] =
{
	{ "help", help },
	{ "date", date },
	{ "mode", smode },
	{ "tscore", tscore },
	{ "playl", play_logon },
	{ 0, 0 }
};


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Help-command.
*/
void help(char *x)
{
	printf("Miracle machine(tm) help:\n");
	printf(" help     - Prints this help.\n");
	printf(" date     - Show or change current date and time.\n");
	printf("            Format YYYYMMDD or hhmmss.\n");
	printf(" mode [x] - Set current mode to given.\n");
	printf(" tscore   - Show tetris score.\n");
	printf(" playl    - Play logon sound.\n");
}


/******************************************************************************/
/**
	Set mode.
*/
void smode(char *x)
{
	/* Variables. */
	int m;
	
	/* Get mode. */
	m = atoi(x);
	
	/* Set mode. */
	set_mode(m);
}


/******************************************************************************/
/**
	View tetris score.
*/
void tscore(char *x)
{
	/* Variables. */
	int i;
	
	/* View score. */
	printf("*** Tetris top ten: ***\n");
	for (i = 0; i < 10; i++)
	{
		printf("***   %02i: %05i     ***\n", i, ee_read_tscore(i));
	}
}


/******************************************************************************/
/**
	Date-command.
*/
void date(char *x)
{
	/* Variables. */
	uint8_t secs;
	uint8_t mins;
	uint8_t hrs;
	uint8_t d;
	uint8_t m;
	uint8_t y;
	uint8_t c;
	uint8_t t;
	int i;
	
	/* Show date and time. */
	if (strlen(x) < 1)
	{
		/* Read values. */
		cli();
		twi_read(0x80, &secs);
		twi_read(0x82, &mins);
		twi_read(0x84, &hrs);
		twi_read(0x86, &d);
		twi_read(0x88, &m);
		twi_read(0x8c, &y);
		twi_read(0x92, &c);
		sei();
		
		/* Print date. */
		printf("Current time: %i%i/%i%i/%i%i%i%i, %i%i:%i%i:%i%i\n",
		       d >> 4, d & 0xf, m >> 4, m & 0xf,
		       c >> 4, c & 0xf, y >> 4, y & 0xf, hrs >> 4, hrs & 0xf,
		       mins >> 4, mins & 0xf, secs >> 4, secs & 0xf);
		       
		return;
	}
	/* Set time. */
	else if (strlen(x) == 6)
	{
		i = sscanf(x, "%c%c%c%c%c%c", &d, &hrs, &m, &mins, &y, &secs);
		
		d -= '0';
		hrs -= '0';
		m -= '0';
		mins -= '0';
		y -= '0';
		secs -= '0';
		
		if (i == 6 && d < 3 && m < 6 && y < 6)
		{
			cli();
			twi_write(0x80, (y << 4) | secs);
			twi_write(0x82, (m << 4) | mins);
			twi_write(0x84, (d << 4) | hrs);
			sei();
			return;
		}
	}
	/* Set date. */
	else if (strlen(x) == 8)
	{
		i = sscanf(x, "%c%c%c%c%c%c%c%c", &c, &hrs, &y, &mins, &m, &secs, &d, &t);
		
		c -= '0';
		hrs -= '0';
		y -= '0';
		mins -= '0';
		m -= '0';
		secs -= '0';
		d -= '0';
		t -= '0';
		
		if (i == 8 && m < 2 && d < 4)
		{
			cli();
			twi_write(0x86, (d << 4) | t);
			twi_write(0x88, (m << 4) | secs);
			twi_write(0x8c, (y << 4) | mins);
			twi_write(0x92, (c << 4) | hrs);
			sei();
			return;
		}
	}
	
	/* Print help. */
	printf("Date usage:\n");
	printf("\"date\"          - prints current date and time.\n");
	printf("\"date hhmmss\"   - set current time.\n");
	printf("\"date YYYYMMDD\" - set current date.\n");
	
}


/******************************************************************************/
/**
	Execute command.
*/
void exec_command(char *cmd)
{
	/* Variables. */
	int i;
	int k;
	char b = 0;
	
	/* Check empty command. */
	if (strlen(cmd) < 1)
	{
		return;
	}
	
	/* Find space. */
	for (k = 0;
	     cmd[k] != '\0' && cmd[k] != ' ';
	     k++);

	cmd[k] = '\0';
	k++;
			
	/* Compare commands. */
	for (i = 0; cmds[i].func != 0; i++)
	{
		if (strcmp(cmd, cmds[i].cmd) == 0)
		{
			cmds[i].func(&cmd[k]);
			b = 1;
			break;
		}
	}
	
	/* If command not found. */
	if (!b)
	{
		printf("Invalid command.\n");
	}
	
	/* Return. */
	return;
}


/******************************************************************************/
/**
	Display login.
*/
void login(void)
{
	printf("\nConnected to Miracle machine(tm) through serial RS232 line.\n");
	printf("Copyright (c) 2004-2006 Antti Partanen.\n");
	printf("(type help, if you dont known what you are doing)\n");
}


/******************************************************************************/
/**
	Display prompt.
*/
void prompt(void)
{
	printf("> ");
}


/******************************************************************************/
/**
	Main application function.
*/
int main(void)
{
	/* Variables. */
	char line[64];
	int n;
	
	/* Disable interrupts. */
	cli();
	
	/* Port E as inputs. */
	DDRE = 0x00;

	/* Pull ups high. */
	PORTE = 0xff;

	/* Init leds. */
	leds_init();
	leds_set(0xff);
	
	/* Init eeprom. */
	ee_init();
	
	/* Init SPI. */
	spi_reset();

	/* Reset display. */
	display_reset();

	/* Init twi. */
	twi_init();

	/* Set random seed. */
	twi_read(0x80, &n);
	srand(n);

	/* Initialize USART to 38400 (8.000MHz clock assumed). */
	USART_Init(12);
	
	/* Initialize USART as default input/output. */
	stdin = stdout = fdevopen(USART_Transmit, USART_Receive);

	/* Display login. */
	login();
	
	/* Enable interrupts. */
	sei();

	/* Set leds. */
	leds_set(0x00);

	/* Try to play some mp3! */
	play_logon();
		
	/* Loop. */
	while (1)
	{
		/* Print prompt. */
		prompt();
		
		/* Receive line. */
		receive_line(line, 64);
		
		/* Execute command. */
		exec_command(line);
		
		/* Reset received line. */
		memset(line, 0, 64);
	}
	
	/* Return. */
	return (0);
}
/* END OF FUNCTION */


/* END OF SOURCE FILE */
/******************************************************************************/


