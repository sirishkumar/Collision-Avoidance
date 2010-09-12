################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CollisionAvoidance.cpp \
../src/ControlStation.cpp \
../src/LocationSubSysSim.cpp \
../src/Message.cpp \
../src/Section.cpp \
../src/Train.cpp 

OBJS += \
./src/CollisionAvoidance.o \
./src/ControlStation.o \
./src/LocationSubSysSim.o \
./src/Message.o \
./src/Section.o \
./src/Train.o 

CPP_DEPS += \
./src/CollisionAvoidance.d \
./src/ControlStation.d \
./src/LocationSubSysSim.d \
./src/Message.d \
./src/Section.d \
./src/Train.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/sirishkumar/projects/collisionavoidance/inc" -I"/home/sirishkumar/projects/collisionavoidance/inc" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


