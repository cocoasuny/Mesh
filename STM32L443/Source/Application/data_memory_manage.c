/**
  ****************************************************************************************
  * @file    data_memory_manage.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-10-25
  * @brief   ÂÆûÁé∞Êï∞ÊçÆÂ≠òÂÇ®ÁÆ°ÁêÜÂäüËÉΩ
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "data_memory_manage.h"


/*******************************************************************************
* Function Name  : GetCPUConfigInfo
* Description    : Get Cpu Config Information when hard fault
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetCPUConfigInfo(void)
{
    /* Cpu Config Info : 0:Month;
                         1:Day;
                         2:Hour
                         3:Min
                         4:Sec
                         5: lr (4 byte)
                         7: pc (4 byte)
    */

//    uint8_t ConfigFile[20] = {0};   //max 20byte of Sys Config File
//    unsigned int stacked_lr = 0;
//    unsigned int stacked_pc = 0;

//    /* Read CPU Config File From STM32 Internal EEPROM */
//    DataEEPROM_MultiByteRead(ConfigFile,EEPROM_CONFIG_CPUINFO,sizeof(ConfigFile));

//	printf ("[Hard fault handler]\r\n");
//	printf("Hard fault occured Time(MM/DD/HH/MM/SS): %d:%d:%d:%d:%d\r\n",
//						ConfigFile[0],ConfigFile[1],ConfigFile[2],ConfigFile[3],ConfigFile[4]);

//    /* lr */
//    stacked_lr = ((ConfigFile[5] << 24) & 0xFF000000) | (uint8_t)((ConfigFile[6] << 16) & 0x00FF0000)
//               | ((ConfigFile[7] << 8)  & 0x0000FF00) | ((ConfigFile[8] << 0)  & 0x000000FF);

//    /* pc */
//    stacked_pc = ((ConfigFile[9] << 24) & 0xFF000000) | (uint8_t)((ConfigFile[10] << 16) & 0x00FF0000)
//               | ((ConfigFile[11] << 8)  & 0x0000FF00) | ((ConfigFile[12] << 0)  & 0x000000FF);

//    printf ("LR = %0.8x\r\n", stacked_lr);
//	printf ("PC = %0.8x\r\n", stacked_pc);
}
/*******************************************************************************
* Function Name  : SetCPUConfigInfo
* Description    : Set Cpu Config Information when hard fault
* Input          : SysConfigCPUInfo_t *info
* Output         : Nont
* Return         : None
*******************************************************************************/
void SetCPUConfigInfo(SysConfigCPUInfo_t info)
{
    /* Cpu Config Info : 0:Month;
                         1:Day;
                         2:Hour
                         3:Min
                         4:Sec
                         5: lr (4 byte)
                         7: pc (4 byte)
    */

//    uint8_t ConfigFile[20] = {0};   //max 20byte of Sys Config File


//    ConfigFile[0] = info.Month;
//    ConfigFile[1] = info.Day;
//    ConfigFile[2] = info.Hour;
//    ConfigFile[3] = info.Min;
//    ConfigFile[4] = info.Sec;

//    /* lr */
//    ConfigFile[5] = (uint8_t)((info.lr >> 24) & 0x000000FF);
//	ConfigFile[6] = (uint8_t)((info.lr >> 16) & 0x000000FF);
//	ConfigFile[7] = (uint8_t)((info.lr >> 8)  & 0x000000FF);
//	ConfigFile[8] = (uint8_t)((info.lr >> 0)  & 0x000000FF);

//    /* pc */
//	ConfigFile[9] = (uint8_t)((info.pc >> 24) & 0x000000FF);
//	ConfigFile[10] = (uint8_t)((info.pc >> 16) & 0x000000FF);
//	ConfigFile[11] = (uint8_t)((info.pc >> 8)  & 0x000000FF);
//	ConfigFile[12] = (uint8_t)((info.pc >> 0)  & 0x000000FF);


//    /* Write Cpu Config File From STM32 Internal EEPROM */
//    DataEEPROM_MultiByteWrite(ConfigFile,EEPROM_CONFIG_CPUINFO,sizeof(ConfigFile));
}

