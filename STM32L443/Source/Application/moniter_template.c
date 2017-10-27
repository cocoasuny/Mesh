/**
  ****************************************************************************************
  * @file    moniter template.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-28
  * @brief   实现监护方案功能
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "moniter_template.h"


/* Private variables ---------------------------------------------------------*/
Monitor_Template_Typedef                            g_MonitorTemplate;
static Excetion_Monitor_Template_Typedef            ExcetionMonitorTemplate;
static bool                                         flagIsFreeRunKickOff = false; //FreeRun模式运行状态标志  
 
/* Private function prototypes -----------------------------------------------*/
static char *strtok_r(char *s, const char *delim, char **save_ptr);
static bool isFreeRunKickOff(void);
static void free_run_mode_init(Monitor_Template_Typedef v_MonitorTemplate);
static void free_run_mode_deInit(Monitor_Template_Typedef v_MonitorTemplate);
static bool get_light_monitor_template_status(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime);
static void free_run_monitor_template_set_light_Ctl_Time(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime);
static void free_run_monitor_template_set_night_Ctl_Time(Monitor_Template_Typedef v_MonitorTemplate, uint16_t CurTime);
static void monitor_template_alarmA_IRQHandler(void);
static uint8_t adjust_moniter_template_run_time(Monitor_Template_Typedef v_MonitorTemplate, uint16_t CurTime, uint16_t NextTime);
static void free_run_monitor_template_changeTo_ninghtMT(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime);
static int8_t find_next_alarm_time_point(Monitor_Template_Typedef v_MonitorTemplate,
                                        uint16_t CurrTime,uint8_t *AlarmHH,uint8_t *AlarmMM);
static void free_run_monitor_template_status_monitor(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime);
static void FreeRun_SampleData_Storage_Callback(void);
static void set_freeRun_night_start_frame(Monitor_Template_Typedef v_MonitorTemplate,RTC_DateTypeDef date_s,
                                            RTC_TimeTypeDef rtc_time);
static void freeRun_sample_data_storageFunc(Monitor_Template_Typedef v_MonitorTemplate);


/**
  * @brief  monitor_template_task_handler
  * @note   monitor template task handler
  * @param  *pvParameters
  * @retval None
  */
void monitor_template_task_handler(void *pvParameters)
{
    BaseType_t                      xResult = pdFALSE;
    const TickType_t                xTicksToWait = 100 / portTICK_PERIOD_MS; /* 最大等待时间100ms */
	MONITOR_TEMPLATE_MSG_T			monitorTemplateQueueMsgValue;
    Monitor_Template_Typedef        v_MonitorTemplate;
    uint8_t 		                workDate = 0x01;//work day
    RTC_DateTypeDef                 date_s;
    RTC_TimeTypeDef                 rtc_time;
    uint16_t                        CurTime = 0;    //当前时间装换为分钟
    bool                            flagLightMonitorTemplateStatus = OFF;  //ON:白天，OFF：晚上
    uint8_t                         AlarmHH = 0;
    uint8_t                         AlarmMM = 0;
    
	/* monitor template event queue init */
    monitorTemplateQueueMsgValue.eventID = MONITOR_TEMPLATE_EVENT_NULL;   
    
    while(1)
    {
        //{code block} handle with Queue 
        {
			/* Wait for a message */
            xResult = xQueueReceive(monitorTemplateEvevtQueue,&monitorTemplateQueueMsgValue,xTicksToWait);
            if(xResult == pdPASS)
            { 			
				xResult = pdFALSE;
				switch(monitorTemplateQueueMsgValue.eventID)
				{
					case MONITOR_TEMPLATE_EVENT_SET:  //monitor template set event
					{						
						/* load the monitor template to local variable */
						portENTER_CRITICAL();
						memcpy(&v_MonitorTemplate,&g_MonitorTemplate,sizeof(v_MonitorTemplate));
						portEXIT_CRITICAL();
						
						/* get the current time and convernt to minutes */
						calendar_get(&date_s,&rtc_time);
						CurTime = rtc_time.Hours*60 + rtc_time.Minutes;
						#ifdef MONITOR_TEMPLATE_DEBUG
							printf("monitor template set:%d:%d %d\r\n",rtc_time.Hours,rtc_time.Minutes,date_s.WeekDay);
						#endif

						if((v_MonitorTemplate.SampleWorkDay & (workDate << (date_s.WeekDay))) 
							== (workDate << (date_s.WeekDay)))      //当天有监护方案需要运行
						{
							rtc_disable_alarm_A();
							
							if(v_MonitorTemplate.SampleFreq == 0) //按照时间点采集
							{
								//暂时未使用
							}
							else // 按照时间段采集
							{
								CurTime = rtc_time.Hours*60 + rtc_time.Minutes; //时间转换
								
								if(v_MonitorTemplate.MTSwitch == 0)  //暂停监测模板
								{
									#ifdef MONITOR_TEMPLATE_DEBUG
										printf("Monitor Template Suspend\r\n");
									#endif
									
									rtc_disable_alarm_A();
									if(
										(v_MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID) && 
										(true == isFreeRunKickOff())
									)
									{
										//处于FreeRun模式类监护方案，并且在运行
										free_run_mode_deInit(v_MonitorTemplate);  //停止FreeRun模式
									}                            
								}
								else //监测模板未暂停,运行状态
								{
									if(v_MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID)  //FreeRun模式类方案
									{
										flagLightMonitorTemplateStatus = get_light_monitor_template_status(v_MonitorTemplate,CurTime);
										#ifdef MONITOR_TEMPLATE_DEBUG
											printf("Light MT Status:%d\r\n",flagLightMonitorTemplateStatus);
										#endif
										if(flagLightMonitorTemplateStatus == ON) //在白天方案运行时间段内
										{
											free_run_monitor_template_set_light_Ctl_Time(v_MonitorTemplate,CurTime);
										}
										else  //不在白天方案运行时间段内，夜间方案运行
										{
											free_run_monitor_template_set_night_Ctl_Time(v_MonitorTemplate,CurTime);
										}
									}
									else//v_MonitorTemplate.MTID != kICTMonitorTemplateFreeRunID(个性化方案)
									{
										if(true == isFreeRunKickOff())  //Free run is kick off,stop the free run
										{
											#ifdef MONITOR_TEMPLATE_DEBUG
												printf("deinit free run when change MT\r\n");
											#endif
											
											rtc_disable_alarm_A();
											free_run_mode_deInit(v_MonitorTemplate);
										}
										
										if(0 == find_next_alarm_time_point(v_MonitorTemplate,CurTime,&AlarmHH,&AlarmMM))
										{
											#ifdef MONITOR_TEMPLATE_DEBUG
												printf("Set light MT start alarm:%d,%d\r\n",AlarmHH,AlarmMM);
											#endif                                    
											rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
										}
										#ifdef MONITOR_TEMPLATE_DEBUG
										else
										{
											printf("find next alarm time err\r\n");
										}
										#endif
									}
								}
							}
						}
						#ifdef MONITOR_TEMPLATE_DEBUG
						else  //当天没有检测模板需要运行
						{
							printf("There is no monitor template in the day:%d",date_s.WeekDay);
						}
						#endif
					}//end of if(uxBits & MONITOR_TEMPLATE_EVENT_SET)
					break;
					case MONITOR_TEMPLATE_EVENT_SAMPLE_DATA:  //monitor template sample data event
					{
						/* get the current time and convernt to minutes */
						calendar_get(&date_s,&rtc_time);
						CurTime = rtc_time.Hours*60 + rtc_time.Minutes;
						
						#ifdef MONITOR_TEMPLATE_DEBUG
							printf("The current time is :  %0.2d:%0.2d:%0.2d\r\n",rtc_time.Hours,rtc_time.Minutes,rtc_time.Seconds);
						#endif
						/* load the monitor template to local variable */
						portENTER_CRITICAL();
						memcpy(&v_MonitorTemplate,&g_MonitorTemplate,sizeof(v_MonitorTemplate));
						portEXIT_CRITICAL();
			
						/* ##1 FreeRun Mode 状态调整*/
						if(v_MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID)
						{
							flagLightMonitorTemplateStatus = get_light_monitor_template_status(v_MonitorTemplate,CurTime);
							if(
								(flagLightMonitorTemplateStatus == OFF) ||
								((flagLightMonitorTemplateStatus == ON) && (flagIsFreeRunKickOff == true))
							)  //不在白天方案运行中，夜间方案运行
							{
								free_run_monitor_template_status_monitor(v_MonitorTemplate,CurTime);
							}
						}
						
						/* ##2 监护方案参数采集 */
						flagLightMonitorTemplateStatus = get_light_monitor_template_status(v_MonitorTemplate,CurTime);
						if(
							(v_MonitorTemplate.MTID != kICTMonitorTemplateFreeRunID) ||
							((v_MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID) && (flagLightMonitorTemplateStatus == ON))
						)  //个性化方案运行或者管理类方案白天部分运行
						{
							/* 产生当前监测模板采集数据事件 */
							if(v_MonitorTemplate.SampleID & SAMPLE_ID_SPO2)  //采集血氧
							{
								#ifdef MONITOR_TEMPLATE_DEBUG
									printf("Monitor Template SpO2\r\n");
								#endif
								gSubFunc_Stat_Set(SpO2_MonitorTemplate_State,ON); //SpO2监测模板在运行
								
								/* 开启血氧测量 */
		//                        apl_collection_event_post(CollectionStart);
							}

							if(v_MonitorTemplate.SampleID & SAMPLE_ID_HR)   //采集心率
							{
								#ifdef MONITOR_TEMPLATE_DEBUG
									printf("Monitor Template Sample HeartRate\r\n");
								#endif
								gSubFunc_Stat_Set(HR_MonitorTemplate_State, ON);    //HR监测模板在运行

								/* 开启心率测量 */
		//                        apl_collection_event_post(CollectionStart);
							}
						}
						
						/* ##3 计算个性化方案下次测量时间  */
						if(v_MonitorTemplate.MTID != kICTMonitorTemplateFreeRunID)
						{
							if(v_MonitorTemplate.SampleFreq == 0) //按照时间点采集
							{
								//暂时未使用
							}
							else  //按照时间段采集
							{
								if(v_MonitorTemplate.MTSwitch == 0) //监护方案暂停
								{
									rtc_disable_alarm_A();
									#ifdef MONITOR_TEMPLATE_DEBUG
										printf("Suspend MT\r\n");
									#endif                            
								}
								else  //计算下次采集时间点
								{
									if(0 == find_next_alarm_time_point(v_MonitorTemplate,CurTime,&AlarmHH,&AlarmMM))
									{
										#ifdef MONITOR_TEMPLATE_DEBUG
											printf("next alarm:%d,%d\r\n",AlarmHH,AlarmMM);
										#endif                                    
										rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
									}                        
								}
							}
						}
						
						/* ##4 计算管理类方案白天部分方案下次测量时间 */
						if(
							(v_MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID) && 
							(v_MonitorTemplate.LightMTinfo.LightSampleFreq != 0)
						)
						{
							if(v_MonitorTemplate.MTSwitch == 0) //监护方案暂停
							{
								rtc_disable_alarm_A();
								#ifdef MONITOR_TEMPLATE_DEBUG
									printf("Suspend MT\r\n");
								#endif                            
							}
							else  //方案未暂停，计算管理类方案白天部分下次测量时间
							{
								flagLightMonitorTemplateStatus = get_light_monitor_template_status(v_MonitorTemplate,CurTime);
								if(flagLightMonitorTemplateStatus == ON) //在白天方案运行时间段内
								{
									free_run_monitor_template_set_light_Ctl_Time(v_MonitorTemplate,CurTime);
								}
								else  //不在白天方案运行时间段内，夜间方案运行
								{
									free_run_monitor_template_set_night_Ctl_Time(v_MonitorTemplate,CurTime);
								}
							}
						}
					}//end of if(uxBits & MONITOR_TEMPLATE_EVENT_SAMPLE_DATA)
					break;
					case MONITOR_TEMPLATE_EVENT_STORE_DATA:  //monitor template storage data event
					{						
						/* load the monitor template to local variable */
						portENTER_CRITICAL();
						memcpy(&v_MonitorTemplate,&g_MonitorTemplate,sizeof(v_MonitorTemplate));
						portEXIT_CRITICAL();

						/* write freerun mode monitor template data to memory */ 
						freeRun_sample_data_storageFunc(v_MonitorTemplate);
						
					}//end of if(uxBits & MONITOR_TEMPLATE_EVENT_STORE_DATA)
					break;
					default:
						break;
				}//end of switch(monitorTemplateQueueMsgValue.eventID)
			}// end of if(xResult == pdPASS)
        }
		//end of {code block}handle with event group             
    }
}


