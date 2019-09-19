/*
	- Ihmevekotin -
	
	Copyright (c) Antti Partanen 2004.
*/

#ifndef USART_H
#define USART_H


/******************************************************************************/
/* INCLUDES */
#include <inttypes.h>


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void USART_Init(uint16_t);
void USART_Transmit(char);
uint16_t USART_Receive(void);
uint16_t USART_ReceiveD(uint16_t);
void USART_CRLF(void);
void receive_line(char *, int);
void USART_Disable(void);
void USART_Enable(void);
void USART_EchoOff(void);
void USART_EchoOn(void);

void xmodem_receive(void);


#endif /* END OF HEADER FILE */
/******************************************************************************/
