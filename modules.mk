ifeq ($(CONFIG_PLATFORM), wcm)
PLATFORM_PATH:=$(MODULE_PATH)
 platform.a_OBJS = \
  $(PLATFORM_PATH)/platform_exos.o\
  $(PLATFORM_PATH)/demo/platform_demo_efp.o\
  $(PLATFORM_PATH)/src/platform_utils.o\
  $(PLATFORM_PATH)/third_party/system_init.o\
  $(PLATFORM_PATH)/third_party/crypt_rng.o\
  $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/bsp_sys_init.o\
  $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/drv_wifi_config.o\
  $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/custom_http_app.o\
  $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/http_print.o\
  $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/mpfs_img2.o\
  $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/wcm_queue.o\
  $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/system_config/wcm_dev_kit_1/tcpip_stack_init.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/nvm/src/dynamic/drv_nvm.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/nvm/src/dynamic/drv_nvm_media.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/spi/src/dynamic/drv_spi.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/spi/src/dynamic/drv_spi_api.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/spi/src/drv_spi_sys_queue_fifo.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/tmr/src/dynamic/drv_tmr.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_com.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_commands.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_config_data.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_connect.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_connection_algorithm.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_connection_manager.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_connection_profile.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_debug_output.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_easy_config.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_eint.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_event_handler.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_events.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_init.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_mac.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_mac_pic32.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_mgmt_msg.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_param_msg.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_pbkdf2.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_power_save.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_raw.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_scan.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_spi.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_spi_init.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_tx_power.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_update_firmware_common.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_update_firmware_uart.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_update_firmware_tcpclient_txt.o\
  $(wcm_VENDOR_SDK_PATH)/framework/driver/wifi/mrf24w/src/drv_wifi_client_table.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/system/drivers/db_appio.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/system/drivers/usart.o\
  $(wcm_VENDOR_SDK_PATH)/framework/peripheral/eth/src/plib_eth_lib.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/clk/src/sys_clk.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/clk/src/sys_clk_pic32mx.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/devcon/src/sys_devcon.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/devcon/src/sys_devcon_pic32mx.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/dma/src/sys_dma.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/fs/src/dynamic/sys_fs.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/fs/src/dynamic/sys_fs_media_manager.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/fs/mpfs/src/mpfs.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/fs/fat_fs/src/file_system/ff.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/fs/fat_fs/src/hardware_access/diskio.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/int/src/sys_int_pic32.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/ports/src/sys_ports.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/random/src/sys_random.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/reset/src/sys_reset.o\
  $(wcm_VENDOR_SDK_PATH)/framework/system/tmr/src/sys_tmr.o
  
 networkstack.a_OBJS += \
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/common/big_int.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/common/big_int_helper_c32.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/common/hashes.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/common/helpers.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/common/lfsr.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/common/sys_fs_wrapper.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/arcfour.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/arp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/berkeley_api.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/ddns.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/dhcp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/dhcps.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/dns.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/dnss.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/ftp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/hash_fnv.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/http.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/icmp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/icmpv6.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/iperf.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/ipv4.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/ipv6.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/nbns.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/ndp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/oahash.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/rsa.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/smtp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/snmp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/snmpv3.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/snmpv3_usm.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/sntp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/ssl.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_announce.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_commands.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_heap_alloc.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_helpers.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_mac_object.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_manager.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_notify.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_packet.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_reboot.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/telnet.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/udp.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/zero_conf_helper.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/zero_conf_link_local.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/zero_conf_multicast_dns.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/tcpip_helper_c32.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/system/system_command.o\
  $(wcm_VENDOR_SDK_PATH)/framework/tcpip/src/system/system_debug.o
 
  CFLAGS += -I$(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src
  CFLAGS += -I$(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/system_config/wcm_dev_kit_1
  CFLAGS += -I$(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/system_config/wcm_dev_kit_1/tcpip_profile
  CFLAGS += -I$(wcm_VENDOR_SDK_PATH)/framework
  CFLAGS += -I$(PLATFORM_PATH)/include
  
  #LDFLAGS += -u _InterruptHandler_TMR1

  $(TARGET_NAME)_OBJS +=   $(wcm_VENDOR_SDK_PATH)/apps/tcpip/wcm_dev_kit_1/firmware/src/system_config/wcm_dev_kit_1/system_interrupt.o
  $(TARGET)_LIBRARIES += platform.a networkstack.a
endif
