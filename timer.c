#include "iostm8s103F3.h"
#include "stdint.h"

#include "timer.h"
static volatile int32_t timeout_ticks = 0;
static volatile int32_t time1out_ticks = 0;

uint8_t timer_init()
{
	//128分频 256计数，在16MHz下是2.048ms一次中断！
		TIM4_CR1 = 0x00;								//关闭计数器
	TIM4_IER = 0x01;								//更新中断使能
	TIM4_EGR = 0x01;
	TIM4_CNTR = 255;								//计数器值
	TIM4_ARR = 255; 								//自动重装的值
	TIM4_PSCR = 0x07;								//分频值
	TIM4_CR1 = 0x01;								//使能计数器
	return 0;
}

uint8_t timer_close()
{
	TIM4_CR1 = 0x00;	
	return 0;
}

uint8_t timer_stop(){
	timeout_ticks = 0;
    return 0;
}


uint8_t timer_start(uint16_t seconds)
{	
	//while(timeout_ticks > 0)
	//	;
	timeout_ticks = seconds*(uint16_t)55;
	TIM4_CR1 = 0x01;
	asm("rim");
	return 0;
}

uint8_t timer_ot()
{
	if(timeout_ticks > 0)
	{
		return 0;
	}else{
		return 1;
	}
}



#pragma vector					=TIM4_OVR_UIF_vector//0x19


__interrupt void TIM4_OVR_UIF_IRQHandler(void) //对应IAP的中断地址：0x8060
{
	TIM4_SR = 0x00;
	asm("sim");
	timeout_ticks --;
	
	asm("rim");
}


uint8_t timer1_init()
{

  TIM1_PSCRL=19;
 
  TIM1_IER=0X01;
 
  TIM1_ARRH=(unsigned char)(50000>>8);
  TIM1_ARRH=(unsigned char)50000;
 
  TIM1_CNTRH=(unsigned char)(50000>>8);
  TIM1_CNTRL=(unsigned char)50000;
	return 0;
}

uint8_t timer1_close()
{
	TIM1_CR1 = 0x00;	
	return 0;
}

uint8_t timer1_stop(){
	time1out_ticks = 0;
    return 0;
}


uint8_t timer1_start(uint16_t seconds)
{	
	//while(timeout_ticks > 0)
	//	;
	time1out_ticks = seconds;
	TIM1_CR1 |= 0x01;
	asm("rim");
	return 0;
}

uint8_t timer1_ot()
{
	if(time1out_ticks > 0)
	{
		return 0;
	}else{
		return 1;
	}
}



#pragma vector					=TIM1_OVR_UIF_vector//0x19


__interrupt void TIM1_OVR_UIF_IRQHandler(void) //对应IAP的中断地址：0x8060
{
	TIM1_SR1 &= 0XFE;
	asm("sim");
	time1out_ticks --;
	
	asm("rim");
}


