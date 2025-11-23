#include "rms_scheduler.h"
#include <stdio.h>
#include "../Globals/globals.h"

//Transforms a bit from 0 to 1
#define SET_BIT(number, position) \
number |= (1 << position);

//Transforms a bit from 1 to 0
#define RESET_BIT(number, position) \
number &= ~(1 << position);

//Gets the number of trailing zeros from number
#define GET_TRAILING_ZEROS_COUNT(number) \
((number > 0 ? __builtin_clz(number) : sizeof(long) * 8))

const static float RMS_BOUNDS[10] = {
    1.0000,   // n=1
    0.8284,   // n=2
    0.7798,   // n=3
    0.7568,   // n=4
    0.7435,   // n=5
    0.7348,   // n=6
    0.7286,   // n=7
    0.7241,   // n=8
    0.7206,   // n=9
    0.7177,   // n=10
};

static int periods_per_priority[4];

static float utilization = 0;

static int new_task_condition(const TCB_t* task, const TCB_t *task2) {
    return task->period < task2->period;
}

static int return_task_from_template(const TCB_t *task) {
    return 1;
}

static int pending_enqueue(const TCB_t *task1, const TCB_t *task2) {
    return task1->next_release_time > task2->next_release_time;
}

static int should_restore(const TCB_t *task) {
    return task->next_release_time -1 == current_time;
}

void scheduler_init(scheduler_t* scheduler, rms_task_templates_t* templates) {
    h_init(&templates->tasks);

    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        queue_init(&scheduler->queues[i]);
    }

    h_init(&scheduler->pending);
}

short rms_task_templates_add(rms_task_templates_t* templates,TCB_t *task) {
    h_enqueue(&templates->tasks, task, new_task_condition);
    utilization += (float)(task->execution_time) / (float)(task->period);
#ifdef DEBUG
    printf("Utilization = %f\n", utilization);
#endif

    return 1;
}

float calculate_rms_bound(size_t number_of_tasks) {
    return RMS_BOUNDS[number_of_tasks - 1];
}

short is_schedulable(const rms_task_templates_t* templates) {
    if(utilization <= 0.69)
        return 1;

    float rms_bound = calculate_rms_bound(h_get_size(&templates->tasks) - 1);
#ifdef DEBUG
    printf("Utilization %f <= Max possible time %f", utilization, rms_bound);
#endif
    return utilization <= rms_bound;
}

short start_scheduler(rms_task_templates_t* templates, scheduler_t *scheduler) {
    if(!is_schedulable(templates) || h_is_empty(&templates->tasks))
        return 0;

    unsigned long current_priority = 0;
    int current_period = 0;

    TCB_t *task = h_dequeue(&templates->tasks, return_task_from_template, new_task_condition);
    current_period =  task->period;
    task->priority = current_priority;
    scheduler_add_task(scheduler, task);
    periods_per_priority[current_priority] = current_period;

    while(!h_is_empty(&templates->tasks)) {
        task = h_dequeue(&templates->tasks, return_task_from_template, new_task_condition);
        if(task->period != current_period) {
            current_period = task->period;
            current_priority--;
            periods_per_priority[current_priority] = current_period;
        }

        task->priority = current_priority;
        scheduler_add_task(scheduler, task);
    }

    #ifdef DEBUG
        printf("Finished setting up the scheduler\n");
        print_task_queues(&task_queues);
    #endif
    return 1;
}

short scheduler_add_task(scheduler_t* scheduler, TCB_t *task) {
    if (task->priority < 0 || task->priority > NUM_PRIORITY_LEVELS) {
        return 0;
    }

    enqueue(&scheduler->queues[task->priority], task);
    SET_BIT(scheduler->bitmap, task->priority);

    return 1;
}

TCB_t *scheduler_get_task(scheduler_t* scheduler) {
    const unsigned long priority = GET_TRAILING_ZEROS_COUNT(scheduler->bitmap);

    if(priority == 32)//It means 32 trailing zeros => number 0
        return 0x00;

    RESET_BIT(scheduler->bitmap, priority);

    return dequeue(&scheduler->queues[priority]);
}

short scheduler_sleep_task(scheduler_t* scheduler, TCB_t *task) {
    h_enqueue(&scheduler->pending, task, pending_enqueue);
#ifdef DEBUG
    print_task_queues(tq);
#endif

    return 1;
}

short scheduler_release_tasks(scheduler_t* scheduler) {
    TCB_t *pending_task = 0x00;

    while((pending_task = h_dequeue(&scheduler->pending, should_restore, pending_enqueue)) != 0x00) {
        pending_task->remaining_time = pending_task->execution_time;
        scheduler_add_task(scheduler, pending_task);
        return 1;
    }

#ifdef DEBUG
    print_task_queues(tq);
    print_task(&pending_task);
#endif

    return 0x00;
}

short exists_higher_priority_task(const scheduler_t* scheduler, const TCB_t *task) {
    if(task->priority == GET_TRAILING_ZEROS_COUNT(scheduler->bitmap))
        return 0;

    return 1;
}

void print_scheduler(const scheduler_t* scheduler) {
    printf("Printing task queues: \n");
    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        switch(i) {
            case 0: {
                printf("Very high : ");
                break;
            }
            case 1: {
                printf("High : ");
                break;
            }
            case 2: {
                printf("Medium : ");
                break;
            }
            case 3: {
                printf("Low : ");
                break;
            }
            default: {
                printf("Unknown priority : %d\n", i);
            }
        }
        print_queue(&scheduler->queues[i]);
    }


    printf("Printing pending queue: \n");
    h_print_queue(&scheduler->pending);
}

