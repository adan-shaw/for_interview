################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../array_struct/array_queue.c \
../array_struct/array_stack.c 

OBJS += \
./array_struct/array_queue.o \
./array_struct/array_stack.o 

C_DEPS += \
./array_struct/array_queue.d \
./array_struct/array_stack.d 


# Each subdirectory must supply rules for building sources it contributes
array_struct/%.o: ../array_struct/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

