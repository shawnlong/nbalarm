

/*sensor.c*/

#include "stm8s.h"

#include "sensor.h"
#include "gpio.h"

static uint8_t port_value;
static uint8_t port_value_valid;
//each sensor status value
static  SENSOR_STATUS_T sensor_statuses[SENSOR_NUMBER] = {
	{SENSOR_CLOSE, 0 ,0},
	{SENSOR_CLOSE, 0 ,0},
	{SENSOR_CLOSE, 0 ,0}
};
static GPIO_TypeDef* ID_TO_PORT[SENSOR_NUMBER] =
{
	GPIOC, GPIOC, GPIOC
};
		
static GPIO_Pin_TypeDef ID_TO_PIN[SENSOR_NUMBER] = 
{
	GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7
};

uint8_t sensor_set_value(uint8_t value){
	port_value_valid = 1;
	port_value = value;
	return 0;
}


static uint8_t sensor_get_value()
{
	if(port_value_valid != 0)
	{
		port_value_valid = 0;
		return port_value;
	}else{
		return GPIO_ReadInputData(GPIOC);
	}
	
}
uint8_t sensor_init()
{
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_IT);
	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_IT);
	GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_FL_IT);
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
SENSOR_STATUS_T *sensor_get_status()
{
	uint8_t i, value, new_status;
	for(i = 0; i < SENSOR_NUMBER; i++)
	{
		value = sensor_get_value() & ID_TO_PIN[i];
		new_status = (value != 0) ? SENSOR_OPEN: SENSOR_CLOSE;
		if(new_status == SENSOR_OPEN && sensor_statuses[i].status == SENSOR_CLOSE)
		{
			sensor_statuses[i].open_count ++;
		}
		if(new_status == SENSOR_CLOSE && sensor_statuses[i].status == SENSOR_OPEN)
		{
			sensor_statuses[i].close_count ++;
		}
		sensor_statuses[i].status = new_status;
	}
	return sensor_statuses;
}

uint8_t sensor_get_change()
{
	uint8_t i, opened, new_opened, new_closed, new_status;
	
	for(i = 0, opened = 0, new_opened = 0, new_closed = 0; i < SENSOR_NUMBER; i++)
	{
		new_status = ((sensor_get_value() & ID_TO_PIN[i]) != 0) ? SENSOR_OPEN: SENSOR_CLOSE;

  		if(new_status == SENSOR_OPEN)
  		{
  			if(sensor_statuses[i].status == SENSOR_CLOSE)
			{
				new_opened++;
			}
			opened++;
		}
		else
		{ // status == SENSOR_CLOSE

			if(sensor_statuses[i].status == SENSOR_OPEN)
			{
				new_closed++;
			}
		}
	}

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




