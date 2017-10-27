/**
  ****************************************************************************************
  * @file    bma250e.h
  * @author  上海团队
  * @version V1.0.0
  * @date    $$$$-$$-$$
  * @brief   header file of bma250e.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
#ifndef __BMA250E_H
#define __BMA250E_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "stdint.h"
#include "bma2x2.h"
#include "main.h"     
     
#define BMA250E_SPI_DMA
     
#define BMA250E_SPI_WRITE           0x00
#define BMA250E_SPI_READ            0x80
#define BMA250E_CHIP_ID             0xF9   
	 
HAL_StatusTypeDef bma250e_init(void);
void accel_data_get(int16_t *v_piXdata,int16_t *v_piYdata,int16_t *v_piZdata);
	 
#ifdef __cplusplus
}
#endif
#endif /*__BMA250E_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/
