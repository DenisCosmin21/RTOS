cmake_minimum_required(VERSION 3.29)
project(Rtos C)

set(CMAKE_C_STANDARD 99)

add_executable(Rtos main.c
        barrier.c
        barrier.h
        mutex.c
        mutex.h)
