

/*awu.h*/
#ifndef _AWU_H_
#define _AWU_H_
uint8_t awu_init();
uint8_t awu_sleep(uint32_t seconds);

#define AWU_ONE_SECONDS			1l
#define AWU_TEN_SECONDS			10l
#define AWU_ONE_MINUTE			60l
#define AWU_ONE_HOUR			(3600l - 180l)//adjust 3minutes
#define AWU_ONE_DAY 			(86400l - 4320l)//adjust 72minuts
#define AWU_INFINITE			0

#define LSI_CALIBRATION 1

#endif

