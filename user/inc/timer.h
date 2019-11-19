/*timer.h*/
#ifndef _TIMER_H_
#define _TIMER_H_

#define TIMER_MAX_COUNT 	5
#define TIMER_ONE_MINUTE	60
#define TIMER_ONE_HOUR		(uint16_t)(60*TIMER_ONE_MINUTE)
#define TIMER_ONE_DAY		(uint32_t)(24*TIMER_ONE_HOUR)
#define TIMER_INFINITE		0

uint8_t timer_init();
uint8_t timer_close();
uint8_t timer_stop();
uint8_t timer_start();
uint32_t timer_get_tick();
uint8_t timer_delay(uint8_t seconds);

#endif

