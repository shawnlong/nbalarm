

/*sensor.h*/
#ifndef _SENSOR_H_
#define _SENSOR_H_
#define SENSOR_NUMBER			3
#define SENSOR_CLOSE			0
#define SENSOR_OPEN 			1
#define SENSOR_UNKNOWN			2


//all sensors status overview
#define SENSORS_NEW_CLOSED		0
#define SENSORS_NEW_OPENED		1
#define SENSORS_UNCHANGED		2
#define SENSORS_JUST_ALL_CLOSED 3

typedef struct sensor_status_t{
	uint8_t status;
	uint16_t open_count;
	uint16_t close_count;
} SENSOR_STATUS_T;

uint8_t sensor_init();
uint8_t sensor_close();
uint8_t sensor_get_status(SENSOR_STATUS_T *status);
//uint8_t sensor_get_change();
uint8_t sensor_set_value(uint8_t value);


#endif

