################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../cre_sock.c \
../if_event.c \
../main.c \
../main_func.c \
../pthread_listen.c \
../pthread_recv.c 

OBJS += \
./cre_sock.o \
./if_event.o \
./main.o \
./main_func.o \
./pthread_listen.o \
./pthread_recv.o 

C_DEPS += \
./cre_sock.d \
./if_event.d \
./main.d \
./main_func.d \
./pthread_listen.d \
./pthread_recv.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


