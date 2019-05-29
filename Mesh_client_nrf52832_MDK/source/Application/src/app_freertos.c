/**
  ****************************************************************************************
  * @file    freertos.c
  * @author  Jason
  * @version V1.0.0
  * @date    2019-5-11
  * @brief   the implement of task
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"
#include "cli_cmd.h"
#include "bsp.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_mesh_implement.h"
#include "log.h"

/* private variable declare --------------------------------------------------*/
static TaskHandle_t m_logger_thread;                                /**< Definition of Logger thread. */
static TaskHandle_t m_measurement_thread;                           /**< Definition of measurement thread. */
static TaskHandle_t m_ble_mesh_thread;                           	/**< Definition of ble mesh thread. */


/* private function declare --------------------------------------------------*/
static void logger_thread(void * arg);
static void measurement_thread(void * arg);



/**
  * @brief  start the main task to start the system
  * @param  None
  * @retval None
  */
void task_create(void)
{
	/* creat a thread for logger */	
    if (pdPASS != xTaskCreate(logger_thread, "LOG", TASK_LOG_STACK, NULL, TASK_LOG_PRIORITY, &m_logger_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }  
	
	/* creat a thread for Measurement */	
    if (pdPASS != xTaskCreate(measurement_thread, "meas", MEASUREMENT_STACK, NULL, MEASUREMENT_PRIORITY, &m_measurement_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }	
	
	/* creat a thread for Measurement */	
    if (pdPASS != xTaskCreate(ble_mesh_top_thread, "mesh", BLE_MESH_STACK, NULL, BLE_MESH_PRIORITY, &m_ble_mesh_thread))
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }	
}


/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
static void logger_thread(void * arg)
{
    UNUSED_PARAMETER(arg);
	
	/* init for Log(mast init befor Cli,beacase cli used for log) */
	log_init_entity();
	
    __LOG_INIT(LOG_SRC_APP | LOG_SRC_ACCESS | LOG_SRC_BEARER, LOG_LEVEL_INFO, LOG_CALLBACK_DEFAULT);
    __LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "----- BLE Mesh Light Switch Client Demo -----\n");	
	
	/* init for command line interface */
	cli_init();
	cli_start();
		
    while (1)
    {
		cli_process();
		NRF_LOG_FLUSH();
		vTaskDelay(50);
    }
}

/**@brief Thread for handling the logger.
 *
 * @details This thread is responsible for processing log entries if logs are deferred.
 *          Thread flushes all log entries and suspends. It is resumed by idle task hook.
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
static void measurement_thread(void * arg)
{
    UNUSED_PARAMETER(arg);
	
	while(1)
	{
		//NRF_LOG_INFO("HRS FreeRTOS example started.");
		//__LOG(LOG_SRC_APP, LOG_LEVEL_INFO, "----- BLE Mesh Light Switch Client Demo -----\n");	
		vTaskDelay(500);
	}	
}




/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/

