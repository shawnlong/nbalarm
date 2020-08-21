#ifndef  __KEY_SCAN_H
#define  __KEY_SCAN_H
#include "type_def.h"
#define BitStatus bool
#define Buttom_ON   0
#define Buttom_OFF  1
#define PD5_In  (BitStatus)(PD_IDR&0x20)
void Buttom_conf(void);
BitStatus Key_Scan(void);
#endif



