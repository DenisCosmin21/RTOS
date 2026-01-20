#include "rms_scheduler.h"
#include <stdio.h>

#include "globals.h"

// Transforms a bit from 0 to 1
#define SET_BIT(number, position) number |= (1 << position);

// Transforms a bit from 1 to 0
#define RESET_BIT(number, position) number &= ~(1 << position);

// Gets the number of trailing zeros from number
#define GET_TRAILING_ZEROS_COUNT(number)                                       \
  ((number > 0 ? ((MAX_PRIORITY_COUNT - 1) - __builtin_clz(number))            \
               : sizeof(long) * 8))

static float utilization =
    0; // utilization of CPU to identify easily if rots is schedulable without
       // computing the whole sum each time

static scheduler_t global_scheduler; // global variable for scheduler

static rms_task_templates_t
    global_task_templates; // global variable dor task templates

static TCB_t *task_to_add = 0x00;

static int task_cnt =
    0; // Global flag to identify if rtos started or not, for new task logic

const static float RMS_BOUNDS[10] = {
    1.0000, // n=1
    0.8284, // n=2
    0.7798, // n=3
    0.7568, // n=4
    0.7435, // n=5
    0.7348, // n=6
    0.7286, // n=7
    0.7241, // n=8
    0.7206, // n=9
    0.7177, // n=10
};

static int periods_per_priority[NUM_PRIORITY_LEVELS];

void scheduler_set_idle_task(TCB_t *task) { internal_idle_task = task; }

static int new_task_condition(const TCB_t *task, const TCB_t *task2) {
  return task->period < task2->period;
}

static int return_task_from_template(const TCB_t *task) { return 1; }

static int pending_enqueue(const TCB_t *task1, const TCB_t *task2) {
  if (task1->next_release_time > task2->next_release_time)
    return 1;
  if (task1->next_release_time == task2->next_release_time)
    return task1->went_to_sleep_time > task2->went_to_sleep_time;
  return 0;
}

static int should_restore(const TCB_t *task) {
  return task->next_release_time - 1 <= current_time;
}

void scheduler_init(void) {
  scheduler_t *scheduler = &global_scheduler;
  rms_task_templates_t *task_templates = &global_task_templates;

  h_init(&task_templates->tasks);

  for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
    queue_init(&scheduler->queues[i]);
  }

  h_init(&scheduler->pending);
}

void prepare_next_task(TCB_t *task) { task_to_add = task; }

long compute_task_priority(TCB_t *task) {
  for (long i = 0; i < NUM_PRIORITY_LEVELS; i++) {
    if (periods_per_priority[i] == 0) {
      return i;
    }

    if (periods_per_priority[i] == task->period)
      return i;

    if (periods_per_priority[i] < task->period)
      return i;
  }
  return 0;
}

short rms_task_templates_add(TCB_t *task) {
  task_cnt++;
  rms_task_templates_t *task_templates = &global_task_templates;
  h_enqueue(&task_templates->tasks, task, new_task_condition);

  volatile int w = task->worst_case_execution_time;
  volatile int p = task->period;

  utilization +=
      (float)(task->worst_case_execution_time) / (float)(task->period);
#ifdef DEBUG
  printf("Utilization = %f\n", utilization);
#endif

  return 1;
}

void add_next_task() {
  utilization +=
      (task_to_add->worst_case_execution_time * 1000) / task_to_add->period;
  task_cnt++;
  task_to_add->priority = compute_task_priority(task_to_add);
  scheduler_add_task(task_to_add);
}

int calculate_rms_bound(size_t number_of_tasks) {
  if (number_of_tasks > 30)
    return 690;
  return RMS_BOUNDS[number_of_tasks - 1];
}

short is_schedulable(void) {
  if (utilization <= 0.69)
    return 1;

  if (!started) {
    int rms_bound = calculate_rms_bound(task_cnt - 1);

    return utilization <= rms_bound;
  }

#ifdef DEBUG
  printf("Utilization %f <= Max possible time %f", utilization, rms_bound);
#endif
  return (utilization + ((task_to_add->worst_case_execution_time * 1000) /
                         task_to_add->period)) <
         calculate_rms_bound(task_cnt + 1);
}

