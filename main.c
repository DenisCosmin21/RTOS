#define DEBUG
#include <stdio.h>

#include "rtos.h"

int main(void) {
#ifdef DEBUG
    printf("Testing RTOS\n");
    #endif
    rtos_task_create(3, 20, "1");

    rtos_task_create(2, 5, "2");

    rtos_task_create(2, 10, "3");

    rtos_start();

    simulate_rtos();
}