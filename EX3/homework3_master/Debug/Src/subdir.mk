################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/modem.c \
../Src/usart2.c \
../Src/usart3.c 

OBJS += \
./Src/main.o \
./Src/modem.o \
./Src/usart2.o \
./Src/usart3.o 

C_DEPS += \
./Src/main.d \
./Src/modem.d \
./Src/usart2.d \
./Src/usart3.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32 -DSTM32F3 -DDEBUG -DSTM32F303RETx -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.d ./Src/main.o ./Src/modem.d ./Src/modem.o ./Src/usart2.d ./Src/usart2.o ./Src/usart3.d ./Src/usart3.o

.PHONY: clean-Src

