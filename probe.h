/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2017.
*/

#ifndef PROBE_H
#define PROBE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "itwi.h"

void probe_init(void);
void probe_send(char *data);


#endif
