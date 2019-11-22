
/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 ********************
 * 文件名  ：main.c
 * 描述    ：流水灯    
 * 实验平台：iCreate STM8开发板
 * 寄存器版本  ：V1.0.0
 * 作者    ：ling_guansheng  QQ：779814207
 * 博客    ：
 * 淘宝    ：http://shop71177993.taobao.com/
 *修改时间 ：2012-6-12
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "clk_conf.h"
#include <iostm8s103f3.h>
#include "led.h"



/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int main(void)
{
  /* Infinite loop */

  /*设置内部高速时钟16M为主时钟*/ 
  Clk_conf();
  
  /* LED初始化 */
  LED_conf();
  
  /* LED全灭 */
  Set_Led_Off();
  
  while(1)
  {
    /* 添加你的代码  */
    /* LED流水灯 */
    LED_Display();
    
    /* 51风格实现LED灯亮灭 */
    //LED_Display_Bit();
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
