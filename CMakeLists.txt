cmake_minimum_required(VERSION 3.29)
project(Rtos C)

set(CMAKE_C_STANDARD 99)

add_executable(Rtos main.c
        queue.c
        task.c
        memory_pool.c
        task_queues.c
        rms_scheduler.c
        priority_queue.c
        rtos.c)

