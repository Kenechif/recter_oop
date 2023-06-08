################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/timers/timers.c 

C_DEPS += \
./Core/timers/timers.d 

OBJS += \
./Core/timers/timers.o 


# Each subdirectory must supply rules for building sources it contributes
Core/timers/%.o: ../Core/timers/%.c Core/timers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/flash_mem -I../Core/ds1307 -I../Core/timers -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-timers

clean-Core-2f-timers:
	-$(RM) ./Core/timers/timers.d ./Core/timers/timers.o

.PHONY: clean-Core-2f-timers

