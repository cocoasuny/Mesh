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

/* Private variables ---------------------------------------------------------*/


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
    /* init for uart */
    uart_init();

    printf("%s",Dev_Msg);
}




/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



