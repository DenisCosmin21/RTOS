//
// Created by Denis on 11/22/2025.
//
#include "rtos.h"
#include <string.h>
#include "rms_scheduler.h"
#include "task.h"
#include <stdio.h>
#include <windows.h>

static rms_task_templates_t task_templates;
static short initialized = 0;
int current_time = 0;
static int max_simulation_time = 40;

short rtos_task_create(const int execution_time, const int period,const char *name) {
    if(!initialized) {
        rms_task_templates_init(&task_templates);
        initialized = 1;
    }

    TCB_t task = init_task(WAITING, 128, execution_time, period, name);

    rms_task_templates_add(&task_templates, task);

#ifdef DEBUG
    printf("Debug\n");
#endif

    return 1;
}

short rtos_start(void) {
    return start_scheduler(&task_templates);
}

void simulate_rtos(void) {
    running_task = get_next_task();
    for(;current_time < max_simulation_time;current_time++) {
        printf("%d ", current_time);
        print_task(&running_task);
        printf("\n");

        release_tasks();

        if(!should_switch())
            continue;

        TCB_t next_task = get_next_task();

        TCB_t finished_task = context_switch(next_task);

        if(finished_task.remaining_time == 0)
            wait_task(finished_task);
        else
            store_back_task(&finished_task);
    }
}
