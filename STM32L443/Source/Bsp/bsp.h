/**
  ****************************************************************************************
  * @file    bsp.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   header file of bsp.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H_
#define __BSP_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"



/* Exported macros -----------------------------------------------------------*/

#define App_Error_Check(ERR_CODE)                         \
    do                                                      \
    {                                                       \
        app_error_handler((ERR_CODE), __LINE__, (uint8_t*) __FILE__);  \
    } while (0)





/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef bsp_init(void);
void mco_outputLSE(bool status);
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name);
void Sys_SoftReset(void);
void peripheral_powerCtl(uint8_t power);

#endif /* __BSP_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

