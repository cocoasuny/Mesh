/**
  ******************************************************************************
  * @file    StoreManageFatfs.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-February-2013
  * @brief   This file contains all the functions prototypes for the RTC firmware 
  *          library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
#ifndef __STOREMANAGEFATFS_H
#define __STOREMANAGEFATFS_H

	 
/* Includes ------------------------------------------------------------------*/

#include "stm32l4xx.h"
#include "main.h"


#define MAX_FILE_NAME_LEN                   (13)
#define PRE_BUFFER_SIZE                     (256)
#define MAX_FILE_TOTLE_NUM                  (20)
#define MAX_SINGLE_FILE_SIZE				(1024000) //Byte

/* alarm space size define */
#define ALARM_SIZE                          (100)   //kByte
#define DELETE_SIZE                         (10)    //kByte


typedef struct
{
    char            fileName[MAX_FILE_NAME_LEN];
    uint32_t        size;
}FILE_INFO_T;

typedef struct 
{
	uint16_t 	len;
	uint8_t		buf[PRE_BUFFER_SIZE];
}PRE_WRITE_INFO_T;

typedef struct
{
    FILE_INFO_T     fileInfo;
    uint32_t        pRdLen;
    bool            isFileToBeDelete;
}READ_FILE_INFO_T;

typedef struct
{
    uint8_t     fileTotleNum;
    FILINFO     info[MAX_FILE_TOTLE_NUM];
}FILE_INPATH_INFO_T;


void format_disk(void);
void FatFs_Test(void);
FRESULT store_manage_init(void);
void creatOSAFile(void);
void creatCustomTemplateFile(void);
void closeOSAFile(void);
void File_Creat(uint32_t date);
uint8_t write_data_to_store_manage(uint8_t type,uint8_t * pu8Src,uint8_t u8Length);
uint8_t read_data_from_store_manage(uint8_t type,uint8_t * pu8Src,uint8_t u8Length,uint8_t *avalibLen);
void findCurrentWRFile(FILE_INFO_T *file);
void findCurrentRDFile(FILE_INFO_T *file);
void getTotleValidDataLength(uint32_t *len);
void getTotleValidFileNum(uint8_t *num);
void adjustReadOutDataLen(uint8_t len);
uint8_t ack_datalen_to_store_manage(uint8_t len);


/**********测试使用函数************/
void TestCreatOSAFile(void);
void TestFileDir(void);
void TestWriteData(void);
void TestStoreManage(void);
void TestGetFreeSpace(void);
FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
);
FRESULT wtite_data_to_currentFile(uint8_t *pbuf,uint16_t len);

#endif /* __STOREMANAGEFATFS_H */


