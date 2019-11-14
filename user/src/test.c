#include "stdint.h"
#include "iostm8s103F3.h"

#include "msg.h"
#include "dtu.h"
#include "test.h"
#include "awu.h"


void test_entry()
{
	//test_dtu_PSM();
}
#if 0



uint8_t flag = 0;

/*
void set_flag()
{
	flag = 1;
}


void test_timer()
{
	flag = 0;
	timer_start(2, set_flag);

	while(flag != 1)
		;

	return;
}


void test_pb4()
{
	bsp_power_ctrl(POWER_ON);
	bsp_sleep(2);
	bsp_power_ctrl(POWER_OFF);
	bsp_sleep(2);
}


void test_speaker()
{
	speaker_alarm();
	bsp_sleep(2);
	speaker_stop();
	bsp_sleep(2);
}



*/
	void test_power()
	{
		// dtu_power_off();
	//	bsp_awu_disable();
	//	bsp_init();
/*		
		 PA_DDR |= 0xFF;
		PA_ODR = 0;
		PB_DDR |= 0xFF;
		PB_ODR = 0;
		PC_DDR |= 0xFF;
		PC_ODR = 0;
		PD_DDR |= 0xFF;
		PD_ODR = 0;   
                CLK_ICKR |= 0X01;
		CLK_CKDIVR = 0x08;   
		while(! (CLK_ICKR & 0x02))
			;
	
		CLK_SWR = 0xE1;
		CLK_ICKR |= MASK_CLK_ICKR_REGAH | MASK_CLK_ICKR_FHW;
		FLASH_CR1 &= MASK_FLASH_CR1_HALT;
		TIM4_CR1 = 0;
		
		AWU_TBR &= ~MASK_AWU_TBR_AWUTB;
          
*/	         
                asm("sim");
		asm("halt");
		//http_waitfor_response(10);
	}

void test_ext_wakeup()
{
	uint8_t sensor_status;
	PA_CR1 = 0;
	PA_CR2 = 0;
	PB_CR1 = 0;
	PB_CR2 = 0;
	PC_CR1 = 0;
	PC_CR2 = 0;
	PD_CR1 = 0;
	PD_CR2 = 0;

	//pull up interrupt input
	PC_DDR &= ~MASK_PC_DDR_DDR5;
	PC_CR1 |= MASK_PC_CR1_C15;
	PC_CR2 |= MASK_PC_CR2_C25;

	PC_DDR &= ~MASK_PC_DDR_DDR6;
	PC_CR1 |= MASK_PC_CR1_C16;
	PC_CR2 |= MASK_PC_CR2_C26;

	PC_DDR &= ~MASK_PC_DDR_DDR7;
	PC_CR1 |= MASK_PC_CR1_C17;
	PC_CR2 |= MASK_PC_CR2_C27;

	//rise edge triiger
	EXTI_CR1 |= MASK_EXTI_CR1_PCIS;

	while(1)
	{
		asm("halt");
		sensor_status = sensor_get_status(0);
		sensor_status = sensor_get_status(1);
		sensor_status = sensor_get_status(2);
	}
}

void test_sensor(){
}

void test_signal()
{
	
	static uint8_t sensor_statuses[SENSOR_NUMBER] =
	{
		SENSOR_CLOSE, SENSOR_CLOSE, SENSOR_CLOSE
	};
	while(1)
	{
		msg_send(MSG_TYPE_ALARM, 0, sensor_statuses, 3);
		awu_sleep(10);
	}
}


void test_adc()
{
	adc_init(2);
	uint16_t value;
	while(1)
	{
		value = adc_get_value(2);
	}
}
#endif

