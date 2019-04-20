/**
  ****************************************************************************************
  * @file    bsp.h
  * @author  Jason
  * @version V1.0.0
  * @date    2017-7-10
  * @brief   the header of bsp.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP__
#define __BSP__


/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"
#include "bsp_cli.h"



/* function declares */
void bsp_init(void);
void uart_init(void);



#endif /* end of __BSP__ */


/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



