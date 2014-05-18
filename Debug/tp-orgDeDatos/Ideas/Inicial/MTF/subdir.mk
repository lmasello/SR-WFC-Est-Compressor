################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tp-orgDeDatos/Ideas/Inicial/MTF/Mtf.cpp 

OBJS += \
./tp-orgDeDatos/Ideas/Inicial/MTF/Mtf.o 

CPP_DEPS += \
./tp-orgDeDatos/Ideas/Inicial/MTF/Mtf.d 


# Each subdirectory must supply rules for building sources it contributes
tp-orgDeDatos/Ideas/Inicial/MTF/%.o: ../tp-orgDeDatos/Ideas/Inicial/MTF/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


