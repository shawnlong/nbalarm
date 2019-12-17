
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
#include "msg.h"
#include "battery.h"
#include "speaker.h"
#include "awu.h"
#include "dtu.h"
#include "stm8s_clk.h"
#include "timer.h"

/*Action rules:
1.Snesor opened, send alarm message immediately
2.if send succeed, waiting for sensor close, and send alarm messge after 1m、10m、1hour、per 24hours
3.once all sensor closed, send alarm message then enter inspection state, send status message per 24 hours
4.once send failed, try again after 10s, 10s, 10s, 1m, per 24hours
*/
#define STATE_INSPECTION		0
#define STATE_ALARM_1			1
#define STATE_ALARM_2			2
#define STATE_ALARM_3			3
#define STATE_ALARM_4			4
#define STATE_ALARM_PER_DAY 	5
#define STATE_RETRY_1			6
#define STATE_RETRY_2			7
#define STATE_RETRY_3			8
#define STATE_RETRY_4			9
#define STATE_RETRY_5			10
#define STATE_RETRY_PER_DAY 	11
#define STATE_NUMBER			12

#define FACTORY_TEST_COUNT		0
#define MAX_RETRY_TIMES			10

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

static void bsp_init()
{
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8); /*16M/8=2Mhz clock*/
    awu_init();
	timer_init();
	timer_start();
    sensor_init();      
	enableInterrupts();
	/*start windows watchdog*/
	//WWDG_Init(0x7F, 0x7F);//393ms window@2Mhz main clock
	battery_adc_init();
}

static void bsp_feed_dog()
{
	live_flag = 0xFE;
}


main()
{
	uint8_t sensor_change = SENSORS_UNCHANGED;
	uint8_t state = STATE_INSPECTION;
	uint8_t state_before_retry = STATE_INSPECTION;
	uint8_t ret;
	MSG_T msg, *pmsg;

	
    //0.initilize board
    bsp_init();
	
	
	while(1)
	{
          
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8); /*16M/8=2Mhz clock*/
	
		bsp_feed_dog();
		
		//1.check sensors
		sensor_change = sensor_get_status(msg.sensors);

		//2.if new sensor open, alarm speaker
		if(sensor_change == SENSORS_NEW_OPENED)
		{
			speaker_init();
			speaker_alarm();
		}

		//3.push message into stack
		if(sensor_change == SENSORS_UNCHANGED && state == STATE_INSPECTION)
		{ 
			msg.type = MSG_TYPE_STATUS;
		}else{
			msg.type = MSG_TYPE_ALARM;
		}
		msg.status = battery_get_status();
		msg.tm = timer_get_tick();
		if(!(state >= STATE_RETRY_1 && sensor_change == SENSORS_UNCHANGED))
		{
			msg_push(&msg);
		}

		//4.try to send all stacked message to server
		dtu_init();
		while(1){
			pmsg = msg_top();
			if(pmsg != 0 && SUCCESS == (ret = msg_send(pmsg)))
			{
				msg_pop();
				continue;
			}
			break;
		}
		dtu_close();
		bsp_feed_dog();
		//4.wait for speaker play finish
		while(speaker_check_playing() == 1)
			;

		speaker_close();

		//5.update state
		switch(sensor_change)
		{
			case SENSORS_NEW_CLOSED:
			case SENSORS_NEW_OPENED:
				state = STATE_ALARM_1;
				break;
			case SENSORS_UNCHANGED:
				state = NEXT_STATE[state];
				break;
			case SENSORS_JUST_ALL_CLOSED:
				state = STATE_INSPECTION;
				break;
			default:
				break;
		}

		if(sensor_change == SENSORS_NEW_CLOSED || sensor_change == SENSORS_NEW_OPENED)
		{
			state = STATE_ALARM_1;
		}
		if(sensor_change == SENSORS_UNCHANGED)
		{
			state = NEXT_STATE[state];
		}
		if(sensor_change == SENSORS_JUST_ALL_CLOSED)
		{
			state = STATE_INSPECTION;
		}
		if(ret == SUCCESS)
		{
			if(state >= STATE_RETRY_1)
			{
				state = state_before_retry;
			}
		}else{
			if(state < STATE_RETRY_1)
			{
				state_before_retry = state;
				state = STATE_RETRY_1;
			}
		}
		
		//6.sleep 
		//dtu_print_n(state);
       	awu_sleep(SLEEP_PERIODS[state]);
	}
}




