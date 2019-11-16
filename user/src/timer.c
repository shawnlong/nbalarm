#include "stm8s.h"


#include "timer.h"
/*2Mhz HSI clock condition*/
#define TIM1_PERSCALE       19999 	/*20K clock devide, 2Mhz/20K = 100hz, 1count = 10ms*/
#define TIM1_PERIOD			100	  	/*10ms * 100 = 1s*/
#define TIM1_REPEATCOUNT	0		/*no repeat*/

 __IO uint32_t ticks_second = 0;

uint8_t timer_init()
{
	/*use TIM1 to generate 1 sencond conter*/
	TIM1_TimeBaseInit(TIM1_PERSCALE, TIM1_COUNTERMODE_UP, TIM1_PERIOD, TIM1_REPEATCOUNT);
	TIM1_ARRPreloadConfig(ENABLE);
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);	
	return 0;
}

uint8_t timer_close()
{
	TIM1_DeInit();
	return 0;
}

uint8_t timer_stop(){
	TIM1_Cmd(DISABLE);
	return 0;
}


uint8_t timer_start()
{	
	TIM1_Cmd(ENABLE);
	ticks_second = 0;
	enableInterrupts();
	return 0;
}

uint32_t timer_get_tick()
{
	return ticks_second;
}