/* Parse S into tokens separated by characters in DELIM.
   If S is NULL, the saved pointer in SAVE_PTR is used as
   the next starting point.  For example:
        char s[] = "-abc-=-def";
        char *sp;
        x = strtok_r(s, "-", &sp);      // x = "abc", sp = "=-def"
        x = strtok_r(NULL, "-=", &sp);  // x = "def", sp = NULL
        x = strtok_r(NULL, "=", &sp);   // x = NULL
                // s = "abc\0-def\0"
*/
static char *strtok_r(char *s, const char *delim, char **save_ptr)
{
    char *token;

    if (s == NULL)
	{
		s = *save_ptr;
	}

    /* Scan leading delimiters.  */
    s += strspn(s, delim);
    if (*s == '\0')
	{
		return NULL;
	}

    /* Find the end of the token.  */
    token = s;
    s = strpbrk(token, delim);
    if (s == NULL)
	{
        /* This token finishes the string.  */
        *save_ptr = strchr(token, '\0');
	}
    else
	{
        /* Terminate the token and make *SAVE_PTR point past it.  */
        *s = '\0';
        *save_ptr = s + 1;
    }

    return token;
}

/**
  * @brief  UnPackMonitorTemplate 
  * @note   解包监护模板数据
  * @param  p_MonitorTemplate,len
  * @retval None
  */
