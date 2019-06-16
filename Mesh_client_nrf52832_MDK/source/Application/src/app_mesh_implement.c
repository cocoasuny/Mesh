/**
  ****************************************************************************************
  * @file    app_mesh_implement.c
  * @author  Jason
  * @version V1.0.0
  * @date    2019-5-10
  * @brief   the top level implement of ble mesh
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"
#include "ble_softdevice_support.h"
#include "app_mesh_implement.h"
#include "nrf_mesh_config_core.h"
#include "nrf_mesh_configure.h"
#include "nrf_mesh_defines.h"
#include "nrf_mesh.h"
#include "mesh_stack.h"
#include "mesh_provisionee.h"
#include "mesh_app_utils.h"
#include "generic_onoff_client.h"
#include "platform.h"


#define APP_STATE_OFF                (0)
#define APP_STATE_ON                 (1)

#define APP_UNACK_MSG_REPEAT_COUNT   (2)

/* Private variable define ---------------------------------------------------*/
static generic_onoff_client_t m_clients[CLIENT_MODEL_INSTANCE_COUNT];
static bool                   m_device_provisioned;

/* Private function declare --------------------------------------------------*/
static void mesh_init(void);
static void models_init_cb(void);
static void app_gen_onoff_client_publish_interval_cb(access_model_handle_t handle, void * p_self);
static void app_generic_onoff_client_status_cb(const generic_onoff_client_t * p_self,
                                               const access_message_rx_meta_t * p_meta,
                                               const generic_onoff_status_params_t * p_in);
static void app_gen_onoff_client_transaction_status_cb(access_model_handle_t model_handle,
                                                       void * p_args,
                                                       access_reliable_status_t status);
static void node_reset(void);													  
static void config_server_evt_cb(const config_server_evt_t * p_evt);
static void provisioning_complete_cb(void);
static void device_identification_start_cb(uint8_t attention_duration_s);	
static void provisioning_aborted_cb(void);				
static void start(void);													   
													   
const generic_onoff_client_callbacks_t client_cbs =
{
    .onoff_status_cb = app_generic_onoff_client_status_cb,
    .ack_transaction_status_cb = app_gen_onoff_client_transaction_status_cb,
    .periodic_publish_cb = app_gen_onoff_client_publish_interval_cb
};													   

/**@brief Thread for handling the ble mesh.
 *
 * @details This thread is responsible for processing top level ble mesh
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
void ble_mesh_top_thread(void * arg)
{
    UNUSED_PARAMETER(arg);
	
	 ble_stack_init();
	
#if MESH_FEATURE_GATT_ENABLED
    gap_params_init();
    conn_params_init();
#endif

    mesh_init();	
	start();
	
	while(1)
	{
		(void)sd_app_evt_wait();
	}
}

/**
  * @brief  mesh_init
  * @note   Function for initializing the mesh
  * @param  None
  * @retval None
  */
static void mesh_init(void)
{
    uint8_t dev_uuid[NRF_MESH_UUID_SIZE];
    uint8_t node_uuid_prefix[NODE_UUID_PREFIX_LEN] = CLIENT_NODE_UUID_PREFIX;

    ERROR_CHECK(mesh_app_uuid_gen(dev_uuid, node_uuid_prefix, NODE_UUID_PREFIX_LEN));
    mesh_stack_init_params_t init_params =
    {
        .core.irq_priority       = NRF_MESH_IRQ_PRIORITY_LOWEST,
        .core.lfclksrc           = DEV_BOARD_LF_CLK_CFG,
        .core.p_uuid             = dev_uuid,
        .models.models_init_cb   = models_init_cb,
        .models.config_server_cb = config_server_evt_cb
    };
    ERROR_CHECK(mesh_stack_init(&init_params, &m_device_provisioned));
}


static void start(void)
{
    if (!m_device_provisioned)
    {
        static const uint8_t static_auth_data[NRF_MESH_KEY_SIZE] = STATIC_AUTH_DATA;
        mesh_provisionee_start_params_t prov_start_params =
        {
            .p_static_data    = static_auth_data,
            .prov_complete_cb = provisioning_complete_cb,
            .prov_device_identification_start_cb = device_identification_start_cb,
            .prov_device_identification_stop_cb = NULL,
            .prov_abort_cb = provisioning_aborted_cb,
            .p_device_uri = NULL
        };
        ERROR_CHECK(mesh_provisionee_prov_start(&prov_start_params));
    }

    const uint8_t *p_uuid = nrf_mesh_configure_device_uuid_get();
    UNUSED_VARIABLE(p_uuid);
    __LOG_XB(LOG_SRC_APP, LOG_LEVEL_INFO, "Device UUID ", p_uuid, NRF_MESH_UUID_SIZE);

    ERROR_CHECK(mesh_stack_start());

    //hal_led_mask_set(LEDS_MASK, LED_MASK_STATE_OFF);
    //hal_led_blink_ms(LEDS_MASK, LED_BLINK_INTERVAL_MS, LED_BLINK_CNT_START);
}

