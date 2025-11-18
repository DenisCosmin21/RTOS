#include <stdio.h>
#include "rms_scheduler.h"
#include "rms_simulator.h"
#include "task.h"
#include "task_queues.h"

int main(void) {
    printf("=== Rate Monotonic Scheduling (RMS) Simulation ===\n\n");

    // Initialize simulator for 10 time units
    rms_simulator_t simulator;
    rms_simulator_init(&simulator, 10, true);

    // Add tasks with hardcoded priorities - one for each priority level
    TCB_t task1 = newTask("Task1", 2, 4, VERY_HIGH);  // Highest priority
    task1.nextReleaseTime = 0;

    TCB_t task2 = newTask("Task2", 4, 4, VERY_HIGH); //SE POT SCOATE PRIORITATILE HARDCODATE DACA FACEM FUNCTIE
    task2.nextReleaseTime = 0;

    TCB_t task3 = newTask("Task3", 4, 8, MEDIUM);
    task3.nextReleaseTime = 0;

    TCB_t task4 = newTask("Task4", 3, 10, LOW);  // Lowest priority
    task4.nextReleaseTime = 0;

    rms_simulator_add_task(&simulator, task1);
    rms_simulator_add_task(&simulator, task2);
    rms_simulator_add_task(&simulator, task3);
    rms_simulator_add_task(&simulator, task4);

    // Run simulation
    rms_simulator_run(&simulator);

    return 0;
}