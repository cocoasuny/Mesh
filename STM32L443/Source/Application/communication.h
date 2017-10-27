/**
  ****************************************************************************************
  * @file    communication.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-27
  * @brief   header file of communication.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMUNICATION_H_
#define __COMMUNICATION_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"





/* Exported macros -----------------------------------------------------------*/




/* Exported functions --------------------------------------------------------*/
void CMU_Task_Handler(void *pvParameters);



#endif /* __COMMUNICATION_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

