
#ifndef __IRQHANDLER_H__
#define __IRQHANDLER_H__

#include "HAL_conf.h"

extern uint32_t SysCount;

/************ÖÐ¶Ïº¯ÊýÉùÃ÷***********/
void    Exception_NMI_Handler(void);
void    Exception_HardFault_Handler(void);
void    Exception_SVCall_Handler(void);
void    Exception_PendSV_Handler(void);
void    Exception_SysTick_Handler(void);

#endif
