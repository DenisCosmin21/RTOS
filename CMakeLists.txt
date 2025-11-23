cmake_minimum_required(VERSION 3.29)
project(Rtos C)

set(CMAKE_C_STANDARD 99)

add_executable(Rtos main.c
        Globals/globals.c
        Queue/queue.c
        Task/task.c
        Memory/memory_pool.c
        RmsScheduler/rms_scheduler.c
        Queue/priority_queue.c
        RtosApi/rtos.c
        Queue/queue.c
        Queue/binary_heap_queue.c)

