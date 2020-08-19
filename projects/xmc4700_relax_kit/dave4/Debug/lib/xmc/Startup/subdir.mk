################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Startup/system_xmc4700.c 

S_UPPER_SRCS += \
C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Startup/startup_xmc4700.S 

OBJS += \
./lib/xmc/Startup/startup_xmc4700.o \
./lib/xmc/Startup/system_xmc4700.o 

S_UPPER_DEPS += \
./lib/xmc/Startup/startup_xmc4700.d 

C_DEPS += \
./lib/xmc/Startup/system_xmc4700.d 


# Each subdirectory must supply rules for building sources it contributes
lib/xmc/Startup/startup_xmc4700.o: C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Startup/startup_xmc4700.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC Assembler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -x assembler-with-cpp -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -Wall -Wa,-adhlns="$@.lst" -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/xmc/Startup/system_xmc4700.o: C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Startup/system_xmc4700.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

