/**
  ****************************************************************************************
  * @file    bsp_rtc.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   bsp_rtc.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "bsp_rtc.h"


/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle;         //RTC handler declaration
static void RTC_CalendarConfig(void);
static RTC_Wakeup_Callback m_pRTC_Wakup_Callback;
static RTC_AlarmA_Callback m_pRTC_AlarmA_Callback;
static RTC_AlarmB_Callback m_pRTC_AlarmB_Callback;


/* Private function prototypes -----------------------------------------------*/
static void Calendar_WeekDayNum(const RTC_DateTypeDef* date_s, week_str_typedef *week_s);



/**
  * @brief  bsp_rtc_init
  * @note   bsp rtc init
  * @param  None
  * @retval HAL status
  */
HAL_StatusTypeDef bsp_rtc_init(void)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    /*##-1- Configure the RTC peripheral #######################################*/
    /* Configure RTC prescaler and RTC data registers */
    /* RTC configured as follows:
        - Hour Format    = Format 24
        - Asynch Prediv  = Value according to source clock
        - Synch Prediv   = Value according to source clock
        - OutPut         = Output Disable
        - OutPutPolarity = High Polarity
        - OutPutType     = Open Drain 
    */ 
    RtcHandle.Instance = RTC; 
    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
    RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    ret = HAL_RTC_Init(&RtcHandle);
    if (ret != HAL_OK)
    {
        /* Initialization Error */
        App_Error_Check(ret);
    }

    /*##-2- Check if Data stored in BackUp register1: No Need to reconfigure RTC#*/
    /* Read the Back Up Register 1 Data */
    if (HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) != 0x32F2)
    {
        /* Configure RTC Calendar */
        RTC_CalendarConfig();
    }
    else
    {
        /* Check if the Power On Reset flag is set */
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) != RESET)
        {

        }
        /* Check if Pin Reset flag is set */
        if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
        {

        }
        /* Clear source Reset Flag */
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }    
    
    return ret;
}
/**
  * @brief  get the current time and date.
  * @param  date_s:  pointer to buffer
  * @param  rtc_time: pointer to buffer
  * @retval status
  */
HAL_StatusTypeDef calendar_get(RTC_DateTypeDef *date_s,RTC_TimeTypeDef *rtc_time)
{
    RTC_DateTypeDef         sdatestructureget;
    RTC_TimeTypeDef         stimestructureget;
    HAL_StatusTypeDef       ret = HAL_ERROR;

    /* Get the RTC current Time */
    ret = HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
    if(ret != HAL_OK)
    {
        App_Error_Check(ret);
    }
    /* Get the RTC current Date */
    ret = HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
    if(ret != HAL_OK)
    {
        App_Error_Check(ret);
    }

    date_s->Year = sdatestructureget.Year;
    date_s->Month = sdatestructureget.Month;
    date_s->Date = sdatestructureget.Date;
    date_s->WeekDay = sdatestructureget.WeekDay;
    
    rtc_time->Hours = stimestructureget.Hours;
    rtc_time->Minutes = stimestructureget.Minutes;
    rtc_time->Seconds = stimestructureget.Seconds;
    
    return ret;
}

/**
  * @brief  set the current time and date.
  * @param  date_s:  pointer to buffer
  * @param  rtc_time: pointer to buffer
  * @retval status
  */
HAL_StatusTypeDef calendar_set(RTC_DateTypeDef *date_s,RTC_TimeTypeDef *rtc_time)
{
    RTC_DateTypeDef         sdatestructure;
    RTC_TimeTypeDef         stimestructure;
    week_str_typedef        m_rtc_week;
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    /* Convent the date to week */
    Calendar_WeekDayNum(date_s, &m_rtc_week);
        
    /* Get the date and time */
    sdatestructure.Year = date_s->Year;
    sdatestructure.Month = date_s->Month;
    sdatestructure.Date = date_s->Date;
    sdatestructure.WeekDay = m_rtc_week.u8WkDayNum;
    
    stimestructure.Hours = rtc_time->Hours;
    stimestructure.Minutes = rtc_time->Minutes;
    stimestructure.Seconds = rtc_time->Seconds;
    stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
    
    /* Set Date */
    ret = HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BIN);
    if(ret != HAL_OK)
    {
        /* Initialization Error */
        App_Error_Check(ret);
    } 
    
    /* Set Time */
    ret = HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BIN);
    if(ret != HAL_OK)
    {
        /* Initialization Error */
        App_Error_Check(ret);
    }
    
    return ret;
}
/**
  * @brief  Wake Up Timer callback.
  * @param  hrtc: RTC handle
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    if(m_pRTC_Wakup_Callback!=NULL)
    {
        m_pRTC_Wakup_Callback();
    }    
}
/**
  * @brief  calendar_rtc_period_wakeup_init()
  * @brief  init the RTC to period wakeup, resolution = 1s/(2^15)
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef calendar_rtc_period_wakeup_init(uint32_t u32SecCnt, RTC_Wakeup_Callback pCallBack)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    /*Init the callback function*/
    m_pRTC_Wakup_Callback = pCallBack; 

    HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);    
    
    /*Configure the NVIC for RTC Wake up ###################################*/
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, RTC_WAKE_UP_EXIT_PREPRIORITY, RTC_WAKE_UP_EXIT_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn); 

    /* Set the wake up time */
    ret = HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle,u32SecCnt,RTC_WAKEUPCLOCK_RTCCLK_DIV2); 
        
    return ret;
}
/**
  * @brief  calendar_rtc_period_wakeup_deinit()
  * @brief  deinit the RTC to period wakeup
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef calendar_rtc_period_wakeup_deinit(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    /*Reset the callback function*/
    m_pRTC_Wakup_Callback = NULL;    
    
    ret = (HAL_StatusTypeDef)HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle);
    
    return ret;
}

