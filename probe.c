/*
    - Ihmevekotin -

    Copyright (c) Antti Partanen 2017.
*/

/******************************************************************************/
/* INCLUDES */
#include "defaults.h"
#include "probe.h"

#include <inttypes.h>
#include <avr/io.h>


ISR(USART0_RX_vect)
{
	static char line[64];
	static int line_c = 0;

	uint8_t c = UDR0;

	if (c == '\n' || c == '\r' || line_c >= (sizeof(line) - 1))
	{
		if (line_c <= 0)
		{
			return;
		}
		line[line_c] = '\0';

		if (strcmp(line, "get:config") == 0)
		{
			probe_send("");
			probe_send("config:channels:2");
			probe_send("config:channel:A:static:name:Ihmevekotin");
			probe_send("config:channel:A:type:datetime");
			probe_send("config:channel:A:mode:source");
			probe_send("config:channel:A:method:pull");
			probe_send("config:channel:B:static:name:Ihmevekotin-sink");
			probe_send("config:channel:B:type:datetime");
			probe_send("config:channel:B:mode:sink");
			probe_send("config:channel:B:method:push");
			probe_send("config:channel:B:parent:A");
		}
		else if (line[0] == 'A' && line[1] == '\0')
		{
			uint8_t secs;
			uint8_t mins;
			uint8_t hrs;
			uint8_t d;
			uint8_t m;
			uint8_t y;
			uint8_t c;
			twi_read(0x80, &secs);
			twi_read(0x82, &mins);
			twi_read(0x84, &hrs);
			twi_read(0x86, &d);
			twi_read(0x88, &m);
			twi_read(0x8c, &y);
			twi_read(0x92, &c);
			sprintf(line, "A%i%i%i%i-%i%i-%i%iT%i%i:%i%i:%i%i",
			        c >> 4, c & 0xf, y >> 4, y & 0xf,
			        m >> 4, m & 0xf,
			        d >> 4, d & 0xf,
			        hrs >> 4, hrs & 0xf,
			        mins >> 4, mins & 0xf, secs >> 4, secs & 0xf);
			probe_send(line);
		}
		else if (line[0] == 'B' && line[1] != '\0')
		{
			int i;
			uint8_t y1, y2, y3, y4;
			uint8_t m1, m2;
			uint8_t d1, d2;
			uint8_t h1, h2;
			uint8_t i1, i2;
			uint8_t s1, s2;
			i = sscanf(line + 1, "%c%c%c%c-%c%c-%c%cT%c%c:%c%c:%c%c",
			           &y1, &y2, &y3, &y4,
			           &m1, &m2,
			           &d1, &d2,
			           &h1, &h2,
			           &i1, &i2,
			           &s1, &s2);
			if (i == 14)
			{
				twi_write(0x80, ((s1 - '0') << 4) | (s2 - '0'));
				twi_write(0x82, ((i1 - '0') << 4) | (i2 - '0'));
				twi_write(0x84, ((h1 - '0') << 4) | (h2 - '0'));

				twi_write(0x86, ((d1 - '0') << 4) | (d2 - '0'));
				twi_write(0x88, ((m1 - '0') << 4) | (m2 - '0'));
				twi_write(0x8c, ((y3 - '0') << 4) | (y4 - '0'));
				twi_write(0x92, ((y1 - '0') << 4) | (y2 - '0'));
			}
		}

		line_c = 0;
	}
	else
	{
		line[line_c] = c;
		line_c++;
	}
}

void probe_init(void)
{
	/* set baud rate of usart0 to 38400 (8MHz clock) */
	UBRR0H = 0;
	UBRR0L = 12;

	/* enable receiver and transmitter */
	UCSR0B = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE0);

	/* set frame format: 8 data, 1 stop, no parity */
	UCSR0C = (0x3 << UCSZ0);
}

void probe_send(char *data)
{
	for ( ; *data != '\0'; data++)
	{
		while (!(UCSR0A & (1 << UDRE)));
		UDR0 = *data;
	}

	/* send '\n' */
	while (!(UCSR0A & (1 << UDRE)));
	UDR0 = '\n';
}
