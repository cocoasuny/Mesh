/**
  ****************************************************************************************
  * @file    bsp_uart.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   header file of bsp_uart.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_UART_H_
#define __BSP_UART_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#ifdef APP_TRACE
	int fputc(int ch, FILE *file);
	#define APP_TRACE_LOG(format,...) printf(format,##__VA_ARGS__)
#else
	#define APP_TRACE_LOG(format,...)
#endif

/* Exported variables--------------------------------------------------------*/
extern UART_HandleTypeDef huart3;
extern uint8_t  g_aRxBuffer[];

/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef bsp_uart_init(void);



#endif /* __BSP_UART_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

