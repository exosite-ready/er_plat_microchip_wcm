#include <lib/type.h>
#include <lib/error.h>
#include <platform_utils.h>
#include <porting/system_port.h>
#include <porting/system/exos/platform_exos.h>

#include <sys/time.h>
#include <app.h>

static char mac_addr_str[13];

int32_t platform_exos_init(void)
{
    return ERR_SUCCESS;
}

void platform_reset(void)
{
    SYS_RESET_SoftwareReset();
}

uint32_t platform_get_time_ms(void)
{
    return SYS_TMR_TickCountGet();
}

/*
 * This is the stack base, the *end* of the stack
 * The stack starts at stack base + stack size and grows to
 * lower addresses
 **/
extern uint32_t _splim __attribute__((section(".data")));
uint32_t *platform_get_stack_location()
{
    return &_splim;
}

extern uint32_t _stack __attribute__((section(".data")));
uint32_t *platform_get_stack_base()
{
    return &_stack;
}


/* We use the MAC address as the serial number */
const char *platform_get_sn(void)
{
    uint8_t mac[6];

    DRV_WIFI_MacAddressGet(mac);
    /*  convert to string, all capitals to conform with portals defaults for SN */
    /* SN is case sensitive */
    snprintf(mac_addr_str, 13, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return mac_addr_str;
}

int32_t platform_get_cik(char *cik, size_t len)
{
    int32_t status;
    struct wifi_config_data config;

    status = config_read(&config);
    if (status != ERR_SUCCESS)
        return status;

      strncpy(cik, config.cik, len);
      cik[40] = '\0';
      return ERR_SUCCESS;
}

int32_t platform_set_cik(const char *cik)
{
    /*
    * There should be a 4 byte aligned container, because
    * NVM uses the data from 4byte aligned memory, if it's not aligned
    * it will write the preceding garbage
    **/
    config_write(NULL, NULL, NULL, cik);
    return ERR_SUCCESS;
}

/*TODO with this implementation it cannot be nested*/
int32_t platform_set_global_interrupts(int32_t status)
{
  if (status == FALSE)
    return __builtin_disable_interrupts();
  else {
    __builtin_mtc0(12, 0, status);
    return 0;
  }
}

/* NOT IMPLEMENTED */
int32_t platform_get_global_interrupts(void)
{
  return 0;
}

void platform_ll_device_init(struct link_layer_if **ll_if)
{
    *ll_if = NULL;
}