/*******************************************************************************
* Function Name  : GetSysConfigInfo
* Description    : Get System Config Information
* Input          : SysConfigInfo_t *info
* Output         : SysConfigInfo_t *info
* Return         : None
*******************************************************************************/
void GetSysConfigInfo(SysConfigInfo_t *info)
{
//    /* Config Factory Info : 0:Flag of App active or not;
//                             1:Flag of App Modify SN or not;
//                             2~5:App Modify SN Info
//                             6:  Moniter Template ID   //not Needed from HWV4.1
//                             7:  FreeRunMonitor datalength
//    */

//    uint8_t ConfigFile[20] = {0};   //max 20byte of Sys Config File

//    /* Read Sys Config File From STM32 Internal EEPROM */
//    DataEEPROM_MultiByteRead(ConfigFile,EEPROM_CONFIG_FACTORYINFO,sizeof(ConfigFile));

//    info->AppActiveStatus = ConfigFile[0];
//    info->AppModifySNStatus = ConfigFile[1];
//    info->AppModifySNInfo[0] = ConfigFile[2];
//    info->AppModifySNInfo[1] = ConfigFile[3];
//    info->AppModifySNInfo[2] = ConfigFile[4];
//    info->AppModifySNInfo[3] = ConfigFile[5];
//    info->MTID = ConfigFile[6];
//    info->DataLength = ConfigFile[7];
//    info->DataFormate = ConfigFile[8];
}
/*******************************************************************************
* Function Name  : SetSysConfigInfo
* Description    : Set System Config Information
* Input          : SysConfigInfo_t *info
* Output         : SysConfigInfo_t *info
* Return         : None
*******************************************************************************/
void SetSysConfigInfo(SysConfigInfo_t info)
{
//    /* Config Factory Info : 0:Flag of App active or not;
//                         1:Flag of App Modify SN or not;
//                         2~5:App Modify SN Info
//                         6:  Moniter Template ID
//                         7:  FreeRunMonitor datalength
//    */

//    uint8_t ConfigFile[20] = {0};   //max 20byte of Sys Config File


//    ConfigFile[0] = info.AppActiveStatus;
//    ConfigFile[1] = info.AppModifySNStatus;
//    ConfigFile[2] = info.AppModifySNInfo[0];
//    ConfigFile[3] = info.AppModifySNInfo[1];
//    ConfigFile[4] = info.AppModifySNInfo[2];
//    ConfigFile[5] = info.AppModifySNInfo[3];
//    ConfigFile[6] = info.MTID;
//    ConfigFile[7] = info.DataLength;
//    ConfigFile[8] = info.DataFormate;

//    /* Write Sys Config File From STM32 Internal EEPROM */
//    DataEEPROM_MultiByteWrite(ConfigFile,EEPROM_CONFIG_FACTORYINFO,sizeof(ConfigFile));
}

