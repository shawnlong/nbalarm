

/*speaker.h*/
#ifndef _SPEAKER_H_
#define _SPEAKER_H_
#define SPEAKER_PORT_PIN		GPIO_PIN_1
#define SPEAKER_PORT_CHECK_MODE		GPIO_MODE_INPUT | GPIO_MODE_PULL_UP
#define SPEAKER_PORT_CTRL_MODE		GPIO_MODE_OUTPUT|GPIO_MODE_PP
#define SPEAKER_ALARM_TIME		60 //60 seconds

#define SPEAKER_CONNECT_MINV 200
#define SPEAKER_CONNECT_MAXV 400	
#define SPEAKER_ENABLE 1

uint8_t speaker_init();
uint8_t speaker_check_exist();
uint8_t speaker_close();
uint8_t speaker_start();
uint8_t speaker_check_playing();


#endif
