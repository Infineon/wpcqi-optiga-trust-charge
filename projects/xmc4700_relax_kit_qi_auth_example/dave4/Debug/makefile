################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include wpc/PTx/subdir.mk
-include wpc/PRx/subdir.mk
-include wpc/subdir.mk
-include optiga_hostlib/xmc/XMCLib/src/subdir.mk
-include optiga_hostlib/xmc/Startup/subdir.mk
-include optiga_hostlib/xmc/Newlib/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/USBD_VCOM/virtual_com/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/USBD_VCOM/class_driver/device/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/USBD_VCOM/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/USBD/usb/core/xmc4000/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/USBD/usb/core/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/USBD/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/UART/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/TIMER/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/INTERRUPT/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/I2C_MASTER/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/GLOBAL_CCU4/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/DIGITAL_IO/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/CPU_CTRL_XMC4/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/CLOCK_XMC4/subdir.mk
-include optiga_hostlib/xmc/Dave/Generated/subdir.mk
-include optiga_hostlib/pal/xmc4700/subdir.mk
-include optiga_hostlib/pal/subdir.mk
-include optiga_hostlib/optiga/util/subdir.mk
-include optiga_hostlib/optiga/crypt/subdir.mk
-include optiga_hostlib/optiga/comms/ifx_i2c/subdir.mk
-include optiga_hostlib/optiga/comms/subdir.mk
-include optiga_hostlib/optiga/common/subdir.mk
-include optiga_hostlib/optiga/cmd/subdir.mk
-include optiga_hostlib/examples/utilities/subdir.mk
-include 3rd_party/mbedtls/library/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
xmc4700_qi_auth_example.hex \

SECONDARY_LIST += \
xmc4700_qi_auth_example.lst \

SECONDARY_SIZE += \
xmc4700_qi_auth_example.siz \


LIST_FILES = $(addsuffix .o.lst, $(basename $(OBJS))) 
MAP_FILE = "xmc4700_qi_auth_example.map"

TOOLCHAIN_ROOT=C:/Users/Yushev/DAVE_4-4-2_32bit_2018-02-23/DAVE-IDE-4.4.2-32Bit/eclipse/ARM-GCC-49
PROJECT_LOC=C:/Users/Yushev/git/wpcqi-optiga-trust-charge/projects/xmc4700_relax_kit_qi_auth_example/dave4

# All Target
all: xmc4700_qi_auth_example.elf secondary-outputs

# Tool invocations
xmc4700_qi_auth_example.elf: $(OBJS) ../linker_script.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM-GCC C Linker'
	$(file >objects.rsp,$(OBJS))
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -T"../linker_script.ld" -nostartfiles -Xlinker --gc-sections -specs=nano.specs -specs=nosys.specs -Wl,-Map,$(MAP_FILE) -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mthumb -g -gdwarf-2 -o "xmc4700_qi_auth_example.elf" "@objects.rsp" $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo.
xmc4700_qi_auth_example.hex: xmc4700_qi_auth_example.elf
	@echo 'Invoking: ARM-GCC Create Flash Image'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy" -O ihex "xmc4700_qi_auth_example.elf" "xmc4700_qi_auth_example.hex"
	@echo 'Finished building: $@'
	@echo.
xmc4700_qi_auth_example.lst: xmc4700_qi_auth_example.elf
	@echo 'Invoking: ARM-GCC Create Listing'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objdump" -h -S "xmc4700_qi_auth_example.elf" > "xmc4700_qi_auth_example.lst"
	@echo 'Finished building: $@'
	@echo.
xmc4700_qi_auth_example.siz: xmc4700_qi_auth_example.elf
	@echo 'Invoking: ARM-GCC Print Size'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-size" --format=berkeley "xmc4700_qi_auth_example.elf"
	@echo 'Finished building: $@'
	@echo.
