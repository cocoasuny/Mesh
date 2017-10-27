/**
  ****************************************************************************************
  * @file    spi.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-20
  * @brief   spi.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "spi.h"

/* Global variables ----------------------------------------------------------*/
SPI_HandleTypeDef       hspi3;
DMA_HandleTypeDef       hdma_spi3_rx;
DMA_HandleTypeDef       hdma_spi3_tx;
SPI_HandleTypeDef       hspi1;

/* Private variables ---------------------------------------------------------*/

 
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  spi3 init 
  * @note   spi3 init
  * @param  None
  * @retval HAL status
  */
HAL_StatusTypeDef MX_SPI3_Init(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    hspi3.Instance                  = SPI3;
    hspi3.Init.Mode                 = SPI_MODE_MASTER;
    hspi3.Init.Direction            = SPI_DIRECTION_2LINES;
    hspi3.Init.DataSize             = SPI_DATASIZE_8BIT;
    hspi3.Init.CLKPolarity          = SPI_POLARITY_LOW;
    hspi3.Init.CLKPhase             = SPI_PHASE_1EDGE;
    hspi3.Init.NSS                  = SPI_NSS_SOFT;
    hspi3.Init.BaudRatePrescaler    = SPI_BAUDRATEPRESCALER_64;
    hspi3.Init.FirstBit             = SPI_FIRSTBIT_MSB;
    hspi3.Init.TIMode               = SPI_TIMODE_DISABLE;
    hspi3.Init.CRCCalculation       = SPI_CRCCALCULATION_DISABLE;
    hspi3.Init.CRCPolynomial        = 7;
    hspi3.Init.CRCLength            = SPI_CRC_LENGTH_8BIT;
    hspi3.Init.NSSPMode             = SPI_NSS_PULSE_DISABLE;
    
    ret = HAL_SPI_Init(&hspi3);
    if (ret != HAL_OK)
    {
        App_Error_Check(ret);
    }
    
    return ret;
}
/**
  * @brief  spi3_dma_write_read
  * @note   spi3 dma write & read data
  * @param  uint8_t *wrData,uint8_t *rdData, uint16_t len
  * @retval HAL status
  */
HAL_StatusTypeDef spi3_dma_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len)
{
    HAL_StatusTypeDef   ret = HAL_ERROR;
    
//    ret = HAL_SPI_TransmitReceive_DMA(&hspi3,wrData,rdData,len);
    ret = HAL_SPI_TransmitReceive(&hspi3,wrData,rdData,len,1000);
    
    return ret;
}
/**
  * @brief  spi1 init 
  * @note   spi1 init
  * @param  None
  * @retval HAL status
  */
HAL_StatusTypeDef MX_SPI1_Init(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    hspi1.Instance                      = SPI1;
    hspi1.Init.Mode                     = SPI_MODE_MASTER;
    hspi1.Init.Direction                = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize                 = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity              = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase                 = SPI_PHASE_1EDGE;
    hspi1.Init.NSS                      = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler        = SPI_BAUDRATEPRESCALER_64;
    hspi1.Init.FirstBit                 = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode                   = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation           = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial            = 7;
    hspi1.Init.CRCLength                = SPI_CRC_LENGTH_DATASIZE;
    hspi1.Init.NSSPMode                 = SPI_NSS_PULSE_ENABLE;
    
    ret = HAL_SPI_Init(&hspi1);
    if (ret != HAL_OK)
    {
        App_Error_Check(ret);
    }
    
    return ret;
}
/**
  * @brief  spi1_write_read
  * @note   spi1 write & read data
  * @param  uint8_t *wrData,uint8_t *rdData, uint16_t len
  * @retval HAL status
  */
HAL_StatusTypeDef spi1_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len)
{
    HAL_StatusTypeDef   ret = HAL_ERROR;
    
    ret = HAL_SPI_TransmitReceive(&hspi1,wrData,rdData,len,0xFFFF);
    
    return ret;
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == FLASH_SPI)
    {
        printf("SPI tx Cplt\r\n");
    }
    else
    {
        printf("not spi3\r\n");
    }
}

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

