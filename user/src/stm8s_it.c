
/**
  ******************************************************************************
  * @file	  stm8s_it.c
  * @author   MCD Application Team
  * @version  V2.0.4
  * @date	  26-April-2018
  * @brief	  Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  * 	   http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm8s_it.h"
#include "sensor.h"
#include "uart.h"
//#include "stm8s_eval.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//extern uint16_t Conversion_Value;
extern __IO uint32_t ticks_quarter_second;
extern __IO uint8_t live_flag;

#if INTERRUPT_ENABLE
extern __IO uint8_t uart1_rx_buffer[];
extern __IO uint8_t uart1_rx_head;
extern __IO uint8_t uart1_rx_tail;
extern __IO uint8_t uart1_rx_count;
#endif 
__IO uint8_t sensor_interrupt = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/** @addtogroup IT_Functions
  * @{
  */
#ifdef _COSMIC_
/**
  * @brief	Dummy interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*_COSMIC_*/

/**
  * @brief	TRAP interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	Top Level Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	Auto Wake Up Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
	AWU_GetFlagStatus();
}


/**
  * @brief	Clock Controller Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	External Interrupt PORTA Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	External Interrupt PORTB Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	External Interrupt PORTC Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
	sensor_interrupt ++;
}


/**
  * @brief	External Interrupt PORTD Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	External Interrupt PORTE Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#ifdef STM8S903
/**
  * @brief	External Interrupt PORTF Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*STM8S903*/

#if defined 					(STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	CAN TX Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*STM8S208 || STM8AF52Ax */

/**
  * @brief	SPI Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	Timer1 Update/Overflow/Trigger/Break Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
	static uint8_t live_ticks = 0;

	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
	ticks_quarter_second++;

	if(live_ticks++ > 240)
	{
		live_ticks = 0;
		if(live_flag != 0xFE)
		{
			return;
		}
		else
		{
			live_flag = 0;
		}
	}

	//WWDG_SetCounter(0x7F);

}


/**
  * @brief	Timer1 Capture/Compare Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#ifdef STM8S903
/**
  * @brief	Timer5 Update/Overflow/Break/Trigger Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	Timer5 Capture/Compare Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8S001 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

/**
  * @brief	Timer2 Update/Overflow/Break Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	Timer2 Capture/Compare Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*STM8S903*/

#if defined(STM8S208)			|| defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
		defined(STM8S005) || defined(STM8AF62Ax) || defined(STM8AF52Ax) || defined(STM8AF626x)

/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	Timer3 Capture/Compare Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*STM8S208, STM8S207 or STM8S105 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

#if defined(STM8S208)			|| defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
		defined(STM8S003) || defined(STM8S001) || defined(STM8AF62Ax) || defined(STM8AF52Ax) || defined(STM8S903)

/**
  * @brief	UART1 TX Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	UART1 RX Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
#if INTERRUPT_ENABLE
	uint8_t u8RxByte;
	/* Get the character from UART Buffer */
	if(UART1->SR & UART1_SR_OR)
	{
		UART1->SR &= ~UART1_SR_OR;
	}
		
		u8RxByte = UART1_ReceiveData8();

		/* Check if buffer full */
		if (uart1_rx_count < UART1_RX_BUFFER_SIZE)
		{
			/* Enqueue the character */
			uart1_rx_buffer[uart1_rx_tail] = u8RxByte;
			uart1_rx_tail = (uart1_rx_tail == (UART1_RX_BUFFER_SIZE -1))?0:(uart1_rx_tail+1);
			uart1_rx_count++;
		}
		UART1_ClearITPendingBit(UART1_IT_RXNE);

#endif
}


#endif /*STM8S208 or STM8S207 or STM8S103 or STM8S001 or STM8S903 or STM8AF62Ax or STM8AF52Ax */

/**
  * @brief	I2C Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#if defined(STM8S105)			|| defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief	UART2 TX interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	UART2 RX interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /* STM8S105 or STM8AF626x */

#if defined(STM8S207)			|| defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief	UART3 TX interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @brief	UART3 RX interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#if defined(STM8S207)			|| defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief	ADC2 interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
	/* Get converted value */
	Conversion_Value = ADC2_GetConversionValue();

	if(Conversion_Value == 0x0)
	{
		/* Turn off LED1..4*/
		STM_EVAL_LEDOff(LED1);
		STM_EVAL_LEDOff(LED2);
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOff(LED4);
	}
	else if((Conversion_Value <= 0xFF) && (Conversion_Value > 0x0))
	{
		/* Turn on LED1 */
		STM_EVAL_LEDOn(LED1);

		/* Turn off LED2..4 */
		STM_EVAL_LEDOff(LED2);
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOff(LED4);
	}
	else if((Conversion_Value >= 0x100) && (Conversion_Value <= 0x1FF))
	{
		/* Turn on LED1 and LED2 */
		STM_EVAL_LEDOn(LED1);
		STM_EVAL_LEDOn(LED2);

		/* Turn off LED3 and LED4 */
		STM_EVAL_LEDOff(LED3);
		STM_EVAL_LEDOff(LED4);
	}
	else if((Conversion_Value >= 0x200) && (Conversion_Value <= 0x2FF))
	{
		/* Turn on LED1..3 */
		STM_EVAL_LEDOn(LED1);
		STM_EVAL_LEDOn(LED2);
		STM_EVAL_LEDOn(LED3);

		/* Turn off LED4 */
		STM_EVAL_LEDOff(LED4);
	}
	else
	{
		/* Turn on LED1..4*/
		STM_EVAL_LEDOn(LED1);
		STM_EVAL_LEDOn(LED2);
		STM_EVAL_LEDOn(LED3);
		STM_EVAL_LEDOn(LED4);
	}

	ADC2_ClearITPendingBit();
}


#else /*STM8S105, STM8S103 or STM8S903 or STM8AF626x */

/**
  * @brief	ADC1 interrupt routine.
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*STM8S208 or STM8S207 or STM8AF52Ax or STM8AF62Ax */

#ifdef STM8S903
/**
  * @brief	Timer6 Update/Overflow/Trigger Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#else /*STM8S208, STM8S207, STM8S105 or STM8S103 or STM8S001 or STM8AF62Ax or STM8AF52Ax or STM8AF626x */

/**
  * @brief	Timer4 Update/Overflow Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


#endif /*STM8S903*/

/**
  * @brief	Eeprom EEC Interrupt routine
  * @param	None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
	/* In order to detect unexpected events during development,
	   it is recommended to set a breakpoint on the following instruction.
	*/
}


/**
  * @}
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/