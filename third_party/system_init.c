/*******************************************************************************
 System Initialization File

  File Name:
    sys_init.c

  Summary:
    System Initialization.

  Description:
    This file contains source code necessary to initialize the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2012 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END
#include "app.h"
#include "system_config.h"
#include "driver/nvm/drv_nvm.h"
#include "driver/nvm/drv_nvm_media.h"
#include "tcpip/tcpip.h"

#include "tcpip_stack_init.h"
#include "system/clk/sys_clk.h"
#include "system/clk/src/sys_clk_local.h"
#include "system/random/sys_random.h"
#include "system/devcon/sys_devcon.h"
#include "system/tmr/sys_tmr.h"  

// Device Configuration
// PIC32 WCM_Dev_Kit_1 configuration fuses
#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FPBDIV = DIV_1, POSCMOD = OFF, FNOSC = FRCPLL, CP = OFF
#pragma config OSCIOFNC = ON, ICESEL = ICS_PGx1

// To take care of warnings

// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
const DRV_NVM_INIT drvNVMInit =
{
    .nvmID = NVM_ID_0,
    .moduleInit.sys.powerState = SYS_MODULE_POWER_RUN_FULL,
    .interruptSource = INT_SOURCE_FLASH_CONTROL,
};

#ifdef MICROCHIP_WCM_WIFI_CONFIG
const DRV_NVM_MEDIA_INIT drvNVMMediaInit =
{
    .mediaStartAddress = (uintptr_t)NVM_MEDIA_DATA,
    .nSectors = NVM_MEDIA_SIZE / 512,      // 64 KB in 512 bytes sectors
    .sectorSizeInBytes = 512,
    .drvNVMIndex = DRV_NVM_INDEX_0,
};
#endif

const SYS_FS_REGISTRATION_TABLE sysMPFSInit [ SYS_FS_MAX_FILE_SYSTEM_TYPE ] =
{
    {
        .nativeFileSystemType = MPFS2,
        .nativeFileSystemFunctions = &MPFSFunctions
    }
};
const DRV_TMR_INIT tmrInitData =
{
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},
    .tmrId = TMR_ID_1,
    .clockSource = DRV_TMR_CLKSOURCE_INTERNAL,
    .prescale = TMR_PRESCALE_VALUE_256,
    .interruptSource = INT_SOURCE_TIMER_1,
    .mode = DRV_TMR_OPERATION_MODE_16_BIT,
    .asyncWriteEnable = false,

};

SYS_TMR_INIT sysTmrInitData =
{
    .moduleInit = {SYS_TMR_MODULE_INIT},
    .drvIndex = SYS_TMR_DRIVER_INDEX,
    .tmrFreq = SYS_TMR_FREQUENCY,
};

//SYS_MODULE_OBJ          sysTmrObject;

// ****************************************************************************
// ****************************************************************************
// Section: System Initialization
// ****************************************************************************
// ****************************************************************************
/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules

  Description:
    This routine initializes the board, services, drivers, application and other
    modules as configured at build time.  In a bare-metal environment (where no
    OS is supported), this routine should be called almost immediately after
    entering the "main" routine.

  Precondition:
    The C-language run-time environment and stack must have been initialized.

  Parameters:
    data        - Pointer to the system initialzation data structure containing
                  pointers to the board, system service, and driver
                  initialization routines
  Returns:
    None.

  Example:
    <code>
    SYS_INT_Initialize(NULL);
    </code>

  Remarks:
    Basic System Initialization Sequence:

    1.  Initilize minimal board services and processor-specific items
        (enough to use the board to initialize drivers and services)
    2.  Initialize all supported system services
    3.  Initialize all supported modules
        (libraries, drivers, middleware, and application-level modules)
    4.  Initialize the main (static) application, if present.

    The order in which services and modules are initialized and started may be
    important.

    For a static system (a system not using the ISP's dynamic implementation
    of the initialization and "Tasks" services) this routine is implemented
    for the specific configuration of an application.
 */
