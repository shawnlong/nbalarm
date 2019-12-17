

/*battery.c*/
#include "stm8s.h"
#include "battery.h"

uint8_t battery_adc_init()
{
	/*need config pin?*/
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
#if 0
	ADC1_DeInit();
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_2, ADC1_PRESSEL_FCPU_D2, \
            ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_LEFT, ADC1_SCHMITTTRIG_CHANNEL2,\
            DISABLE);
	ADC1_Cmd(ENABLE);
#endif
	return 0;
}


uint8_t battery_adc_close()
{
#if 1
	ADC1_DeInit();
	ADC1_Cmd(DISABLE);
#endif
	return 0;
}


uint8_t battery_get_status()
{
	uint8_t status = BATTERY_STATUS_OK;
	uint16_t adc_value = 400;
#if 1
	ADC1_StartConversion();
	adc_value = ADC1_GetConversionValue();
#endif

	if(adc_value > BATTERY_ADC_LOW_THRESHOLD)
	{
		status = BATTERY_STATUS_OK;
	}
	else
	{
		status = BATTERY_STATUS_LOWPOWER;
	}

	return status;
}