/**
  * @brief  AlarmA callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
//    printf("RTC AlarmA\r\n");
    if(m_pRTC_AlarmA_Callback!=NULL)
    {
        m_pRTC_AlarmA_Callback();
    }   
}

/**
  * @brief  set the alarm A.
  * @param  Alarm_HH,Alarm_MM,Alarm_SS
  * @retval status
  */
HAL_StatusTypeDef rtc_set_alarm_A(uint8_t Alarm_HH,uint8_t Alarm_MM,uint8_t Alarm_SS,RTC_AlarmA_Callback pCallBack)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    RTC_AlarmTypeDef        salarmstructure;
    
    /*Init the callback function*/
    m_pRTC_AlarmA_Callback = pCallBack;
    
    /* Disable the Alarm */
    HAL_RTC_DeactivateAlarm(&RtcHandle,RTC_ALARM_A);
    
    /* Configure the RTC Alarm peripheral #################################*/
    /* Set Alarm , RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
    salarmstructure.Alarm = RTC_ALARM_A;
    salarmstructure.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
    salarmstructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    salarmstructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
    salarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    salarmstructure.AlarmTime.Hours = Alarm_HH;
    salarmstructure.AlarmTime.Minutes = Alarm_MM;
    salarmstructure.AlarmTime.Seconds = Alarm_SS;
    salarmstructure.AlarmTime.SubSeconds = 0x56;

    ret = HAL_RTC_SetAlarm_IT(&RtcHandle,&salarmstructure,RTC_FORMAT_BIN);
    
    return ret;
}
/**
  * @brief  disable the alarm A.
  * @param  None
  * @retval status
  */
HAL_StatusTypeDef rtc_disable_alarm_A(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    /*Reset the callback function*/
    m_pRTC_AlarmA_Callback = NULL;
    
    /* Disable the Alarm */
    ret = HAL_RTC_DeactivateAlarm(&RtcHandle,RTC_ALARM_A);
    
    return ret;
}
/**
  * @brief  AlarmB callback
  * @param  hrtc : RTC handle
  * @retval None
  */
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{
//    printf("RTC AlarmB\r\n");
    if(m_pRTC_AlarmB_Callback!=NULL)
    {
        m_pRTC_AlarmB_Callback();
    }     
}

/**
  * @brief  set the alarm B.
  * @param  Alarm_HH,Alarm_MM,Alarm_SS
  * @retval status
  */
HAL_StatusTypeDef rtc_set_alarm_B(uint8_t Alarm_HH,uint8_t Alarm_MM,uint8_t Alarm_SS,RTC_AlarmB_Callback pCallBack)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    RTC_AlarmTypeDef        salarmstructure;
    
    /*Init the callback function*/
    m_pRTC_AlarmB_Callback = pCallBack;
    
    /* Disable the Alarm */
    HAL_RTC_DeactivateAlarm(&RtcHandle,RTC_ALARM_B);
    
    /* Configure the RTC Alarm peripheral #################################*/
    /* Set Alarm , RTC Alarm Generation: Alarm on Hours, Minutes and Seconds */
    salarmstructure.Alarm = RTC_ALARM_B;
    salarmstructure.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY;
    salarmstructure.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
    salarmstructure.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
    salarmstructure.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
    salarmstructure.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
    salarmstructure.AlarmTime.Hours = Alarm_HH;
    salarmstructure.AlarmTime.Minutes = Alarm_MM;
    salarmstructure.AlarmTime.Seconds = Alarm_SS;
    salarmstructure.AlarmTime.SubSeconds = 0x56;

    ret = HAL_RTC_SetAlarm_IT(&RtcHandle,&salarmstructure,RTC_FORMAT_BIN);
    
    return ret;
}
/**
  * @brief  disable the alarm B.
  * @param  None
  * @retval status
  */
