################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tp-orgDeDatos/Ideas/Inicial/FileManager/FileManager.cpp 

OBJS += \
./tp-orgDeDatos/Ideas/Inicial/FileManager/FileManager.o 

CPP_DEPS += \
./tp-orgDeDatos/Ideas/Inicial/FileManager/FileManager.d 


# Each subdirectory must supply rules for building sources it contributes
tp-orgDeDatos/Ideas/Inicial/FileManager/%.o: ../tp-orgDeDatos/Ideas/Inicial/FileManager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


