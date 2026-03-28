#define DEBUG
#include <stdio.h>

#include "Memory/memory_pool.h"
#include "RtosApi/rtos.h"

int main(void) {
    rtos_init();
    rtos_task_create(3, 20, "1");

    rtos_task_create(2, 10, "2");

    rtos_task_create(2, 15, "3");

    rtos_start();

    simulate_rtos();
}