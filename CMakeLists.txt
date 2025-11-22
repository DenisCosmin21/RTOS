cmake_minimum_required(VERSION 3.29)
project(Rtos C)

set(CMAKE_C_STANDARD 99)

add_executable(Rtos main.c
        Queue/queue.c
        Task/task.c
        Memory/memory_pool.c
        Queue/task_queues.c
        RmsScheduler/rms_scheduler.c
        Queue/priority_queue.c
        RtosApi/rtos.c
        Queue/queue.c
        Queue/binary_heap_queue.c
        Queue/binary_heap_queue.h)

target_compile_definitions(Rtos
        PRIVATE
        # Defines 'DEBUG' only if the configuration is 'Debug' (e.g., -DCMAKE_BUILD_TYPE=Debug)
        $<$<CONFIG:Debug>:DEBUG>)