void unpack_monitor_template(const uint8_t *p_MonitorTemplate,uint16_t Len)
{
    uint8_t                                     i = 0;
    static char                                 OldTemplate[MAX_MT_DATA_LEN + 1] = {0};
    char                                        Template[MAX_MT_DATA_LEN+1]={0};
    char                                        *buf=NULL;
    char                                        *p[20]={0};
    char                                        *inner_ptr=NULL;
    char                                        *outer_ptr=NULL;
    uint16_t                                    in=0;
    uint8_t 	                                str_len_t=0;
    uint8_t                                     TempH1=0;
    uint8_t                                     TempL1=0;
    uint8_t                                     TempH2=0;
    uint8_t                                     TempL2=0;
    uint8_t                                     MTID = kICTMonitorTemplateNone;
    Monitor_Template_Typedef                    v_MonitorTemplate;
    Excetion_Monitor_Template_Typedef           v_ExcetionMonitorTemplate;
	MONITOR_TEMPLATE_MSG_T						monitorTemplateQueueMsgValue;
	const TickType_t                			xTicksToWait = 100 / portTICK_PERIOD_MS; /* 最大等待时间100ms */
    
    #ifdef MONITOR_TEMPLATE_DEBUG
        printf("Unpack MonitorTemplate\r\n");
        for(i=0;i<Len;i++)
        {
            printf("%c",p_MonitorTemplate[i]);
        }
        printf("\r\n");
    #endif

	/* monitor template event init */
	monitorTemplateQueueMsgValue.eventID = MONITOR_TEMPLATE_EVENT_NULL;
		
    for(i=0;i<(Len);i++)
    {
        Template[i]=p_MonitorTemplate[i];
    }

    if(0 == memcmp(OldTemplate, Template, sizeof(Template)))
    {
        #ifdef MONITOR_TEMPLATE_DEBUG
            printf("***The same template!!!\r\n");
        #endif
        return ;
    }
    memcpy(OldTemplate, Template, sizeof(Template));
    Template[i] = '\0';		/* 	Add the end delimt 		*/ 

    buf=Template;
    
    while((p[in] = strtok_r(buf, "#", &outer_ptr))!=NULL)   //提取监测模板
    {
        buf=p[in];
        while((p[in]=strtok_r(buf, "|", &inner_ptr))!=NULL)  
        {
            in++;
            buf=NULL;
        }
        in++;
        buf=NULL;
    }

    #ifdef MONITOR_TEMPLATE_DEBUG
    for(i=0;i<(in-1);i++)   //打印分割后的监测模板
    {
        printf("p[%d]= %s,len=%d\r\n",i,p[i],strlen(p[i]));
    }
    #endif
    
    /* 解析监护方案 */
    v_MonitorTemplate.SampleFreq = atoi(p[0]);    //p[0]保存监测频率
    v_MonitorTemplate.SampleWorkDay = 0x00;       //清除WorkDay
    for(i=0;i<7;i++)                              //p[2]保存WorkDay
    {
        if(p[2][i] == 0x31)
        {
            v_MonitorTemplate.SampleWorkDay |= (0x80 >> (i+1));
        }
    }
    v_MonitorTemplate.SampleLastTime = atoi(p[3]);    //p[3]保存每次采集持续时间
    v_MonitorTemplate.SampleID = 0x0000;              //清除采集参数ID   
    str_len_t = strlen(p[4]);
    for(i = 0;i < str_len_t;i++)                      //p[4]保存采集参数ID
    {
        if(p[4][i] == 0x31)
        {
            v_MonitorTemplate.SampleID |= (0x0001 << (str_len_t-i-1));
        }
    }
    v_ExcetionMonitorTemplate.SampleLastTime = atoi(p[6]);    //p[6]保存异常连接采集时间
    v_ExcetionMonitorTemplate.SampleFreq = atoi(p[7]);        //p[7]保存异常采集频率
    v_MonitorTemplate.MTSwitch = atoi(p[8]);                  //p[8]保存监测模板开关
    v_MonitorTemplate.VibrateSwitch = atoi(p[9]);             //p[9]振动开关
    if((in-1) > 10) //兼容监测模板中未发运行历史采集频率及监测模板ID标示
    {
        v_MonitorTemplate.SetMTID = atoi(p[10]);              //p[10]保存监测模板ID
        v_MonitorTemplate.MotionInfoUpdateFreq = atoi(p[11]); //p[11]保存运动数据更新频率
        //p[12]:白天采集时间段
        v_MonitorTemplate.LightMTinfo.LightSampleFreq = atoi(p[13]);   //p[13]保存白天采集频率
    }
    else   //以前版本App，未加，用默认
    {
        v_MonitorTemplate.SetMTID = kICTMonitorTemplateAltitudeStressID;  //默认监测模板ID
        v_MonitorTemplate.SampleID = 7;
        v_ExcetionMonitorTemplate.SampleID = 7;
        v_MonitorTemplate.MotionInfoUpdateFreq = 900; //默认运行数据更新频率
        v_MonitorTemplate.LightMTinfo.LightSampleFreq = 0;
    }

    /* Change kICTMonitorTemplateCOPD/kICTMonitorTemplateHypoxemia/kICTMonitorTemplateOSA to kICTMonitorTemplateFreeRunID */
    if(
        (v_MonitorTemplate.SetMTID == kICTMonitorTemplateCOPD) ||
        (v_MonitorTemplate.SetMTID == kICTMonitorTemplateHypoxemia) ||
        (v_MonitorTemplate.SetMTID == kICTMonitorTemplateOSA) ||
        (v_MonitorTemplate.SetMTID == kICTMonitorTemplateSleepApneaManagerID) ||
        (v_MonitorTemplate.SetMTID == kIctmonitorTemplateNightLowSpo2ManagerID)
    )
    {
        v_MonitorTemplate.MTID = kICTMonitorTemplateFreeRunID;
    }
    else
    {
        v_MonitorTemplate.MTID = v_MonitorTemplate.SetMTID;
    }  

    if(
        (v_MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID) &&
        (v_MonitorTemplate.SampleFreq > kICTMonitorTemplateFreeRunMaxSampleFreq)
    )
    {
         v_MonitorTemplate.SampleFreq = 1;  //set the OSAS SampleFreq to default
    }
    v_MonitorTemplate.SampleTimeClock.DataLen = (strlen(p[1]) / 3)+1;
    
    if(v_MonitorTemplate.SampleTimeClock.DataLen > MAX_MT_LEN)  //接收的监测模板长度大于最大监测模板长度
    {
        #ifdef MONITOR_TEMPLATE_DEBUG
            printf("Err1:  Receive MT Len Beyond MaxMTLen\r\n");
        #endif
        return ;
    }
    memset(v_MonitorTemplate.SampleTimeClock.Context,0,MAX_MT_LEN*sizeof(uint8_t));
    if(v_MonitorTemplate.SampleFreq == 0)//按照时间点采集
    {
        for(i=0;i<(v_MonitorTemplate.SampleTimeClock.DataLen);i++)        //p[1]保存每天监测时间点
        {
            v_MonitorTemplate.SampleTimeClock.Context[i]=(uint8_t)(((p[1][i*3])-0x30)*10 + (p[1][i*3+1]-0x30));
        }
    }
    else  //按照时间段采集，将其划分为时间点
    {
        
        memset(v_MonitorTemplate.ClockArr,0,MAX_CLOCK_ARR_LEN*sizeof(uint8_t));
        
        if(v_MonitorTemplate.SampleTimeClock.DataLen > MAX_CLOCK_ARR_LEN)  //接收的监测模板长度大于最大监测模板长度
        {
            return ;
        }

        for(i=0;i<(v_MonitorTemplate.SampleTimeClock.DataLen);i++)        //p[1]保存每天监测时间段
        {
            v_MonitorTemplate.ClockArr[i]=(uint8_t)(((p[1][i*3])-0x30)*10 + (p[1][i*3+1]-0x30));
        }

        if((v_MonitorTemplate.SampleTimeClock.DataLen/4) == 2)  //有跨天方案
        {
            TempH1 = v_MonitorTemplate.ClockArr[4];
            TempL1 = v_MonitorTemplate.ClockArr[5];
            TempH2 = v_MonitorTemplate.ClockArr[6];
            TempL2 = v_MonitorTemplate.ClockArr[7];

            v_MonitorTemplate.ClockArr[4] = v_MonitorTemplate.ClockArr[0];
            v_MonitorTemplate.ClockArr[5] = v_MonitorTemplate.ClockArr[1];
            v_MonitorTemplate.ClockArr[6] = v_MonitorTemplate.ClockArr[2];
            v_MonitorTemplate.ClockArr[7] = v_MonitorTemplate.ClockArr[3];

            v_MonitorTemplate.ClockArr[0] = TempH1;
            v_MonitorTemplate.ClockArr[1] = TempL1;
            v_MonitorTemplate.ClockArr[2] = TempH2;
            v_MonitorTemplate.ClockArr[3] = TempL2;
        }

        /*  stop the current FreeRun Monitor, when Monitor Changed  */
        /* 	Read device config file from DataEEPROM 	*/
//        GetSysConfigInfo(&DeviceCFInfo);
//        MTID = DeviceCFInfo.MTID;

        if(v_MonitorTemplate.SetMTID != MTID)   //更换监护方案时，停止当前FreeRun模式
        {
            MTID = v_MonitorTemplate.SetMTID;

//            DeviceCFInfo.MTID = MTID;

//            /* 	Write Modify MT ID into DataEEPROM */
//            SetSysConfigInfo(DeviceCFInfo);

//            /*  Stop while in free run */
//            if((v_MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID) && (true == isFreeRunKickOff()))
//            {
//                TS_SendEvent(gTsSpO2TaskID_c,gSpO2EventStop);			/* 	Stop HR/SPO2 realtime sample 	*/

//                /* 	De-init RTC autowake up interrupt 		*/
//                Calendar_RTC_Period_Wakeup_DeInit();

//                /* 	Set Flag to indicate that FreeRun Mode has stoped 		*/
//                flagIsFreeRunKickOff = false;
//            }
        }
    }//end of 按时间段采集

    /* Light Monitor Templte info Analysis */
    v_MonitorTemplate.LightMTinfo.DataLen = (strlen(p[12]) / 3) + 1;
    if(v_MonitorTemplate.LightMTinfo.DataLen > MAX_CLOCK_ARR_LEN)  //接收的白天监护方案长度大于最大长度
    {
        #ifdef MONITOR_TEMPLATE_DEBUG
            printf("Err1:  Receive Light MT Len Beyond MaxMTLen\r\n");
        #endif
        return ;
    }
    memset(v_MonitorTemplate.LightMTinfo.LightMTClockArr,0,MAX_CLOCK_ARR_LEN*sizeof(uint8_t));
    if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
    {
        //白天监护方案监测频率为0，不执行白天部分

    }
    else  //有白天监测方案
    {
        for(i=0;i<(v_MonitorTemplate.LightMTinfo.DataLen);i++)   //p[12]保存白天监护方案时间段
        {
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[i] = (uint8_t)(((p[12][i*3])-0x30)*10 + (p[12][i*3+1]-0x30));
        }

        if((v_MonitorTemplate.LightMTinfo.DataLen/4) == 2)  //有跨天
        {
            TempH1 = v_MonitorTemplate.LightMTinfo.LightMTClockArr[4];
            TempL1 = v_MonitorTemplate.LightMTinfo.LightMTClockArr[5];
            TempH2 = v_MonitorTemplate.LightMTinfo.LightMTClockArr[6];
            TempL2 = v_MonitorTemplate.LightMTinfo.LightMTClockArr[7];

            v_MonitorTemplate.LightMTinfo.LightMTClockArr[4] = v_MonitorTemplate.LightMTinfo.LightMTClockArr[0];
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[5] = v_MonitorTemplate.LightMTinfo.LightMTClockArr[1];
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[6] = v_MonitorTemplate.LightMTinfo.LightMTClockArr[2];
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[7] = v_MonitorTemplate.LightMTinfo.LightMTClockArr[3];

            v_MonitorTemplate.LightMTinfo.LightMTClockArr[0] = TempH1;
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[1] = TempL1;
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[2] = TempH2;
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[3] = TempL2;
        }
    }

    /* Copy the MonitorTemplate */
    portENTER_CRITICAL();
    memcpy(&g_MonitorTemplate,&v_MonitorTemplate,sizeof(v_MonitorTemplate));
    memcpy(&ExcetionMonitorTemplate,&v_ExcetionMonitorTemplate,sizeof(v_ExcetionMonitorTemplate));
    portEXIT_CRITICAL();
    
    #ifdef MONITOR_TEMPLATE_DEBUG
        printf("Monitor Template info...\r\n");
        printf("MT_Freq:%d\r\n",g_MonitorTemplate.SampleFreq);
//			for(i=0;i<g_MonitorTemplate.SampleTimeClock.DataLen;i++)
//			{
//				printf("MT_STC[%d]=%d\r\n",i,g_MonitorTemplate.SampleTimeClock.Context[i]);
//			}
//			printf("\r\n");
        printf("MT_WorkDay=%x\r\n",g_MonitorTemplate.SampleWorkDay);
        printf("MT_LastTime=%d\r\n",g_MonitorTemplate.SampleLastTime);
        printf("MT_SampleID=%x\r\n",g_MonitorTemplate.SampleID);
        printf("MT_VibrateSwitch=%d\r\n",g_MonitorTemplate.VibrateSwitch);
        printf("MT_MTID=%d\r\n",g_MonitorTemplate.MTID);
        printf("MT_MotionInfoUpdateFreq=%d\r\n",g_MonitorTemplate.MotionInfoUpdateFreq);
    #endif 

    /* 设置监护方案 */
	monitorTemplateQueueMsgValue.eventID = MONITOR_TEMPLATE_EVENT_SET;
    xQueueSend(monitorTemplateEvevtQueue,(void *)&monitorTemplateQueueMsgValue,xTicksToWait);
}
/**
  * @brief  isFreeRunKickOff 
  * @note   判断FreeRun模式是否在运行
  * @param  None
  * @retval None
  */
