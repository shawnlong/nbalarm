

/*adc.h*/
#ifndef _ADC_H_
#define _ADC_H_
uint8_t adc_init(uint8_t channel);
uint8_t adc_close(uint8_t channel);
uint16_t adc_get_value(uint8_t channel);

#endif

