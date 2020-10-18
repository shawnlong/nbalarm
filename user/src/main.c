

/*******************************************************************************
file: main.c
Company: Leadien
Project: BN-ALARM device
Platforam:STM8+M5313
Function: Monitoring 3 switch-type sensors and report state change to server,
		  by M5313 from NB-IOT/GPRS network. And control a speaker alarm.
*******************************************************************************/
#include "stm8s.h"
#include "sensor.h"

#include "battery.h"
#include "ac.h"
#include "speaker.h"
#include "awu.h"
#include "dtu.h"
#include "msg.h"

#include "stm8s_clk.h"
#include "timer.h"

/*Action rules:
1.Snesor opened, send alarm message immediately
2.if send succeed, waiting for sensor close, and send alarm messge after 1m、10m、1hour、per 24hours
3.once all sensor closed, send alarm message then enter inspection state, send status message per 24 hours
4.once send failed, try again after 10s, 10s, 10s, 1m, per 24hours
*/
#define STATE_INSPECTION						0
#define STATE_ALARM_1							1
#define STATE_ALARM_2							2
#define STATE_ALARM_3							3
#define STATE_ALARM_4							4
#define STATE_ALARM_PER_DAY 					5
#define STATE_RETRY_1							6
#define STATE_RETRY_2							7
#define STATE_RETRY_3							8
#define STATE_RETRY_4							9
#define STATE_RETRY_5							10
#define STATE_RETRY_PER_DAY 					11
#define STATE_NUMBER							12

#define FACTORY_TEST_COUNT						0
#define MAX_RETRY_TIMES 						10

static uint8_t NEXT_STATE[STATE_NUMBER] =
{
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


/*We feed the wwdg in the tim1 inttrupt. To make sure main program not dead,
  tim1 ISR should watch this flag to be set, in a period of 60seconds*/
__IO uint8_t live_flag = 0;
extern __IO uint8_t sensor_interrupt;


static void bsp_init()
{
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8); /*16M/8=2Mhz clock*/
	awu_init();
	timer_init();
	timer_start();
	sensor_init();
	ac_init();
	battery_init();
	speaker_init();
	enableInterrupts();
}

/*check device status
1.OK
2.Battery low power
3.Device fail(reserved)
4.AC power fail
*/
static uint8_t get_dev_status()
{
	if (battery_get_status() == BATTERY_STATUS_LOWPOWER)
	{
		return MSG_DEV_STATUS_LOWPOWER;
	}

	if (ac_get_status() == AC_POWER_OFF)
	{
		return MSG_DEV_STATUS_NOAC;
	}

	return MSG_DEV_STATUS_OK;

}


static uint8_t update_state(SENSOR_EVENT_T * pevent, uint8_t pre_state, uint8_t dev_status, uint8_t send_ret)
{
	uint8_t state;
	static uint8_t pre_dev_status = MSG_DEV_STATUS_OK;

	if (pre_state >= STATE_RETRY_1)
	{
		state = pevent->state;
	}
	else if (((pevent->val == pevent->pre_val) && (dev_status == pre_dev_status)))
	{
		state = NEXT_STATE[pre_state];
	}
	else if (dev_status == MSG_DEV_STATUS_OK && pre_dev_status != MSG_DEV_STATUS_OK)
	{
		state = pre_state;
	}
	else 
	{
		state = STATE_ALARM_1;
	}

	if (dev_status == MSG_DEV_STATUS_OK && pevent->val == 0x00)
	{
		state = STATE_INSPECTION;
	}

	pevent->state = state;

	if (send_ret != SUCCESS)
	{
		if (pre_state >= STATE_RETRY_1)
		{
			state = NEXT_STATE[pre_state];
		}
		else 
		{
			state = STATE_RETRY_1;
		}
	}

	pre_dev_status = dev_status;
	return state;
}


main()
{
	uint8_t ret, dev_status, state = STATE_INSPECTION;
	SENSOR_EVENT_T sensor_event;

	/*1.initilize board*/
	bsp_init();

	while (1)
	{
		/*2.sleep for specified period, determine by state*/
		awu_sleep(SLEEP_PERIODS[state]);
                
		if (sensor_stack_empty() == TRUE)
		{
			sensor_push_raw();
		}

		/*3.wakeup from sleep, reconfig the HSI clock and feed watchdog*/
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8); /*16M/8=2Mhz clock*/

		/*4.send message - try to send all stacked event*/
		dtu_init();

		while (sensor_stack_empty() == FALSE)
		{
			sensor_pop(&sensor_event);
			dev_status = get_dev_status();
			ret = msg_send(dev_status, &sensor_event);

			/*update state*/
			state = update_state(&sensor_event, state, dev_status, ret);

			if (ERROR == ret)
			{
				sensor_push_back(&sensor_event);
				break;
			}
		}

		dtu_close(state);

		/*5.wait for speaker play finish*/
		while (speaker_check_playing() == TRUE);

		speaker_close();

		/*6.go to 1st step -- sleep*/
		continue;
	}
}