static bool isFreeRunKickOff(void)
{
	return flagIsFreeRunKickOff;
}
/**
  * @brief  FreeRun_SampleData_Storage_Callback 
  * @note   FreeRun采集数据回调函数，！！！这是一个中断服务函数
  * @param  None
  * @retval None
  */
static void FreeRun_SampleData_Storage_Callback(void)
{
    BaseType_t          		xHigherPriorityTaskWoken,xResult;
	MONITOR_TEMPLATE_MSG_T		monitorTemplateQueueMsgValue;

	monitorTemplateQueueMsgValue.eventID = MONITOR_TEMPLATE_EVENT_STORE_DATA;
	xResult = xQueueSendFromISR(monitorTemplateEvevtQueue,(void *)&monitorTemplateQueueMsgValue,&xHigherPriorityTaskWoken);

    /* Was the message posted successfully? */
    if( xResult != pdFAIL )
    {
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }  
}
/**
  * @brief  FreeRunModeInit 
  * @note   FreeRun模式初始化，开始运行
  * @param  None
  * @retval None
  */
static void free_run_mode_init(Monitor_Template_Typedef v_MonitorTemplate)
{
    RTC_DateTypeDef                 date_s;
    RTC_TimeTypeDef                 rtc_time;
    
    /* Get the time */
    calendar_get(&date_s,&rtc_time);
    
    /* Disable immediately measure when FreeRun mode */
    gSubFunc_Stat_Set(HR_RealTimeSample_State | SpO2_RealTimeSample_State,OFF);
    
    /* Notice App, FreeRun Monitor Template is running, if in Device_Mode_CheckUp */
    // @todo
    
    /* Create The Moniter Template Data Store Partition */
    creatOSAFile(); 

    /* Set The Free Run Start Frame */
    set_freeRun_night_start_frame(v_MonitorTemplate,date_s,rtc_time);
    
    /* Init gHRSpO2Val.HR_Value and gHRSpO2Val.SpO2_Value to 0 */
    g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8HrVal = 0;
    g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8SpO2Val = 0;

	/* 	Set Flag to indicate that FreeRun Mode has kicked off 		*/
	flagIsFreeRunKickOff = true;

	/* 	Kick off RTC autowake up interrupt 		*/
	calendar_rtc_period_wakeup_init(16384*v_MonitorTemplate.SampleFreq,FreeRun_SampleData_Storage_Callback);    
}
/**
  * @brief  FreeRunModeDeInit 
  * @note   FreeRun模式停止
  * @param  None
  * @retval None
  */
