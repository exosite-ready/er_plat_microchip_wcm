/*
 * platform_utils.h
 *
 *  Created on: Oct 7, 2015
 *      Author: szilveszterb
 */

#ifndef PLATFORM_UTILS_H_
#define PLATFORM_UTILS_H_

#define MAX_SSID_SIZE 32
#define MAX_PASSPHRASE_SIZE 64
#define MAX_CIK_SIZE 40
#define CONFIG_MAGIC 0xcefe6767

struct wifi_config_data {
  int32_t magic; /* To check whether the config is valid or not */
  BOOL wifi_configured;
  char ssid[MAX_SSID_SIZE + 1];
  char passphrase[MAX_PASSPHRASE_SIZE + 1];
  char cik[MAX_CIK_SIZE + 1];
};

struct platform_init_class;
struct sNetworkParams;

int config_read(struct wifi_config_data *config);

void config_write(BOOL *wifi_configured, char *ssid, char *passphrase, const char *cik);

int32_t platform_utils_init(struct platform_init_class *init);

uint8_t platform_utils_is_network_ready(void);

int32_t platform_utils_join_network(char *ssid, char *passwd);

BOOL platform_utils_is_network_configured(void);

int32_t platform_utils_get_network_configuration(struct sNetworkParams *params);

int32_t platform_utils_save_network_configuration(struct sNetworkParams *params);

int32_t platform_utils_configure_network(void);

int32_t platform_utils_connect_ap_mode(void);

#endif /* PLATFORM_UTILS_H_ */
