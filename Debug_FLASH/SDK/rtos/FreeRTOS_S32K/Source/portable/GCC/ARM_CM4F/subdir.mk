################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../../../../../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"C:/NXP/S32DS_ARM_v2018.R1/S32DS/S32SDK_S32K1xx_RTM_3.0.0/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.c" \

C_SRCS += \
C:/NXP/S32DS_ARM_v2018.R1/S32DS/S32SDK_S32K1xx_RTM_3.0.0/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.c \

OBJS_OS_FORMAT += \
./SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.o \

C_DEPS_QUOTED += \
"./SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.d" \

OBJS += \
./SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.o \

OBJS_QUOTED += \
"./SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.o" \

C_DEPS += \
./SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.d \


# Each subdirectory must supply rules for building sources it contributes
SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.o: C:/NXP/S32DS_ARM_v2018.R1/S32DS/S32SDK_S32K1xx_RTM_3.0.0/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "SDK/rtos/FreeRTOS_S32K/Source/portable/GCC/ARM_CM4F/port.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '


