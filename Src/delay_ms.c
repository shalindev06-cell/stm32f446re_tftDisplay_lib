#include "delay_ms.h"

void delay_ms(uint32_t time)

{
	SysTick->LOAD=16000-1;
	SysTick->VAL=0;
	SysTick->CTRL=(1U<<0)|(1U<<2);
	while(time>0)
	{

		while((SysTick->CTRL&(1U<<16))==0){}
		time--;

	}
    SysTick->CTRL = 0;
}
