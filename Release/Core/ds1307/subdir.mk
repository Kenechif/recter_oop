################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ds1307/ds1307_for_stm32_hal.c 

C_DEPS += \
./Core/ds1307/ds1307_for_stm32_hal.d 

OBJS += \
./Core/ds1307/ds1307_for_stm32_hal.o 


# Each subdirectory must supply rules for building sources it contributes
Core/ds1307/%.o: ../Core/ds1307/%.c Core/ds1307/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/flash_mem -I../Core/ds1307 -I../Core/timers -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-ds1307

clean-Core-2f-ds1307:
	-$(RM) ./Core/ds1307/ds1307_for_stm32_hal.d ./Core/ds1307/ds1307_for_stm32_hal.o

.PHONY: clean-Core-2f-ds1307

