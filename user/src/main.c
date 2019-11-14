/*******************************************************************************
file: main.c
Company: Leadien
Project: BN-ALARM device
Platforam:STM8+M5313
Function: Monitoring 3 switch-type sensors and report state change to server,
		  by M5313 from NB-IOT/GPRS network. And control a speaker alarm.
*******************************************************************************/
//#include "stdint.h"
#include "stm8s.h"
#include "sensor.h"
#include "msg.h"
#include "battery.h"
#include "speaker.h"
#include "awu.h"
#include "test.h"
#include "dtu.h"
#include "gpio.h"
#include "stm8s_clk.h"
#include "timer.h"


/*Action rules:
1.Snesor opened, send alarm message immediately
2.if send succeed, waiting for sensor close, and send alarm messge after 1m、10m、1hour、per 24hours
3.once all sensor closed, send alarm message then enter inspection state, send status message per 24 hours
4.once send failed, try again after 10s, 10s, 10s, 1m, per 24hours
*/
#define STATE_FACTORY_TEST		0
#define STATE_INSPECTION		1
#define STATE_ALARM_1			2
#define STATE_ALARM_2			3
#define STATE_ALARM_3			4
#define STATE_ALARM_4			5
#define STATE_ALARM_PER_DAY 	6
#define STATE_RETRY_1			7
#define STATE_RETRY_2			8
#define STATE_RETRY_3			9
#define STATE_RETRY_4			10
#define STATE_RETRY_5			11
#define STATE_RETRY_PER_DAY 	12
#define STATE_NUMBER			13

#define FACTORY_TEST_COUNT		5
#define FACTORY_TEST_PERIOD		AWU_ONE_SECONDS

static uint8_t NEXT_STATE[STATE_NUMBER] =
{
	STATE_FACTORY_TEST,
	STATE_INSPECTION, 
	STATE_ALARM_2, 
	STATE_ALARM_3, 
	STATE_ALARM_4, 
	STATE_ALARM_PER_DAY, 
	STATE_ALARM_PER_DAY, 
	STATE_RETRY_2, 
	STATE_RETRY_3, 
	STATE_RETRY_4, 
	STATE_RETRY_5, 
	STATE_RETRY_PER_DAY, 
	STATE_RETRY_PER_DAY
};


static uint32_t SLEEP_PERIODS[STATE_NUMBER] =
{
	FACTORY_TEST_PERIOD,
	AWU_ONE_DAY, 
	AWU_ONE_MINUTE, 
	AWU_ONE_MINUTE * 10, 
	AWU_ONE_HOUR, 
	AWU_ONE_DAY, 
	AWU_ONE_DAY, 
	AWU_TEN_SECONDS, 
	AWU_TEN_SECONDS, 
	AWU_TEN_SECONDS, 
	AWU_ONE_MINUTE, 
	AWU_ONE_DAY, 
	AWU_ONE_DAY
};


main()
{
	uint8_t sensor_change = SENSORS_UNCHANGED;
	uint8_t state = STATE_FACTORY_TEST;
	uint8_t state_before_retry = STATE_INSPECTION;
	SENSOR_STATUS_T * sensor_status;
	SENSOR_STATUS_T retry_sensor_status[SENSOR_NUMBER];
	uint8_t i, factory_test_times = 0;
	

	while(1)
	{
		//0.initilize board
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);/*16M/8=2Mhz clock*/
		awu_init();
    	sensor_init();
		timer_init();
		timer_start();
		//1.check sensors
		if(state >= STATE_RETRY_1)
		{
			sensor_change = SENSORS_UNCHANGED;
			sensor_status = retry_sensor_status;
		}else{
			sensor_change = sensor_get_change();
			sensor_status = sensor_get_status();
		}
		//2.if new sensor open, alarm speaker
		if(sensor_change == SENSORS_NEW_OPENED)
		{
			speaker_init();
			speaker_alarm();						
		}

		//3.update state
		if(sensor_change == SENSORS_UNCHANGED)
		{
			state = NEXT_STATE[state];
		}
		else
		{
			state = STATE_ALARM_1;
		}

		//4.send message to server
		//4.1 if in inspection action, report device health status
		if((state == STATE_INSPECTION))
		{
			battery_adc_init();
			msg_send(MSG_TYPE_STATUS, battery_get_status(), 0, 0);
			battery_adc_close();
		}
		else
		{
			//4.2 if not inspection state, send alarm message
			if(sensor_change == SENSORS_JUST_ALL_CLOSED)
			{
				state = STATE_INSPECTION;
			}

			if(msg_send(MSG_TYPE_ALARM, 0, sensor_status, SENSOR_NUMBER) != SUCCESS)
			{
				//retry action
				if(state < STATE_RETRY_1)
				{
					for(i = 0; i < SENSOR_NUMBER; i++)
					{
						retry_sensor_status[i].status = sensor_status[i].status;
						retry_sensor_status[i].open_count = sensor_status[i].open_count;
						retry_sensor_status[i].close_count = sensor_status[i].close_count;
					}
					state_before_retry = state;
					state = STATE_RETRY_1;
				}
			}
			else
			{
				//secceed, exit retry
				if(state >= STATE_RETRY_1)
				{
					state = state_before_retry;
				}

			}
		}

		//5.close unused peripheral and modules to save power
		while(speaker_check_playing() == 1)
			;

		speaker_close();

		//gpio_close_all();

		//6.sleep 
		//check sensor change before sleep
		if(sensor_get_change() == SENSORS_UNCHANGED)
		{
			awu_sleep(SLEEP_PERIODS[state]);
		}else{
		//else restart cycle to alarm new
			awu_sleep(1);
        }
		if(state == STATE_FACTORY_TEST && ++factory_test_times >= FACTORY_TEST_COUNT)
		{
			state = STATE_INSPECTION;
		}
	}
}





