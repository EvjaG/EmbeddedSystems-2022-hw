################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/motion_detector.c \
../Src/spi1.c \
../Src/usart2.c 

OBJS += \
./Src/main.o \
./Src/motion_detector.o \
./Src/spi1.o \
./Src/usart2.o 

C_DEPS += \
./Src/main.d \
./Src/motion_detector.d \
./Src/spi1.d \
./Src/usart2.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F3 -DDEBUG -DSTM32F303RETx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.d ./Src/main.o ./Src/motion_detector.d ./Src/motion_detector.o ./Src/spi1.d ./Src/spi1.o ./Src/usart2.d ./Src/usart2.o

.PHONY: clean-Src

