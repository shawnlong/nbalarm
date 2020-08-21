
/******************** (C) COPYRIGHT  ���iCreateǶ��ʽ���������� ********************
 * �ļ���  ��main.c
 * ����    ��ADת��ʵ��   
 * �������󣺱�����ͨ����ʼ��ADC��AIN10ͨ��������������ADCֵͨ�����ڽ��д�ӡ
 * ʵ��ƽ̨��iCreate STM8������
 * �Ĵ����汾  ��V1.0.0
 * ����    ��ling_guansheng  QQ��779814207
 * ����    ��
 * �Ա�    ��http://shop71177993.taobao.com/
 * �޸�ʱ�� ��2012-6-12
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "clk_conf.h"
#include "uart.h"
#include "adc.h"

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void delay(u32 nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }

}


int main(void)
{
  /* Infinite loop */

  /*�����ڲ�����ʱ��16MΪ��ʱ��*/ 
  Clk_conf();
  
  /* ���ڳ�ʼ�� */
  uart_conf();
  
  /* ADCģ���ʼ�� */
  ADC_conf();
  printf("\r\nӲ��ƽ̨Ϊ:%s\r\n","iCreate STM8 ������");
  while(1)
  {
      printf("ADCת�����Ϊ:%d\r\n",ADC_GetConversionValue());
      delay(0xffff);
      delay(0xffff);
      delay(0xffff);      
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

/******************* (C) COPYRIGHT ���iCreateǶ��ʽ���������� *****END OF FILE****/
