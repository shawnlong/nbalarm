/*ac.c*/
#include "stm8s.h"
#include "ac.h"

uint8_t g_ac_alarm;
uint8_t ac_init()
{
	/*need config pin?*/
	GPIO_Init(AC_DET_PORT, AC_DET_PIN, GPIO_MODE_IN_FL_IT);
	/*enable AC_DET_PORT-AC_DET_PIN interrupt*/
	EXTI_SetExtIntSensitivity(AC_DET_INT_PORT, EXTI_SENSITIVITY_RISE_FALL);
	g_ac_alarm = TRUE;
	return 0;
}


uint8_t ac_close()
{
	return 0;
}


uint8_t ac_get_status()
{
	if((g_ac_alarm == TRUE) && (GPIO_ReadInputPin(AC_DET_PORT, AC_DET_PIN) == 0))
	{
		return AC_POWER_OFF;
	}
	else
	{
		return AC_POWER_ON;
	}
}


