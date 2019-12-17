

/*msg.c*/
#include "stm8s.h"
#include "string.h"
#include "stdio.h"
#include "msg.h"
#include "dtu.h"
#include "awu.h"
#include "timer.h"


#define SN_SIZE 				15
static MSG_T g_msg_stack[MSG_STACK_DEEPTH];
static uint8_t g_top = 0;
static uint8_t g_count = 0;
static uint32_t msg_serial = 0;

/*
function: msg_send_alarm
		  send alarm message to server
		  rule: retry until success
param:
		  sn - the device serial number, unique
		  sensor_status - array that storing status of all the sensors
		  sensor_number - number of all sensors
return:
		  0 - success
		  other - error code
*/
uint8_t msg_send(MSG_T *msg)
{
	static uint8_t sn_ok = 0;
	 
	uint8_t network_type, signal_strength;
	//static char tx_buffer[MSG_TX_BUFFER_SIZE];
	static uint8_t  sn[DTU_IMEI_LEN+1] = {0};
	
	//1. wait for dtu online
	if(dtu_online() != 0)
	{
		//return ERROR;
	}
	timer_delay(1);
	//2. check registration
	if(dtu_check_reg() != 0)
	{
		return ERROR;
	}
	timer_delay(1);
	//3.get sn, we use imei to be sn
	if(sn_ok != 1 && dtu_get_imei(sn) != 0)
	{
		return ERROR;
	}else{
		msg->dev_id = sn;
	}
	timer_delay(1);
	//4.get network type and signal strength
	if(dtu_get_ntype(&network_type) != 0)
	{
		return ERROR;
	}else{
		msg->net_type = network_type;
	}
	timer_delay(1);
	if(dtu_get_signal(&signal_strength) != 0)
	{
		return ERROR;
	}else{
		msg->signal = signal_strength;
	}
	timer_delay(1);
	//5. send message
	if(dtu_httppost(SERVER_URL, POST_HEADER, msg, SERVER_PATH, 30) == 0)
	{
		sn_ok = 1;
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
}

uint8_t msg_push(MSG_T *msg)
{
	msg->serial = msg_serial++;
	memcpy(&(g_msg_stack[g_top]), msg, sizeof(MSG_T));
	g_top = (g_top + 1) % MSG_STACK_DEEPTH;
	g_count += (g_count < MSG_STACK_DEEPTH)?1:0;
	return SUCCESS;
}

MSG_T* msg_top()
{
	if(g_count > 0)
	{
		return &(g_msg_stack[(g_top == 0)?MSG_STACK_DEEPTH-1:g_top-1]);
	}else{
		return 0;
	}
}

MSG_T* msg_pop()
{
	uint8_t last_top = g_top;
	if(g_count > 0)
	{
		g_top = (g_top == 0)?MSG_STACK_DEEPTH -1:g_top-1;
		g_count --;
		return &(g_msg_stack[(last_top == 0)?MSG_STACK_DEEPTH-1:last_top-1]);
	}else{
		return 0;
	}
}

