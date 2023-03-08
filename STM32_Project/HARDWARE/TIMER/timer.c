#include "timer.h"
#include "stm32f4xx.h"                  // Device header
#include "usart.h"

void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitStructure.TIM_Period = arr;
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);	//TIM_TimeBaseInit use update event to reload the Prescaler value immediatly.
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_Cmd(TIM3,DISABLE); //Disable TIM3
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_Cmd(TIM3,DISABLE); //Disable TIM3
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		DMA_Cmd(DMA2_Stream2, DISABLE);
		USART1_DMA_Init((u32)USART1_RX_BUFF0, (u32)USART1_RX_BUFF1);
		DMA_Cmd(DMA2_Stream2, ENABLE);
		printf("Usart1_RX_DMA RESET!\r\n");
	}
}

