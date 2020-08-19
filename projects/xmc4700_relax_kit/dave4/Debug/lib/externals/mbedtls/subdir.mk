################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/aes.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/ccm.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/cipher.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/cipher_wrap.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/md.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/md_wrap.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/platform_util.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/sha256.c \
C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/ssl_ciphersuites.c 

OBJS += \
./lib/externals/mbedtls/aes.o \
./lib/externals/mbedtls/ccm.o \
./lib/externals/mbedtls/cipher.o \
./lib/externals/mbedtls/cipher_wrap.o \
./lib/externals/mbedtls/md.o \
./lib/externals/mbedtls/md_wrap.o \
./lib/externals/mbedtls/platform_util.o \
./lib/externals/mbedtls/sha256.o \
./lib/externals/mbedtls/ssl_ciphersuites.o 

C_DEPS += \
./lib/externals/mbedtls/aes.d \
./lib/externals/mbedtls/ccm.d \
./lib/externals/mbedtls/cipher.d \
./lib/externals/mbedtls/cipher_wrap.d \
./lib/externals/mbedtls/md.d \
./lib/externals/mbedtls/md_wrap.d \
./lib/externals/mbedtls/platform_util.d \
./lib/externals/mbedtls/sha256.d \
./lib/externals/mbedtls/ssl_ciphersuites.d 


# Each subdirectory must supply rules for building sources it contributes
lib/externals/mbedtls/aes.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/aes.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/ccm.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/ccm.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/cipher.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/cipher.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/cipher_wrap.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/cipher_wrap.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/md.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/md.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/md_wrap.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/md_wrap.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/platform_util.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/platform_util.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/sha256.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/sha256.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
lib/externals/mbedtls/ssl_ciphersuites.o: C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/library/ssl_ciphersuites.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4700_F144x2048 -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/XMCLib/inc" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Include" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Libraries/CMSIS/Infineon/XMC4700_series/Include" -I"$(PROJECT_LOC)" -I"C:/Users/Yushev/git/optiga-trust-qi/projects/xmc4700_relax_kit/common/Dave/Generated" -I"C:/Users/Yushev/git/optiga-trust-qi/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/examples/optiga/include" -I"C:/Users/Yushev/git/optiga-trust-qi/externals/mbedtls/include" -Os -ffunction-sections -fdata-sections -Wall -Werror -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

