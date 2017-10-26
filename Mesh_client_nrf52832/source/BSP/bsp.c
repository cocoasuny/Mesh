/**
  ****************************************************************************************
  * @file    bsp.c
  * @author  Jason
  * @version V1.0.0
  * @date    2017-7-10
  * @brief   the implement of bsp
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"
#include "bsp_cli.h"

/* Private variables ---------------------------------------------------------*/

/* private function */
static void timer_handle(void * p_context);

const char Dev_Msg[] =
	"/*********************  Mesh client for nRF52832 *************/\r\n"
    "/*                                                            */\r\n"
    "/*                                                            */\r\n"
    "/*                    BSP Init Complate,Start...              */\r\n"
	"\r\n";

/**
  * @brief  bsp_init
  * @note   the init of bsp
  * @param  None
  * @retval None
  */
void bsp_init(void)
{
    ret_code_t ret;
    
    ret = app_timer_init();
    APP_ERROR_CHECK(ret);

    APP_TIMER_DEF(timer_0);
    ret = app_timer_create(&timer_0, APP_TIMER_MODE_REPEATED, timer_handle);
    APP_ERROR_CHECK(ret);

    ret = app_timer_start(timer_0, APP_TIMER_TICKS(1000), NULL);
    APP_ERROR_CHECK(ret);
    
//    /* init for uart */
    //uart_init();
    cli_init();
    cli_start();

    printf("%s",Dev_Msg);
}


static void timer_handle(void * p_context)
{
    UNUSED_PARAMETER(p_context);

//    if (m_counter_active)
//    {
//        m_counter++;
//        NRF_LOG_RAW_INFO("counter = %d\r\n", m_counter);
//    }
}

/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



