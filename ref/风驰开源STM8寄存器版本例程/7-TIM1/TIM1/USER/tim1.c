/******************** (C) COPYRIGHT  ���iCreateǶ��ʽ���������� ***************************
 * �ļ���  ��tim1.c
 * ����    ����ʱ��TM1���ú�����   
 * ʵ��ƽ̨��iCreate STM8������
 * �Ĵ����汾  ��V1.0.0
 * ����    ��ling_guansheng  QQ��779814207
 * ����    ��
 * �޸�ʱ�� ��2011-12-20



****************************************************************************************/

#include "tim1.h"

static  u32 TimingDelay; 
/**************************************************************************
 * ��������Tim1_Time_Upmode_conf
 * ����  ��TIM1��ʱ�����ܳ�ʼ��
 * ����  ��uint8_t TIM1_Prescaler 
           uint16_t TIM1_Period   1��65535
 *
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ���� 
 *************************************************************************/
void Tim1_Time_Upmode_conf(uint16_t TIM1_Prescaler,
                           uint16_t TIM1_Period,
                           uint8_t TIM1_RepetitionCounter)
{
      /* Set the Autoreload value */
    TIM1_ARRH = (uint8_t)(TIM1_Period >> 8);
    TIM1_ARRL = (uint8_t)(TIM1_Period);

    /* Set the Prescaler value */
    TIM1_PSCRH = (uint8_t)(TIM1_Prescaler >> 8);
    TIM1_PSCRL = (uint8_t)(TIM1_Prescaler);

    /* Select the Counter Mode */
    TIM1_CR1 = (uint8_t)((uint8_t)(TIM1_CR1 & (uint8_t)(~(MASK_TIM1_CR1_CMS  | MASK_TIM1_CR1_DIR)))
                           | (uint8_t)(TIM1_COUNTERMODE_UP));

    /* Set the Repetition Counter value */
    TIM1_RCR = TIM1_RepetitionCounter;
    /* Set the ARPE Bit */
    TIM1_CR1 |= MASK_TIM1_CR1_ARPE;
    /* Enable the Interrupt Upmode sources */
    TIM1_IER |= (uint8_t)TIM1_IT_UPDATE;
    /* set or Reset the CEN Bit */
    TIM1_CR1 |= MASK_TIM1_CR1_CEN;
}
/**************************************************************************
 * ��������TimingDelay_Decrement
 * ����  ��
 * ����  ����        
 *
 * ���  ����
 * ����  ���� 
 * ����  ���жϷ��������� 
 *************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
     { 
	TimingDelay--;
      }
}
/**************************************************************************
 * ��������delay_ms
 * ����  ��
 * ����  ��nTime       
 *
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ���� 
 *************************************************************************/
void delay_ms( u32 nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/******************* (C) COPYRIGHT ���iCreateǶ��ʽ���������� *****END OF FILE****/