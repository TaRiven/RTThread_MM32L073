#include "led.h"

/********************************************************************************************************
**函数信息 ：LED_Init(void)                        
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LED_Init(void)
{
    
    RCC->AHBENR|=0x7<<17;   //使能GPIOA时钟	   	 
    
    GPIOA->CRH|=GPIO_CRH_MODE15;         //PA15 推挽输出   	 
    GPIOA->ODR|=GPIO_ODR_ODR15;          //PA15 输出高
    
    GPIOB->CRL|=GPIO_CRL_MODE3|GPIO_CRL_MODE4|GPIO_CRL_MODE5;      ////PB3,PB4,PB5推挽输出
    GPIOB->ODR|=GPIO_ODR_ODR3|GPIO_ODR_ODR4|GPIO_ODR_ODR5;          ////PB.3,PB4,PB5输出高 
    
    LED1_OFF();
    LED2_OFF();
    LED3_OFF();
    LED4_OFF();
}

