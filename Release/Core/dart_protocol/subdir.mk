################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/dart_protocol/drt.c 

C_DEPS += \
./Core/dart_protocol/drt.d 

OBJS += \
./Core/dart_protocol/drt.o 


# Each subdirectory must supply rules for building sources it contributes
Core/dart_protocol/%.o: ../Core/dart_protocol/%.c Core/dart_protocol/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/flash_mem -I../Core/ds1307 -I../Core/timers -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-dart_protocol

clean-Core-2f-dart_protocol:
	-$(RM) ./Core/dart_protocol/drt.d ./Core/dart_protocol/drt.o

.PHONY: clean-Core-2f-dart_protocol

