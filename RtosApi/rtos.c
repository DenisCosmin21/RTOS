//
// Created by Denis on 11/22/2025.
//
#include "../Globals/globals.h"
#include "rtos.h"
#include "../RmsScheduler/rms_scheduler.h"
#include "../Task/task.h"
#include <stdio.h>


static int max_simulation_time = 40;

void rtos_init(void) { //Will get called from the reset procedure
    scheduler_init();
}

short rtos_task_create(const int execution_time, const int period,const char *name) {
    //Assign lowest priority first when creating it,becouse in RMS it should'nt know exact priority on creation
    TCB_t *task = init_task(0, BASE_TASK_STACK_SIZE, execution_time, period, name);

    rms_task_templates_add(task);

    return 1;
}

short rtos_start(void) {
    return start_scheduler();
}

//Tell the kernel that current running task finished execution, and should switch
void rtos_task_wait(void) {
    next_task = scheduler_get_task();

    if(running_task != 0x00) {
        reset_task(running_task);

        scheduler_sleep_task(running_task);
    }

    context_switch();
}

//It stops the current running task to allow another task to run
void rtos_task_yeld(void) {
    next_task = scheduler_get_task();
    scheduler_add_task(running_task);
    context_switch();
}

void simulate_rtos(void) {
    running_task = scheduler_get_task();
    for(;current_time < max_simulation_time;current_time++) {
        printf("%d ", current_time);
        print_task(running_task);
        printf("\n");

        //Would get called in systick handler
        scheduler_release_tasks();

        should_switch();
        //
    }
}
