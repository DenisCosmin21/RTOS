#ifndef RMS_SIMULATOR_H
#define RMS_SIMULATOR_H

#include "rms_scheduler.h"
#include "task_queues.h"
#include <stdbool.h>

// Global time for simulation
extern int global_time;

// Structure to hold simulation state
typedef struct {
    rms_task_templates_t task_templates;  // Task templates for release checking
    task_queues_t ready_queues;           // Queues for ready tasks
    int simulation_time;     // Current simulation time
    int max_simulation_time; // Maximum time to simulate
    TCB_t running_task;      // Currently running task (empty if idle)
    bool has_running_task;   // Whether there's a running task
    bool verbose;            // Print detailed information
} rms_simulator_t;

// Initialize the simulator
void rms_simulator_init(rms_simulator_t* sim, int max_time, bool verbose);

// Add a task to the simulator
bool rms_simulator_add_task(rms_simulator_t* sim, TCB_t task);

// Run the simulation
void rms_simulator_run(rms_simulator_t* sim);

// Advance simulation by one time unit
void rms_simulator_step(rms_simulator_t* sim);

// Check for task releases at current time
void rms_simulator_check_releases(rms_simulator_t* sim);

// Select the highest priority ready task
TCB_t* rms_simulator_select_task(rms_simulator_t* sim);

// Execute the selected task for one time unit
void rms_simulator_execute_task(rms_simulator_t* sim, TCB_t* task);

// Print current simulation state
void rms_simulator_print_state(const rms_simulator_t* sim);

// Reset tasks for next period
void rms_simulator_reset_periods(rms_simulator_t* sim);

#endif // RMS_SIMULATOR_H

