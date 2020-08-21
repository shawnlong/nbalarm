
#ifndef __LED_CONF_H
#define __LED_CONF_H
#include "type_def.h"

#define PD0_out  PD_ODR_ODR0  
#define PD1_out  PD_ODR_ODR1  
#define PD2_out  PD_ODR_ODR2  
#define PD3_out  PD_ODR_ODR3  
void LED_conf(void);
void Set_Led_On(void);
void Set_Led_Off(void);
void LED_Display(void);
void LED_Display_Bit(void);
#endif