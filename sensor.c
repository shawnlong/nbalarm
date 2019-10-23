

/*sensor.c*/

#include "stdint.h"

#include "sensor.h"
#include "gpio.h"

//each sensor status value
static SENSOR_STATUS_T sensor_statuses[SENSOR_NUMBER];
static uint8_t ID_TO_PORTS[SENSOR_NUMBER] =
	{
		SENSOR1_PORT_PIN, SENSOR2_PORT_PIN, SENSOR3_PORT_PIN
	};

uint8_t sensor_init()
{
	gpio_init(SENSOR1_PORT_PIN, SENSOR_PORT_MODE);
	gpio_init(SENSOR2_PORT_PIN, SENSOR_PORT_MODE);
	gpio_init(SENSOR3_PORT_PIN, SENSOR_PORT_MODE);
	uint8_t i;
	for(i = 0; i < SENSOR_NUMBER; i ++)
	{
		sensor_statuses[i].status = SENSOR_CLOSE;
		sensor_statuses[i].open_count = 0;
		sensor_statuses[i].close_count = 0;
	}
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
		value = gpio_get_value(ID_TO_PORTS[i]);
		new_status = (value != GPIO_VALUE_LOW) ? SENSOR_OPEN: SENSOR_CLOSE;
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
		new_status = (gpio_get_value(ID_TO_PORTS[i]) != GPIO_VALUE_LOW) ? SENSOR_OPEN: SENSOR_CLOSE;

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