short start_scheduler(void) {
  rms_task_templates_t *task_templates = &global_task_templates;

  if (!is_schedulable() || h_is_empty(&task_templates->tasks))
    return 0;

  // Lowest priority is MAX_PRIORITY_COUNT - 1
  unsigned long current_priority = 0;

  int current_period = 0;

  // Get a task, get it's period and assign it to lowest priority.
  // This is a priority queue based on period, so first task will be lowest
  // priority always
  TCB_t *task = h_dequeue(&task_templates->tasks, return_task_from_template,
                          new_task_condition);
  current_period = task->period;
  task->priority = current_priority;

  scheduler_add_task(task);

  // Add the period to priority for when i need to add a new task at runtime to
  // know which priority to assign it to
  periods_per_priority[current_priority] = current_period;

  // Execute while we still have tasks to add to scheduler
  while (!h_is_empty(&task_templates->tasks)) {

    task = h_dequeue(&task_templates->tasks, return_task_from_template,
                     new_task_condition);

    // If the period is different it means we have another priority
    if (task->period != current_period) {

      current_period = task->period;

      // As a safety measure we can add higher period tasks to last priority if
      // it exceds the biggest number. Maybe change the task period too? Maybe
      // compute if the rtos can still run like this to know if it should accept
      // it? Maybe return an error code regarding the error found?
      if (current_priority < NUM_PRIORITY_LEVELS - 1) {
        current_priority++;
        periods_per_priority[current_priority] = current_period;
      }
    }

    task->priority = current_priority;
    scheduler_add_task(task);
  }

  running_task = scheduler_get_task();

  started = 1;
#ifdef DEBUG
  printf("Finished setting up the scheduler\n");
  print_task_queues(&task_queues);
#endif
  return 1;
}

short scheduler_add_task(TCB_t *task) {
  scheduler_t *scheduler = &global_scheduler;

  // Chekcs if task has good priority
  if (task->priority < 0 || task->priority >= NUM_PRIORITY_LEVELS) {
    return 0;
  }

  enqueue(&scheduler->queues[task->priority], task);
  SET_BIT(scheduler->bitmap, task->priority);

  return 1;
}

TCB_t *scheduler_get_task(void) {
  scheduler_t *scheduler = &global_scheduler;

  const unsigned long priority = GET_TRAILING_ZEROS_COUNT(scheduler->bitmap);

  if (priority == MAX_PRIORITY_COUNT) // It means 32 trailing zeros => number 0
    return internal_idle_task;

  TCB_t *task = dequeue(&scheduler->queues[priority]);

  if (queue_is_empty(&scheduler->queues[priority]))
    RESET_BIT(scheduler->bitmap, priority);

  return task;
}

void scheduler_dequeue_prio(unsigned long priority) {
  scheduler_t *scheduler = &global_scheduler;
  dequeue(&scheduler->queues[priority]);
}



void scheduler_update_task_priority(TCB_t *task, unsigned long new_priority) {
  scheduler_t *scheduler = &global_scheduler;

  // Remove from old priority queue
  if (task->priority < NUM_PRIORITY_LEVELS) {
    if (queue_remove_task(&scheduler->queues[task->priority], task)) {
      // If queue is now empty, clear the bit
      if (queue_is_empty(&scheduler->queues[task->priority])) {
        RESET_BIT(scheduler->bitmap, task->priority);
      }

      // Update priority and re-add to new queue
      task->priority = new_priority;
      scheduler_add_task(task);
      return;
    }
  }

  // If task was not in the queue (e.g. Running or Blocked), just update the
  // priority
  task->priority = new_priority;
}


short scheduler_sleep_task(TCB_t *task) {
  scheduler_t *scheduler = &global_scheduler;

  h_enqueue(&scheduler->pending, task, pending_enqueue);

  return 1;
}

void scheduler_release_tasks(void) {
  __disable_irq();
  scheduler_t *scheduler = &global_scheduler;

  TCB_t *pending_task = 0x00;

  while ((pending_task = h_dequeue(&scheduler->pending, should_restore,
                                   pending_enqueue)) != internal_idle_task) {
    pending_task->budget_time = pending_task->worst_case_execution_time;
    scheduler_add_task(pending_task);
  }

  __enable_irq();

#ifdef DEBUG
  print_task_queues(tq);
  print_task(&pending_task);
#endif
}

short exists_higher_priority_task(const TCB_t *task) {
  const scheduler_t *scheduler = &global_scheduler;

  unsigned long max_priority = GET_TRAILING_ZEROS_COUNT(scheduler->bitmap);

  if (max_priority == MAX_PRIORITY_COUNT || task->priority >= max_priority)
    return 0;

  return 1;
}

short exists_any_task(void) {
  const scheduler_t *scheduler = &global_scheduler;

  unsigned long max_priority = GET_TRAILING_ZEROS_COUNT(scheduler->bitmap);

  return max_priority != MAX_PRIORITY_COUNT;
}

void print_scheduler() {
  const scheduler_t *scheduler = &global_scheduler;

  printf("Printing task queues: \n");
  for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
    switch (i) {
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