static void device_identification_start_cb(uint8_t attention_duration_s)
{
    /*hal_led_mask_set(LEDS_MASK, false);
    hal_led_blink_ms(BSP_LED_2_MASK  | BSP_LED_3_MASK, 
                     LED_BLINK_ATTENTION_INTERVAL_MS, 
                     LED_BLINK_ATTENTION_COUNT(attention_duration_s));
    */
}

static void provisioning_aborted_cb(void)
{
    //hal_led_blink_stop();
}

static void provisioning_complete_cb(void)
{
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Successfully provisioned\n");

#if MESH_FEATURE_GATT_ENABLED
    /* Restores the application parameters after switching from the Provisioning
     * service to the Proxy  */
    gap_params_init();
    conn_params_init();
#endif

    dsm_local_unicast_address_t node_address;
    dsm_local_unicast_addresses_get(&node_address);
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Node Address: 0x%04x \n", node_address.address_start);

    //hal_led_blink_stop();
    //hal_led_mask_set(LEDS_MASK, LED_MASK_STATE_OFF);
    //hal_led_blink_ms(LEDS_MASK, LED_BLINK_INTERVAL_MS, LED_BLINK_CNT_PROV);
}

/* This callback is called periodically if model is configured for periodic publishing */
static void app_gen_onoff_client_publish_interval_cb(access_model_handle_t handle, void * p_self)
{
     __LOG(LOG_SRC_APP, LOG_LEVEL_WARN, "Publish desired message here.\n");
}

/* Acknowledged transaction status callback, if acknowledged transfer fails, application can
* determine suitable course of action (e.g. re-initiate previous transaction) by using this
* callback.
*/
static void app_gen_onoff_client_transaction_status_cb(access_model_handle_t model_handle,
                                                       void * p_args,
                                                       access_reliable_status_t status)
{
    switch(status)
    {
        case ACCESS_RELIABLE_TRANSFER_SUCCESS:
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Acknowledged transfer success.\n");
            break;

        case ACCESS_RELIABLE_TRANSFER_TIMEOUT:
            //hal_led_blink_ms(LEDS_MASK, LED_BLINK_SHORT_INTERVAL_MS, LED_BLINK_CNT_NO_REPLY);
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Acknowledged transfer timeout.\n");
            break;

        case ACCESS_RELIABLE_TRANSFER_CANCELLED:
            __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Acknowledged transfer cancelled.\n");
            break;

        default:
            ERROR_CHECK(NRF_ERROR_INTERNAL);
            break;
    }
}

/* Generic OnOff client model interface: Process the received status message in this callback */
static void app_generic_onoff_client_status_cb(const generic_onoff_client_t * p_self,
                                               const access_message_rx_meta_t * p_meta,
                                               const generic_onoff_status_params_t * p_in)
{
    if (p_in->remaining_time_ms > 0)
    {
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "OnOff server: 0x%04x, Present OnOff: %d, Target OnOff: %d, Remaining Time: %d ms\n",
              p_meta->src.value, p_in->present_on_off, p_in->target_on_off, p_in->remaining_time_ms);
    }
    else
    {
        __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "OnOff server: 0x%04x, Present OnOff: %d\n",
              p_meta->src.value, p_in->present_on_off);
    }
}

static void node_reset(void)
{
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "----- Node reset  -----\n");
    //hal_led_blink_ms(LEDS_MASK, LED_BLINK_INTERVAL_MS, LED_BLINK_CNT_RESET);
    /* This function may return if there are ongoing flash operations. */
    mesh_stack_device_reset();
}

static void config_server_evt_cb(const config_server_evt_t * p_evt)
{
    if (p_evt->type == CONFIG_SERVER_EVT_NODE_RESET)
    {
        node_reset();
    }
}

static void models_init_cb(void)
{
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "Initializing and adding models\n");

    for (uint32_t i = 0; i < CLIENT_MODEL_INSTANCE_COUNT; ++i)
    {
        m_clients[i].settings.p_callbacks = &client_cbs;
        m_clients[i].settings.timeout = 0;
        m_clients[i].settings.force_segmented = APP_CONFIG_FORCE_SEGMENTATION;
        m_clients[i].settings.transmic_size = APP_CONFIG_MIC_SIZE;

        ERROR_CHECK(generic_onoff_client_init(&m_clients[i], i + 1));
    }
}








/************************ (C) COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/