# Other Targets
clean:
	-$(RM) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ARM_GCC_FOR_XMC_LINKER_OUTPUT_TYPE_OUTPUTS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)
	-$(RM) 3rd_party/mbedtls/library/aes.o 3rd_party/mbedtls/library/aesni.o 3rd_party/mbedtls/library/arc4.o 3rd_party/mbedtls/library/aria.o 3rd_party/mbedtls/library/asn1parse.o 3rd_party/mbedtls/library/asn1write.o 3rd_party/mbedtls/library/base64.o 3rd_party/mbedtls/library/bignum.o 3rd_party/mbedtls/library/blowfish.o 3rd_party/mbedtls/library/camellia.o 3rd_party/mbedtls/library/ccm.o 3rd_party/mbedtls/library/certs.o 3rd_party/mbedtls/library/chacha20.o 3rd_party/mbedtls/library/chachapoly.o 3rd_party/mbedtls/library/cipher.o 3rd_party/mbedtls/library/cipher_wrap.o 3rd_party/mbedtls/library/cmac.o 3rd_party/mbedtls/library/ctr_drbg.o 3rd_party/mbedtls/library/debug.o 3rd_party/mbedtls/library/des.o 3rd_party/mbedtls/library/dhm.o 3rd_party/mbedtls/library/ecdh.o 3rd_party/mbedtls/library/ecdsa.o 3rd_party/mbedtls/library/ecjpake.o 3rd_party/mbedtls/library/ecp.o 3rd_party/mbedtls/library/ecp_curves.o 3rd_party/mbedtls/library/entropy.o 3rd_party/mbedtls/library/entropy_poll.o 3rd_party/mbedtls/library/error.o 3rd_party/mbedtls/library/gcm.o 3rd_party/mbedtls/library/havege.o 3rd_party/mbedtls/library/hkdf.o 3rd_party/mbedtls/library/hmac_drbg.o 3rd_party/mbedtls/library/md.o 3rd_party/mbedtls/library/md2.o 3rd_party/mbedtls/library/md4.o 3rd_party/mbedtls/library/md5.o 3rd_party/mbedtls/library/md_wrap.o 3rd_party/mbedtls/library/memory_buffer_alloc.o 3rd_party/mbedtls/library/net_sockets.o 3rd_party/mbedtls/library/nist_kw.o 3rd_party/mbedtls/library/oid.o 3rd_party/mbedtls/library/padlock.o 3rd_party/mbedtls/library/pem.o 3rd_party/mbedtls/library/pk.o 3rd_party/mbedtls/library/pk_wrap.o 3rd_party/mbedtls/library/pkcs11.o 3rd_party/mbedtls/library/pkcs12.o 3rd_party/mbedtls/library/pkcs5.o 3rd_party/mbedtls/library/pkparse.o 3rd_party/mbedtls/library/pkwrite.o 3rd_party/mbedtls/library/platform.o 3rd_party/mbedtls/library/platform_util.o 3rd_party/mbedtls/library/poly1305.o 3rd_party/mbedtls/library/ripemd160.o 
	-$(RM) 3rd_party/mbedtls/library/rsa.o 3rd_party/mbedtls/library/rsa_internal.o 3rd_party/mbedtls/library/sha1.o 3rd_party/mbedtls/library/sha256.o 3rd_party/mbedtls/library/sha512.o 3rd_party/mbedtls/library/ssl_cache.o 3rd_party/mbedtls/library/ssl_ciphersuites.o 3rd_party/mbedtls/library/ssl_cli.o 3rd_party/mbedtls/library/ssl_cookie.o 3rd_party/mbedtls/library/ssl_srv.o 3rd_party/mbedtls/library/ssl_ticket.o 3rd_party/mbedtls/library/ssl_tls.o 3rd_party/mbedtls/library/threading.o 3rd_party/mbedtls/library/timing.o 3rd_party/mbedtls/library/version.o 3rd_party/mbedtls/library/version_features.o 3rd_party/mbedtls/library/x509.o 3rd_party/mbedtls/library/x509_create.o 3rd_party/mbedtls/library/x509_crl.o 3rd_party/mbedtls/library/x509_crt.o 3rd_party/mbedtls/library/x509_csr.o 3rd_party/mbedtls/library/x509write_crt.o 3rd_party/mbedtls/library/x509write_csr.o 3rd_party/mbedtls/library/xtea.o main_xmc4700_example.o optiga_hostlib/examples/utilities/ecdsa_utils.o optiga_hostlib/optiga/cmd/optiga_cmd.o optiga_hostlib/optiga/common/optiga_lib_common.o optiga_hostlib/optiga/common/optiga_lib_logger.o optiga_hostlib/optiga/comms/ifx_i2c/ifx_i2c.o optiga_hostlib/optiga/comms/ifx_i2c/ifx_i2c_config.o optiga_hostlib/optiga/comms/ifx_i2c/ifx_i2c_data_link_layer.o optiga_hostlib/optiga/comms/ifx_i2c/ifx_i2c_physical_layer.o optiga_hostlib/optiga/comms/ifx_i2c/ifx_i2c_presentation_layer.o optiga_hostlib/optiga/comms/ifx_i2c/ifx_i2c_transport_layer.o optiga_hostlib/optiga/comms/optiga_comms_ifx_i2c.o optiga_hostlib/optiga/crypt/optiga_crypt.o optiga_hostlib/optiga/util/optiga_util.o optiga_hostlib/pal/pal.o optiga_hostlib/pal/pal_crypt_mbedtls.o optiga_hostlib/pal/xmc4700/pal_gpio.o optiga_hostlib/pal/xmc4700/pal_i2c.o optiga_hostlib/pal/xmc4700/pal_ifx_i2c_config.o optiga_hostlib/pal/xmc4700/pal_logger.o optiga_hostlib/pal/xmc4700/pal_os_datastore.o optiga_hostlib/pal/xmc4700/pal_os_event.o 
	-$(RM) optiga_hostlib/pal/xmc4700/pal_os_lock.o optiga_hostlib/pal/xmc4700/pal_os_memory.o optiga_hostlib/pal/xmc4700/pal_os_timer.o optiga_hostlib/xmc/Dave/Generated/CLOCK_XMC4/clock_xmc4.o optiga_hostlib/xmc/Dave/Generated/CLOCK_XMC4/clock_xmc4_conf.o optiga_hostlib/xmc/Dave/Generated/CPU_CTRL_XMC4/cpu_ctrl_xmc4.o optiga_hostlib/xmc/Dave/Generated/CPU_CTRL_XMC4/cpu_ctrl_xmc4_conf.o optiga_hostlib/xmc/Dave/Generated/DAVE.o optiga_hostlib/xmc/Dave/Generated/DIGITAL_IO/digital_io.o optiga_hostlib/xmc/Dave/Generated/DIGITAL_IO/digital_io_conf.o optiga_hostlib/xmc/Dave/Generated/GLOBAL_CCU4/global_ccu4.o optiga_hostlib/xmc/Dave/Generated/GLOBAL_CCU4/global_ccu4_conf.o optiga_hostlib/xmc/Dave/Generated/I2C_MASTER/i2c_master.o optiga_hostlib/xmc/Dave/Generated/I2C_MASTER/i2c_master_conf.o optiga_hostlib/xmc/Dave/Generated/INTERRUPT/interrupt.o optiga_hostlib/xmc/Dave/Generated/INTERRUPT/interrupt_conf.o optiga_hostlib/xmc/Dave/Generated/TIMER/timer.o optiga_hostlib/xmc/Dave/Generated/TIMER/timer_conf.o optiga_hostlib/xmc/Dave/Generated/UART/uart.o optiga_hostlib/xmc/Dave/Generated/UART/uart_conf.o optiga_hostlib/xmc/Dave/Generated/USBD/usb/core/usb_task.o optiga_hostlib/xmc/Dave/Generated/USBD/usb/core/xmc4000/usbd_endpoint_stream_xmc4000.o optiga_hostlib/xmc/Dave/Generated/USBD/usb/core/xmc4000/usbd_endpoint_xmc4000.o optiga_hostlib/xmc/Dave/Generated/USBD/usbd.o optiga_hostlib/xmc/Dave/Generated/USBD/usbd_conf.o optiga_hostlib/xmc/Dave/Generated/USBD_VCOM/class_driver/device/cdc_class_device.o optiga_hostlib/xmc/Dave/Generated/USBD_VCOM/usbd_vcom.o optiga_hostlib/xmc/Dave/Generated/USBD_VCOM/usbd_vcom_conf.o optiga_hostlib/xmc/Dave/Generated/USBD_VCOM/virtual_com/descriptors.o optiga_hostlib/xmc/Newlib/syscalls.o optiga_hostlib/xmc/Startup/startup_xmc4700.o optiga_hostlib/xmc/Startup/system_xmc4700.o optiga_hostlib/xmc/XMCLib/src/xmc4_eru.o optiga_hostlib/xmc/XMCLib/src/xmc4_flash.o 
	-$(RM) optiga_hostlib/xmc/XMCLib/src/xmc4_gpio.o optiga_hostlib/xmc/XMCLib/src/xmc4_rtc.o optiga_hostlib/xmc/XMCLib/src/xmc4_scu.o optiga_hostlib/xmc/XMCLib/src/xmc_can.o optiga_hostlib/xmc/XMCLib/src/xmc_ccu4.o optiga_hostlib/xmc/XMCLib/src/xmc_ccu8.o optiga_hostlib/xmc/XMCLib/src/xmc_common.o optiga_hostlib/xmc/XMCLib/src/xmc_dac.o optiga_hostlib/xmc/XMCLib/src/xmc_dma.o optiga_hostlib/xmc/XMCLib/src/xmc_dsd.o optiga_hostlib/xmc/XMCLib/src/xmc_ebu.o optiga_hostlib/xmc/XMCLib/src/xmc_ecat.o optiga_hostlib/xmc/XMCLib/src/xmc_eru.o optiga_hostlib/xmc/XMCLib/src/xmc_eth_mac.o optiga_hostlib/xmc/XMCLib/src/xmc_eth_phy_dp83848.o optiga_hostlib/xmc/XMCLib/src/xmc_eth_phy_ksz8031rnl.o optiga_hostlib/xmc/XMCLib/src/xmc_eth_phy_ksz8081rnb.o optiga_hostlib/xmc/XMCLib/src/xmc_fce.o optiga_hostlib/xmc/XMCLib/src/xmc_gpio.o optiga_hostlib/xmc/XMCLib/src/xmc_hrpwm.o optiga_hostlib/xmc/XMCLib/src/xmc_i2c.o optiga_hostlib/xmc/XMCLib/src/xmc_i2s.o optiga_hostlib/xmc/XMCLib/src/xmc_ledts.o optiga_hostlib/xmc/XMCLib/src/xmc_posif.o optiga_hostlib/xmc/XMCLib/src/xmc_rtc.o optiga_hostlib/xmc/XMCLib/src/xmc_sdmmc.o optiga_hostlib/xmc/XMCLib/src/xmc_spi.o optiga_hostlib/xmc/XMCLib/src/xmc_uart.o optiga_hostlib/xmc/XMCLib/src/xmc_usbd.o optiga_hostlib/xmc/XMCLib/src/xmc_usbh.o optiga_hostlib/xmc/XMCLib/src/xmc_usic.o optiga_hostlib/xmc/XMCLib/src/xmc_vadc.o optiga_hostlib/xmc/XMCLib/src/xmc_wdt.o wpc/PRx/qi_auth_prx.o wpc/PRx/qi_auth_prx_crypt.o wpc/PTx/qi_auth_ptx.o wpc/PTx/qi_auth_ptx_crypt.o wpc/qi_auth_self_test.o  xmc4700_qi_auth_example.elf $(LIST_FILES) $(MAP_FILE) objects.rsp
	-@echo.
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
