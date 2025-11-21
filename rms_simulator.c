#include "rms_simulator.h"
#include "circular_queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global time variable
int global_time = 0;


void rms_simulator_init(rms_simulator_t* sim, int max_time, bool verbose) {
    if (sim == NULL) {
        return;
    }

    rms_task_templates_init(&sim->task_templates);
    task_queues_init(&sim->ready_queues);
    sim->simulation_time = 0;
    sim->max_simulation_time = max_time;
    sim->running_task = createEmptyTask();
    sim->has_running_task = false;
    sim->verbose = verbose; //PT PRINTURI
    global_time = 0;
}

bool rms_simulator_add_task(rms_simulator_t* sim, TCB_t task) {
    if (sim == NULL) {
        return false;
    }

    // Set initial release time to 0 (all tasks start at time 0)
    task.nextReleaseTime = 0;
    task.remainingTime = task.executionTime;

    // Debug: Print priority when adding task
    const char* priority_names[] = { "VERY_HIGH", "HIGH", "MEDIUM", "LOW" };
    printf("Adding task %s with priority %s (value=%d)\n",
        task.name, priority_names[task.priority], task.priority);

    // Add to templates for release checking
    bool result = rms_task_templates_add(&sim->task_templates, task);

    // Verify priority was preserved in template
    if (result && sim->task_templates.num_tasks > 0) {
        int idx = sim->task_templates.num_tasks - 1;
        printf("  -> Template priority: %s (value=%d)\n",
            priority_names[sim->task_templates.task_templates[idx].priority],
            sim->task_templates.task_templates[idx].priority);
    }

    return result;
}

void rms_simulator_check_releases(rms_simulator_t* sim) {
    if (sim == NULL) {
        return;
    }

    // Check all task templates for release
    for (int i = 0; i < sim->task_templates.num_tasks; i++) {
        TCB_t* template = &sim->task_templates.task_templates[i];

        // Check if task should be released
        if (template->nextReleaseTime == sim->simulation_time) {
            // Create a ready task instance from the template
            TCB_t ready_task = *template;
            ready_task.remainingTime = template->executionTime;

            // Enqueue to the appropriate priority queue
            if (task_queues_enqueue(&sim->ready_queues, ready_task)) {
                if (sim->verbose) {
                    const char* priority_names[] = { "VERY_HIGH", "HIGH", "MEDIUM", "LOW" };
                    printf("[Time %3d] Task %s RELEASED -> Enqueued to %s queue (Priority: %d)\n",
                        sim->simulation_time, template->name,
                        priority_names[template->priority], template->priority);
                }
            }
            else {
                if (sim->verbose) {
                    printf("[Time %3d] Task %s RELEASED but FAILED to enqueue!\n",
                        sim->simulation_time, template->name);
                }
            }

            // Calculate next release time in template
            template->nextReleaseTime = sim->simulation_time + template->period;
        }
    }
}

TCB_t* rms_simulator_select_task(rms_simulator_t* sim) {
    if (sim == NULL) {
        return NULL;
    }

    // Check if there's already a running task that should continue
    if (sim->has_running_task && sim->running_task.remainingTime > 0) {
        // Check if a higher priority task is available in ready queues
        bool higher_priority_available = false;

        // Peek at highest priority queue to check for preemption
        for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
            if (!circular_queue_is_empty(&sim->ready_queues.priority_queues[i])) {
                TCB_t peeked_task = circular_peek(&sim->ready_queues.priority_queues[i]);
                // If peeked task has higher priority (lower enum value), preempt
                if (peeked_task.priority < sim->running_task.priority) {
                    higher_priority_available = true;
                }
                break;
            }
        }

        // If no higher priority task, continue with current
        if (!higher_priority_available) {
            return &sim->running_task;
        }

        // Otherwise, preempt current task - move to pending queue
        task_queues_move_to_pending(&sim->ready_queues, sim->running_task);
        if (sim->verbose) {
            printf("[Time %3d] Task %s PREEMPTED (moved to pending)\n",
                sim->simulation_time, sim->running_task.name);
        }
    }

    // Dequeue the highest priority ready task
    TCB_t selected = task_queues_dequeue(&sim->ready_queues);

    // Check if we got a valid task
    if (selected.executionTime < 0) {
        sim->has_running_task = false;
        return NULL;
    }

    // Store as running task
    sim->running_task = selected;
    sim->has_running_task = true;
    return &sim->running_task;
}