static void free_run_mode_deInit(Monitor_Template_Typedef v_MonitorTemplate)
{
    RTC_DateTypeDef                 date_s;
    RTC_TimeTypeDef                 rtc_time;
    uint16_t                        CurTime = 0;
    bool                            flagLightMonitorTemplateStatus = OFF;  //ON:白天，OFF：晚上
    
    
    /* Get the time */
    calendar_get(&date_s,&rtc_time);    
    CurTime = rtc_time.Hours*60 + rtc_time.Minutes;
    
    /* 	De-init RTC autowake up interrupt   */
    calendar_rtc_period_wakeup_deinit();
    
 	/* 	Set Flag to indicate that FreeRun Mode has stoped   */
	flagIsFreeRunKickOff = false;
    
    /* Start Sync Date when OSAS Stopped,if Connected with App */
//    if((Device_State == Device_WorkWithAPP) &&
//       (MonitorTemplate.MTID == kICTMonitorTemplateFreeRunID)
//    )
//    {
//        TS_SendEvent(gTsSyncDataTaskID_c,gSyncDataFormat2EventStart);  //Start Sync Data in format1
//        #ifdef SyncData_DEBUG
//        printf("Sync Data Start By OSAS...\r\n");
//        #endif
//    }
    
    if((v_MonitorTemplate.LightMTinfo.LightSampleFreq != 0) && (v_MonitorTemplate.MTSwitch != 0))
    {
        flagLightMonitorTemplateStatus = get_light_monitor_template_status(v_MonitorTemplate,CurTime);
        if(flagLightMonitorTemplateStatus == ON) //在白天方案运行时间段内
        {
            free_run_monitor_template_set_light_Ctl_Time(v_MonitorTemplate,CurTime);
        }
        else  //不在白天方案运行时间段内，夜间方案运行
        {
            free_run_monitor_template_set_night_Ctl_Time(v_MonitorTemplate,CurTime);
        }   
    } 
    closeOSAFile();    
}
/**
  * @brief  GetLightMonitorTemplateStatus 
  * @note   获取白天监护方案状态
  * @param  uint16_t CurTime
  * @retval status:ON:白天，OFF：夜晚
  */
static bool get_light_monitor_template_status(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime)
{
    bool                LightMonitorTemplateStatus = OFF; //ON:白天，OFF：夜晚
    
    if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)  //白天监护方案频率为0，不执行白天监护方案
    {
        LightMonitorTemplateStatus = OFF;                 //运行夜间监护方案
    }
    else
    {
        if((v_MonitorTemplate.SampleTimeClock.DataLen/4) == 2)//夜间方案有跨天
        {
            if(
                (CurTime >= v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            &&  (CurTime < v_MonitorTemplate.LightMTinfo.LightMTClockArr[2]*60 + v_MonitorTemplate.LightMTinfo.LightMTClockArr[3])
            )
            {
                LightMonitorTemplateStatus = ON; //运行白天监护方案
            }
            else
            {
                LightMonitorTemplateStatus = OFF; //运行夜间监护方案
            }
        }
        else if((v_MonitorTemplate.LightMTinfo.DataLen / 4) == 2) //白天方案有跨天
        {
            if(
                (CurTime >= v_MonitorTemplate.LightMTinfo.LightMTClockArr[2]*60 + v_MonitorTemplate.LightMTinfo.LightMTClockArr[3])
            &&  (CurTime < v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            )
            {
                LightMonitorTemplateStatus = OFF;   //运行夜间监护方案
            }
            else
            {
                LightMonitorTemplateStatus = ON;    //运行白天监护方案
            }
        }
        else   //白天、夜晚均未跨天
        {
            if(
                (v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1])
                >= (v_MonitorTemplate.LightMTinfo.LightMTClockArr[0]*60 + v_MonitorTemplate.LightMTinfo.LightMTClockArr[1])
            )
            {
                if(
                    (CurTime < v_MonitorTemplate.LightMTinfo.LightMTClockArr[2]*60 + v_MonitorTemplate.LightMTinfo.LightMTClockArr[3])
                )
                {
                    LightMonitorTemplateStatus = ON;   //运行白天监护方案
                }
                else
                {
                    LightMonitorTemplateStatus = OFF;  //运行夜间监护方案
                }
            }
            else
            {
                if(
                    (CurTime >= v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3] )
                &&  (CurTime < v_MonitorTemplate.LightMTinfo.LightMTClockArr[2]*60 + v_MonitorTemplate.LightMTinfo.LightMTClockArr[3])
                )
                {
                    LightMonitorTemplateStatus = ON;   //运行白天监护方案
                }
                else
                {
                    LightMonitorTemplateStatus = OFF;  //运行夜间监护方案
                }
            }
        }
    }

    #ifdef MONITOR_TEMPLATE_DEBUG
//        printf("LightMonitorTemplateStatus = %d\r\n",LightMonitorTemplateStatus);
    #endif
    
    return LightMonitorTemplateStatus;
}
/**
  * @brief  free_run_monitor_template_set_light_Ctl_Time 
  * @note   设置FreeRun模式白天运行时间
  * @param  Monitor_Template_Typedef v_MonitorTemplate, uint16_t CurTime
  * @retval None
  */
static void free_run_monitor_template_set_light_Ctl_Time(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime)
{
    uint16_t        i=0;
    uint16_t        MTTime=0;   //监测模板时间转换为分钟
    uint32_t        AlarmHH=0;  //闹钟小时
    uint32_t        AlarmMM=0;  //闹钟分钟

    for(i=0;i<(v_MonitorTemplate.LightMTinfo.DataLen/4);i++)
    {
        if(
            CurTime < (v_MonitorTemplate.LightMTinfo.LightMTClockArr[i*4]*60 + 
            v_MonitorTemplate.LightMTinfo.LightMTClockArr[i*4+1])
        )
        {
            AlarmHH = v_MonitorTemplate.LightMTinfo.LightMTClockArr[i*4];
            AlarmMM = v_MonitorTemplate.LightMTinfo.LightMTClockArr[i*4+1];
            MTTime = CurTime + (v_MonitorTemplate.LightMTinfo.LightSampleFreq/60);
            break;
        }
        else if((CurTime + (v_MonitorTemplate.LightMTinfo.LightSampleFreq/60)) <
            (v_MonitorTemplate.LightMTinfo.LightMTClockArr[i*4+2]*60 + v_MonitorTemplate.LightMTinfo.LightMTClockArr[i*4+3]))
        {
            //AlarmHH = (CurTime + (MonitorTemplate.LightMTinfo.LightSampleFreq/60)) / 60;
            //AlarmMM = (CurTime + (MonitorTemplate.LightMTinfo.LightSampleFreq/60)) % 60;

            /********************************************************************************
            //                               Cur_Time - Orgin_Time
            //   Next_Time = Orgin_Time + (------------------------- + 1)*freg  (min)
            //                                        freq
            **********************************************************************************/
            
            MTTime = CurTime + (v_MonitorTemplate.LightMTinfo.LightSampleFreq/60);

            AlarmHH = (MTTime / 60);
            if(AlarmHH >= 24)
            {
                AlarmHH = 0;
            }
            AlarmMM = (MTTime % 60);
            break;
        }
        else if((CurTime + (v_MonitorTemplate.LightMTinfo.LightSampleFreq/60)) >= 1440) //大于24点
        {
            MTTime = CurTime + (v_MonitorTemplate.LightMTinfo.LightSampleFreq/60);
            MTTime = MTTime - 1440;

            AlarmHH = (MTTime / 60);
            if(AlarmHH >= 24)
            {
                AlarmHH = 0;
            }
            AlarmMM = (MTTime % 60);
            break;
        }
        else if(CurTime <= (v_MonitorTemplate.ClockArr[i*4]*60 + v_MonitorTemplate.ClockArr[i*4+1]))
        {
            i=2;  //在夜间方案运行时间段内，切换至夜间方案
            break;
        }
    }
    if(MTTime >= 1440)
    {
        MTTime = MTTime - 1440;
    }
    if(adjust_moniter_template_run_time(v_MonitorTemplate,CurTime,MTTime) == 2)
    {
        i = 2;
    }

    if(i<(v_MonitorTemplate.LightMTinfo.DataLen/4))
    {
        rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
    }
    else if(v_MonitorTemplate.LightMTinfo.LightSampleFreq != 0) //切换至夜间方案
    {
        free_run_monitor_template_changeTo_ninghtMT(v_MonitorTemplate,CurTime);
    }

    #ifdef MONITOR_TEMPLATE_DEBUG
    if(i<(v_MonitorTemplate.LightMTinfo.DataLen/4))
    {
        printf("Next Light Alarm Time: Hour=%d,Minute=%d\r\n",AlarmHH,AlarmMM);
    }
    else if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
    {
        printf("This day Light monitor template have passed\r\n");
    }
    #endif
}
/**
  * @brief  free_run_monitor_template_set_night_Ctl_Time 
  * @note   设置夜间监护方案运行时间
  * @param  None
  * @retval status
  */
