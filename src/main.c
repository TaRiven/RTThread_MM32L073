

/* includes --------------------------------------------------------------------------------------*/
#include "HAL_conf.h"
#include "rtthread.h"
#include "rthw.h"
/* private typedef -------------------------------------------------------------------------------*/

/* private define --------------------------------------------------------------------------------*/	

/* private macro ---------------------------------------------------------------------------------*/
#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_8)	// PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_8)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_8))?(GPIO_ResetBits(GPIOB,GPIO_Pin_8)):(GPIO_SetBits(GPIOB,GPIO_Pin_8))	// PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_9)	// PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_9)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_9))?(GPIO_ResetBits(GPIOB,GPIO_Pin_9)):(GPIO_SetBits(GPIOB,GPIO_Pin_9))	// PB5

/* private variables -----------------------------------------------------------------------------*/
//extern int Image$$RW_IRAM1$$ZI$$Limit;
extern int _ebss;
#define MM32L073_SRAM_SIZE         4
#define MM32L073_SRAM_END          (0x20000000 + MM32L073_SRAM_SIZE * 1024)

struct rt_semaphore s_tLedSem;
/* private function prototypes -------------------------------------------------------------------*/
static void rt_hw_board_init(void);
static void led_thread_entry(void *pdata);
static void sem_thread_entry(void *pdata);
static void rt_application_init(void);
static void rtthread_startup(void);


/***************************************************************************************************
                                           外 部 函 数
***************************************************************************************************/
int main(void)
{
	/* disable interrupt first */
	rt_hw_interrupt_disable();
	
	/* startup RT-Thread RTOS */
	rtthread_startup();

	while (1);
}

/***************************************************************************************************
                                           内 部 函 数
***************************************************************************************************/
static void rt_hw_board_init(void)
{
	SysTick_Config( 48000000 / RT_TICK_PER_SECOND );
}

static void led_thread_entry(void *pdata)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);  //?a??GPIOA,GPIOBê±?ó
      
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	LED1_ON();
	LED2_OFF();

	pdata = pdata;	
	while (1) {
		rt_sem_take(&s_tLedSem, RT_WAITING_FOREVER);
		LED1_TOGGLE();
		LED2_TOGGLE();
	}
}

static void sem_thread_entry(void *pdata)
{
	pdata = pdata;
	while (1) {
		rt_thread_delay(RT_TICK_PER_SECOND/10);
		rt_sem_release(&s_tLedSem);
	}
}

static void rt_application_init(void)
{
	rt_thread_t user_thread;

	rt_sem_init(&s_tLedSem, "led_sem", 0, RT_IPC_FLAG_FIFO);

	/* Create led thread */
	user_thread = rt_thread_create("led",
		led_thread_entry, 
		RT_NULL,
		256, 19, 20);
	if(user_thread != RT_NULL)
		rt_thread_startup(user_thread);

	/* Create led thread */
	user_thread = rt_thread_create("sem",
		sem_thread_entry, RT_NULL,
		256, 16, 20);
	if(user_thread != RT_NULL)
		rt_thread_startup(user_thread);
}	

static void rtthread_startup(void)
{
	/* init board */
	rt_hw_board_init();

	/* show version */
	rt_show_version();

	/* init tick */
	rt_system_tick_init();

	/* init kernel object */
	rt_system_object_init();

	/* init timer system */
	rt_system_timer_init();

#ifdef RT_USING_HEAP
	rt_system_heap_init((void*)&_ebss, (void*)MM32L073_SRAM_END);
#endif

	/* init scheduler system */
	rt_system_scheduler_init();

	/* init application */
	rt_application_init();

	/* init timer thread */
	rt_system_timer_thread_init();

	/* init idle thread */
	rt_thread_idle_init();

	/* start scheduler */
	rt_system_scheduler_start();

	/* never reach here */
	return ;
}


/* end of file -----------------------------------------------------------------------------------*/
