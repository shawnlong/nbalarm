#include "stm8s.h"
#include "uart.h"
#include "timer.h"
#include "stdio.h"

#if INTERRUPT_ENABLE
__IO uint8_t uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
__IO uint8_t uart1_rx_head = 0;
__IO uint8_t uart1_rx_tail = 0;
__IO uint8_t uart1_rx_count = 0;
#endif
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
#if INTERRUPT_ENABLE
	UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
#endif
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

#if INTERRUPT_ENABLE
uint8_t uart_getchar(char* data)
{
    uint8_t tmp = uart1_rx_tail; //avoid iar compiler warning
	if(uart1_rx_count == 0)
	{
		return 1;
	}
	else{
		*data = uart1_rx_buffer[uart1_rx_head];
		uart1_rx_head = (uart1_rx_head == (UART1_RX_BUFFER_SIZE - 1))?0:(uart1_rx_head + 1);
		uart1_rx_count--;
		return 0;
	 }
}
#else
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

#endif


