/*speaker.c*/
#include "stm8s.h"

#include "speaker.h"
#include "timer.h"

static uint8_t playing;
static uint32_t start_tick;
uint8_t speaker_init()
{
	playing = 0;
	return 0;
}

static uint8_t check_exist()
{
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
	if(GPIO_ReadInputPin(GPIOA, GPIO_PIN_3) != 0)
	{
		return 1;
	}else{
		return 0;
	}
}

uint8_t speaker_close()
{
	GPIO_Init(GPIOA, GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_WriteLow(GPIOA, GPIO_PIN_3);
	GPIO_Init(GPIOA, GPIO_PIN_3,GPIO_MODE_IN_FL_NO_IT);
	playing = 0;
	return 0;
}


/*
function:   speaker_alarm
            triger the speaker to alram
            alarm 3 times, 15 seconds peroid
param:      none
return:     0 - success
            other - error code
*/
uint8_t speaker_alarm()
{
	if(check_exist() == 0){
		playing = 0;
		return 1;
	}else{
		GPIO_Init(GPIOA, GPIO_PIN_3,GPIO_MODE_OUT_PP_HIGH_FAST);
		GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
		start_tick = timer_get_tick();
    	playing = 1;
		return 0;
	}
}

uint8_t speaker_check_playing()
{
	if(playing == 0)
	{
		return 0;
	}else{
		playing = (timer_get_tick() - start_tick > 45)?0:1;
		return playing;
	}
}