void SYS_Initialize(void* data)
{
    // Disable JTAG port so we get our I/O pins back
    // So far in Harmony, this is Explorer 16 Board only
    DDPCONbits.JTAGEN = 0;

    BSP_Initialize();

    SYS_MODULE_OBJ     objectHandle;
    SYS_DEVCON_INIT    devInitConfig;

    // Populate the devcon initialization structure
    devInitConfig.moduleInit.value     = SYS_MODULE_POWER_RUN_FULL;

    objectHandle = SYS_DEVCON_Initialize (SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)&devInitConfig);
    if (SYS_MODULE_OBJ_INVALID == objectHandle)
    {
        return;
    }

    SYS_DEVCON_PerformanceConfig( SYS_DEVCON_SYSTEM_CLOCK );

     /* set priority for NVM interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_FCE, INT_PRIORITY_LEVEL3);

    /* set sub-priority for NVM interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_FCE, INT_SUBPRIORITY_LEVEL3);

     /* set priority for TMR2 interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_T1, INT_PRIORITY_LEVEL4);

    /* set sub-priority for TMR2 interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_T1, INT_SUBPRIORITY_LEVEL3);

    /* Enable the change notice interrupt source, set its priority level to 2,
       set its subpriority level to 0 */
    PLIB_INT_SourceEnable(INT_ID_0, INT_SOURCE_CHANGE_NOTICE);
    PLIB_INT_VectorPrioritySet(INT_ID_0, INT_VECTOR_CN, INT_PRIORITY_LEVEL2);
    PLIB_INT_VectorSubPrioritySet(INT_ID_0, INT_VECTOR_CN, INT_SUBPRIORITY_LEVEL0);

    /* set priority for ETHERNET interrupt source */
    //SYS_INT_VectorPrioritySet(DRV_ETHMAC_INTERRUPT_VECTOR, INT_PRIORITY_LEVEL5);

    /* set sub-priority for ETHERNET interrupt source */
    //SYS_INT_VectorSubprioritySet(DRV_ETHMAC_INTERRUPT_VECTOR, INT_SUBPRIORITY_LEVEL3);


    /* Initialize the interrupt system  */
    SYS_INT_Initialize();

    SYS_CLK_Initialize(NULL);

    /* Enable and setup INT1 (MRF24W Interrupt) */
    SYS_PORTS_PinDirectionSelect(PORTS_ID_0,
                                 SYS_PORTS_DIRECTION_INPUT,
                                 WF_INT_PORT_CHANNEL,
                                 WF_INT_BIT_POS
                                );
    // Enable change notification
   // SYS_PORTS_ChangeNotificationEnable(PORTS_ID_0,CN9,SYS_PORTS_PULLUP_ENABLE);

    // Turn on change notification
   // SYS_PORTS_ChangeNotificationGlobalEnable(PORTS_ID_0);
    
    //Turn ON the system clock
    appDrvObjects.drvTmrObject= DRV_TMR_Initialize(DRV_TMR_INDEX_0, (const SYS_MODULE_INIT  * const)&tmrInitData);
    appDrvObjects.sysTmrObject = SYS_TMR_Initialize(SYS_TMR_INDEX_0, (const SYS_MODULE_INIT  * const)&sysTmrInitData);


    SYS_INT_Enable();


    /* Initialize the NVM driver */
    SYS_INT_VectorPrioritySet(INT_VECTOR_FCE, INT_PRIORITY_LEVEL3);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_FCE, INT_SUBPRIORITY_LEVEL3);
    
    appDrvObjects.drvNVMObject = DRV_NVM_Initialize(DRV_NVM_INDEX_0,
            (SYS_MODULE_INIT *)&drvNVMInit);

#ifdef MICROCHIP_WCM_WIFI_CONFIG
    appDrvObjects.drvNVMMediaObject = DRV_NVM_MEDIA_Initialize(DRV_NVM_MEDIA_INDEX_0,
            (SYS_MODULE_INIT *)&drvNVMMediaInit);
#endif

    /* Initialize the SYS_FS Layer */
    SYS_FS_Initialize( (const void *) sysMPFSInit );

//    if(!_SYS_DEBUG_INIT(SYS_DEBUG_PORT))
//    {
//         return;
//    }

    if(!_SYS_CONSOLE_INIT(SYS_CONSOLE_PORT))
    {
        return;
    }

    //SYS_RANDOM_Initialize(0, 0);

//    if (!SYS_COMMAND_INIT())
//    {
//        return;
//    }


    //APP_Initialize();

    // Initialize the TCPIP stack
    appDrvObjects.tcpipObject = TCPIP_STACK_Init();
    if (appDrvObjects.tcpipObject == SYS_MODULE_OBJ_INVALID)
    {
       return;
    }

    return;

} //SYS_Initialize

/*******************************************************************************/
/*******************************************************************************
 End of File
*/

