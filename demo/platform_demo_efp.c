#include <lib/type.h>
#include <lib/error.h>

#include <porting/system_port.h>
#include <platform_demo.h>
#include <platform_utils.h>
#include <sys/time.h>
#include <app.h>
#include <system/reset/sys_reset.h>

inline int32_t platform_init(struct platform_init_class *init)
{
  return platform_utils_init(init);
}

inline platform_debug_callback platform_get_debug_printf(void)
{
    return (platform_debug_callback)_SYS_CONSOLE_PRINT;
}

inline int32_t platform_join_network(struct sNetworkParams *params)
{
    return platform_utils_join_network(params->SSID, params->PASSW);
}

inline uint8_t platform_is_network_ready(void)
{
    return platform_utils_is_network_ready();
}

int32_t platform_set_led(int32_t led, BOOL on)
{
    switch (led) {
    case EXO_LED_1:
        (on)?BSP_LEDOn(BSP_LED_1):BSP_LEDOff(BSP_LED_1);
        break;
    case EXO_LED_2:
        (on)?BSP_LEDOn(BSP_LED_2):BSP_LEDOff(BSP_LED_2);
        break;

    default:
        return ERR_NOT_APPLICABLE;
    }

    return ERR_SUCCESS;
}

