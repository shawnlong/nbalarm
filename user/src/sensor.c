

/*sensor.c*/
#include "string.h"
#include "stm8s.h"

#include "sensor.h"

extern uint8_t last_port_value;
//each sensor status value
static  SENSOR_STATUS_T sensor_statuses[SENSOR_NUMBER] = {
	{SENSOR_CLOSE, 0 ,0},
	{SENSOR_CLOSE, 0 ,0},
	{SENSOR_CLOSE, 0 ,0}
};
		
static GPIO_Pin_TypeDef ID_TO_PIN[SENSOR_NUMBER] = 
{
	GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7
};

uint8_t sensor_init()
{
#if 1
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_IT);
	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_IT);
	GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_FL_IT);
#else
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_IT);
	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_IT);
	GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_IT);
#endif
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_FALL);
	return 0;
}

uint8_t sensor_close()
{
	//never close, because we need interrupt wakeup
	return 0;
}


/*
function: sensor_get_status
		  get the open|close status of the sensor
params: 
		  id - the id of the sensor to get value
return:
		  SENSOR_STATUS
*/
uint8_t sensor_get_status(SENSOR_STATUS_T *status)
{
	uint8_t i, value, new_status, opened, new_opened, new_closed;
	value = GPIO_ReadInputData(GPIOC);
	last_port_value = value;
	for(i = 0, opened = 0, new_opened = 0, new_closed = 0; i < SENSOR_NUMBER; i++)
	{
		new_status = ((value & ID_TO_PIN[i]) != 0) ? SENSOR_OPEN: SENSOR_CLOSE;
		if(new_status == SENSOR_OPEN) 
		{
			if(sensor_statuses[i].status == SENSOR_CLOSE)
			{
				sensor_statuses[i].open_count ++;
				new_opened ++;
			}
			opened ++;
		}
		if(new_status == SENSOR_CLOSE)
		{
			if(sensor_statuses[i].status == SENSOR_OPEN)
			{
				sensor_statuses[i].close_count ++;
				new_closed ++;
			}
		}
		sensor_statuses[i].status = new_status;
	}
	memcpy(status,  sensor_statuses, sizeof(SENSOR_STATUS_T) * SENSOR_NUMBER);
	if(opened == 0 && new_closed != 0)
	{
		return SENSORS_JUST_ALL_CLOSED;
	}

	if(new_opened != 0)
	{
		return SENSORS_NEW_OPENED;
	}

	if(new_closed != 0)
	{
		return SENSORS_NEW_CLOSED;
	}

	return SENSORS_UNCHANGED;
}


