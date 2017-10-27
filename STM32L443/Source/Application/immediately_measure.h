/**
  ****************************************************************************************
  * @file    immediate_measure.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-10-9
  * @brief   header file of immediate_measure.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IMMEDIATELY_MEASURE_H_
#define __IMMEDIATELY_MEASURE_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"



/* Exported macros -----------------------------------------------------------*/
#define IMMDM_HR_TIMER_ID    0x01
#define IMMDM_SPO2_TIMERID   0x02





/* Exported functions --------------------------------------------------------*/
void immediately_measure_task_handler(void *pvParameters);





#endif /* __IMMEDIATELY_MEASURE_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

