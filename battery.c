

/*battery.c*/
#include "stdint.h"
#include "battery.h"
#include "adc.h"

uint8_t battery_adc_init()
{
	return adc_init(BATTERY_ADC_CHANNEL);
}


uint8_t battery_adc_close()
{
	return adc_close(BATTERY_ADC_CHANNEL);
}


uint8_t battery_get_status()
{
	uint8_t status;

	if(adc_get_value(BATTARY_ADC_CHANNEL) > BATTERY_ADC_LOW_THRESHOLD)
	{
		status = BATTERY_STATUS_OK;
	}
	else
	{
		status = BATTERY_STATUS_LOWPOWER;
	}

	return status;
}


