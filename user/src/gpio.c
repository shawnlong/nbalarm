
#include "iostm8s103F3.h"
#include "stdint.h"

#include "gpio.h"

volatile uint8_t sensor_interrupt = 0;
 

uint8_t gpio_init(uint8_t pin, uint8_t mode)
{
	switch(pin)
	{
		case GPIO_PIN_1:
			PA_DDR |= MASK_PA_DDR_DDR3;
			PA_CR1 |= MASK_PA_CR1_C13;
			PA_CR2 &= ~MASK_PA_CR2_C23;
			break;

		case GPIO_PIN_2:
			break;

		case GPIO_PIN_3:
			PC_DDR |= MASK_PC_DDR_DDR3;
			PC_CR1 |= MASK_PC_CR1_C13;
			PC_CR2 &= ~MASK_PC_CR2_C23;
			break;
#if 1
		case GPIO_PIN_4:
			PC_DDR &= ~MASK_PC_DDR_DDR5;
			PC_CR1 |= MASK_PC_CR1_C15;
			PC_CR2 |= MASK_PC_CR2_C25;

			//rise edge triiger
			EXTI_CR1 |= MASK_EXTI_CR1_PCIS;
			break;

		case GPIO_PIN_5:
			PC_DDR &= ~MASK_PC_DDR_DDR6;
			PC_CR1 |= MASK_PC_CR1_C16;
			PC_CR2 |= MASK_PC_CR2_C26;

			//rise edge triiger
			EXTI_CR1 |= MASK_EXTI_CR1_PCIS;
			break;

		case GPIO_PIN_6:
			PC_DDR &= ~MASK_PC_DDR_DDR7;
			PC_CR1 |= MASK_PC_CR1_C17;
			PC_CR2 |= MASK_PC_CR2_C27;

			//rise edge triiger
			EXTI_CR1 |= MASK_EXTI_CR1_PCIS;
			break;
#else
			case GPIO_PIN_4:
			PC_DDR &= ~MASK_PC_DDR_DDR5;
			PC_CR1 &= ~MASK_PC_CR1_C15;
			PC_CR2 &= ~MASK_PC_CR2_C25;

			//rise edge triiger
			//EXTI_CR1 |= MASK_EXTI_CR1_PCIS;
			break;

		case GPIO_PIN_5:
			PC_DDR &= ~MASK_PC_DDR_DDR6;
			PC_CR1 &= ~MASK_PC_CR1_C16;
			PC_CR2 &= ~MASK_PC_CR2_C26;

			//rise edge triiger
			//EXTI_CR1 |= MASK_EXTI_CR1_PCIS;
			break;

		case GPIO_PIN_6:
			PC_DDR &= ~MASK_PC_DDR_DDR7;
			PC_CR1 &= ~MASK_PC_CR1_C17;
			PC_CR2 &= ~MASK_PC_CR2_C27;

			//rise edge triiger
			//EXTI_CR1 |= MASK_EXTI_CR1_PCIS;
			break;

#endif
		case GPIO_PIN_7:
			PD_DDR |= MASK_PD_DDR_DDR2;
			PD_CR1 |= MASK_PD_CR1_C12;
			PD_CR2 &= ~MASK_PD_CR2_C22;
			break;

		case GPIO_PIN_8:
			PD_DDR |= MASK_PD_DDR_DDR4;
			PD_CR1 |= MASK_PD_CR1_C14;
			PD_CR2 &= ~MASK_PD_CR2_C24;
			break;

		default:
			break;
	}

	return 0;
}

uint8_t gpio_close_all(){
	PA_DDR = 0xFF;
	PA_CR1 = 0xFF;
	PA_CR2 = 0;
	PA_ODR = 0;

	PB_DDR = 0xFF;
	PB_CR1 = 0xFF;
	PB_CR2 = 0;
	PB_ODR = 0;

	PC_DDR = 0xFF;
	PC_CR1 = 0xFF;
	PC_CR2 = 0;
	PC_ODR = 0;

	PD_DDR = 0xFF;
	PD_CR1 = 0xFF;
	PD_CR2 = 0;
	PD_ODR = 0;
	return 0;
}


//enter low power mode
uint8_t gpio_close(uint8_t pin)
{
	switch(pin)
	{
		case GPIO_PIN_1:
			PA_DDR |= MASK_PA_DDR_DDR3;
			PA_CR1 |= MASK_PA_CR1_C13;
			PA_CR2 &= ~MASK_PA_CR2_C23;
			PA_ODR &= ~MASK_PA_CR2_C23;
			break;

		case GPIO_PIN_2:
			break;

		case GPIO_PIN_3:
			PC_DDR |= MASK_PC_DDR_DDR3;
			PC_CR1 |= MASK_PC_CR1_C13;
			PC_CR2 &= ~MASK_PC_CR2_C23;
			PC_ODR &= ~MASK_PC_CR2_C23;
			break;

		case GPIO_PIN_4:
		case GPIO_PIN_5:
		case GPIO_PIN_6:
			break;

		case GPIO_PIN_7:
			PD_DDR |= MASK_PD_DDR_DDR2;
			PD_CR1 |= MASK_PD_CR1_C12;
			PD_CR2 &= ~MASK_PD_CR2_C22;
			PD_ODR &= ~MASK_PD_CR2_C22;
			break;

		case GPIO_PIN_8:
			PD_DDR |= MASK_PD_DDR_DDR4;
			PD_CR1 |= MASK_PD_CR1_C14;
			PD_CR2 &= ~MASK_PD_CR2_C24;
			PD_ODR &= ~MASK_PD_CR2_C24;
			break;
			
		default:
			break;
	}

	return 0;
}


uint8_t gpio_get_value(uint8_t pin)
{
	uint16_t value;
	switch(pin)
	{
		case GPIO_PIN_4:
			value = (PC_IDR & MASK_PC_IDR_IDR5) == 0 ? GPIO_VALUE_LOW: GPIO_VALUE_HIGH;
			break;
		case GPIO_PIN_5:
			value = (PC_IDR & MASK_PC_IDR_IDR6) == 0 ? GPIO_VALUE_LOW: GPIO_VALUE_HIGH;
			break;
		case GPIO_PIN_6:
			value = (PC_IDR & MASK_PC_IDR_IDR7) == 0 ? GPIO_VALUE_LOW: GPIO_VALUE_HIGH;
			break;
		default:
			value = GPIO_VALUE_INVALID;
	}
	return value;
}


uint8_t gpio_set_value(uint8_t pin, uint8_t value)
{
	return 0;
}


#pragma vector					=0x07//PortC_vector


__interrupt void EXTI_PC() //PD外中断
{
	asm("nop");
	sensor_interrupt = 1;
	//PC_CR2 = 0x0; //关中断 	
}








