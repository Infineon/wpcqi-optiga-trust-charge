################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_gpio.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_i2c.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_ifx_i2c_config.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_logger.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_datastore.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_event.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_lock.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_memory.c \
C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_timer.c 

OBJS += \
./optiga_hostlib/pal/xmc4700/pal_gpio.o \
./optiga_hostlib/pal/xmc4700/pal_i2c.o \
./optiga_hostlib/pal/xmc4700/pal_ifx_i2c_config.o \
./optiga_hostlib/pal/xmc4700/pal_logger.o \
./optiga_hostlib/pal/xmc4700/pal_os_datastore.o \
./optiga_hostlib/pal/xmc4700/pal_os_event.o \
./optiga_hostlib/pal/xmc4700/pal_os_lock.o \
./optiga_hostlib/pal/xmc4700/pal_os_memory.o \
./optiga_hostlib/pal/xmc4700/pal_os_timer.o 

C_DEPS += \
./optiga_hostlib/pal/xmc4700/pal_gpio.d \
./optiga_hostlib/pal/xmc4700/pal_i2c.d \
./optiga_hostlib/pal/xmc4700/pal_ifx_i2c_config.d \
./optiga_hostlib/pal/xmc4700/pal_logger.d \
./optiga_hostlib/pal/xmc4700/pal_os_datastore.d \
./optiga_hostlib/pal/xmc4700/pal_os_event.d \
./optiga_hostlib/pal/xmc4700/pal_os_lock.d \
./optiga_hostlib/pal/xmc4700/pal_os_memory.d \
./optiga_hostlib/pal/xmc4700/pal_os_timer.d 


# Each subdirectory must supply rules for building sources it contributes
optiga_hostlib/pal/xmc4700/pal_gpio.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_i2c.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_ifx_i2c_config.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_ifx_i2c_config.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_logger.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_logger.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_os_datastore.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_datastore.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_os_event.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_event.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_os_lock.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_lock.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_os_memory.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_memory.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
optiga_hostlib/pal/xmc4700/pal_os_timer.o: C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/pal/xmc4700/pal_os_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -DPTX_WITH_SE -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/externals/mbedtls/include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/examples/utilities" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PTx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/wpc/PRx" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/common/Dave/Generated" -I"C:/Users/Yushev/git/wpcqi-optiga-trust-charge/optiga/optiga/include" -O0 -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

