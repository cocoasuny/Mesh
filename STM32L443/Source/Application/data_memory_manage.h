/**
  ****************************************************************************************
  * @file    data_memory_manage.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-10-25
  * @brief   header file of data_memory_manage.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DATA_MEMORY_MANAGE_H
#define __DATA_MEMORY_MANAGE_H

#include "stm32l4xx_hal.h"
#include "main.h"
#include "Global_Typedef.h"


/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define 	APP_SUCCESS          0x00
#define     ERR_WRITE_EEPROM     0x01
#define     ERR_READ_EEPROM      0x02


/* Exported functions ------------------------------------------------------- */
void GetSysConfigInfo(SysConfigInfo_t *info);
void SetSysConfigInfo(SysConfigInfo_t info);
void GetFWUpDateConfigInfo(FWUpdateInfo_t *info);
void SetFWUpDateConfigInfo(FWUpdateInfo_t info);
uint8_t data_memory_write(uint8_t type,uint8_t * pu8Src,uint8_t u8Length);
uint8_t data_memory_read(uint8_t type,MTDataBuffer_t * p_storageExtBuffer,uint8_t u8Length);
uint32_t get_data_memory_totle_length(void);
uint8_t DataMemoryReadACK(uint8_t u8ValidLength);
void DataMemoryRestore(void);
void DataMemoryEraseAll(void);
void SetCPUConfigInfo(SysConfigCPUInfo_t info);
void SetDeviceModeConfigInfo(void);
void GetCPUConfigInfo(void);


#endif /* __DATA_MEMORY_MANAGE_H */

/***************** (C) COPYRIGHT iCareTech Healthcare Co., Ltd.(END OF FILE)***********/




