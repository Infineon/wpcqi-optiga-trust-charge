################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Startup/system_xmc4700.c 

S_UPPER_SRCS += \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Startup/startup_xmc4700.S 

OBJS += \
./optiga_hostlib/xmc/Startup/startup_xmc4700.o \
./optiga_hostlib/xmc/Startup/system_xmc4700.o 

S_UPPER_DEPS += \
./optiga_hostlib/xmc/Startup/startup_xmc4700.d 

C_DEPS += \
./optiga_hostlib/xmc/Startup/system_xmc4700.d 


# Each subdirectory must supply rules for building sources it contributes
optiga_hostlib/xmc/Startup/startup_xmc4700.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Startup/startup_xmc4700.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC Assembler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -x assembler-with-cpp -DXMC4700_F144x2048 -I"$(PROJECT_LOC)" -Wall -Wa,-adhlns="$@.lst" -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/xmc/Startup/system_xmc4700.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Startup/system_xmc4700.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