/*******************************************************************************
* Function Name  : GetFWUpDateConfigInfo
* Description    : Get FW Update Config Information
* Input          : FWUpdateInfo_t *info
* Output         : FWUpdateInfo_t *info
* Return         : None
*******************************************************************************/
void GetFWUpDateConfigInfo(FWUpdateInfo_t *info)
{
//    /* Config FW Update Config Info : 0:Flag of FW Update UPGRADE Status;
//                                      1:Flag of FW Update FWDATA  Status;
//                                      2~5:FW Update CRC Info
//                                      6:Flag of FW Update FIRMWARE Type
//    */

//    uint8_t ConfigFile[10] = {0};   //max 10byte of FW Update Config File

//    /* Read Sys Config File From STM32 Internal EEPROM */
//    DataEEPROM_MultiByteRead(ConfigFile,EEPROM_CONFIG_UPGRADE_FLAG,sizeof(ConfigFile));

//    info->FWUpdateUPGRADEStatus = ConfigFile[0];
//    info->FWUpdateFWDATAStatus = ConfigFile[1];
//    info->FWUpdateCRC[0] = ConfigFile[2];
//    info->FWUpdateCRC[1] = ConfigFile[3];
//    info->FWUpdateCRC[2] = ConfigFile[4];
//    info->FWUpdateCRC[3] = ConfigFile[5];
//    info->FWUpdateFIRMWAREType = ConfigFile[6];
}
/*******************************************************************************
* Function Name  : SetFWUpDateConfigInfo
* Description    : Set FW Update Config Information
* Input          : FWUpdateInfo_t *info
* Output         : FWUpdateInfo_t *info
* Return         : None
*******************************************************************************/
void SetFWUpDateConfigInfo(FWUpdateInfo_t info)
{
//    /* Config FW Update Config Info : 0:Flag of FW Update UPGRADE Status;
//                                      1:Flag of FW Update FWDATA  Status;
//                                      2~5:FW Update CRC Info
//                                      6:Flag of FW Update FIRMWARE Type
//    */

//    uint8_t ConfigFile[10] = {0};   //max 10byte of Sys Config File


//    ConfigFile[0] = info.FWUpdateUPGRADEStatus;
//    ConfigFile[1] = info.FWUpdateFWDATAStatus;
//    ConfigFile[2] = info.FWUpdateCRC[0];
//    ConfigFile[3] = info.FWUpdateCRC[1];
//    ConfigFile[4] = info.FWUpdateCRC[2];
//    ConfigFile[5] = info.FWUpdateCRC[3];
//    ConfigFile[6] = info.FWUpdateFIRMWAREType;

//    /* Write Sys Config File From STM32 Internal EEPROM */
//    DataEEPROM_MultiByteWrite(ConfigFile,EEPROM_CONFIG_UPGRADE_FLAG,sizeof(ConfigFile));
}
/**
  * @brief  data_memory_write
  * @note   Write Data to Memory witch Store the data
  * @param  store_type_e,pu8Src,u8Length
  * @retval None
  */
uint8_t data_memory_write(uint8_t type,uint8_t * pu8Src,uint8_t u8Length)
{
    uint8_t          Stat = 0;;

    Stat = write_data_to_store_manage(type,pu8Src,u8Length);

    return Stat;
}
/**
  * @brief  get_data_memory_totle_length
  * @note   get the totle length of the data in the DataMemory
  * @param  None
  * @retval length
  */
uint32_t get_data_memory_totle_length(void)
{
    uint32_t dataLen = 0;
    
    getTotleValidDataLength(&dataLen);
    
    dataLen = dataLen*8/9;
    
    return  (dataLen);
}
/**
  * @brief  data_memory_read
  * @note   Read Data from Memory witch Store the data
  * @param  store_type_e,pu8Src,u8Length
  * @retval status
  */
uint8_t data_memory_read(uint8_t type,MTDataBuffer_t * p_storageExtBuffer,uint8_t u8Length)
{
    uint8_t             Msg;
    uint8_t             Stat;
    uint8_t             buffer[256] = {0};
    uint8_t             bufferOut[256] = {0};
    uint8_t             avalibLen = 0;
    uint8_t             i = 0;
    uint32_t            u32Date = 0;
    RTC_DateTypeDef     date_s;
    RTC_TimeTypeDef     rtc_time;
    RTC_DateTypeDef     olddate_s;

    u8Length = (u8Length/8) * 9;  //Êï∞ÊçÆÈïøÂ∫¶ËΩ¨Êç¢
    
    Msg = read_data_from_store_manage(type,buffer,u8Length,&avalibLen);
    
//    printf("original data:\r\n");
//    for(i=0;i<avalibLen;i++)
//    {
//        printf("0x%02x, ",buffer[i]);
//    }
    u32Date = (uint32_t)((buffer[0]<<24) | (buffer[1]<<16) | (buffer[2]<<8) | (buffer[3]));
    OutputGMTIME(u32Date,&olddate_s,&rtc_time);
    u32Date = (uint32_t)(((olddate_s.Year+2000) << 16) | (olddate_s.Month << 8) | (olddate_s.Date));
    p_storageExtBuffer->start_time = u32Date; 
    
    /* Êï∞ÊçÆËΩ¨Êç¢ */
    for(i=0;i<(avalibLen/9);i++)
    {
        u32Date = (uint32_t)((buffer[i*9]<<24) | (buffer[i*9+1]<<16) | (buffer[i*9+2]<<8) | (buffer[i*9+3]));
        OutputGMTIME(u32Date,&date_s,&rtc_time);
        
        if((olddate_s.Year != date_s.Year) || (olddate_s.Month != date_s.Month) || (olddate_s.Date != date_s.Date))
        {
            /* Ë∞ÉÊï¥Ëé∑ÂèñÁöÑÊï∞ÊçÆÈïøÂ∫¶ */
            adjustReadOutDataLen(i*9);
            avalibLen = i*9;
            break;
        }
        bufferOut[i*8] = rtc_time.Hours;
        bufferOut[i*8+1] = rtc_time.Minutes;
        bufferOut[i*8+2] = rtc_time.Seconds;
        bufferOut[i*8+3] = buffer[i*9+4];
        bufferOut[i*8+4] = buffer[i*9+5];
        bufferOut[i*8+5] = buffer[i*9+6];
        bufferOut[i*8+6] = buffer[i*9+7];
        bufferOut[i*8+7] = buffer[i*9+8];
    }
    
    avalibLen = (avalibLen/9) * 8; //Êï∞ÊçÆÈïøÂ∫¶ËΩ¨Êç¢

    p_storageExtBuffer->availabe_length = avalibLen;
    memcpy(p_storageExtBuffer->Buffer,bufferOut,avalibLen);


    if(Msg == true)
    {
        Stat = APP_SUCCESS;
    }
    else
    {
        Stat = ERR_WRITE_EEPROM;
    }

    return Stat;
}
/**
  * @brief  data_memory_read_ACK
  * @note   Ack of Read Data
  * @param  u8Length
  * @retval status
  */
