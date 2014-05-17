################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FileManager/FileManager.cpp 

OBJS += \
./FileManager/FileManager.o 

CPP_DEPS += \
./FileManager/FileManager.d 


# Each subdirectory must supply rules for building sources it contributes
FileManager/%.o: ../FileManager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


