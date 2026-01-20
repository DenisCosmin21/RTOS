################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/binary_heap_queue.c \
../Src/button.c \
../Src/globals.c \
../Src/led.c \
../Src/main.c \
../Src/memory_pool.c \
../Src/message_queue.c \
../Src/motor.c \
../Src/mutex.c \
../Src/osKernel.c \
../Src/priority_queue.c \
../Src/py_demo.c \
../Src/queue.c \
../Src/rms_scheduler.c \
../Src/rtos.c \
../Src/semaphore.c \
../Src/servo.c \
../Src/seven_segment.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/task.c \
../Src/timebase.c \
../Src/uart.c \
../Src/ultrasonic.c 

OBJS += \
./Src/binary_heap_queue.o \
./Src/button.o \
./Src/globals.o \
./Src/led.o \
./Src/main.o \
./Src/memory_pool.o \
./Src/message_queue.o \
./Src/motor.o \
./Src/mutex.o \
./Src/osKernel.o \
./Src/priority_queue.o \
./Src/py_demo.o \
./Src/queue.o \
./Src/rms_scheduler.o \
./Src/rtos.o \
./Src/semaphore.o \
./Src/servo.o \
./Src/seven_segment.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/task.o \
./Src/timebase.o \
./Src/uart.o \
./Src/ultrasonic.o 

C_DEPS += \
./Src/binary_heap_queue.d \
./Src/button.d \
./Src/globals.d \
./Src/led.d \
./Src/main.d \
./Src/memory_pool.d \
./Src/message_queue.d \
./Src/motor.d \
./Src/mutex.d \
./Src/osKernel.d \
./Src/priority_queue.d \
./Src/py_demo.d \
./Src/queue.d \
./Src/rms_scheduler.d \
./Src/rtos.d \
./Src/semaphore.d \
./Src/servo.d \
./Src/seven_segment.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/task.d \
./Src/timebase.d \
./Src/uart.d \
./Src/ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DSTM32U575xx -c -I../Inc -I/home/traian/RTOS/chip_headers/NUCLEO_CMSIS/Include -I/home/traian/RTOS/chip_headers/NUCLEO_CMSIS/Device -I/home/traian/RTOS/chip_headers/NUCLEO_CMSIS/Device/ST/STM32U5xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/binary_heap_queue.cyclo ./Src/binary_heap_queue.d ./Src/binary_heap_queue.o ./Src/binary_heap_queue.su ./Src/button.cyclo ./Src/button.d ./Src/button.o ./Src/button.su ./Src/globals.cyclo ./Src/globals.d ./Src/globals.o ./Src/globals.su ./Src/led.cyclo ./Src/led.d ./Src/led.o ./Src/led.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/memory_pool.cyclo ./Src/memory_pool.d ./Src/memory_pool.o ./Src/memory_pool.su ./Src/message_queue.cyclo ./Src/message_queue.d ./Src/message_queue.o ./Src/message_queue.su ./Src/motor.cyclo ./Src/motor.d ./Src/motor.o ./Src/motor.su ./Src/mutex.cyclo ./Src/mutex.d ./Src/mutex.o ./Src/mutex.su ./Src/osKernel.cyclo ./Src/osKernel.d ./Src/osKernel.o ./Src/osKernel.su ./Src/priority_queue.cyclo ./Src/priority_queue.d ./Src/priority_queue.o ./Src/priority_queue.su ./Src/py_demo.cyclo ./Src/py_demo.d ./Src/py_demo.o ./Src/py_demo.su ./Src/queue.cyclo ./Src/queue.d ./Src/queue.o ./Src/queue.su ./Src/rms_scheduler.cyclo ./Src/rms_scheduler.d ./Src/rms_scheduler.o ./Src/rms_scheduler.su ./Src/rtos.cyclo ./Src/rtos.d ./Src/rtos.o ./Src/rtos.su ./Src/semaphore.cyclo ./Src/semaphore.d ./Src/semaphore.o ./Src/semaphore.su ./Src/servo.cyclo ./Src/servo.d ./Src/servo.o ./Src/servo.su ./Src/seven_segment.cyclo ./Src/seven_segment.d ./Src/seven_segment.o ./Src/seven_segment.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/task.cyclo ./Src/task.d ./Src/task.o ./Src/task.su ./Src/timebase.cyclo ./Src/timebase.d ./Src/timebase.o ./Src/timebase.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su ./Src/ultrasonic.cyclo ./Src/ultrasonic.d ./Src/ultrasonic.o ./Src/ultrasonic.su

.PHONY: clean-Src

