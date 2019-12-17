

/*msg.h*/
#ifndef _MSG_H_
#define _MSG_H_
#include "sensor.h"
#define MSG_TX_BUFFER_SIZE		300

//#define SERVER_URL				"\"http://test.answer2u.com/\""
#define SERVER_URL				"\"http://power.cadywell.com/\""
#define SERVER_PATH 			"\"/API/Device/Index\""

#define POST_HEADER				"\"Content-Type:application/json\\r\\n\""
//#define MSG_ALARM_FORMAT		"{\"msg\":{\"tm\":%s,\"type\":1,\"dev_id\":\"%s\",\"net_type\":%d,\"signal\":%d,\"sensor_msg\":[{\"sensor_id\":\"1\",\"sensor_status\":%d,\"open\":%d,\"close\":%d},{\"sensor_id\":\"2\",\"sensor_status\":%d,\"open\":%d,\"close\":%d},{\"sensor_id\":\"3\",\"sensor_status\":%d,\"open\":%d,\"close\":%d}]},\"msg_serial\":%d}"
//#define MSG_STATUS_FORMAT		"{\"msg\":{\"tm\":%s,\"type\":2,\"dev_id\":\"%s\",\"net_type\":%d,\"signal\":%d,\"dev_status\":%d},\"msg_serial\":%d}"

#define MSG_TYPE_ALARM			1 
#define MSG_TYPE_STATUS 		2
#define MSG_DEV_STATUS_OK		1
#define MSG_DEV_STATUS_LOWPOWER 2
#define MSG_DEV_STATUS_FAILURE	3

typedef struct MSG_tag
{
	uint32_t tm;
	uint8_t type;
	uint8_t *dev_id;
	uint8_t net_type;
	uint8_t signal;
	uint8_t status;
	SENSOR_STATUS_T sensors[SENSOR_NUMBER];
	uint32_t serial;
}MSG_T;

#define MSG_STACK_DEEPTH 5
uint8_t msg_send(MSG_T *msg);
uint8_t msg_push(MSG_T *msg);
MSG_T* msg_top();
MSG_T* msg_pop();
uint8_t msg_stack_full();
uint8_t msg_stack_size();

#endif




