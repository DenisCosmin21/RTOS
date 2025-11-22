#include "rms_scheduler.h"
#include <stdio.h>
#include "../Queue/task_queues.h"

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

static task_queues_t task_queues;

static int new_task_condition(const TCB_t* task, const TCB_t *task2) {
    return task->period < task2->period;
}

static int return_task_from_template(const TCB_t *task) {
    return 1;
}

void rms_task_templates_init(rms_task_templates_t* templates) {
    h_init(&templates->tasks);
}

short rms_task_templates_add(rms_task_templates_t* templates,const TCB_t task) {
    h_enqueue(&templates->tasks, task, new_task_condition);
    utilization += (float)(task.execution_time) / (float)(task.period);
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

short start_scheduler(rms_task_templates_t* templates) {
    if(!is_schedulable(templates) || h_is_empty(&templates->tasks))
        return 0;

    task_queues_init(&task_queues);

    priority_t current_priority = LOW;
    int current_period = 0;

    TCB_t task = h_dequeue(&templates->tasks, return_task_from_template, new_task_condition);
    current_period =  task.period;
    task.priority = current_priority;
    task_queues_enqueue(&task_queues, task);
    periods_per_priority[current_priority] = current_period;

    while(!h_is_empty(&templates->tasks)) {
        task = h_dequeue(&templates->tasks, return_task_from_template, new_task_condition);
        if(task.period != current_period) {
            current_period = task.period;
            current_priority--;
            periods_per_priority[current_priority] = current_period;
        }

        task.priority = current_priority;
        task_queues_enqueue(&task_queues, task);
    }

    #ifdef DEBUG
        printf("Finished setting up the scheduler\n");
        print_task_queues(&task_queues);
    #endif
    return 1;
}

TCB_t get_next_task(void) {
    return task_queues_dequeue(&task_queues);
}

void wait_task(const TCB_t task) {
    task_queues_move_to_pending(&task_queues, task);
}

void release_tasks(void) {
    while(task_queues_restore_from_pending(&task_queues));
}

short should_preempt(const TCB_t *task) {
    return exists_higher_priority_task(&task_queues, task);
}

void store_back_task(const TCB_t *task) {
    task_queues_enqueue(&task_queues, *task);
}

/*
void rms_task_templates_print(const rms_task_templates_t* templates) {
    if (!templates || templates->num_tasks == 0) {
        printf("Task template set is empty.\n");
        return;
    }

    const char* prio_name[] = { "VERY_HIGH", "HIGH", "MEDIUM", "LOW" };

    printf("\n=== RMS Task Templates ===\n");
    printf("%-12s %-10s %-10s %-12s %-12s\n",
        "Task", "Period", "Exec", "Priority", "Util");
    printf("---------------------------------------------------------\n");

    for (int i = 0; i < templates->num_tasks; i++) {
        const TCB_t* t = &templates->task_templates[i];
        double util = (double)t->executionTime / (double)t->period;
        const char* pname = "(invalid)";
        if (t->priority >= 0 && t->priority <= 3)
            pname = prio_name[t->priority];

        printf("%-12s %-10d %-10d %-12s %-12.4f\n",
            t->name ? t->name : "Unnamed",
            t->period,
            t->executionTime,
            pname,
            util);
    }

    printf("---------------------------------------------------------\n\n");
}

void rms_print_analysis(const rms_task_templates_t* templates) {
    if (!templates || templates->num_tasks == 0) {
        printf("Cannot analyze an empty task set.\n");
        return;
    }

    double U = calculate_utilization(templates);
    double bound = calculate_rms_bound(templates->num_tasks);

    printf("\n=== RMS Schedulability Analysis ===\n\n");
    printf("Task count           : %d\n", templates->num_tasks);
    printf("Total utilization U  : %.6f\n", U);
    printf("RMS bound            : %.6f\n", bound);

    if (U <= bound)
        printf("\nSchedulable under RMS.\n");
    else
        printf("\nNot guaranteed schedulable under RMS.\n");

    printf("====================================\n\n");
}
*/
