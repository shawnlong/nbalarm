
/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 ********************
 * 文件名  ：main.c
 * 描述    ：TIM1定时器的使用  
 * 实验平台：iCreate STM8开发板
 * 寄存器版本  ：V1.0.0
 * 作者    ：ling_guansheng  QQ：779814207
 * 博客    ：
 * 淘宝    ：http://shop71177993.taobao.com/
 *修改时间 ：2011-12-20
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "clk_conf.h"
#include "tim1.h"
#include "led.h"


/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Infinite loop */

  /*设置内部高速时钟16M为主时钟*/ 
  Clk_conf();
  Tim1_Time_Upmode_conf(16,1000,0);
  LED_conf();
  Set_Led_Off();
  EnableInterrupt;
  while(1)
  {
    /************标准的51编程风格*************************/
    PD0_out = 0;    /* PD0引脚输出0 */
    delay_ms(1000); 
    PD0_out = 1;    /* PD0引脚输出1 */
    delay_ms(1000); 
  /**************标准的51编程风格*************************/
  }
}






#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/
