

#include "stdint.h"
#include "string.h"
#include "dtu.h"
#include "uart.h"
#include "timer.h"
#include "awu.h"
#include "IOSTM8S103F3.h"
#include "gpio.h"
#include "msg.h"


static char rx_buffer[RX_BUFFER_SIZE];

static char * at_cmd(char * cmd, char * option)
{
	uint8_t i = 0;
	char ch;

	uart_send(cmd, strlen(cmd));
	uart_send(option, strlen(option));
	uart_send("\r\n", 2);

	//receive the response,strip '\r'and'\n'
	timer_start(2);

	while(timer_ot() == 0)
	{
		if((uart_getchar(&ch) != 0) || (ch == '\r') || (ch == '\n'))
		{
			continue;
		}

		rx_buffer[i++] = ch;

		if((i >= 2) && (rx_buffer[i - 2] == 'O') && (rx_buffer[i - 1] == 'K'))
		{
			return rx_buffer;
		}

		if(i >= RX_BUFFER_SIZE - 1) // || ()
		{
			return rx_buffer;
		}
	}

	rx_buffer[i] = '\0';

	return rx_buffer;
}




static uint8_t waitfor_response(uint8_t timeout_seconds, char *pattern)
{
	char ch;
	int8_t pos = -1;
	uint8_t len = strlen(pattern);
	timer_start(timeout_seconds);
	
	while(timer_ot() == 0)
	{
		if(uart_getchar(&ch) != 0)
			continue;
		if(ch == pattern[pos + 1])
		{
			pos++;
			if(pos == len - 1)
			{
				return ACK_OK;
			}
		}else{
			pos = -1;
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
	uint16_t i;
        //init the GPIO and power on 
	//NB power enable io output
	uart_init(1, 57600);
	gpio_init(DTU_POWER_PIN, GPIO_MODE_OUTPUT | GPIO_MODE_PP);
	gpio_init(DTU_RESET_PIN, GPIO_MODE_OUTPUT | GPIO_MODE_PP);
	//gpio_close(DTU_WAKEUP_PIN);
	PC_ODR &= ~MASK_PC_ODR_ODR3;
	for(i = 0; i < 1024; i ++);
	PC_ODR |= MASK_PC_ODR_ODR3;
	timer_init();
/*	at_cmd(AT_CMD_ATE, "1");
	at_cmd(AT_CMD_CPSMS, "");
	at_cmd(AT_CMD_NVSETPM, "1");
	at_cmd(AT_CMD_CSCLK, "2");
	at_cmd(AT_CMD_W, "");
	*/return 0;
}

/*
close power or sleep*/
uint8_t dtu_close()
{
	uint16_t i;
	PC_ODR &= ~MASK_PC_ODR_ODR3;
	//wait for a while to power down dtu
	for(i = 0; i < 10240; i ++);
	//awu_sleep(1);
	gpio_close(DTU_POWER_PIN);
	gpio_close(DTU_RESET_PIN);
	gpio_close(DTU_WAKEUP_PIN);
	timer_close();

	return 0;
}


uint8_t dtu_reset()
{
	PD_ODR |= MASK_PD_ODR_ODR2;
	awu_sleep(1);
	PD_ODR &= ~MASK_PD_ODR_ODR2;
	uart_init(1, 57600);
	return 0;
}

uint8_t dtu_online(){
	if(waitfor_response(60, DTU_ONLINE) != ACK_OK) 
	{
		return 1;
	}
	return 0;
}

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
    char *response;

	response = at_cmd(AT_CMD_CEREG, "?");
	
	if((response = strstr(response, "+CEREG:")) == 0)
	{
		return 1;
	}

	if(response[11] == '1' | response[11] == '5')
	{
		
//	at_cmd(AT_CMD_ATE, "0");
		return 0;
	}
	else
	{
		return response[11];
	}
}


/*
  function:dtu_httppost
		post to http server
  param:
		header: 	a string contains http header 
		content:	a string contains http body
  return:
		0 - success
		n - error code
*/
uint8_t dtu_httppost(char * url, char * header, char * content, char * path, uint16_t timeout)
{
	uint8_t response;
    uint16_t len = strlen(content);
	char *pos = content;
	char cut1,cut2;
	//at_cmd(AT_CMD_ATE, "1");

	at_cmd(AT_CMD_HTTPCREATE, url);
	at_cmd(AT_CMD_HTTPHEADER, header);
	do
	{
		if(len > DTU_CONTENT_MAX_SIZE)
		{
			cut1 = pos[DTU_CONTENT_MAX_SIZE-1];
			cut2 = pos[DTU_CONTENT_MAX_SIZE-2];
			pos[DTU_CONTENT_MAX_SIZE-2] = '"';
			pos[DTU_CONTENT_MAX_SIZE-1] = '\0';
			at_cmd(AT_CMD_HTTPCONTENT, pos);
			pos[DTU_CONTENT_MAX_SIZE-2] = cut2;
			pos[DTU_CONTENT_MAX_SIZE-1] = cut1;
			pos[DTU_CONTENT_MAX_SIZE-3] = '"';
			pos += DTU_CONTENT_MAX_SIZE - 3;
			len -= DTU_CONTENT_MAX_SIZE - 2;
		}else{
			at_cmd(AT_CMD_HTTPCONTENT, pos);
			break;
		}
	}while(len > 0);
	
	at_cmd(AT_CMD_HTTPSEND, path);
	response = waitfor_response(timeout, HTTP_ACK_OK);
	at_cmd(AT_CMD_HTTPCLOSE, "=0");

	return response;
}


uint8_t dtu_get_imei(char * imei)
{
	char * response;
	uint8_t i;

	response = at_cmd(AT_CMD_GSN, "");

	response = strstr(response, "86");

	if(response == 0)
	{
		return 1;
	}
	else
	{
		for(i = 0; i < DTU_IMEI_LEN; i++)
		{
			imei[i] = response[i];
		}
	}

	imei[DTU_IMEI_LEN] = '\0';

	return 0;
}


uint8_t dtu_get_ntype(uint8_t * network_type)
{
	char * response;

	response = at_cmd(AT_CMD_COPS, "?");

	if((response = strstr(response, "+COPS:")) == 0)
	{
		return 1;
	}
	else
	{
		response = strstr(response, ", ");

		if(response[2] == '9')
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

	response = at_cmd(AT_CMD_CSQ, "");

	if((response = strstr(response, "+CSQ:")) == 0)
	{
		return 1;
	}
	else
	{
		response = strstr(response, ",");
		*signal_strength = (response[-2] - '0') * 10 + (response[-1] - '0');
	}

	return 0;
}


