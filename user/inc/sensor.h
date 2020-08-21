

/*sensor.h*/
#ifndef _SENSOR_H_
#define _SENSOR_H_
#define SENSOR_NUMBER			3
#define SENSOR_CLOSE			0
#define SENSOR_OPEN 			1
#define SENSOR_UNKNOWN			2

#define SENSOR_STACK_DEPTH		10
typedef struct SENSOR_EVENT_t{
	uint32_t tm;
	uint8_t val;
	uint8_t pre_val;
	uint8_t state;
}SENSOR_EVENT_T;

#define SENSOR_GPIO_PORT GPIOC

uint8_t sensor_init();
uint8_t sensor_close();
uint8_t sensor_stack_empty();
void sensor_push_raw();
void sensor_push_back(SENSOR_EVENT_T *pevent);
uint8_t sensor_pop(SENSOR_EVENT_T *pevent);
uint8_t sensor_check_open(uint8_t value, uint8_t id);
uint8_t sensor_get_open_times(uint8_t id);
uint8_t sensor_get_close_times(uint8_t id);

#endif

