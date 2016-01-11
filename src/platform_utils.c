/*
 * platform_utils.c
 *
 *  Created on: Oct 7, 2015
 *      Author: szilveszterb
 */

#include <lib/type.h>
#include <lib/error.h>

#include <system/reset/sys_reset.h>
#include <platform_utils.h>
#include <sys/time.h>
#include <app.h>

/*TODO missing declarations from VENDOR SDK*/
/*Put this into the PATCH file somewhere*/
void wifi_nvm_write(uint8_t *data_config, int32_t size);
void wifi_nvm_read(uint8_t *data_comfig, int32_t size);

/*TODO missing declarations from VENDOR SDK*/
/*Put this into the PATCH file somewhere*/
extern bool isDhcpDone(void);

APP_DRV_OBJECTS appDrvObjects;
APP_DATA appData;

bool g_scan_done;
bool g_prescan_waiting;

struct platform_init_class {
    void (*platform_periodic_fn)(void *data);
    int32_t platform_fn_period;
};

struct sNetworkParams {
    char *SSID;
    char *PASSW;
};

void SYS_Tasks(void)
{
    DRV_TMR_Tasks(appDrvObjects.drvTmrObject);

    SYS_TMR_Tasks(appDrvObjects.sysTmrObject);

    TCPIP_STACK_Task(appDrvObjects.tcpipObject);

    BSP_SWITCH_Tasks();

    SYS_FS_Tasks();
}

int32_t config_read(struct wifi_config_data *config)
{
    wifi_nvm_read((uint8_t *)config, sizeof(*config));
    if (config->magic == CONFIG_MAGIC)
        return ERR_SUCCESS;
    else
        return ERR_FAILURE;
}

void config_write(BOOL *wifi_configured, char *ssid, char *passphrase, const char *cik)
{
    struct wifi_config_data config;

    wifi_nvm_read((uint8_t *)&config, sizeof(config));
    /* If Config is not valid, initalize the data to be written back with all 0s */
    /* And set the magic */
    if (config.magic != CONFIG_MAGIC)  {
        memset(&config, 0, sizeof(config));
        config.magic = CONFIG_MAGIC;
    }

    if (wifi_configured)
        config.wifi_configured = *wifi_configured;
    if (ssid) {
        strncpy(config.ssid, ssid, MAX_SSID_SIZE);
        config.ssid[MAX_SSID_SIZE] = '\0';
    }
    if (passphrase) {
        strncpy(config.passphrase, passphrase, MAX_PASSPHRASE_SIZE);
        config.passphrase[MAX_PASSPHRASE_SIZE] = '\0';
    }
    if (cik) {
        strncpy(config.cik, cik, MAX_CIK_SIZE);
        config.cik[MAX_CIK_SIZE] = '\0';
    }

    wifi_nvm_write((uint8_t *)&config, sizeof(config));
}

static void sys_tasks_pclb(void *data)
{
    SYS_Tasks();
}

int32_t platform_utils_init(struct platform_init_class *init)
{
    volatile SYS_STATUS tcpipStat = SYS_STATUS_BUSY;
    int32_t status, net_count;
    TCPIP_NET_HANDLE handle;

    SYS_Initialize(NULL);

    while (tcpipStat != SYS_STATUS_READY) {
        tcpipStat = TCPIP_STACK_Status(appDrvObjects.tcpipObject);
        if (tcpipStat < 0) {
            return ERR_FAILURE;
            SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack initialization failed!\r\n");
        }
        SYS_Tasks();
    }

    net_count = TCPIP_STACK_NumberOfNetworksGet();
    if (net_count > 0) {
        handle = TCPIP_STACK_IndexToNet(0);
        if (TCPIP_DNS_Enable(handle) != true) {
            SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack failure, couldn't enable DNS\r\n");
            return ERR_FAILURE;
        }
    } else {
        SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack failure, no network handles available\r\n");
        return ERR_FAILURE;
    }

    SYS_PRINT("Number of Networks: &i\r\n", net_count);

    init->platform_periodic_fn = sys_tasks_pclb;
    init->platform_fn_period = 10;

    return status;
}

