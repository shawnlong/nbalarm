#ifndef __TIM1_H
#define __TIM1_H
#include "type_def.h"
#define TIM1_COUNTERMODE_UP  0x00
#define TIM1_IT_UPDATE       0x01
void Tim1_Time_Upmode_conf(uint16_t TIM1_Prescaler,uint16_t TIM1_Period,uint8_t TIM1_RepetitionCounter);
void TimingDelay_Decrement(void);
void delay_ms( u32 nTime);
#endif