/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.1.1
  * @date    13-April-2012
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GLOBAL_TYPEDEF_H
#define _GLOBAL_TYPEDEF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"
#include "platform.h"
#include <stdio.h>


typedef struct
{
	uint8_t                  DevSupportCap;
	const uint8_t           * pDevREV;
	const uint8_t           * pFWREV;
    const uint8_t           * pBootREV;
	const uint8_t	        * pSeriNum;
	uint8_t                 DeviceInfo_Status;
    bool                    isBleConnect;    //true:connected; false:disconnnect
}DEVICE_INFORMATION_T;

/* Define Pass Key data parameters */
typedef struct  _PassKey_Parameters
{
	bool		ConfirmStatus;      //密码验证是否通过状态
	bool        Status;             //是否收到验证密码
	uint8_t     len;
	uint8_t     p_contex[4];
	bool        Connect_Status;
}PASSKEY_T;

typedef struct
{
	uint8_t  DevSupportCap;
	const uint8_t  * pDevREV;
	const uint8_t  * pFWREV;
    const uint8_t  * pBootREV;
	const uint8_t	* pSeriNum;
	uint8_t  DeviceInfo_Status;
}DeviceInfomation_t;

typedef struct
{
    uint8_t     AppActiveStatus;
    uint8_t     AppModifySNStatus;
    uint8_t     AppModifySNInfo[4];
    uint8_t     MTID;
    uint8_t     DataLength;
    uint8_t     DataFormate;
}SysConfigInfo_t;

typedef struct
{
    uint8_t     Month;
    uint8_t     Day;
    uint8_t     Hour;
    uint8_t     Min;
    uint8_t     Sec;
	unsigned int lr;  
	unsigned int pc;    
}SysConfigCPUInfo_t;

typedef struct
{
    uint8_t    FWUpdateUPGRADEStatus;
    uint8_t    FWUpdateFWDATAStatus;
    uint8_t    FWUpdateCRC[4];
    uint8_t    FWUpdateFIRMWAREType;
}FWUpdateInfo_t;

typedef struct {
	uint32_t 	start_time;
	uint8_t 	availabe_length;	
	uint8_t		Buffer[EEPROMReadMaxData];
}MTDataBuffer_t;


typedef struct{
	uint32_t 	start_time;
	uint16_t	start_page_addr;
	uint16_t  	end_page_addr;
}flash_partition_t;



#endif /* _GLOBAL_TYPEDEF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
