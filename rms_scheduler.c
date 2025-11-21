#include "rms_scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void rms_task_templates_init(rms_task_templates_t* templates) {
    if (!templates) return;
    templates->num_tasks = 0;
    for (int i = 0; i < MAX_TASKS; i++)
        templates->task_templates[i] = createEmptyTask();  //SE POT INITIALIZA COZILE DIRECT SI SCAPI DE STRUCTURA TEMPLATES
}

bool rms_task_templates_add(rms_task_templates_t* templates, TCB_t task) {
    if (!templates) return false;
    if (templates->num_tasks >= MAX_TASKS) return false;
    if (task.period <= 0 || task.executionTime <= 0) return false;
    templates->task_templates[templates->num_tasks++] = task;     //TASK UL ARE DEJA O PRIORITATE CAND IL CREEZI SI DOAR VOOM DA ADD TASK
    return true;
}

double calculate_utilization(const rms_task_templates_t* templates) {
    if (!templates || templates->num_tasks == 0) return 0.0;
    double U = 0.0;
    for (int i = 0; i < templates->num_tasks; i++)
        U += (double)templates->task_templates[i].executionTime /
        (double)templates->task_templates[i].period;
    return U;
}

double calculate_rms_bound(int n) {
    if (n <= 0) return 0.0;
    return n * (pow(2.0, 1.0 / n) - 1.0);
}

bool is_schedulable(const rms_task_templates_t* templates) {
    if (!templates || templates->num_tasks == 0) return false;
    return calculate_utilization(templates) <= calculate_rms_bound(templates->num_tasks);
}

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
