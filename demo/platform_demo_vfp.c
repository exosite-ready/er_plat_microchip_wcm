/*
 * platform_demo_vfp.c
 *
 *  Created on: Oct 18, 2015
 *      Author: zoltanr
 */
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

inline BOOL platform_is_network_configured(void)
{
  return platform_utils_is_network_configured();
}

int32_t platform_configure_network(void)
{
    return platform_utils_configure_network();
}

int32_t platform_connect_ap_mode(void)
{
  return platform_utils_connect_ap_mode();
}

int32_t platform_save_network_configuration(struct sNetworkParams *params)
{
    return platform_utils_save_network_configuration(params);
}

int32_t platform_get_network_configuration(struct sNetworkParams *params)
{
    return platform_utils_get_network_configuration(params);
}

int32_t platform_get_sensor_value(int32_t type, int32_t id, char *buffer, size_t len)
{
  int32_t value = ADC1BUF0;

  snprintf(buffer, len, "%d", value / 10);
  buffer[len - 1] = 0;
  return ERR_SUCCESS;
}

int32_t platform_get_switch(int32_t sw, BOOL *state)
{
  BSP_SWITCH_STATE button_state = FALSE;
  BSP_SWITCH wcm_sw = BSP_SWITCH_1;
  /*Transform EXO_SWITCHn (which start from 0) to Microchip BSP_Switch which start from 1*/
  switch (sw) {
  case EXO_SWITCH_1:
      wcm_sw = BSP_SWITCH_1;
      break;
  case EXO_SWITCH_2:
      wcm_sw = BSP_SWITCH_2;
      break;
  case EXO_SWITCH_3:
      wcm_sw = BSP_SWITCH_3;
      break;
  case EXO_SWITCH_4:
      wcm_sw = BSP_SWITCH_4;
      break;
  default:
      break;
  }
  button_state = BSP_SWITCH_SwitchTest(wcm_sw);
  if (button_state == BSP_SWITCH_STATE_ASSERTED)
    *state = TRUE;
  else
    *state = FALSE;

  return ERR_SUCCESS;
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
    case EXO_LED_3:
        (on)?BSP_LEDOn(BSP_LED_3):BSP_LEDOff(BSP_LED_3);
        break;
    case EXO_LED_4:
        (on)?BSP_LEDOn(BSP_LED_4):BSP_LEDOff(BSP_LED_4);
        break;

    default:
        return ERR_NOT_APPLICABLE;
    }

    return ERR_SUCCESS;
}

