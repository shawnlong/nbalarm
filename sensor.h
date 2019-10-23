

/*sensor.h*/
#ifndef _SENSOR_H_
#define _SENSOR_H_
#define SENSOR_NUMBER			3
#define SENSOR_CLOSE			0
#define SENSOR_OPEN 			1
#define SENSOR_UNKNOWN			2

#define SENSOR1_PORT_PIN		GPIO_PIN_4
#define SENSOR2_PORT_PIN		GPIO_PIN_5
#define SENSOR3_PORT_PIN		GPIO_PIN_6
#define SENSOR_PORT_MODE 	GPIO_MODE_INPUT|GPIO_MODE_PULL_UP|GPIO_MODE_INT
//#define SENSOR_PORT_MODE 	GPIO_MODE_INPUT|GPIO_MODE_FLOAT|GPIO_MODE_INT

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
SENSOR_STATUS_T *sensor_get_status();
uint8_t sensor_get_change();

#endif

