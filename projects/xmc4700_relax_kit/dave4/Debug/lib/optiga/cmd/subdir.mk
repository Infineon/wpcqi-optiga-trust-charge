################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/optiga-trust-qi/optiga/cmd/optiga_cmd.c 

OBJS += \
./lib/optiga/cmd/optiga_cmd.o 

C_DEPS += \
./lib/optiga/cmd/optiga_cmd.d 


# Each subdirectory must supply rules for building sources it contributes
lib/optiga/cmd/optiga_cmd.o: C:/Users/Yushev/git/optiga-trust-qi/optiga/cmd/optiga_cmd.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
