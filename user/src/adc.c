
#include "iostm8s103F3.h"
#include "stdint.h"
#include "adc.h"



uint8_t adc_init(uint8_t channel)
{
	ADC_CR1 |= MASK_ADC_CR1_ADON;
	ADC_CSR = 0x02;				// AIN2/PD2
	ADC_CR2 = 0;				// align left
	ADC_TDRL = 0x01;
	return 0;
}



uint16_t adc_get_value(uint8_t channel)
{
	int i = 0;
	uint16_t value = 0;
        uint8_t csr = 0;

	ADC_CR1 |= 0x01;

	for(i = 0; i < 1000; i++)
		; // 延时一段时间，至少7uS，保证ADC 模块上电王城

	ADC_CR1 |= 0x01;								// 再次将CR1寄存器的最低位置1,开启AD转换

        
	while((csr & 0x80) != 0x80)
        {
          csr = ADC_CSR;
        }
		//; // 等待转换结束

	value = (ADC_DRH << 2);
	value += ADC_DRL;
	ADC_CSR &= (~0x80);
	return value;
}


uint8_t adc_close(uint8_t channel)
{
	//todo:fix this, close adc module
	ADC_CSR &= (~0x80);
	return 0;
}


