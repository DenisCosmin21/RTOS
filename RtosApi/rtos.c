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
    scheduler_init(&scheduler, &rms_task_templates);
}

short rtos_task_create(const int execution_time, const int period,const char *name) {
    //Assign lowest priority first when creating it,becouse in RMS it should'nt know exact priority on creation
    TCB_t *task = init_task(0, 128, execution_time, period, name);

    rms_task_templates_add(&rms_task_templates, task);

#ifdef DEBUG
    printf("Debug\n");
#endif

    return 1;
}

short rtos_start(void) {
    return start_scheduler(&rms_task_templates, &scheduler);
}

void simulate_rtos(void) {
    running_task = scheduler_get_task(&scheduler);
    for(;current_time < max_simulation_time;current_time++) {
        printf("%d ", current_time);
        print_task(running_task);
        printf("\n");

        scheduler_release_tasks(&scheduler);

        if(!should_switch())
            continue;

        next_task = scheduler_get_task(&scheduler);

        TCB_t *finished_task = context_switch();
        if(finished_task) {
            if(finished_task->remaining_time == 0)
                scheduler_sleep_task(&scheduler, finished_task);
            else
                scheduler_add_task(&scheduler, finished_task);
        }
    }
}
