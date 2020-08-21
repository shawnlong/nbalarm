/*speaker.c*/
#include "stm8s.h"

#include "speaker.h"
#include "timer.h"

static uint8_t playing;
static uint32_t start_tick;
uint8_t speaker_init()
{
	playing = 0;
	ADC1->CR1 = 0x00;
	ADC1->CR2 = 0x00;
	return 0;
}

uint8_t speaker_check_exist()
{
#if SPEAKER_ENABLE

	uint16_t adc_value;

        uint16_t i;
        GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
        CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
         for(i = 0; i < 100; i ++)
          ;
        ADC1->CR1 = 0x00;
	ADC1->CR2 = 0x00;
	ADC1->CSR = ADC1_CHANNEL_4;
	ADC1->CR1 |= ADC1_CR1_ADON;
        for(i = 0; i < 100; i ++)
          ;
        ADC1->CR1 |= ADC1_CR1_ADON;

	
	while( !(ADC1->CSR & ADC1_CSR_EOC));

    adc_value = ADC1->DRL;
	adc_value += (((uint16_t)ADC1->DRH)<<2);
	
	if(adc_value > SPEAKER_CONNECT_MINV && adc_value < SPEAKER_CONNECT_MAXV)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

#else
	return FALSE;
#endif
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
function:   speaker_start
            triger the speaker to alram
            alarm 3 times, 15 seconds peroid
param:      none
return:     0 - success
            other - error code
*/
uint8_t speaker_start()
{
	if(speaker_check_exist() == FALSE)
	{
		return 1;
	}
	GPIO_Init(GPIOA, GPIO_PIN_3,GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
	start_tick = timer_get_tick();
	playing = 1;
	return 0;
}

uint8_t speaker_stop()
{
	GPIO_WriteLow(GPIOA, GPIO_PIN_3);
	playing = 0;
	return 0;
}

uint8_t speaker_check_playing()
{
	if(playing == 1 && (timer_get_tick() - start_tick < 45))
	{
		return TRUE;
	}else{
		return FALSE;
	}
}


