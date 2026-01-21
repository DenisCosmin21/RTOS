#include <stdint.h>
#include <stdio.h>
#include "led.h"
#include "uart.h"
#include "mutex.h"
#include "osKernel.h"
#include "globals.h"

typedef	uint32_t TaskProfiler;

mutex_t resource_mutex;

TaskProfiler Task0_Profiler, Task1_Profiler, Task2_Profiler;

volatile uint32_t work_dummy = 0;


void hard_work(int iterations) {
    for (int i = 0; i < iterations; i++) {
        work_dummy++;
    }
}


int locked = 0;

void task_low(void) {
    while(1) {

    	Task0_Profiler++;


   /*    if(locked == 0){
       printf("LOW:  Trying Lock...\r\n");
       mutex_lock(&resource_mutex);


       printf("LOW:  LOCKED. Working (Critical Section)...\r\n");


       rtos_task_wait();

       printf("LOW:  Finishing work. Unlocking...\r\n");

       mutex_unlock(&resource_mutex);
       }

       locked++;
*/


    }
}

void task_med(void) {
    while(1) {

    // printf("MED:  Running! I am annoying!\r\n");

     Task1_Profiler++;
    }
}


void task_high(void) {
    while(1) {

     	Task2_Profiler++;


    //printf("HIGH: I need Mutex NOW!\r\n");
    // mutex_lock(&resource_mutex);






    // mutex_unlock(&resource_mutex);
    // printf("HIGH: Got Mutex! Thanks Low.\r\n");
    // printf("HIGH: Done.\r\n");


    }
}

// 2 - > HIGH -> 7 -> MEDIUM -> LOW -> 10 -> 12 -> HIGH -> 12 + 7 = 19(idle) -> 20 -> HIGH

int main(void) {
    uart_tx_init();


    mutex_init(&resource_mutex);

    osKernelInit();


    osKernelAddThreads(&task_high, 2, 10, "HIGH");


    osKernelAddThreads(&task_med, 5, 30, "MED");

    osKernelAddThreads(&task_low, 10, 100, "LOW");

    osKernelLaunch(QUANTA);
}
