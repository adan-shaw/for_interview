################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../file_operation/xfile.c 

OBJS += \
./file_operation/xfile.o 

C_DEPS += \
./file_operation/xfile.d 


# Each subdirectory must supply rules for building sources it contributes
file_operation/%.o: ../file_operation/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


