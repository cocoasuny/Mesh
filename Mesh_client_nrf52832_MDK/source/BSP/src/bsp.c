/**
  ****************************************************************************************
  * @file    bsp.c
  * @author  Jason
  * @version V1.0.0
  * @date    2019-5-10
  * @brief   the implement of command line interface
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "nordic_common.h"
#include "nrf_drv_clock.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

/**
  * @brief  clock_init
  * @note   Function for initializing the clock.
  * @param  None
  * @retval None
  */
void clock_init(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
}

/**
  * @brief  log_init
  * @note   Function for initializing the log.
  * @param  None
  * @retval None
  */
void log_init_entity(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);
}

/**
  * @brief  log_callback_cli
  * @note   log output by cli module
  * @param  None
  * @retval None
  */
void log_callback_default(uint32_t dbg_level, const char * p_filename, uint16_t line,uint32_t timestamp, const char * format, va_list arguments)
{
    char buf[100] = {0};
    
    memset(buf,0,sizeof(buf));
    vsprintf(buf,format,arguments);
    NRF_LOG_INFO("%s",buf);
}








/************************ (C) COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/


