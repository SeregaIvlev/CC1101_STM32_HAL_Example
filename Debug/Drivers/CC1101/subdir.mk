################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CC1101/CC1101.c \
../Drivers/CC1101/CC1101_port.c 

OBJS += \
./Drivers/CC1101/CC1101.o \
./Drivers/CC1101/CC1101_port.o 

C_DEPS += \
./Drivers/CC1101/CC1101.d \
./Drivers/CC1101/CC1101_port.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CC1101/%.o: ../Drivers/CC1101/%.c Drivers/CC1101/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"C:/!MEGA/!myFiles/STM/CC1101_HAL_Example/Drivers" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CC1101

clean-Drivers-2f-CC1101:
	-$(RM) ./Drivers/CC1101/CC1101.d ./Drivers/CC1101/CC1101.o ./Drivers/CC1101/CC1101_port.d ./Drivers/CC1101/CC1101_port.o

.PHONY: clean-Drivers-2f-CC1101

