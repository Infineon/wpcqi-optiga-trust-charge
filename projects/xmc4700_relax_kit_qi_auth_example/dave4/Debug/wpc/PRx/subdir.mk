################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx/qi_auth_prx.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx/qi_auth_prx_crypt.c 

OBJS += \
./wpc/PRx/qi_auth_prx.o \
./wpc/PRx/qi_auth_prx_crypt.o 

C_DEPS += \
./wpc/PRx/qi_auth_prx.d \
./wpc/PRx/qi_auth_prx_crypt.d 


# Each subdirectory must supply rules for building sources it contributes
wpc/PRx/qi_auth_prx.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx/qi_auth_prx.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
wpc/PRx/qi_auth_prx_crypt.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx/qi_auth_prx_crypt.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

