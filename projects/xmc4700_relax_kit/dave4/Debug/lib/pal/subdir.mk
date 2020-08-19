################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/pal_crypt_mbedtls.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_gpio.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_i2c.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_ifx_i2c_config.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_logger.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_datastore.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_event.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_lock.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_memory.c \
C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_timer.c 

OBJS += \
./lib/pal/pal.o \
./lib/pal/pal_crypt_mbedtls.o \
./lib/pal/pal_gpio.o \
./lib/pal/pal_i2c.o \
./lib/pal/pal_ifx_i2c_config.o \
./lib/pal/pal_logger.o \
./lib/pal/pal_os_datastore.o \
./lib/pal/pal_os_event.o \
./lib/pal/pal_os_lock.o \
./lib/pal/pal_os_memory.o \
./lib/pal/pal_os_timer.o 

C_DEPS += \
./lib/pal/pal.d \
./lib/pal/pal_crypt_mbedtls.d \
./lib/pal/pal_gpio.d \
./lib/pal/pal_i2c.d \
./lib/pal/pal_ifx_i2c_config.d \
./lib/pal/pal_logger.d \
./lib/pal/pal_os_datastore.d \
./lib/pal/pal_os_event.d \
./lib/pal/pal_os_lock.d \
./lib/pal/pal_os_memory.d \
./lib/pal/pal_os_timer.d 


# Each subdirectory must supply rules for building sources it contributes
lib/pal/pal.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_crypt_mbedtls.o: C:/Users/Yushev/git/optiga-trust-qi/pal/pal_crypt_mbedtls.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_gpio.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_i2c.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_ifx_i2c_config.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_ifx_i2c_config.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_logger.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_logger.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_os_datastore.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_datastore.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_os_event.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_event.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_os_lock.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_lock.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_os_memory.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_memory.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/pal/pal_os_timer.o: C:/Users/Yushev/git/optiga-trust-qi/pal/xmc4700/pal_os_timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

