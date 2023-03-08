#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

extern u8 USART1_RX_BUFF0[1024];
extern u8 USART1_RX_BUFF1[1024];

void uart_init(u32 baudrate);
void USART1_DMA_Init(u32 DMA_Memory0BaseAddr, u32 DMA_Memory1BaseAddr);

#endif