uint8_t platform_utils_is_network_ready(void)
{
    return TRUE;
}

static void set_wifi_parameters(char *essid, char *passphrase)
{
    p_wifi_ConfigData->networkType = DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE;
    strcpy((char *)p_wifi_ConfigData->netSSID, (char *)essid);
    p_wifi_ConfigData->SsidLength = strlen((char *)(essid));
    if (strlen(passphrase) == 0) {
        p_wifi_ConfigData->SecurityMode = DRV_WIFI_SECURITY_OPEN;
    } else {
        p_wifi_ConfigData->SecurityMode = DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE;
        memcpy(p_wifi_ConfigData->SecurityKey, passphrase, strlen(passphrase));
        p_wifi_ConfigData->SecurityKeyLength = strlen(passphrase);
    }
}

int32_t platform_utils_join_network(char *ssid, char *passwd)
{
    volatile TCPIP_MAC_RES wifi_stat =  TCPIP_MAC_RES_IS_BUSY;

    set_wifi_parameters(ssid, passwd);
    /*TODO Have to wait here otherwise the Wifi does not always connect*/
    uint32_t start = SYS_TMR_TickCountGet();
    uint32_t end;

    do {
        end = SYS_TMR_TickCountGet();
    } while (end-start < 1200);

    while (wifi_stat != TCPIP_MAC_RES_OK) {
        SYS_Tasks();
        wifi_stat = Demo_Wifi_Connect();
    }

    /*TODO for this I had to patch the WCM DK, there is no such function there originally
    * How to wait for it?
    **/
    while (!isDhcpDone())
        SYS_Tasks();

    BSP_LEDOff(BSP_LED_1);
    BSP_LEDOn(BSP_LED_5);

    /*
     * Set the random seed here based on the tickcount
     * This is set after the wifi connect because it will add
     * enough variability to the tickcount
     **/
    SYS_RANDOM_PseudoSeedSet(SYS_TMR_TickCountGet());
    return ERR_SUCCESS;
}

BOOL platform_utils_is_network_configured(void)
{
      int32_t status;
      struct wifi_config_data config;

    if (BSP_SwitchStateGet(BSP_SWITCH_1) == 0) /* 0 == pressed */
        return FALSE;

    status = config_read(&config);

    if (status != ERR_SUCCESS)
        return FALSE;

    return config.wifi_configured;
}

static char wcm_ssid[MAX_SSID_SIZE + 1];
static char wcm_passphrase[MAX_PASSPHRASE_SIZE + 1];

int32_t platform_utils_save_network_configuration(struct sNetworkParams *params)
{
    BOOL wifi_configured = TRUE;

    config_write(&wifi_configured, params->SSID, params->PASSW, NULL);

    return ERR_SUCCESS;
}

int32_t platform_utils_get_network_configuration(struct sNetworkParams *params)
{
    int32_t status;
    struct wifi_config_data config;

    status = config_read(&config);
    if (status != ERR_SUCCESS)
        return status;

    strcpy(wcm_ssid, config.ssid);
    strcpy(wcm_passphrase, config.passphrase);
    params->SSID = wcm_ssid;
    params->PASSW = wcm_passphrase;

    return ERR_SUCCESS;
}

