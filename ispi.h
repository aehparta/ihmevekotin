/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

#ifndef ISPI_H
#define ISPI_H


/******************************************************************************/
/* INCLUDES */
#include <inttypes.h>


/******************************************************************************/
/* DEFINES */
#define VS1001_MODE			0
#define VS1001_STATUS		1
#define VS1001_INT_FCTLH	2
#define VS1001_CLOCKF		3
#define VS1001_DECODE_TIME	4
#define VS1001_AUDATA		5
#define VS1001_WRAM			6
#define VS1001_WRAMADDR		7
#define VS1001_HDAT0		8
#define VS1001_HDAT1		9
#define VS1001_A1ADDR		10
#define VS1001_VOLUME		11


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void spi_reset(void);
void spi_sreset_vs1001(void);

void spi_wcmd_vs1001(uint8_t, uint8_t, uint8_t);
void spi_w_vs1001(uint8_t);

void sdi_w_vs1001(uint8_t);
void sdi_w__vs1001(uint8_t);

void vs1001_beeb(void);


#endif /* END OF HEADER FILE */
/******************************************************************************/

