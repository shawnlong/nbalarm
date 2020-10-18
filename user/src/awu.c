
#include "stm8s.h"

#include "awu.h"
#include "sensor.h"

extern __IO uint8_t sensor_interrupt;
extern __IO uint8_t ac_interrupt;
extern __IO uint32_t ticks_quarter_second;

#if LSI_CALIBRATION
static uint32_t LSIMeasurment(void)
{
	uint32_t lsi_freq_hz = 0x0;
	uint32_t fmaster = 0x0;
	uint16_t ICValue1 = 0x0;
	uint16_t ICValue2 = 0x0;

	/* Get master frequency */
	fmaster = CLK_GetClockFreq();

	/* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 */
	AWU->CSR |= AWU_CSR_MSR;

	/* Measure the LSI frequency with TIMER Input Capture 1 */
	/* Capture only every 8 events!!! */
	/* Enable capture of TI1 */
	TIM1_ICInit(TIM1_CHANNEL_1, TIM1_ICPOLARITY_RISING, TIM1_ICSELECTION_DIRECTTI, 
		TIM1_ICPSC_DIV8, 0);

	/* Enable TIM1 */
	TIM1_Cmd(ENABLE);

	/* wait a capture on cc1 */
	while((TIM1->SR1 & TIM1_FLAG_CC1) != TIM1_FLAG_CC1)
		;

	/* Get CCR1 value*/
	ICValue1 = TIM1_GetCapture1();
	TIM1_ClearFlag(TIM1_FLAG_CC1);

	/* wait a capture on cc1 */
	while((TIM1->SR1 & TIM1_FLAG_CC1) != TIM1_FLAG_CC1)
		;

	/* Get CCR1 value*/
	ICValue2 = TIM1_GetCapture1();
	TIM1_ClearFlag(TIM1_FLAG_CC1);

	/* Disable IC1 input capture */
	TIM1->CCER1 &= (uint8_t) (~TIM1_CCER1_CC1E);
	/* Disable timer2 */
	TIM1_Cmd(DISABLE);

	/* Compute LSI clock frequency */
	lsi_freq_hz = (8 * fmaster) / (ICValue2 - ICValue1);

	/* Disable the LSI measurement: LSI clock disconnected from timer Input Capture 1 */
	AWU->CSR &= (uint8_t) (~AWU_CSR_MSR);

	return (lsi_freq_hz);
}

#endif

static void enter_lowpower()
{
	uint8_t i;
	CLK_Peripheral_TypeDef clock_to_close[] = 
		{
			CLK_PERIPHERAL_I2C, 
			CLK_PERIPHERAL_SPI, 
			CLK_PERIPHERAL_UART1, 
			CLK_PERIPHERAL_UART2, 
			CLK_PERIPHERAL_UART3, 
			CLK_PERIPHERAL_TIMER6, 
			CLK_PERIPHERAL_TIMER5, 
			CLK_PERIPHERAL_TIMER4, 
			CLK_PERIPHERAL_TIMER3, 
			CLK_PERIPHERAL_TIMER2, 
			CLK_PERIPHERAL_TIMER1, 
			CLK_PERIPHERAL_ADC, 
			CLK_PERIPHERAL_CAN, 
		};
	GPIO_TypeDef* port_to_flin[] =
	{
		GPIOA, GPIOA, GPIOA,  GPIOB,  GPIOC,  GPIOC,  GPIOD,  GPIOD,  GPIOD,  GPIOD,  GPIOD, GPIOD, 
	};
	GPIO_Pin_TypeDef pin_to_flin[] =
	{
		GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_5,GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3,GPIO_PIN_4, 
		GPIO_PIN_5,GPIO_PIN_6,};
	
	/*switch to low power mode here*/
	/*1.use LPVR*/
	CLK_DeInit();
	CLK_SlowActiveHaltWakeUpCmd(ENABLE);


	/*3.disable fast wakeup*/
	CLK_FastHaltWakeUpCmd(DISABLE);
	
	/*3.set clock source to LSI*/
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_LSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	
	/*4.close all pripherals but awu*/
#if 0
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_SPI, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART3, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER6, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER5, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_CAN, DISABLE);
	
#endif
	for(i = 0; i < 13; i ++)
	{
		CLK_PeripheralClockConfig(clock_to_close[i], DISABLE);
	}
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_AWU, ENABLE);
	/*5.set flash to slow mode*/
	FLASH_SetLowPowerMode(FLASH_LPMODE_POWERDOWN);

	/*6.set gpio pin to float input*/
#if 0
	GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOA, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
#endif
	for(i = 0; i < 12; i ++)
	{
		GPIO_Init(port_to_flin[i], pin_to_flin[i], GPIO_MODE_IN_FL_NO_IT);
	}
}

static void leave_lowpower()
{
	CLK_SlowActiveHaltWakeUpCmd(ENABLE);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
	TIM1_Cmd(ENABLE);
	FLASH_SetLowPowerMode(FLASH_LPMODE_POWERDOWN_STANDBY);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
}

uint8_t awu_init()
{
	AWU_Cmd(ENABLE);
#if LSI_CALIBRATION
	AWU_LSICalibrationConfig(LSIMeasurment());
#endif
	AWU_Init(AWU_TIMEBASE_1S);
	return 0;
}


uint8_t awu_sleep(uint32_t seconds)
{
	uint32_t i = 0;
	uint32_t j;
	AWU_Timebase_TypeDef awu_timebase; 
	static uint8_t ac_value, pre_ac_value = 0xFF;
	/*save power mode*/
	enter_lowpower();
	
	if(seconds >= 30)
	{
		awu_timebase = AWU_TIMEBASE_30S;
		AWU_Init(AWU_TIMEBASE_30S);
		seconds /= 30;
	}else{
		awu_timebase = AWU_TIMEBASE_1S;
		AWU_Init(AWU_TIMEBASE_1S);
	}
	sensor_interrupt = 0;
	ac_interrupt = 0;
	for(i = 0; i < seconds; i++)
	{
        if(sensor_interrupt != 0)		
        {
			break;
		}
		if(ac_interrupt != 0)
		{
			for(j = 0; j < 0xFFFFFFFF; j ++)
			{
				/*delay for a while*/
			}
			ac_value = GPIO_ReadInputData(GPIOB) & 0x10;
			if(ac_value != pre_ac_value)
			{
				pre_ac_value = ac_value;
				break;
			}
		}
		//WWDG_SetCounter(0x7F);
		halt();
		
		if(awu_timebase == AWU_TIMEBASE_30S)
		{
			ticks_quarter_second += 30*4;
		}else{
			ticks_quarter_second += 1*4;
		}		
	}

	sensor_interrupt = 0;
	ac_interrupt = 0;

	/*normal mode*/
	leave_lowpower();
	return 0;
}


