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

void probe_get_config(char *x)
{
	printf("config:channels:2\n");
	printf("config:channel:G:static:name:ihmevekotin\n");
	printf("config:channel:G:type:datetime\n");
	printf("config:channel:G:mode:source\n");
	printf("config:channel:G:method:pull\n");
	printf("config:channel:H:static:name:ihmevekotin-sink\n");
	printf("config:channel:H:type:datetime\n");
	printf("config:channel:H:mode:sink\n");
	printf("config:channel:H:method:push\n");
}

