/**
  ****************************************************************************************
  * @file    spi.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-20
  * @brief   header file of spi.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H_
#define __SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"



/* Exported macros -----------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
extern SPI_HandleTypeDef       hspi3;
extern DMA_HandleTypeDef       hdma_spi3_rx;
extern DMA_HandleTypeDef       hdma_spi3_tx;
extern SPI_HandleTypeDef       hspi1;

/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef MX_SPI3_Init(void);
HAL_StatusTypeDef spi3_dma_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len);
HAL_StatusTypeDef MX_SPI1_Init(void);
HAL_StatusTypeDef spi1_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len);


#endif /* __SPI_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

