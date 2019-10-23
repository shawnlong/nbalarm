

/*awu.h*/
#ifndef _AWU_H_
#define _AWU_H_
uint8_t awu_init();
uint8_t awu_close();
uint8_t awu_sleep(uint32_t seconds);

typedef struct AWU_struct
{
uint8_t 		CSR;								/*!< AWU Control status register */
uint8_t 		APR;								/*!< AWU Asynchronous prescaler buffer */
uint8_t 		TBR;								/*!< AWU Time base selection register */
} AWU_TypeDef;


#define AWU_CSR_AWUF			((uint8_t)0x20) /*!< Interrupt flag mask */
#define AWU_CSR_AWUEN			((uint8_t)0x10) /*!< Auto Wake-up enable mask */
#define AWU_CSR_MSR 			((uint8_t)0x01) /*!< LSI Measurement enable mask */

//#define AWU_APR_APR	  ((uint8_t)0x3F) /*!< Asynchronous Prescaler divider mask */
//#define AWU_TBR_AWUTB   ((uint8_t)0x0F) /*!< Timebase selection mask */
#define AWU_BaseAddress 		0x50F0
#define AWU 					((AWU_TypeDef *) AWU_BaseAddress)

#define AWU_TEN_SECONDS			10
#define AWU_ONE_MINUTE			60
#define AWU_ONE_HOUR			(uint16_t)(60*AWU_ONE_MINUTE)
#define AWU_ONE_DAY 			(uint32_t)(24*AWU_ONE_HOUR)
#define AWU_INFINITE			0


#endif

