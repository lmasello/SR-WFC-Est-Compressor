################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../MTF/Mtf.cpp 

OBJS += \
./MTF/Mtf.o 

CPP_DEPS += \
./MTF/Mtf.d 


# Each subdirectory must supply rules for building sources it contributes
MTF/%.o: ../MTF/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


