/**
  ****************************************************************************************
  * @file    bsp.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-11-25
  * @brief   bsp
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Private variables ---------------------------------------------------------*/


const char Dev_Msg[] =
	"/*********************  Mesh service for nRF51822 *************/\r\n"
    "/*                                                            */\r\n"
    "/*                                                            */\r\n"
    "/*                    BSP Init Complate,Start...              */\r\n"
	"\r\n";
/**
  * @brief  bsp init 
  * @note   This function is called  automatically at the beginning of program 
  * @param  None
  * @retval None
  */
void bsp_init(void)
{
	/* uart init */
	uart_init();
	    
    printf("%s",Dev_Msg);
}



/************************ (C) COPYRIGHT DBGJ Co., Ltd. *****END OF FILE****/

