#include "osKernel.h"
#include "task.h"
#include "globals.h"
#include "rtos.h"
#include "rms_scheduler.h"
#include "stm32u5xx.h"



#define CTRL_ENABLE (1U<<0)
#define CTRL_CLCKSRC (1U<<2)
#define CTRL_TICKINT (1U<<1)
#define CTRL_COUNTFLAG	(1<<16)
#define SYSTICK_RST 	0
#define IDLE_STACK_SIZE 128


uint32_t idle_stack[IDLE_STACK_SIZE];
TCB_t *os_idle_tcb;
uint32_t MILIS_PRESCALER;

static void os_idle_thread(void) {
    while(1) {

      // __asm("wfi");
    }
}


uint32_t SystemCoreClock = 4000000;


void SystemCoreClockUpdate(void) {


    SystemCoreClock = 4000000;
}

void osSchedulerLaunch(void){
	__asm("LDR R0,=running_task");

	__asm("LDR R2, [R0]");

	__asm("LDR SP, [R2]");


	__asm("POP {R4-R11}");

	__asm("POP {R0-R3}");

	__asm("ADD SP, SP, #4");
	__asm("ADD SP, SP, #4");

	__asm("POP {LR}");

	__asm("ADD SP, SP, #4");

	__asm("CPSIE I");

	__asm("BX LR");

}

void osKernelStackInit(TCB_t *task, void (*taskFunc)(void)) {
    uint32_t *stackBase = (uint32_t *)task->base_stack_pointer;
    uint32_t *stackTop  = stackBase + task->stack_size;


    uint32_t *sp = stackTop - 16;


    sp[15] = (1U << 24); // XPSR on thumb mode 2
    sp[14] = (uint32_t)taskFunc; // PC
    sp[13] = (0xAAAAAAAA);   // dummy values
    sp[12] = (0xAAAAAAAA);
    sp[11] = (0xAAAAAAAA);
    sp[10] = (0xAAAAAAAA);
    sp[9] = (0xAAAAAAAA);
    sp[8] = (0xAAAAAAAA);
    sp[7] = (0xAAAAAAAA);
    sp[6] = (0xAAAAAAAA);
    sp[5] = (0xAAAAAAAA);
    sp[4] = (0xAAAAAAAA);
    sp[3] = (0xAAAAAAAA);
    sp[2] = (0xAAAAAAAA);
    sp[1] = (0xAAAAAAAA);
    sp[0] = (0xAAAAAAAA);



    task->stack_pointer = sp;     // where SchedulerLauncher loads SP from
}




uint8_t osKernelAddThreads( void(*taskFunc)(void) , const int execution_time, const int period,const char *name){

	__disable_irq();


	    TCB_t *task = init_task(0, BASE_TASK_STACK_SIZE, execution_time, period, name);
	    osKernelStackInit(task, taskFunc);
	    rms_task_templates_add(task);

	__enable_irq();
 	return 1;
}

void osKernelInit(void){
	SystemCoreClockUpdate();
	MILIS_PRESCALER = (SystemCoreClock / 1000);
	rtos_init();

	os_idle_tcb = init_task(4, 32, 0, 0, "idle");

	    osKernelStackInit(os_idle_tcb, os_idle_thread);
	    scheduler_set_idle_task(os_idle_tcb);
}


void osKernelLaunch(uint32_t quanta){

	QUANTA = quanta;
	// Reset Systick
	SysTick->CTRL = SYSTICK_RST;

	SysTick->VAL = 0;

	SysTick->LOAD = (QUANTA * MILIS_PRESCALER) - 1;

	NVIC_SetPriority(SysTick_IRQn, 15);
	NVIC_SetPriority(PendSV_IRQn, 15);

	SysTick->CTRL = CTRL_CLCKSRC | CTRL_ENABLE;

	SysTick->CTRL |= CTRL_TICKINT;

	start_scheduler();


	//running_task = scheduler_get_task();


	__enable_irq();

	osSchedulerLaunch();

}

void SysTick_Handler(void){

	current_time += 1;
	scheduler_release_tasks();
	should_switch();

}

__attribute__((naked)) void PendSV_Handler(void){
	__asm("CPSID	I");

	__asm("PUSH  {R4-R11}");

	__asm("LDR R0, =running_task");

	__asm("LDR R1, [R0]");

	__asm("STR SP, [R1]");


	__asm("LDR R2, =next_task");

	__asm("LDR R3, [R2]");

	__asm("STR R3, [R0]");



	__asm("LDR SP, [R3]");

	__asm("POP {R4-R11}");

	__asm("CPSIE I");

	__asm("BX LR");

}





