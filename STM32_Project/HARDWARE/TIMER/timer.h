#ifndef __TIMER_H__
#define __TIMER_H__
#include "sys.h"                  // Device header

void TIM3_Int_Init(u16 arr,u16 psc);
uint32_t calculateFrequency(void);
uint32_t calculateDuty(void);
#endif