static void free_run_monitor_template_set_night_Ctl_Time(Monitor_Template_Typedef v_MonitorTemplate, uint16_t CurTime)
{
    uint32_t        AlarmHH=0;  //闹钟小时
	uint32_t        AlarmMM=0;  //闹钟分钟

    if((v_MonitorTemplate.SampleTimeClock.DataLen/4) == 2)  //有跨天
    {
        if(false == isFreeRunKickOff())
        {
            if(
                (CurTime >=  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            && 	(CurTime <  v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5])
            )
            /*  out of the montior range, set to beginning  */
            {
                AlarmHH = v_MonitorTemplate.ClockArr[4];
                AlarmMM = v_MonitorTemplate.ClockArr[5];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("set start alarm:%d, %d\r\n",AlarmHH,AlarmMM);
                #endif
                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
            else if(
                (CurTime >=  v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5])
                || 	(CurTime <  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            )
            {
                /* In the Free Run Time, but the Free Run is not kuck off */
                CurTime = (CurTime + 1) % (24*60);

                if(CurTime == v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
                {
                    /*  already the end time point, then set to next beginning point */
                    if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
                    {
                        AlarmHH = v_MonitorTemplate.ClockArr[4];
                        AlarmMM = v_MonitorTemplate.ClockArr[5];
                        #ifdef MONITOR_TEMPLATE_DEBUG
                            printf("set start alarm:%d, %d\r\n",AlarmHH,AlarmMM);
                        #endif
                        rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
                    }
                    else
                    {
                        free_run_monitor_template_set_light_Ctl_Time(v_MonitorTemplate,CurTime);
                    }
                }
                else
                {
                    /*  Set to next minutes     */
                    AlarmHH = CurTime / 60;
                    AlarmMM = CurTime % 60;
                    #ifdef MONITOR_TEMPLATE_DEBUG
                        printf("set start alarm:%d, %d\r\n",AlarmHH,AlarmMM);
                    #endif
                    rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
                }
            }

        }
        else
        {
            if(
                (CurTime >=  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3]) && 
            	(CurTime <  v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5])
            )
            /*  out of the montior range, set to beginning  */
            {
                /* Stop Free Run */
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("deinit free run when change MT \r\n");
                #endif
                free_run_mode_deInit(v_MonitorTemplate);

                if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
                {
                    AlarmHH = v_MonitorTemplate.ClockArr[4];
                    AlarmMM = v_MonitorTemplate.ClockArr[5];
                    #ifdef MONITOR_TEMPLATE_DEBUG
                        printf("set start alarm:%d, %d\r\n",AlarmHH,AlarmMM);
                    #endif
                    rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
                }
            }
            else if(
                    (CurTime >=  v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5]) ||
                 	(CurTime <  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            )
            {
                /*  running and in the montior range  */
                AlarmHH = v_MonitorTemplate.ClockArr[2];
                AlarmMM = v_MonitorTemplate.ClockArr[3];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("set stop alarm:%d, %d\r\n",AlarmHH,AlarmMM);
                #endif
                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
        }
    }
    else  //没有跨天
    {
        if(false == isFreeRunKickOff())
        {
            if(
                (CurTime <  v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1]) ||
             	(CurTime >=  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            )
            {
                AlarmHH = v_MonitorTemplate.ClockArr[0];
                AlarmMM = v_MonitorTemplate.ClockArr[1];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("set FreeRun MT start alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                #endif

                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
            else if(
                    (CurTime >=  v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1]) &&
                 	(CurTime <  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            )
            {
                /* In the Free Run Time, but the Free Run is not kuck off */
                CurTime = (CurTime + 1) % (24*60);

                if(CurTime == v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
                {
                    /*  already the end time point, then set to next beginning point */
                    if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
                    {
                        AlarmHH = v_MonitorTemplate.ClockArr[0];
                        AlarmMM = v_MonitorTemplate.ClockArr[1];
                        #ifdef MONITOR_TEMPLATE_DEBUG
                            printf("set start alarm:%d, %d\r\n",AlarmHH,AlarmMM);
                        #endif
                        rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
                    }
                    else
                    {
                        free_run_monitor_template_set_light_Ctl_Time(v_MonitorTemplate,CurTime);
                    }
                }
                else
                {
                    /*  Set to next minutes     */
                    AlarmHH = CurTime / 60;
                    AlarmMM = CurTime % 60;

                    #ifdef MONITOR_TEMPLATE_DEBUG
                        printf("set FreeRun MT start alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                    #endif

                    rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
                }
            }
        }
        else
        {
            if(
                (CurTime <  v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1]) ||
             	(CurTime >=  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            )
            {
                /* Stop Free Run */
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("deinit freerun when reach the end \r\n");
                #endif
                
                free_run_mode_deInit(v_MonitorTemplate);

                if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
                {
                    AlarmHH = v_MonitorTemplate.ClockArr[0];
                    AlarmMM = v_MonitorTemplate.ClockArr[1];
                    #ifdef MONITOR_TEMPLATE_DEBUG
                        printf("set start alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                    #endif
                    rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
                }
            }
            else if(
                    (CurTime >=  v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1]) &&
                 	(CurTime <  v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])
            )
            {
                /* Free run is kick off, set the stop time */
                AlarmHH = v_MonitorTemplate.ClockArr[2];
                AlarmMM = v_MonitorTemplate.ClockArr[3];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("set stop alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                #endif
                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
        }
    }    
}
/**
  * @brief  monitor_template_alarmA_IRQHandler 
  * @note   监护方案运行闹钟回调函数，！！！（这是一个中断服务函数）
  * @param  None
  * @retval None
  */
static void monitor_template_alarmA_IRQHandler(void)
{
    BaseType_t          		xHigherPriorityTaskWoken,xResult;
	MONITOR_TEMPLATE_MSG_T		monitorTemplateQueueMsgValue;

    monitorTemplateQueueMsgValue.eventID = MONITOR_TEMPLATE_EVENT_SAMPLE_DATA;
	xResult = xQueueSendFromISR(monitorTemplateEvevtQueue,(void *)&monitorTemplateQueueMsgValue,&xHigherPriorityTaskWoken);

    /* Was the message posted successfully? */
    if( xResult != pdFAIL )
    {
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}
/**
  * @brief  adjust_moniter_template_run_time 
  * @note   根据当前时间和下次方案运行时间确定方案运行时间
  * @param  None
  * @retval status
  */
static uint8_t adjust_moniter_template_run_time(Monitor_Template_Typedef v_MonitorTemplate, uint16_t CurTime, uint16_t NextTime)
{
    uint8_t i=0;

    //当前时间与下次时间之间是否有夜间方案时间
    if(CurTime <= NextTime)  //未跨天
    {
        if((v_MonitorTemplate.SampleTimeClock.DataLen/4) == 2) //夜间方案有跨天
        {
            if(
                ((CurTime < (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])) &&
                 (NextTime > (v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1]))) ||
                ((CurTime < (v_MonitorTemplate.ClockArr[6]*60 + v_MonitorTemplate.ClockArr[7])) &&
                 (NextTime > (v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5])))
            )
            {
                i = 2;   //运行夜间方案
            }
        }
        else    //夜间方案没有跨天
        {
            if(
                (CurTime < (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3])) &&
                (NextTime > (v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1]))
            )
            {
                i = 2;   //运行夜间方案
            }
        }
    }
    else  //跨过0点
    {
        if((v_MonitorTemplate.SampleTimeClock.DataLen/4) == 2) //夜间方案有跨天
        {
            if(CurTime <= (v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5]))
            {
                if(((v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5]) - CurTime)
                    < (v_MonitorTemplate.LightMTinfo.LightSampleFreq/60))
                {
                    i = 2;     //运行夜间方案
                }
            }
            else
            {
                i = 2;     //运行夜间方案
            }
        }
        else    //夜间方案没有跨天
        {
            if(NextTime > (v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1]))
            {
                i = 2;     //运行夜间方案
            }
        }
    }

    return i;
}
/**
  * @brief  free_run_monitor_template_changeTo_ninghtMT 
  * @note   切换至夜间运行方案
  * @param  None
  * @retval status
  */
