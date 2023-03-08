#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "key.h"
#include "lcd.h"
#include "led.h"
#include "OLED.h"
#include "timer.h"

#define USE_TIMER3_TO_RESET_USART1_DMA 1

int main(void)
{
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	delay_init(168);
	uart_init(460800);
	#if USE_TIMER3_TO_RESET_USART1_DMA == 1
		TIM3_Int_Init(5000-1,8400-1);	//period: 500ms
	#endif
	OLED_Init();
	OLED_ShowString(1,1,"Initialized");
	
  while(1)
	{
		//receive new frame(1024 Bytes)
		if(DMA_GetFlagStatus(DMA2_Stream2, DMA_FLAG_TCIF2) == SET)
		{
			DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);	//clear flag first
			printf("New frame received!\r\n");
			
			#if USE_TIMER3_TO_RESET_USART1_DMA == 1
				//Use timer3 to reset usart_1_rx dma while there is no data.
				TIM_SetCounter(TIM3, 0);
				TIM_Cmd(TIM3,ENABLE);
			#endif
			
			if(DMA2_Stream2->CR&(1<<19) != 0)//buf0 if full, which can be processed
			{
				OLED_DrawBMP(USART1_RX_BUFF0);
			}
			else
			{
				OLED_DrawBMP(USART1_RX_BUFF1);
			}
		}
	}
}


