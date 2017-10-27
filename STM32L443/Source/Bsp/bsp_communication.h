/**
  ****************************************************************************************
  * @file    bsp_communication.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-26
  * @brief   header file of bsp_communication.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSPCOMMUNICATION_H_
#define __BSPCOMMUNICATION_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"


/* CMU internal debug switch */
#define CMU_INTERNAL_DEBUG

/* Max Rx Buffer Lengh */
#define CMU_RX_BUF_MAXLEN   (200)


/* Exported macros -----------------------------------------------------------*/
typedef struct
{
    uint8_t buf[CMU_RX_BUF_MAXLEN];
    uint16_t len;
}CMU_DATA_READ_BUFFER_T;



/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef cmu_init(void);
HAL_StatusTypeDef cmu_deinit(void);
void get_data_from_CMU(CMU_DATA_READ_BUFFER_T *buffer);
void cmu_data_send(uint8_t *pDatSend, uint8_t lenDat);


#endif /* __BSPCOMMUNICATION_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

