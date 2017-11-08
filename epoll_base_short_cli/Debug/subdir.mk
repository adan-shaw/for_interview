################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../inet_connect.c \
../main.c \
../main_func.c \
../send_once.c \
../time.c 

OBJS += \
./inet_connect.o \
./main.o \
./main_func.o \
./send_once.o \
./time.o 

C_DEPS += \
./inet_connect.d \
./main.d \
./main_func.d \
./send_once.d \
./time.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