void rms_simulator_execute_task(rms_simulator_t* sim, TCB_t* task) {
    if (sim == NULL || task == NULL) {
        return;
    }

    // Execute the task for one time unit
    task->remainingTime--;

    // Update running task in simulator
    sim->running_task = *task;
    sim->has_running_task = true;

    // Check if task completed after this execution
    if (task->remainingTime == 0) {
        sim->has_running_task = false;

        if (sim->verbose) {
            // Find the template to get next release time
            int next_release = -1;
            for (int i = 0; i < sim->task_templates.num_tasks; i++) {
                if (sim->task_templates.task_templates[i].name == task->name) {
                    next_release = sim->task_templates.task_templates[i].nextReleaseTime;
                    break;
                }
            }
            printf("[Time %3d] Task %s COMPLETED\n", sim->simulation_time, task->name);
        }

        // When task completes, restore tasks from pending queue that need to finish
        if (!circular_queue_is_empty(&sim->ready_queues.pending_queue)) {
            task_queues_restore_from_pending(&sim->ready_queues);
            if (sim->verbose) {
                printf("[Time %3d] Task restored from pending queue\n", sim->simulation_time);
            }
        }
    }
}

void rms_simulator_reset_periods(rms_simulator_t* sim) {
    // Tasks are reset when they complete
}

void rms_simulator_step(rms_simulator_t* sim) {
    if (sim == NULL) {
        return;
    }

    global_time = sim->simulation_time;

    // Check for task releases
    rms_simulator_check_releases(sim);

    // Print queue status after releases (before dequeuing)
    if (sim->verbose) {
        printf("\n[Time %3d] Queue status after releases:\n", sim->simulation_time);
        task_queues_print_status(&sim->ready_queues);
    }

    // Reset completed tasks
    rms_simulator_reset_periods(sim);

    // Select highest priority ready task
    TCB_t* selected_task = rms_simulator_select_task(sim);

    if (selected_task != NULL) {
        // Execute the selected task for one time unit
        rms_simulator_execute_task(sim, selected_task);

        // Print execution info
        const char* priority_names[] = { "VERY_HIGH", "HIGH", "MEDIUM", "LOW" };
        if (sim->verbose) {
            printf("[Time %3d] Executing: %s (Priority: %s, Remaining: %d)\n",
                sim->simulation_time, selected_task->name,
                priority_names[selected_task->priority],
                selected_task->remainingTime);
        }
        else {
            // Compact format
            printf("[Time %3d] %s RUNNING (Remaining: %d)\n",
                sim->simulation_time, selected_task->name,
                selected_task->remainingTime);
        }
    }
    else {
        // CPU is idle
        if (sim->verbose) {
            printf("[Time %3d] CPU IDLE (no ready tasks)\n", sim->simulation_time);
        }
        else {
            printf("[Time %3d] IDLE\n", sim->simulation_time);
        }
        sim->has_running_task = false;
        sim->running_task = createEmptyTask();
    }

    // Advance time
    sim->simulation_time++;
}

void rms_simulator_print_state(const rms_simulator_t* sim) {
    if (sim == NULL) {
        return;
    }

    printf("\n=== Simulation State at Time %d ===\n", sim->simulation_time);
    printf("%-10s %-10s %-10s %-10s %-15s\n",
        "Task", "Priority", "Remaining", "Next Release", "Period");
    printf("------------------------------------------------------------\n");

    // Print task templates (for release tracking)
    for (int i = 0; i < sim->task_templates.num_tasks; i++) {
        const TCB_t* template = &sim->task_templates.task_templates[i];
        const char* priority_names[] = { "VERY_HIGH", "HIGH", "MEDIUM", "LOW" };

        printf("%-10s %-10s %-10d %-10d %-15d\n",
            template->name ? template->name : "Unknown",
            priority_names[template->priority],
            template->executionTime,
            template->nextReleaseTime,
            template->period);
    }

    // Print queue status (includes pending queue)
    printf("\nQueue Status:\n");
    task_queues_print_status(&sim->ready_queues);

    if (sim->has_running_task) {
        printf("\nCurrently Running: %s\n", sim->running_task.name);
    }
    else {
        printf("\nCPU: IDLE\n");
    }
    printf("=====================================\n\n");
}

void rms_simulator_run(rms_simulator_t* sim) {
    if (sim == NULL) {
        return;
    }

    printf("\n=== Starting RMS Simulation ===\n");
    printf("Simulation Time: 0 to %d\n", sim->max_simulation_time);
    printf("Number of Tasks: %d\n", sim->task_templates.num_tasks);
    printf("===============================\n\n");

    // Print initial state
    rms_simulator_print_state(sim);

    // Run simulation
    while (sim->simulation_time < sim->max_simulation_time) {
        rms_simulator_step(sim);

        // Print state every few time units if verbose
        if (sim->verbose && sim->simulation_time % 5 == 0) {
            rms_simulator_print_state(sim);
        }
    }

    // Print final state
    printf("\n=== Final Simulation State ===\n");
    rms_simulator_print_state(sim);

    printf("\n=== Simulation Complete ===\n\n");
}