extern uint8_t g_asyncTaskFlags;
int32_t platform_utils_configure_network(void)
{
    BOOL wifi_configured = TRUE;
    uint8_t channelList[] = WF_DEFAULT_CHANNEL_LIST;
    DRV_WIFI_SCAN_CONTEXT scanContext;

    SYS_CONSOLE_MESSAGE("MAIN: Start Web Config\r\n");

    DRV_WIFI_SsidSet(p_wifi_ConfigData->netSSID, p_wifi_ConfigData->SsidLength);

    DRV_WIFI_NetworkTypeSet(DRV_WIFI_NETWORK_TYPE_SOFT_AP);

    scanContext.scanType       = DRV_WIFI_PASSIVE_SCAN;
    scanContext.minChannelTime = DRV_WIFI_DEFAULT_SCAN_MIN_CHANNEL_TIME;
    scanContext.maxChannelTime = DRV_WIFI_DEFAULT_SCAN_MAX_CHANNEL_TIME;
    scanContext.probeDelay     = DRV_WIFI_DEFAULT_SCAN_PROBE_DELAY;
    scanContext.scanCount      = DRV_WIFI_DEFAULT_SCAN_COUNT;
    DRV_WIFI_ScanContextSet(&scanContext);
    DRV_WIFI_ChannelListSet(channelList, sizeof(channelList));

    DRV_WIFI_ReconnectModeSet(WF_DEFAULT_LIST_RETRY_COUNT,
                              DRV_WIFI_ATTEMPT_TO_RECONNECT,
                              40,
                              DRV_WIFI_ATTEMPT_TO_RECONNECT);

    while (SYS_FS_Mount(SYS_FS_NVM_VOL, LOCAL_WEBSITE_PATH_FS, MPFS2, 0, NULL)  != 0)
        SYS_Tasks();

    while (g_scan_done == FALSE)
        SYS_Tasks();

    DRV_WIFI_SecurityOpenSet();
    DRV_WIFI_Connect();

    /* WifiAsyncClearAllEvents();*/
    g_asyncTaskFlags &= ~ASYNC_EASY_CONFIG_PENDING;
    WifiAsyncSetEventPending(ASYNC_DHCP_CONFIG_PENDING);

    p_wifi_ConfigData->dataValid = 0;

    /* Wait until configured. */
    while ((ASYNC_EASY_CONFIG_PENDING & g_asyncTaskFlags) != ASYNC_EASY_CONFIG_PENDING)
        SYS_Tasks();

    g_asyncTaskFlags &= ~ASYNC_EASY_CONFIG_PENDING;

    config_write(&wifi_configured, (char *)p_wifi_ConfigData->netSSID, (char *)p_wifi_ConfigData->SecurityKey, NULL);

    SYS_RESET_SoftwareReset();
    /*
     *  Should do this, but it causes an instant crash. Need to figure out how to
     * wait for HTTP response to finish then disable http server before this.
     * while(SYS_FS_Unmount(LOCAL_WEBSITE_PATH_FS)  != 0)
     * {
     *    SYS_CONSOLE_MESSAGE("lol nope\r\n");
     *    SYS_Tasks();
     *}
     **/

    return ERR_SUCCESS;
}

int32_t platform_utils_connect_ap_mode(void)
{
    uint8_t channelList[] = WF_DEFAULT_CHANNEL_LIST;
    DRV_WIFI_SCAN_CONTEXT scanContext;

    SYS_CONSOLE_MESSAGE("MAIN: Start Web Config\r\n");

    DRV_WIFI_SsidSet(p_wifi_ConfigData->netSSID, p_wifi_ConfigData->SsidLength);

    DRV_WIFI_NetworkTypeSet(DRV_WIFI_NETWORK_TYPE_SOFT_AP);

    scanContext.scanType       = DRV_WIFI_PASSIVE_SCAN;
    scanContext.minChannelTime = DRV_WIFI_DEFAULT_SCAN_MIN_CHANNEL_TIME;
    scanContext.maxChannelTime = DRV_WIFI_DEFAULT_SCAN_MAX_CHANNEL_TIME;
    scanContext.probeDelay     = DRV_WIFI_DEFAULT_SCAN_PROBE_DELAY;
    scanContext.scanCount      = DRV_WIFI_DEFAULT_SCAN_COUNT;
    DRV_WIFI_ScanContextSet(&scanContext);
    DRV_WIFI_ChannelListSet(channelList, sizeof(channelList));

    DRV_WIFI_ReconnectModeSet(WF_DEFAULT_LIST_RETRY_COUNT,
                              DRV_WIFI_ATTEMPT_TO_RECONNECT,
                              40,
                              DRV_WIFI_ATTEMPT_TO_RECONNECT);

    while (g_scan_done == FALSE)
        SYS_Tasks();

    DRV_WIFI_SecurityOpenSet();
    DRV_WIFI_Connect();

    WifiAsyncSetEventPending(ASYNC_DHCP_CONFIG_PENDING);

    return ERR_SUCCESS;
}
