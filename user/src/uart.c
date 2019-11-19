#if 1
#include "stm8s.h"
#include "uart.h"
#include "timer.h"
#include "stdio.h"
/*
  function:uart_init
		initialize the uart
  param:
		no - the uart number:1/2/3
  return:
		0 - success
		n - error code
*/
uint8_t uart_init(uint8_t id, uint16_t baudrate)
{
	UART1_Init((uint32_t)57600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
	//UART1_ITConfig(UART1_IT_TypeDef UART1_IT, FunctionalState NewState)
	return 0;
}


/*
  function:uart_send
		send data from uart
  param:
		data - data array to send
		len  - length of the data
  return:
		0 - success
		n - error code
*/
uint8_t uart_send(char * data, uint8_t len, UART_Send_Mode_Typedef mode)
{
	uint8_t i;
	char hex[2];
    uint32_t tick = timer_get_tick();

	for(i = 0; i < len; i++)
	{
		while((UART1_GetFlagStatus(UART1_FLAG_TXE)) == 0x00 && (timer_get_tick() - tick < 2))
			;
		if(mode == UART_SEND_MODE_CHAR)
		{
			UART1_SendData8(data[i]);
		}else{
			//sprintf(hex,"%x",data[i]);
			hex[0] = data[i]/16 + '0';
			hex[1] = data[i]%16 + '0';
			if(hex[1] > '9')
			{
				hex[1] += 'A' - ':';
			}
			UART1_SendData8(hex[0]);
			while((UART1_GetFlagStatus(UART1_FLAG_TXE)) == 0x00 && (timer_get_tick() - tick < 2))
				;
			UART1_SendData8(hex[1]);
		}
	}

	return 0;
}

/*
  function:uart_getchar
		get one char from uart
  param:
		data - the char to store received data
  return:
		0 - success
		1 - no char received
*/
uint8_t uart_getchar(char * data)
{
	uint8_t sr = UART1->SR;
	if(sr & UART1_FLAG_RXNE || sr & UART1_FLAG_OR){
		*data = UART1->DR;
		return 0;
	}
	else{
		return 1;
	}
}


#else

#include "iostm8s103F3.h"
#include "stdint.h"
#include "uart.h"

/*
  function:uart_init
		initialize the uart
  param:
		no - the uart number:1/2/3
  return:
		0 - success
		n - error code
*/
uint8_t uart_init(uint8_t id, uint16_t baudrate)
{
	if(id == UART_1)
	{
		UART1_CR1 = 0x00;
		UART1_CR2 = 0x00;
		UART1_CR3 = 0x00;

		// 设置波特率，必须注意以下几点：
		// (1) 必须先写BRR2
		// (2) BRR1存放的是分频系数的第11位到第4位，
		// (3) BRR2存放的是分频系数的第15位到第12位，和第3位
		// 到第0位
		// 例如对于波特率位9600时，分频系数=2000000/9600=208
		// 对应的十六进制数为00D0，BBR1=0D,BBR2=00
		UART1_BRR2 = 0x03;
		UART1_BRR1 = 0x02;

		UART1_CR2 = MASK_UART1_CR2_REN | MASK_UART1_CR2_TEN; //0x2c;//允许接收，发送，开接收中断
	}

	return 0;
}


/*
  function:uart_send
		send data from uart
  param:
		data - data array to send
		len  - length of the data
  return:
		0 - success
		n - error code
*/
uint8_t uart_send(char * data, uint8_t len)
{
	uint8_t i;

	for(i = 0; i < len; i++)
	{
		while((UART1_SR & 0x80) == 0x00)
			;

		UART1_DR = data[i];
	}

	return 0;
}

/*
  function:uart_getchar
		get one char from uart
  param:
		data - the char to store received data
  return:
		0 - success
		1 - no char received
*/
uint8_t uart_getchar(char * data)
{
	if(UART1_SR & 0x20){
		*data = UART1_DR;
		return 0;
	}
	else{
		return 1;
	}
}



#if 0

/***********************************************************
*	函数名称：int getStr(char *Buf,int count)
*
*	功能描述：从接收缓冲区获取一帧数据
*
*	参数列表：*Buf  存储收到的数据 数组
*			  count 要获取的数据长度
*
*	返回结果：返回获取到的数据长度
***********************************************************/
int getStr(unsigned char * Buf, int count)
{
	int i = 0;

	for(i = 0; i < count; i++)
	{
		Buf[i] = getchar();

	}

	Buf[i] = '\0';
	return i;
}


/*
  function:
		send at command to DTU and get responsse
  param:
		command:a string contains the at command 
  return:
		a string contains the repsonse from DTU
*/
unsigned char * DTU_AT(unsigned char * command, int responsesize)
{
	static unsigned char response[MAX_AT_RESPONSE];

	UART1_sendstring(command);
	getStr(response, responsesize);
	return response;
}


#endif



#endif


