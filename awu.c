
#include "iostm8s103F3.h"
#include "stdint.h"

#include "awu.h"

extern volatile uint8_t ext_pc_flag;

static void sleep_1s()
{
	// Set the TimeBase 
	AWU->TBR &= (uint8_t) (~AWU_TBR_AWUTB);
	AWU->TBR |= 12;

	// Set the APR divider 
	AWU->APR &= (uint8_t) (~AWU_APR_APR);
	AWU->APR |= 61;
	asm("rim");
	asm("halt");
	return;
}


#pragma vector					=AWU_vector


__interrupt void AWU_IRQHandler(void)
{
	AWU->CSR = AWU->CSR;
}



uint8_t awu_init()
{
	//Enable the AWU peripheral 
	AWU->CSR |= AWU_CSR_AWUEN;
	AWU->CSR |= AWU_CSR_MSR;
	return 0;
}


uint8_t awu_close()
{
	AWU->CSR &= ~AWU_CSR_AWUEN;
	return 0;
}
#define MASK_CLK_PCKENR1_PCKEN10 0x01
#define MASK_CLK_PCKENR1_PCKEN11 0x02
#define MASK_CLK_PCKENR1_PCKEN12 0x04
#define MASK_CLK_PCKENR1_PCKEN13 0x08
#define MASK_CLK_PCKENR1_PCKEN14 0x10
#define MASK_CLK_PCKENR1_PCKEN15 0x20
#define MASK_CLK_PCKENR1_PCKEN16 0x40
#define MASK_CLK_PCKENR1_PCKEN17 0x80

#define MASK_CLK_PCKENR2_PCKEN20 0x01
#define MASK_CLK_PCKENR2_PCKEN21 0x02
#define MASK_CLK_PCKENR2_PCKEN22 0x04
#define MASK_CLK_PCKENR2_PCKEN23 0x08
#define MASK_CLK_PCKENR2_PCKEN24 0x10
#define MASK_CLK_PCKENR2_PCKEN25 0x20
#define MASK_CLK_PCKENR2_PCKEN27 0x80
#define MASK_CLK_ICKR_SWUAH      0x20


uint8_t awu_sleep(uint32_t seconds)
{
	uint32_t i = 0;

	//config power saving
	CLK_ICKR |= MASK_CLK_ICKR_SWUAH;//use LPVR
	FLASH_CR1 |= MASK_FLASH_CR1_AHALT;
	FLASH_CR1 &= ~MASK_FLASH_CR1_HALT;
	//close all periphrals clock
	CLK_PCKENR1 &= ~MASK_CLK_PCKENR1_PCKEN13;
	CLK_PCKENR1 &= ~MASK_CLK_PCKENR1_PCKEN14;
	//CLK_PCKENR2 = 0;
	//enable auw clock
	CLK_PCKENR2 |= MASK_CLK_PCKENR2_PCKEN22;


	for(i = 0; i < seconds; i++)
	{
		if(ext_pc_flag == 0)
		{
			sleep_1s();
		}else{
			ext_pc_flag = 0;
            break;
		}
	}

	//open periphrals clock
	CLK_PCKENR1 |= MASK_CLK_PCKENR1_PCKEN13;//uart1
	CLK_PCKENR1 |= MASK_CLK_PCKENR1_PCKEN14;//TIM4

	return 0;
}


