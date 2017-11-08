################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../auto_test_sort.c \
../double_bubble_sort.c \
../quick_sort.c \
../random_int.c 

OBJS += \
./auto_test_sort.o \
./double_bubble_sort.o \
./quick_sort.o \
./random_int.o 

C_DEPS += \
./auto_test_sort.d \
./double_bubble_sort.d \
./quick_sort.d \
./random_int.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


