################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/server/serialdebug/SerialDebug.cpp 

OBJS += \
./src/server/serialdebug/SerialDebug.o 

CPP_DEPS += \
./src/server/serialdebug/SerialDebug.d 


# Each subdirectory must supply rules for building sources it contributes
src/server/serialdebug/%.o: ../src/server/serialdebug/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	ccache g++ -D__GXX_EXPERIMENTAL_CXX0X__ -D__cplusplus=201103L -DBOOST_LOG_DYN_LINK -DDEBUG -I/usr/include/i386-linux-gnu/c++/4.8 -I/usr/include/c++/4.8 -O3 -g -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

