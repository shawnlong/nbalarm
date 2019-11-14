

/*msg.c*/
#include "stm8s.h"
#include "string.h"
#include "stdio.h"
#include "msg.h"
#include "dtu.h"
#include "awu.h"


#define SN_SIZE 				15
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
uint8_t msg_send(uint8_t type, uint8_t dev_status, SENSOR_STATUS_T sensor_status[], uint8_t sensor_number)
{
	static uint8_t msg_serial = 0, sn_ok = 0;
	uint8_t network_type, signal_strength;
	static char tx_buffer[MSG_TX_BUFFER_SIZE];
	static char sn[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	//0.init dtu
	dtu_init();
	//1. wait for dtu online
	if(dtu_online() != 0)
	{
		return ERROR;
	}

	//2. check registration
	if(dtu_check_reg() != 0)
	{
		return ERROR;
	}

	//3.get sn, we use imei to be sn
	if(sn_ok != 1 && dtu_get_imei(sn) != 0)
	{
		return ERROR;
	}
	
	//4.get network type and signal strength
	if(dtu_get_ntype(&network_type) != 0)
	{
		return ERROR;
	}

	if(dtu_get_signal(&signal_strength) != 0)
	{
		return ERROR;
	}

	//5. pack massage
	if(type == MSG_TYPE_ALARM)
	{
		sprintf(tx_buffer, MSG_ALARM_FORMAT, sn, network_type, signal_strength, 
			(uint8_t) sensor_status[0].status, 
			sensor_status[0].open_count, sensor_status[0].close_count,
			(uint8_t) sensor_status[1].status, 
			sensor_status[1].open_count, sensor_status[1].close_count,
			(uint8_t) sensor_status[2].status, 
			sensor_status[2].open_count, sensor_status[2].close_count,
			msg_serial++);
	}
	else
	{
		sprintf(tx_buffer, MSG_STATUS_FORMAT, sn, network_type, signal_strength, dev_status, msg_serial++);
	}

	//6. send message
	if(dtu_httppost(SERVER_URL, POST_HEADER, tx_buffer, SERVER_PATH, 30) == 0)
	{
		dtu_close();
		sn_ok = 1;
		return SUCCESS;
	}
	else
	{
		dtu_close();
		return ERROR;
	}
}



