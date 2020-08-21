

/*ac.h*/
#ifndef _AC_H_
#define _AC_H_
#define AC_POWER_ON				1
#define AC_POWER_OFF			2

#define AC_DET_PORT			 	GPIOB
#define AC_DET_PIN				GPIO_PIN_4
#define AC_DET_INT_PORT			EXTI_PORT_GPIOB

#define AC_CONFIG_PORT_1		GPIOA
#define AC_CONFIG_PIN_1			GPIO_PIN_1
#define AC_CONFIG_PORT_2		GPIOA
#define AC_CONFIG_PIN_2			GPIO_PIN_2
#define AC_CONFIG_PORT_3		GPIOB
#define AC_CONFIG_PIN_3			GPIO_PIN_5



uint8_t ac_init();
uint8_t ac_close();
uint8_t ac_get_status();

#endif

