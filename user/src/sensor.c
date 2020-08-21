

/*sensor.c*/
#include "string.h"
#include "stm8s.h"

#include "sensor.h"
#include "timer.h"
extern uint8_t g_ac_alarm;
typedef struct SENSOR_STACK_tag{
	SENSOR_EVENT_T stack[SENSOR_STACK_DEPTH];
	volatile uint8_t head;
	volatile uint8_t count;
	uint8_t last_val;
	uint8_t open_times[SENSOR_NUMBER];
	uint8_t close_times[SENSOR_NUMBER];
}SENSOR_STACK_T;

SENSOR_STACK_T g_sensor_stack = {0};
uint8_t g_ac_enable_pattern[] = {0x00, 0x20, 0x60, 0x20, 0xA0, 0x20, 0xA0, 0xFF};

uint8_t sensor_stack_empty()
{
	return (g_sensor_stack.count == 0)?TRUE:FALSE;
}


void sensor_push_raw()
{
	static uint32_t tick;
	static uint8_t config_pat_idx = 0;
	uint8_t i, value, cur,count = g_sensor_stack.count;
	value = GPIO_ReadInputData(SENSOR_GPIO_PORT) & 0xE0;
	/*1. put event on tail*/
	if(g_sensor_stack.count == SENSOR_STACK_DEPTH)
	{
		/*stack full, flush oldest one*/
		g_sensor_stack.head = (g_sensor_stack.head + 1) % SENSOR_STACK_DEPTH;
		g_sensor_stack.count--;
	}
	cur = (g_sensor_stack.head + count) % SENSOR_STACK_DEPTH; 
	g_sensor_stack.stack[cur].val = value;
	g_sensor_stack.stack[cur].tm = timer_get_tick();
	g_sensor_stack.stack[cur].pre_val = g_sensor_stack.last_val;
	g_sensor_stack.stack[cur].state = 1;
	
	g_sensor_stack.count++;
	
	/*2. update open/close times*/
	for(i = 0; i < SENSOR_NUMBER; i ++)
	{
		if(sensor_check_open(value, i) == SENSOR_OPEN 
			&& sensor_check_open(g_sensor_stack.last_val, i) == SENSOR_CLOSE)
		{
			g_sensor_stack.open_times[i]++;
		}
		if(sensor_check_open(value, i) == SENSOR_CLOSE 
			&& sensor_check_open(g_sensor_stack.last_val, i) == SENSOR_OPEN)
		{
			g_sensor_stack.close_times[i]++;
		}
	}

	/*2.1 check if ac_alarm config*/
	if( value == g_ac_enable_pattern[config_pat_idx + 1])
	{
		if(config_pat_idx == 0)
		{
			tick = timer_get_tick();
		}
		config_pat_idx++;
		if(config_pat_idx == 6 && (timer_get_tick() - tick < 30))
		{
			g_ac_alarm = (g_ac_alarm == TRUE)?FALSE:TRUE;
			config_pat_idx = 0;
		}
	}else
	{
		config_pat_idx = 0;
	}
	/*3. update lasted value*/
	g_sensor_stack.last_val = value;
}

void sensor_push_back(SENSOR_EVENT_T *pevent)
{
	uint8_t cur, count = g_sensor_stack.count;
	if(g_sensor_stack.count == SENSOR_STACK_DEPTH)
	{
		/*stack full, flush oldest one*/
		g_sensor_stack.head = (g_sensor_stack.head + 1) % SENSOR_STACK_DEPTH;
		g_sensor_stack.count--;
	}
	cur = (g_sensor_stack.head + count) % SENSOR_STACK_DEPTH; 
	memcpy(&(g_sensor_stack.stack[cur]), pevent, sizeof(SENSOR_EVENT_T));
	
	g_sensor_stack.count++;
}


uint8_t sensor_pop(SENSOR_EVENT_T *pevent)
{
	if(g_sensor_stack.count == 0)
	{
		return 0;
	}
	memcpy(pevent, &(g_sensor_stack.stack[g_sensor_stack.head]), sizeof(SENSOR_EVENT_T));
	g_sensor_stack.count --;
	g_sensor_stack.head = (g_sensor_stack.head + 1) % SENSOR_STACK_DEPTH;
	return 1;
}

uint8_t sensor_get_open_times(uint8_t id)
{
		return g_sensor_stack.open_times[id];
}

uint8_t sensor_get_close_times(uint8_t id)
{
	return g_sensor_stack.close_times[id];
}

uint8_t sensor_init()
{
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_FL_IT);
	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_FL_IT);
	GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_FL_IT);
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_FALL);

	g_sensor_stack.last_val = GPIO_ReadInputData(GPIOC);
	g_sensor_stack.count = g_sensor_stack.head = 0;
	return 0;
}

uint8_t sensor_close()
{
	//never close, because we need interrupt wakeup
	return 0;
}

uint8_t sensor_check_open(uint8_t value, uint8_t id)
{
	static uint8_t v;
	v = value;
	switch(id){
		case 0:
			if((v & 0x20) != 0)
			{
				return SENSOR_OPEN;
			}
			break;
		case 1:
			if((v & 0x40) != 0)
			{
				return SENSOR_OPEN;
			}
			break;
		case 2:
			if((v & 0x80) != 0)
			{
				return SENSOR_OPEN;
			}
			break;
		default:
			return SENSOR_CLOSE;
	}
	return SENSOR_CLOSE;
}



