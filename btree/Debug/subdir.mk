################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../binary_tree.c \
../btree_test_main.c \
../rbtree.c \
../rbtree_test.c 

OBJS += \
./binary_tree.o \
./btree_test_main.o \
./rbtree.o \
./rbtree_test.o 

C_DEPS += \
./binary_tree.d \
./btree_test_main.d \
./rbtree.d \
./rbtree_test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


