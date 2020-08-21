

/*battery.c*/
#include "stm8s.h"
#include "battery.h"

uint8_t battery_init()
{
	ADC1->CR1 = 0x00;
	ADC1->CR2 = 0x00;
	return 0;
}


uint8_t battery_close()
{
	ADC1_DeInit();
	ADC1_Cmd(DISABLE);
	return 0;
}


uint8_t battery_get_status()
{
	uint8_t i;
        static uint16_t adc_value;
	uint8_t status = BATTERY_STATUS_OK;
	ADC1->CR1 = 0x00;
	ADC1->CR2 = 0x00;
	ADC1->CSR = ADC1_CHANNEL_2;
	ADC1->CR1 |= ADC1_CR1_ADON;
        for(i = 0; i < 100; i ++)
          ;
        ADC1->CR1 |= ADC1_CR1_ADON;
	
	while(!(ADC1->CSR & ADC1_CSR_EOC));

    adc_value = ADC1->DRL;
	adc_value += (((uint16_t)ADC1->DRH)<<2);
		if (adc_value > BATTERY_ADC_LOW_THRESHOLD)
	{
		status = BATTERY_STATUS_OK;
	}
	else 
	{
		status = BATTERY_STATUS_LOWPOWER;
	}

	return status;
}


