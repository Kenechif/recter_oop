################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/EEPROM.c \
../Core/Src/buffer.c \
../Core/Src/command.c \
../Core/Src/interrupt_codes.c \
../Core/Src/keypad_module.c \
../Core/Src/log.c \
../Core/Src/main.c \
../Core/Src/myLibraries.c \
../Core/Src/settings.c \
../Core/Src/states.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/thread1.c \
../Core/Src/write__.c 

C_DEPS += \
./Core/Src/EEPROM.d \
./Core/Src/buffer.d \
./Core/Src/command.d \
./Core/Src/interrupt_codes.d \
./Core/Src/keypad_module.d \
./Core/Src/log.d \
./Core/Src/main.d \
./Core/Src/myLibraries.d \
./Core/Src/settings.d \
./Core/Src/states.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/thread1.d \
./Core/Src/write__.d 

OBJS += \
./Core/Src/EEPROM.o \
./Core/Src/buffer.o \
./Core/Src/command.o \
./Core/Src/interrupt_codes.o \
./Core/Src/keypad_module.o \
./Core/Src/log.o \
./Core/Src/main.o \
./Core/Src/myLibraries.o \
./Core/Src/settings.o \
./Core/Src/states.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/thread1.o \
./Core/Src/write__.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Core/flash_mem -I../Core/ds1307 -I../Core/timers -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/EEPROM.d ./Core/Src/EEPROM.o ./Core/Src/buffer.d ./Core/Src/buffer.o ./Core/Src/command.d ./Core/Src/command.o ./Core/Src/interrupt_codes.d ./Core/Src/interrupt_codes.o ./Core/Src/keypad_module.d ./Core/Src/keypad_module.o ./Core/Src/log.d ./Core/Src/log.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/myLibraries.d ./Core/Src/myLibraries.o ./Core/Src/settings.d ./Core/Src/settings.o ./Core/Src/states.d ./Core/Src/states.o ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/thread1.d ./Core/Src/thread1.o ./Core/Src/write__.d ./Core/Src/write__.o

.PHONY: clean-Core-2f-Src

