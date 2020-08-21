/******************** (C) COPYRIGHT  风驰iCreate嵌入式开发工作室 ***************************
 * 文件名  ：clk_conf.c
 * 描述    ：时钟配置函数库   
 * 实验平台：iCreate STM8开发板
 * 寄存器版本  ：V1.0.0
 * 作者    ：ling_guansheng  QQ：779814207
 * 博客    ：
 *修改时间 ：2011-12-20



****************************************************************************************/
#include "clk_conf.h"

void Clk_conf(void)
{
  CLK_CKDIVR&= (uint8_t)(~0x18);    /*时钟复位*/
  CLK_CKDIVR|= (uint8_t)0x00;       /*设置时钟为内部16M高速时钟*/
  
}

/******************* (C) COPYRIGHT 风驰iCreate嵌入式开发工作室 *****END OF FILE****/