static void free_run_monitor_template_changeTo_ninghtMT(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime)
{
    //切换至夜间监护方案
    #ifdef MONITOR_TEMPLATE_DEBUG
        printf("Change to Ninght MT\r\n");
    #endif
    free_run_monitor_template_set_night_Ctl_Time(v_MonitorTemplate,CurTime);
}
/**
  * @brief  find_next_alarm_time_point 
  * @note   计算个性化方案下一个运行时间点
  * @param[in]  v_MonitorTemplate,CurrTime
  * @param[out] AlarmHH，AlarmMM
  * @retval status
  */
static int8_t find_next_alarm_time_point(Monitor_Template_Typedef v_MonitorTemplate,
                                        uint16_t CurrTime,uint8_t *AlarmHH,uint8_t *AlarmMM)
{
    uint8_t  i = 0;
    uint16_t temp_time = 0;
    int8_t   ret = 0;           // 0 - success,-1 - fail

    #ifdef MONITOR_TEMPLATE_DEBUG
        printf("\r\nFindNextAlarmTimePoint,CurrTime = 0x%08x\r\n",CurrTime);
    #endif

    if((AlarmHH != NULL) && (AlarmMM != NULL))
    {
        temp_time = CurrTime + (v_MonitorTemplate.SampleFreq/60);
        temp_time %= 1440;                              // 24*60 = 1440
        #ifdef MONITOR_TEMPLATE_DEBUG
            printf("temp_time = %d\r\n",temp_time);
        #endif

        for(i = 0;i < (v_MonitorTemplate.SampleTimeClock.DataLen/4);i ++)
        {
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("i = %d\r\n",i);
                printf("MonitorTemplate.ClockArr[i*4] = %d\r\n",v_MonitorTemplate.ClockArr[i*4]);
                printf("MonitorTemplate.ClockArr[i*4+1] = %d\r\n",v_MonitorTemplate.ClockArr[i*4+1]);
                printf("MonitorTemplate.ClockArr[i*4+2] = %d\r\n",v_MonitorTemplate.ClockArr[i*4+2]);
                printf("MonitorTemplate.ClockArr[i*4+3] = %d\r\n",v_MonitorTemplate.ClockArr[i*4+3]);
            #endif
            if(
                (temp_time >= (v_MonitorTemplate.ClockArr[i*4]*60 + v_MonitorTemplate.ClockArr[i*4+1])) &&
                (temp_time <= (v_MonitorTemplate.ClockArr[i*4+2]*60 + v_MonitorTemplate.ClockArr[i*4+3]))
            )
            {
                *AlarmHH = temp_time / 60;
                *AlarmMM = temp_time % 60;
            }
        }

        if(i >= (v_MonitorTemplate.SampleTimeClock.DataLen/4))
        {
            if(i == 1)
            {
                *AlarmHH = v_MonitorTemplate.ClockArr[0];
                *AlarmMM = v_MonitorTemplate.ClockArr[1];   
            }  
            else if(i == 2)
            {
                *AlarmHH = v_MonitorTemplate.ClockArr[4];
                *AlarmMM = v_MonitorTemplate.ClockArr[5];                   
            }
            *AlarmHH %= 24;
        }
    }
    else
    {
        ret = -1;
    }
  
    return (ret);
}
/**
  * @brief  free_run_monitor_template_status_monitor 
  * @note   监护方案FreeRun模式运行状态监测，在监护方案数据采集过程中调用
  * @param  v_MonitorTemplate,CurTime
  * @retval None
  */
static void free_run_monitor_template_status_monitor(Monitor_Template_Typedef v_MonitorTemplate,uint16_t CurTime)
{
    uint8_t         AlarmHH = 0;
    uint8_t         AlarmMM = 0;
    
    if((v_MonitorTemplate.SampleTimeClock.DataLen/4) == 2)  //有跨天
    {
        if(
            CurTime >= (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3]) &&
            CurTime < (v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5])
        )  //stop free run
        {
            /* 	De-init the EEPROM for FreeRun Mode 		*/
            /* 	Overflow the stop time point, then stop the sample 	*/
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("deinit free run \r\n");
            #endif
            free_run_mode_deInit(v_MonitorTemplate);

            if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
            {
                /* reset the start time */
                AlarmHH = v_MonitorTemplate.ClockArr[4];
                AlarmMM = v_MonitorTemplate.ClockArr[5];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("reset alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                #endif
                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
        }
        else if (
            (CurTime >= (v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5])) || 
            (CurTime < (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3]))
        )   // start free run
        {
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("cross start sample \r\n");
            #endif

            /* 	Init the EEPROM for FreeRun Mode 		*/
            if(flagIsFreeRunKickOff == false)
            {
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("init free run \r\n");
                #endif

                free_run_mode_init(v_MonitorTemplate);
            }
            /* set the stop time */
            AlarmHH = v_MonitorTemplate.ClockArr[2];
            AlarmMM = v_MonitorTemplate.ClockArr[3];
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("set FreeRun MT Stop alarm = %d, %d\r\n",AlarmHH,AlarmMM);
            #endif
            rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
        }
    }
    else  //未跨天
    {
        if(
            CurTime >= (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3]) ||
            CurTime < (v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1])
        )  //stop free run
        {
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("stop sample in sample, curr time = %d \r\n",CurTime);
            #endif

            /* 	De-init the EEPROM for FreeRun Mode 		*/
            if(flagIsFreeRunKickOff == true)
            {
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("deinit free run \r\n");
                #endif
                /* 	Overflow the stop time point, then stop the sample 	*/
                 free_run_mode_deInit(v_MonitorTemplate);
            }

            if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
            {
                /* reset the start time */
                AlarmHH = v_MonitorTemplate.ClockArr[0];
                AlarmMM = v_MonitorTemplate.ClockArr[1];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("reset alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                #endif
                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
        }
        else if (
            (CurTime >= (v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1])) && 
            (CurTime < (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3]))
        )   // start free run
        {
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("start sample \r\n");
            #endif

            /* 	Init for FreeRun Mode 		*/
            if(flagIsFreeRunKickOff == false)
            {
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("init free run \r\n");
                #endif

                free_run_mode_init(v_MonitorTemplate);
            }
            /* set the stop time */
            AlarmHH = v_MonitorTemplate.ClockArr[2];
            AlarmMM = v_MonitorTemplate.ClockArr[3];
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("set FreeRun MT Stop alarm = %d, %d\r\n",AlarmHH,AlarmMM);
            #endif
            rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
        }
    }
}
/**
  * @brief  set_freeRun_night_start_frame 
  * @note   设置FreeRun模式夜间方案开始运行标志
  * @param  v_MonitorTemplate,CurTime
  * @retval None
  */
