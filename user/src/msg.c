

/*msg.c*/
#include "stm8s.h"
#include "string.h"
#include "stdio.h"
#include "sensor.h"
#include "dtu.h"
#include "msg.h"
#include "awu.h"
#include "timer.h"
#include "version.h"

extern uint8_t g_ac_alarm;
static uint8_t * int2string(uint32_t n)
{
	static uint8_t string[11];
	uint32_t temp;
	uint8_t i;

	for (i = 0; i < 10; i++)
	{
		temp = n % 10;
		n = n / 10;
		string[9 - i] = temp + '0';
	}

	for (i = 0; i < 10; i++)
	{
		if (string[i] != '0')
		{
			return string + i;
		}
	}

	return & (string[9]);
}


/*
  function:httppost
		post to http server
  param:
		msg:		msg to send
		timeout:	wait server response timeout seconds
  return:
		0 - success
		n - error code
*/
static uint8_t httppost(MSG_T * msg, uint16_t timeout)
{
	uint8_t response;

	dtu_at_cmd(AT_CMD_HTTPCREATE, SERVER_URL);
	dtu_at_cmd(AT_CMD_HTTPHEADER, POST_HEADER);

	dtu_add_content("{\"msg\":{\"dev_type\":", HW_VERSION);
	dtu_add_content(",\"version\":", FW_VERSION);
	dtu_add_content(",\"tm\":", int2string(timer_get_tick() -msg->tm));
	dtu_add_content(",\"dev_id\":\"", msg->dev_id);
	dtu_add_content("\",\"net_type\":", int2string(msg->net_type));
	dtu_add_content(",\"signal\":", int2string(msg->signal));
	dtu_add_content(",\"dev_status\":", int2string(msg->status));
	if(g_ac_alarm == TRUE)
	{
		dtu_add_content(",\"power\":1", "");
	}
	else
	{
		dtu_add_content(",\"power\":0", "");
	}
	
	dtu_add_content(",\"sensor_msg\":[{\"sensor_id\":\"1\",\"sensor_status\":", int2string(msg->sensors[0].status));
	dtu_add_content(",\"open\":", int2string(msg->sensors[0].open_count));
	dtu_add_content(",\"close\":", int2string(msg->sensors[0].close_count));
	dtu_add_content("},{\"sensor_id\":\"2\",\"sensor_status\":", int2string(msg->sensors[1].status));
	dtu_add_content(",\"open\":", int2string(msg->sensors[1].open_count));
	dtu_add_content(",\"close\":", int2string(msg->sensors[1].close_count));
	dtu_add_content("},{\"sensor_id\":\"3\",\"sensor_status\":", int2string(msg->sensors[2].status));
	dtu_add_content(",\"open\":", int2string(msg->sensors[2].open_count));
	dtu_add_content(",\"close\":", int2string(msg->sensors[2].close_count));
	dtu_add_content("}]", "");

	dtu_add_content("},\"msg_serial\":", int2string(msg->serial));
	dtu_add_content("}", "");

	dtu_at_cmd("AT+HTTPCONTENT=0", "");
	dtu_at_cmd(AT_CMD_HTTPSEND, SERVER_PATH);
	response = dtu_waitfor_response(timeout, HTTP_ACK_OK);
	dtu_at_cmd(AT_CMD_ATE, "1");
	dtu_at_cmd(AT_CMD_HTTPCLOSE, "=0");

	return response;
}


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
uint8_t msg_send(uint8_t dev_status, SENSOR_EVENT_T * pevent)
{
	static MSG_T msg =
	{
		0
	};
	uint8_t i;
#if DEBUG_WO_DTU
	return SUCCESS;
#endif

	/*1. check registration*/
	if (dtu_check_reg() != 0)
	{
		return ERROR;
	}

	/*2.get sn, we use imei to be sn*/
	if (dtu_get_imei(msg.dev_id) != 0)
	{
		return ERROR;
	}

	/*3.get network type*/
	if (dtu_get_ntype(&msg.net_type) != 0)
	{
		return ERROR;
	}

	/*4. get signal strength*/
	if (dtu_get_signal(&msg.signal) != 0)
	{
		return ERROR;
	}

	/*5. update message content*/
	msg.tm = pevent->tm;
	msg.status = dev_status;
	for(i = 0; i < SENSOR_NUMBER; i ++)
	{
		msg.sensors[i].status = sensor_check_open(pevent->val, i);
 		msg.sensors[i].close_count = sensor_get_close_times(i);
		msg.sensors[i].open_count = sensor_get_open_times(i);
	}
	msg.serial++;
	
	/*6. send message*/
	if (httppost(&msg, 30) == 0)
	{
		return SUCCESS;
	}
	else 
	{
		return ERROR;
	}
}



