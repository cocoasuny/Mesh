/**
  ****************************************************************************************
  * @file    freertos.h
  * @author  Jason
  * @version V1.0.0
  * @date    2017-7-11
  * @brief   the header of freertos.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPFREERTOS__
#define __APPFREERTOS__

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"


/* task stack and priority define(the lower number,the lower priority) */
#define TASK_LOG_STACK                                  1024
#define TASK_LOG_PRIORITY					            2				
#define BLE_TOP_IMPLEMENTATION_STACK					256
#define BLE_TOP_IMPLEMENTATION_PRIORITY					4
#define MEASUREMENT_STACK                               256
#define MEASUREMENT_PRIORITY                            3




/* function declares */
void main_task_start(void);


#endif // __APPFREERTOS__

/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