static void set_freeRun_night_start_frame(Monitor_Template_Typedef v_MonitorTemplate,RTC_DateTypeDef date_s,
                                            RTC_TimeTypeDef rtc_time)
{
    uint8_t 	        bufForStorage[MT_STORAGE_DATA_LEN] = {0}; //the buffer to data storage
    uint8_t		        Err_Code=0;
    uint32_t            u32Date = 0;

    #ifdef MONITOR_TEMPLATE_DEBUG
        printf("\r\nSetFreeRunNightStartFrame\r\n");
    #endif

    /* Covert the Current Time */
    u32Date = GetTick((date_s.Year+2000),date_s.Month,date_s.Date,rtc_time.Hours,rtc_time.Minutes,rtc_time.Seconds);

	/* 	Init the buffer for data header 	*/
	bufForStorage[0] = (u32Date >> 24);
	bufForStorage[1] = (u32Date >> 16);
	bufForStorage[2] = (u32Date >> 8);
    bufForStorage[3] = (u32Date >> 0);
	bufForStorage[4] = FreeRunNightstartFram;
    bufForStorage[5] = v_MonitorTemplate.SampleFreq;
    bufForStorage[6] = v_MonitorTemplate.SetMTID;
    bufForStorage[7] = 0;
    bufForStorage[8] = 0;

    /*	Write EEPROM 		*/
    Err_Code = data_memory_write(0,bufForStorage,sizeof(bufForStorage));
    if(Err_Code != 0)
    {
        App_Error_Check(Err_Code);
    }        
}

/**
  * @brief  freeRun_sample_data_storageFunc 
  * @note   存储FreeRun模式监护方案数据
  * @param  v_MonitorTemplate
  * @retval None
  */
static void freeRun_sample_data_storageFunc(Monitor_Template_Typedef v_MonitorTemplate)
{
	uint8_t		                    Err_Code = 0;
    uint8_t                         parameterID = 0;
	uint8_t 	                    bufForStorage[MT_STORAGE_DATA_LEN]; //the buffer to data storage
    RTC_DateTypeDef                 date_s;
    RTC_TimeTypeDef                 rtc_time;
    uint16_t                        CurTime = 0;
    uint32_t                        u32Date = 0;
    uint8_t                         AlarmHH = 0;
    uint8_t                         AlarmMM = 0;

	if(isFreeRunKickOff() == false)
    {
        /* The OSAS has Stopped, storage the data when OSAS is running*/
        return;
    }

    /* Get The Current Time */
    calendar_get(&date_s,&rtc_time);
    CurTime = rtc_time.Hours*60 + rtc_time.Minutes;
    u32Date = GetTick((date_s.Year+2000),date_s.Month,date_s.Date,rtc_time.Hours,rtc_time.Minutes,rtc_time.Seconds);

	/* 	Init the buffer for data header 	*/
	bufForStorage[0]	= (uint8_t)(u32Date>>24);		/* 	Hour */
	bufForStorage[1]	= (uint8_t)(u32Date>>16);		/* 	Minutes	*/
    bufForStorage[2]	= (uint8_t)(u32Date>>8);		/* 	Seconds	*/
    bufForStorage[3]	= (uint8_t)(u32Date>>0);		/* 	Seconds	*/

    /* Parameter ID */
    switch(v_MonitorTemplate.SetMTID)
    {
        case kICTMonitorTemplateOSA:
            parameterID = OSASID;
            break;

        case kICTMonitorTemplateHypoxemia:
            parameterID = HypoxemiaID;
            break;

        case kICTMonitorTemplateCOPD:
            parameterID = COPDID;
            break;

        case kICTMonitorTemplateSleepApneaManagerID:
            parameterID = SleepApneaManagerID;
            break;

        case kIctmonitorTemplateNightLowSpo2ManagerID:
            parameterID = NightLowSpo2ManagerID;
            break;

        default:
            break;
    }
    bufForStorage[4] = parameterID;

	/* 	HR value 		*/
	if(v_MonitorTemplate.SampleID & SAMPLE_ID_HR)
	{
        if((g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8HrVal != 0) && (g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8HrVal < 40))
        {
            g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8HrVal = 40;
        }
		bufForStorage[5] = g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8HrVal;
	}

	/* 	SPO2 Value 		*/
	if(v_MonitorTemplate.SampleID & SAMPLE_ID_SPO2)
	{
		bufForStorage[6] = g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8SpO2Val;
	}
    
	/* MoveLevel Value */
	if(v_MonitorTemplate.SampleID & SAMPLE_ID_TEMPERATURE)
	{
		/* represent the motion level	*/
		bufForStorage[7] = (uint8_t)((g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui16MotionVal & 0xFF00) >> 8);   
    }
    bufForStorage[8] = g_HrSpo2Val_st.m_HrSpo2DataRep_pst->m_ui8ConfVal;      //置信度

    /* 	Begin to store the data 			*/
    /*	Write EEPROM 		*/
    Err_Code = data_memory_write(0,bufForStorage,sizeof(bufForStorage));
    if(Err_Code != 0)
    {
        App_Error_Check(Err_Code);
    }

    /* if the CurTime is not included by the FreeRun Time , stop the FreeRun*/
    if((v_MonitorTemplate.SampleTimeClock.DataLen/4) == 2)  //有跨天
    {
        if(
            CurTime > (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3]) && 
            CurTime < (v_MonitorTemplate.ClockArr[4]*60 + v_MonitorTemplate.ClockArr[5])
        )  //stop free run
        {
            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("deinit free run \r\n");
            #endif
            free_run_mode_deInit(v_MonitorTemplate);


            if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
            {
                /* reset the start time */
                AlarmHH = v_MonitorTemplate.ClockArr[4];
                AlarmMM = v_MonitorTemplate.ClockArr[5];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("reset alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                #endif
                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
        }
    }
    else  //未跨天
    {
        if(
            CurTime > (v_MonitorTemplate.ClockArr[2]*60 + v_MonitorTemplate.ClockArr[3]) ||
            CurTime < (v_MonitorTemplate.ClockArr[0]*60 + v_MonitorTemplate.ClockArr[1])
        )  //stop free run
        {

            #ifdef MONITOR_TEMPLATE_DEBUG
                printf("deinit free run \r\n");
            #endif
            /* 	Overflow the stop time point, then stop the sample 	*/
            free_run_mode_deInit(v_MonitorTemplate);

            if(v_MonitorTemplate.LightMTinfo.LightSampleFreq == 0)
            {
                /* reset the start time */
                AlarmHH = v_MonitorTemplate.ClockArr[0];
                AlarmMM = v_MonitorTemplate.ClockArr[1];
                #ifdef MONITOR_TEMPLATE_DEBUG
                    printf("reset alarm = %d, %d\r\n",AlarmHH,AlarmMM);
                #endif
                rtc_set_alarm_A(AlarmHH,AlarmMM,1,monitor_template_alarmA_IRQHandler);
            }
        }
    }
}
/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