uint8_t data_memory_read_ACK(uint8_t u8ValidLength)
{
    uint8_t          Msg;
    uint8_t          Stat;

    u8ValidLength = u8ValidLength*9/8;
    
    Msg = ack_datalen_to_store_manage(u8ValidLength);

    if(Msg == 0)
    {
        Stat = APP_SUCCESS;
    }
    else
    {
        Stat = ERR_WRITE_EEPROM;
    }

    return Stat;
}

/*******************************************************************************
* Function Name  : DataMemoryRestore
* Description    : Recovery the Data of DataMemory
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DataMemoryRestore(void)
{
//    ExtFLASH_RestorePartition();
//    ExtFLASH_StorageInit();
}
/*******************************************************************************
* Function Name  : DataMemoryEraseAll
* Description    : Erase All Data of DataMemory
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DataMemoryEraseAll(void)
{
//	#ifdef FLASH_DEBUG
//		printf("\r\n[FLASH] DataMemoryEraseAll\r\n");
//	#endif
//    MX25_SPI_Configuration();
//    CMD_CE();
//    Delay_ms(200);
//	ExtFLASH_ClearRAM();
}

/*******************************************************************************
* Function Name  : SetDeviceModeConfigInfo
* Description    : º«¬º≤‚¡øƒ£ Ω…Ë÷√ ±º‰µ„£∫’˝≥£ƒ£ Ωor≤‚ ‘ƒ£ Ω
* Input          : SysConfigCPUInfo_t *info
* Output         : Nont
* Return         : None
*******************************************************************************/
void SetDeviceModeConfigInfo(void)
{
//    /* Cpu Config Info : 0:Month;
//                         1:Day;
//                         2:Hour
//                         3:Min
//                         4:Sec
//                         5: Mode
//    */

//    uint8_t ConfigFile[10] = {0};   //max 10byte of Sys Config File

//	date_str_typedef    date_s;               //RTC »’∆⁄
//	RTC_TimeTypeDef     rtc_time;             //RTC  ±º‰
//    
//    Calendar_Get(&date_s,&rtc_time);
//    
//    ConfigFile[0] = date_s.month;
//	ConfigFile[1] = date_s.day;
//	ConfigFile[2] = rtc_time.RTC_Hours;
//	ConfigFile[3]= rtc_time.RTC_Minutes;
//	ConfigFile[4]= rtc_time.RTC_Seconds;
//    
//    ConfigFile[5]= gFlagHR_SpO2Measure;

//    /* Write Work Mode Config File From STM32 Internal EEPROM */
//    DataEEPROM_MultiByteWrite(ConfigFile,EEPROM_CONFIG_DeviceMode,sizeof(ConfigFile));
}

/**
  * @}
  */

/**
  * @}
*/

/***************** (C) COPYRIGHT iCareTech Healthcare Co., Ltd.(END OF FILE)***********/


