#ifndef  __KEY_EXTI_H
#define  __KEY_EXTI_H
#include "type_def.h"
#define BitStatus bool
#define PD5_In  (BitStatus)(PD_IDR&0x20)
void Buttom_conf(void);

#endif



