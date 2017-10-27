/**
  ****************************************************************************************
  * @file    bsp_rtc.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   header file of bsp_rtc.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_RTC_H_
#define __BSP_RTC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"
#include "time.h"


/* Week Structure definition */
typedef struct _week_t
{
  uint8_t u8WkNum;
  uint8_t u8DayNum;
  uint8_t u8WkDayNum;
}week_str_typedef;

/* Exported macros -----------------------------------------------------------*/
typedef void (*RTC_Wakeup_Callback) (void);
typedef void (*RTC_AlarmA_Callback) (void);
typedef void (*RTC_AlarmB_Callback) (void);

/* Exported variables--------------------------------------------------------*/
extern RTC_HandleTypeDef RtcHandle;         //RTC handler declaration


/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef bsp_rtc_init(void);
HAL_StatusTypeDef calendar_get(RTC_DateTypeDef *date_s,RTC_TimeTypeDef *rtc_time);
HAL_StatusTypeDef calendar_set(RTC_DateTypeDef *date_s,RTC_TimeTypeDef *rtc_time);
HAL_StatusTypeDef rtc_set_alarm_A(uint8_t Alarm_HH,uint8_t Alarm_MM,uint8_t Alarm_SS,RTC_AlarmA_Callback pCallBack);
HAL_StatusTypeDef rtc_disable_alarm_A(void);
HAL_StatusTypeDef rtc_set_alarm_B(uint8_t Alarm_HH,uint8_t Alarm_MM,uint8_t Alarm_SS,RTC_AlarmB_Callback pCallBack);
HAL_StatusTypeDef rtc_disable_alarm_B(void);
HAL_StatusTypeDef calendar_rtc_period_wakeup_init(uint32_t u32SecCnt, RTC_Wakeup_Callback pCallBack);
HAL_StatusTypeDef calendar_rtc_period_wakeup_deinit(void);
uint32_t GetTick(int YY,int MM,int DD,int HH,int MMin,int SS);
void OutputGMTIME(long tim,RTC_DateTypeDef *date_s,RTC_TimeTypeDef *rtc_time);
uint32_t CovernDateto32(void);


#endif /* __BSP_RTC_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

