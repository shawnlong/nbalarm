/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 ***************************
 * 文件名  ：tim1.c
 * 描述    ：定时器TM1配置函数库   
 * 实验平台：iCreate STM8开发板
 * 寄存器版本  ：V1.0.0
 * 作者    ：ling_guansheng  QQ：779814207
 * 博客    ：
 * 修改时间 ：2011-12-20



****************************************************************************************/

#include "tim1.h"

static  u32 TimingDelay; 
/**************************************************************************
 * 函数名：Tim1_Time_Upmode_conf
 * 描述  ：TIM1定时器功能初始化
 * 输入  ：uint8_t TIM1_Prescaler 
           uint16_t TIM1_Period   1～65535
 *
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用 
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
 * 函数名：TimingDelay_Decrement
 * 描述  ：
 * 输入  ：无        
 *
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：中断服务函数调用 
 *************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
     { 
	TimingDelay--;
      }
}
/**************************************************************************
 * 函数名：delay_ms
 * 描述  ：
 * 输入  ：nTime       
 *
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用 
 *************************************************************************/
void delay_ms( u32 nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/