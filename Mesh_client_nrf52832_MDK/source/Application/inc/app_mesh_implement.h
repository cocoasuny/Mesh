/**
  ****************************************************************************************
  * @file    app_mesh_implement.h
  * @author  Jason
  * @version V1.0.0
  * @date    2019-5-10
  * @brief   the header of app_mesh_implement.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#ifndef __APP_MESH_IMPLEMENT__
#define __APP_MESH_IMPLEMENT__

#include "sdk_config.h"


/** Number of active servers.
 * Note: If the value of SERVER_NODE_COUNT is increased, you may need to scale up the the replay
 * protection list size (@ref REPLAY_CACHE_ENTRIES), by the appropriate amount, for the provisioner and
 * client examples. For the provisioner example to work as expected, its replay protection list size should
 * be greater than or equal to the total number of nodes it is going to configure after provisioning them.
 * The replay protection list size of the client example should be greater than or equal to the total
 * number of unicast addresses in the network that it can receive a message from.
 */
#define SERVER_NODE_COUNT (30)
#if SERVER_NODE_COUNT > 30
#error Maximum 30 servers currently supported by client example.
#endif

/** Number of active clients nodes. */
#define CLIENT_NODE_COUNT            (1)

/** Number of On-Off client models on the Switch Node */
#define CLIENT_MODEL_INSTANCE_COUNT  (2)

/** Number of group address being used in this example */
#define GROUP_ADDR_COUNT             (2)

/** Static authentication data */
#define STATIC_AUTH_DATA {0x6E, 0x6F, 0x72, 0x64, 0x69, 0x63, 0x5F, 0x65, 0x78, 0x61, 0x6D, 0x70, 0x6C, 0x65, 0x5F, 0x31}

/** UUID prefix length */
#define NODE_UUID_PREFIX_LEN         (4)

/** UUID prefix length */
#define COMMON_UUID_PREFIX_LEN       (3)

/** Common UUID prefix for client nodes */
#define COMMON_CLIENT_UUID              0x00, 0x59, 0xCC

/** Light switch client UUID */
#define CLIENT_NODE_UUID_PREFIX         {COMMON_CLIENT_UUID, 0xAA}

/** Light switch level client UUID */
#define LEVEL_CLIENT_NODE_UUID_PREFIX   {COMMON_CLIENT_UUID, 0xBB}

/** Common UUID prefix for server nodes */
#define COMMON_SERVER_UUID              0x00, 0x59, 0x55

/** UUID prefix for other nodes */
#define SERVER_NODE_UUID_PREFIX         {COMMON_SERVER_UUID, 0xAA}

/** UUID prefix for other level server nodes */
#define LEVEL_SERVER_NODE_UUID_PREFIX   {COMMON_SERVER_UUID, 0xBB}

/**
 * Clock configuration for Nordic development boards.
 */
#if defined(S110)
    #define DEV_BOARD_LF_CLK_CFG  NRF_CLOCK_LFCLKSRC_XTAL_20_PPM
#elif NRF_SD_BLE_API_VERSION >= 5
    #define DEV_BOARD_LF_CLK_CFG  { \
        .source = NRF_SDH_CLOCK_LF_SRC, \
        .rc_ctiv = NRF_SDH_CLOCK_LF_RC_CTIV, \
        .rc_temp_ctiv = NRF_SDH_CLOCK_LF_RC_TEMP_CTIV, \
        .accuracy = NRF_SDH_CLOCK_LF_ACCURACY \
    }
#else
    #define DEV_BOARD_LF_CLK_CFG  { \
        .source = NRF_CLOCK_LF_SRC_XTAL, \
        .rc_ctiv = 0, \
        .rc_temp_ctiv = 0, \
        .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_20_PPM \
    }
#endif



/* function declares */
void ble_mesh_top_thread(void * arg);


#endif  /* __APP_MESH_IMPLEMENT__ */

/************************ (C) COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



