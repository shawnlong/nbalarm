/*speaker.c*/
#include "IOSTM8S103F3.h"
#include "stdint.h"
#include "speaker.h"
#include "gpio.h"
#include "timer.h"

static uint8_t playing;
uint8_t speaker_init()
{
	playing = 0;
	gpio_init(SPEAKER_PORT_PIN, SPEAKER_PORT_CHECK_MODE);
	return 0;
}

static uint8_t check_exist()
{
	gpio_init(SPEAKER_PORT_PIN, SPEAKER_PORT_CHECK_MODE);
	if(gpio_get_value(SPEAKER_PORT_PIN) == GPIO_VALUE_LOW)
	{
		return 0;
	}else{
		return 1;
	}
}

uint8_t speaker_close()
{
	PA_ODR &= ~MASK_PA_ODR_ODR3;
	gpio_close(SPEAKER_PORT_PIN);
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
PA_ODR |= MASK_PA_ODR_ODR3;	
  if(check_exist() == 0){
		playing = 0;
		return 1;
	}
	//1.open power for speaker
	//2.set timer and call back stop
	//todo:fix this
#if 0
    if(timer_start(SPEAKER_ALARM_TIME, stop) > 0)
    {
		PA_ODR |= MASK_PA_ODR_ODR3;
		playing = 1;
		return 0;
    }
#endif
    playing = 0;
	return 1;
}

uint8_t speaker_check_playing()
{
	return playing;
}


