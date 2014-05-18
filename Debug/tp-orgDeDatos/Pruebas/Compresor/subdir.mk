################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tp-orgDeDatos/Pruebas/Compresor/Compresor.cpp 

OBJS += \
./tp-orgDeDatos/Pruebas/Compresor/Compresor.o 

CPP_DEPS += \
./tp-orgDeDatos/Pruebas/Compresor/Compresor.d 


# Each subdirectory must supply rules for building sources it contributes
tp-orgDeDatos/Pruebas/Compresor/%.o: ../tp-orgDeDatos/Pruebas/Compresor/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


