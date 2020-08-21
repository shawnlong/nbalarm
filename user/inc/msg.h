

/*msg.h*/
#ifndef _MSG_H_
#define _MSG_H_

#define SERVER_URL				"\"http://power.cadywell.com/\""
#define SERVER_PATH 			"\"/API/Device/Index\""
#define POST_HEADER				"\"Content-Type:application/json\\r\\n\""


#define MSG_TYPE_ALARM			1
#define MSG_TYPE_STATUS			2

#define MSG_DEV_STATUS_OK		1
#define MSG_DEV_STATUS_LOWPOWER 2
#define MSG_DEV_STATUS_FAIL		3
#define MSG_DEV_STATUS_NOAC		4


typedef struct sensor_status_t{
	uint8_t status;
	uint16_t open_count;
	uint16_t close_count;
} SENSOR_STATUS_T;

typedef struct MSG_tag
{
	uint32_t tm;
	uint8_t dev_id[DTU_IMEI_LEN + 1];
	uint8_t net_type;
	uint8_t signal;
	uint8_t status;
	SENSOR_STATUS_T sensors[SENSOR_NUMBER];
	uint32_t serial;
}MSG_T;
uint8_t msg_send(uint8_t dev_status, SENSOR_EVENT_T *pevent);

#endif