HAL_StatusTypeDef rtc_disable_alarm_B(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    /*Reset the callback function*/
    m_pRTC_AlarmB_Callback = NULL;
    
    /* Disable the Alarm */
    ret = HAL_RTC_DeactivateAlarm(&RtcHandle,RTC_ALARM_B);
    
    return ret;
}

/**
* @brief  将时、分、秒时间转换为unix时间
  * @param  date
  * @retval None
  */
uint32_t GetTick(int YY,int MM,int DD,int HH,int MMin,int SS)
{
    struct tm stm;
    uint32_t tim = 0;

    memset(&stm,0,sizeof(stm));

    stm.tm_year=YY-1900;
    stm.tm_mon=MM-1;
    stm.tm_mday=DD;
    stm.tm_hour=HH;
    stm.tm_min=MMin;
    stm.tm_sec=SS;

    tim = mktime(&stm);
    
    return (tim - 28800);  //转换为北京时间
}
/**
* @brief  将unix时间转换为时分秒
  * @param  tim
  * @retval None
  */
void OutputGMTIME(long tim,RTC_DateTypeDef *date_s,RTC_TimeTypeDef *rtc_time)
{      
	time_t t=0;
	struct tm *p;

	t = tim + 28800; //转换为北京时间
	p = localtime(&t);

    date_s->Year = p->tm_year+1900;
    date_s->Month = p->tm_mon+1;
    date_s->Date = p->tm_mday;
    
    rtc_time->Hours = p->tm_hour;
    rtc_time->Minutes = p->tm_min;
    rtc_time->Seconds = p->tm_sec;
}
/**
  * @brief  将当前日期转换为32位数据标示
  * @param  None
  * @retval date
  */
uint32_t CovernDateto32(void)
{
	uint32_t date=0;

	RTC_DateTypeDef     date_s;      //RTC 日期
	RTC_TimeTypeDef     rtc_time;    //RTC 时间

    calendar_get(&date_s,&rtc_time); //获取当前时间

	date = (uint32_t)(((date_s.Year+2000) << 16) | (date_s.Month << 8) | (date_s.Date));

	return date;
}
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_CalendarConfig(void)
{
    RTC_DateTypeDef sdatestructure;
    RTC_TimeTypeDef stimestructure;
    HAL_StatusTypeDef ret = HAL_ERROR;

    /*##-1- Configure the Date #################################################*/
    /* Set Date: Tuesday February 18th 2014 */
    sdatestructure.Year = 0x14;
    sdatestructure.Month = RTC_MONTH_FEBRUARY;
    sdatestructure.Date = 0x18;
    sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;

    ret = HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD);
    if(ret != HAL_OK)
    {
        /* Initialization Error */
        App_Error_Check(ret);
    }

    /*##-2- Configure the Time #################################################*/
    /* Set Time: 02:00:00 */
    stimestructure.Hours = 0x02;
    stimestructure.Minutes = 0x00;
    stimestructure.Seconds = 0x00;
    stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
    stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
    stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

    ret = HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD);
    if (ret != HAL_OK)
    {
        /* Initialization Error */
        App_Error_Check(ret);
    }

    /*##-3- Writes a data in a RTC Backup data Register1 #######################*/
    HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, 0x32F2);
}
/**
  * @brief  Determines the week number, the day number and the week day number.
  * @param  None
  * @retval None
  */
static void Calendar_WeekDayNum(const RTC_DateTypeDef* date_s, week_str_typedef *week_s)
{
  uint32_t a = 0, b = 0, c = 0, s = 0, e = 0, f = 0, g = 0, d = 0;
  int32_t n = 0;
  if (date_s->Month < 3)
  {
    a = date_s->Year - 1;
  }
  else
  {
    a = date_s->Year;
  }

  b = (a / 4) - (a / 100) + (a / 400);
  c = ((a - 1) / 4) - ((a - 1) / 100) + ((a - 1) / 400);
  s = b - c;
  if (date_s->Month < 3)
  {
    e = 0;
    f =  date_s->Date - 1 + 31 * (date_s->Month - 1);
  }
  else
  {
    e = s + 1;
    f = date_s->Date + (153 * (date_s->Month - 3) + 2) / 5 + 58 + s;
  }
  g = (a + b) % 7;
  d = (f + g - e) % 7;
  n = f + 3 - d;
  if (n < 0)
  {
    week_s->u8WkNum = 53 - ((g - s) / 5);
  }
  else if (n > (364 + s))
  {
    week_s->u8WkNum = 1;
  }
  else
  {
    week_s->u8WkNum = (n / 7) + 1;
  }
  week_s->u8WkDayNum = d + 1;
  week_s->u8DayNum = f + 1;
}



/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

