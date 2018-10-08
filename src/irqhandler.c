#include "irqhandler.h"
#include <rtthread.h>

void NMI_Handler(void)
{
    
}



void SVC_Handler(void)
{
    
}



void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();

	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}
