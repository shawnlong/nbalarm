

/*awu.h*/
#ifndef _AWU_H_
#define _AWU_H_
uint8_t awu_init();
uint8_t awu_sleep(uint32_t seconds);

#define AWU_ONE_SECONDS			1
#define AWU_TEN_SECONDS			10
#define AWU_ONE_MINUTE			60
#define AWU_ONE_HOUR			(uint16_t)(60*AWU_ONE_MINUTE)
#define AWU_ONE_DAY 			(uint32_t)(24*AWU_ONE_HOUR)
#define AWU_INFINITE			0


#endif

