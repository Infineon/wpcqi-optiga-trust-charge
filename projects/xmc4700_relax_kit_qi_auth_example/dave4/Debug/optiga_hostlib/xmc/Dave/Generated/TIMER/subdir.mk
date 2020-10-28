################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated/TIMER/timer.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated/TIMER/timer_conf.c 

OBJS += \
./optiga_hostlib/xmc/Dave/Generated/TIMER/timer.o \
./optiga_hostlib/xmc/Dave/Generated/TIMER/timer_conf.o 

C_DEPS += \
./optiga_hostlib/xmc/Dave/Generated/TIMER/timer.d \
./optiga_hostlib/xmc/Dave/Generated/TIMER/timer_conf.d 


# Each subdirectory must supply rules for building sources it contributes
optiga_hostlib/xmc/Dave/Generated/TIMER/timer.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated/TIMER/timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/xmc/Dave/Generated/TIMER/timer_conf.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated/TIMER/timer_conf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

