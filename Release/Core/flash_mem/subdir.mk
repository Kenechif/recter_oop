################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/flash_mem/w25qxx.c 

C_DEPS += \
./Core/flash_mem/w25qxx.d 

OBJS += \
./Core/flash_mem/w25qxx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/flash_mem/%.o: ../Core/flash_mem/%.c Core/flash_mem/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/flash_mem -I../Core/ds1307 -I../Core/timers -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-flash_mem

clean-Core-2f-flash_mem:
	-$(RM) ./Core/flash_mem/w25qxx.d ./Core/flash_mem/w25qxx.o

.PHONY: clean-Core-2f-flash_mem

