

/*battery.h*/
#ifndef _BATTERY_H_
#define _BATTERY_H_
#define BATTERY_STATUS_OK		1
#define BATTERY_STATUS_LOWPOWER 2
#define BATTERY_STATUS_FAILURE	3

#define BATTERY_ADC_CHANNEL 	2

//battery voltage value guage
#define BATTERY_ADC_LOW_THRESHOLD 352 //2.3V

#define BATTARY_ADC_CHANNEL 	2
uint8_t battery_adc_init();
uint8_t battery_adc_close();

uint8_t battery_get_status();

#endif

