################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SymbolRanking/SymbolRanking.cpp 

OBJS += \
./SymbolRanking/SymbolRanking.o 

CPP_DEPS += \
./SymbolRanking/SymbolRanking.d 


# Each subdirectory must supply rules for building sources it contributes
SymbolRanking/%.o: ../SymbolRanking/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


