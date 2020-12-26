

#include "stm8s.h"
#include "string.h"

#include "dtu.h"
#include "uart.h"
#include "timer.h"
#include "awu.h"




static char rx_buffer[RX_BUFFER_SIZE];


char * dtu_at_cmd(char * cmd, char * option)
{
	uint8_t i = 0;
	char ch;
	uint32_t tick;

	uart_send(cmd, strlen(cmd), UART_SEND_MODE_CHAR);

	if (strcmp(cmd, AT_CMD_HTTPCONTENT) == 0)
	{
		uart_send(option, strlen(option), UART_SEND_MODE_HEX);
		uart_send(",1", 2, UART_SEND_MODE_CHAR);
	}
	else 
	{
		uart_send(option, strlen(option), UART_SEND_MODE_CHAR);
	}

	uart_send("\r\n", 2, UART_SEND_MODE_CHAR);

	for (tick = timer_get_tick(); timer_get_tick() -tick < 3; )
	{
		if ((uart_getchar(&ch) != 0) || (ch == '\r') || (ch == '\n'))
		{
			continue;
		}

		rx_buffer[i++] = ch;

		if ((i >= 2) && (rx_buffer[i - 2] == 'O') && (rx_buffer[i - 1] == 'K'))
		{
			return rx_buffer;
		}

		if (i >= RX_BUFFER_SIZE - 1) // || ()
		{
			return rx_buffer;
		}
	}

	rx_buffer[i] = '\0';

	return rx_buffer;
}




uint8_t dtu_waitfor_response(uint8_t timeout_seconds, char * ok_pattern)
{
	char ch;
	int8_t ok_pos = -1;
	uint32_t tick = timer_get_tick();
	uint8_t ok_len = strlen(ok_pattern);

	while (timer_get_tick() -tick < timeout_seconds)
	{
		if (uart_getchar(&ch) != 0) continue;

		if (ch == ok_pattern[ok_pos + 1])
		{
			ok_pos++;

			if (ok_pos == ok_len - 1)
			{
				return ACK_OK;
			}
		}
		else 
		{
			ok_pos = -1;
		}
	}

	return ACK_FAIL;
}


/*
  function:dtu_init
		initialize the DTU(M5313)
  param:
		none
  return:
		0 - DTU works OK
		n - error code
*/
uint8_t dtu_init()
{
	static uint8_t configured = 0;
#if DEBUG_WO_DTU
	return 0;
#endif
	GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);
	GPIO_WriteHigh(GPIOC, GPIO_PIN_3);
	dtu_waitfor_response(9, "^CINIT: 32");

#if 1

	if (configured == 0)
	{
		uart_init(1, 57600);
		UART1_Cmd(ENABLE);
		dtu_at_cmd("at+IPR=", "9600");
		UART1_Cmd(DISABLE);
		uart_init(1, 9600);
		UART1_Cmd(ENABLE);
		timer_delay(2);
		dtu_at_cmd(AT_CMD_W, "");
		configured = 1;
	}
	else 
	{
		uart_init(1, 9600);
		UART1_Cmd(ENABLE);
	}

#else

	dtu_at_cmd("at+IPR=", "57600");
	dtu_at_cmd(AT_CMD_W, "");

#endif

	dtu_at_cmd(AT_CMD_ATE, "1");
	dtu_at_cmd(AT_CMD_CFGDUALMODE, "1");
	dtu_at_cmd(AT_CMD_CFGRATRRIO, "4");

	return 0;
}


/*
close power or sleep*/
uint8_t dtu_close(uint8_t state)
{
	UART1_DeInit();
	UART1_Cmd(DISABLE);

	GPIO_WriteLow(GPIOC, GPIO_PIN_3);

	//wait for a while to power down dtu
	timer_delay(1);

	return 0;
}


#if 0


uint8_t dtu_reset()
{
	GPIO_WriteHigh(GPIOD, GPIO_PIN_2);
	timer_delay(1);
	GPIO_WriteLow(GPIOD, GPIO_PIN_2);
	timer_delay(1);
	GPIO_WriteHigh(GPIOD, GPIO_PIN_2);
	return 0;
}


#endif


/*
	func:
		check the network registration status
	param:
		none
	return:
		0 - regist ok
		other - error code
*/
uint8_t dtu_check_reg()
{
	char * response;
	uint8_t i;
        dtu_at_cmd(AT_CMD_CCID,"");
	for (i = 0; i < DTU_CHECKREG_RETRY; i++)
	{
		response = dtu_at_cmd(AT_CMD_CEREG, "?");

 		if ((response = strstr(response, "+CEREG:")) != 0 && (response[11] == '1' | response[11] == '5'))
		{
			return 0;
		}
		timer_delay(1 * i);
	}

	return 1;
}





uint8_t dtu_get_imei(uint8_t * imei)
{
	char * response;
	uint8_t i;

	response = dtu_at_cmd(AT_CMD_GSN, "");

	response = strstr(response, "86");

	if (response == 0)
	{
		return 1;
	}
	else 
	{
		for (i = 0; i < DTU_IMEI_LEN; i++)
		{
			imei[i] = response[i];

			if (imei[i] < '0' || imei[i] > '9')
			{
				return 1;
			}
		}
	}

	imei[DTU_IMEI_LEN] = '\0';
        
	return 0;
}


uint8_t dtu_get_ntype(uint8_t * network_type)
{
	static char * response;

	response = dtu_at_cmd(AT_CMD_COPS, "?");

	if ((response = strstr(response, "+COPS:")) == 0)
	{
		return 1;
	}
	else 
	{
		response = strstr(response, ", ");

		if (response[2] == '9')
		{
			*network_type = DTU_NET_TYPE_NB;
		}
		else 
		{
			*network_type = DTU_NET_TYPE_GPRS;
		}
	}

	return 0;
}


uint8_t dtu_get_signal(uint8_t * signal_strength)
{
	char * response;

	response = dtu_at_cmd(AT_CMD_CSQ, "");

	if ((response = strstr(response, "+CSQ:")) == 0)
	{
		return 1;
	}
	else 
	{
		response = strstr(response, ",");

		if (response[-2] > '0' && response[-2] <= '9')
		{
			*signal_strength = (response[-2] - '0') * 10;
		}

		*signal_strength += (response[-1] - '0');
	}

	return 0;
}


uint8_t dtu_add_content(uint8_t * index, uint8_t * content)
{
	dtu_at_cmd(AT_CMD_HTTPCONTENT, (char *) index);

	if (content[0] != 0)
	{
		dtu_at_cmd(AT_CMD_HTTPCONTENT, (char *) content);
	}

	return 0;
}




