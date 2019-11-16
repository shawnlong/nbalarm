

/*msg.h*/
#ifndef _MSG_H_
#define _MSG_H_
#include "sensor.h"
#define MSG_TX_BUFFER_SIZE		350
#define MSG_SEND_RETRY			3
#define _MSG_RELEASE_			1

#if _MSG_RELEASE_
#define SERVER_URL				"\"http://test.answer2u.com/\""
#define SERVER_PATH 			"\"/API/Device/Index\""

#else

#define SERVER_URL				"\"http://easyfacetech.com:9000/\""
#define SERVER_PATH 			"\"/test/jsontTest\""
#endif

#define POST_HEADER				"\"Content-Type:application/json\\r\\n\""
#define MSG_ALARM_FORMAT		"\"{\\\"msg\\\":{\\\"type\\\":1,\\\"dev_id\\\":\\\"%s\\\",\\\"net_type\\\":%d,\\\"signal\\\":%d,\\\"sensor_msg\\\":[{\\\"sensor_id\\\":\\\"1\\\",\\\"sensor_status\\\":%d,\\\"open\\\":%d,\\\"close\\\":%d},{\\\"sensor_id\\\":\\\"2\\\",\\\"sensor_status\\\":%d,\\\"open\\\":%d,\\\"close\\\":%d},{\\\"sensor_id\\\":\\\"3\\\",\\\"sensor_status\\\":%d,\\\"open\\\":%d,\\\"close\\\":%d}]},\\\"msg_serial\\\":%d}\""
#define MSG_STATUS_FORMAT		"\"{\\\"msg\\\":{\\\"type\\\":2,\\\"dev_id\\\":\\\"%s\\\",\\\"net_type\\\":%d,\\\"signal\\\":%d,\\\"dev_status\\\":%d},\\\"msg_serial\\\":%d}\""

#define MSG_TYPE_ALARM			1 
#define MSG_TYPE_STATUS 		2
#define MSG_DEV_STATUS_OK		1
#define MSG_DEV_STATUS_LOWPOWER 2
#define MSG_DEV_STATUS_FAILURE	3



uint8_t msg_send(uint8_t type, uint8_t dev_status, SENSOR_STATUS_T sensor_status[], uint8_t sensor_number);


#endif




