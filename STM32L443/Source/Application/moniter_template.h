/**
  ****************************************************************************************
  * @file    moniter template.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-28
  * @brief   header file of moniter_template.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MONITER_TEMPLATE_H_
#define __MONITER_TEMPLATE_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"



/* Exported macros -----------------------------------------------------------*/
#define MAX_MT_LEN                                  (20)       //最大监测模板采集时间点:1200/4 = 300个时间点
#define MAX_EMT_LEN                                 (80)       //最大异常监测模板采集时间点:80/4 = 20个时间点
#define MAX_MT_DATA_LEN                             (80)
#define MAX_CLOCK_ARR_LEN                           (80)       //最大采集时间点
#define kICTMonitorTemplateFreeRunMaxSampleFreq     (250)      //最大自由采集时间(s)
#define MT_STORAGE_DATA_LEN 		                (9)


/* Monitor Template ID define */
#define 	kICTMonitorTemplateNone                             (0)        //未选择方案
//#define		kICTMonitorTemplateAtrialFibrillationID         (2)        //房颤
//#define     kICTMonitorTemplateCHDID                          (3)        //冠心病
//#define     kICTMonitorTemplateHeartRateID                    (4)        //循环系统-其它心脏病
//#define     kICTMonitorTemplateAsthmaticBronchitis            (5)        //支气管哮喘
//#define     kICTMonitorTemplateSleepApneaID                   (6)        //睡眠综合征
//#define     kIctmonitorTemplateCustomizeID                    (7)        //自定义
//#define     kIctmonitorTemplateSubHealthID                    (8)        //亚健康
#define     kICTMonitorTemplateAltitudeStressID                 (9)        //高原反应
#define     kICTMonitorTemplateCOPD                             (10)       //慢阻肺
#define     kICTMonitorTemplateHypoxemia                        (11)       //夜间低血氧症
#define     kICTMonitorTemplateSleepApneaManagerID              (12)       //睡眠呼吸暂停综合征管理
#define     kIctmonitorTemplateNightLowSpo2ManagerID            (13)       //夜间低血氧症管理
#define     kICTMonitorTemplateOSA                              (0x10)     //OSAS
#define 	kICTMonitorTemplateFreeRunID					    (0xFF)	   // Free run, will start as "real time sample", 1s update rate

/* Monitot Template Data ID define */
#define OSASID                             (61)
#define HypoxemiaID                        (62)
#define COPDID                             (63)
#define SleepApneaManagerID                (64)
#define NightLowSpo2ManagerID              (65)
#define FreeRunNightstartFram              (0xFE)


/* type define */
typedef struct
{
	uint8_t  Context[MAX_MT_LEN];
	uint16_t DataLen;
}SampleTemplate_Typedef;

typedef struct
{
	uint8_t  Context[MAX_EMT_LEN];
	uint16_t DataLen;
}SampleExcetionTemplate_Typedef;
typedef struct
{
    uint32_t     LightSampleFreq;                       //白天监护方案采集频率
    uint8_t      LightMTClockArr[MAX_CLOCK_ARR_LEN];    //白天监护方案数据采集时间点
    uint8_t      DataLen;
}LightMTinfo_Typedef;

typedef struct
{
	uint32_t                   SampleFreq;                      //夜间监护方案采集频率
	uint8_t 				   ClockArr[MAX_CLOCK_ARR_LEN];     //夜间监护方案数据采集时间段
	SampleTemplate_Typedef     SampleTimeClock;			        //夜间监护方案采集时间点
	uint8_t                    SampleWorkDay;                   //采集Work Day
	uint16_t                   SampleLastTime;			        //每次采集持续时间
	uint16_t              	   SampleID;				        //采集参数ID
	uint8_t                    MTSwitch;                        //监护模板开发
	uint8_t                    VibrateSwitch;                   //振动开关
	uint8_t                    MTID;                            //调整之后的方案ID
    uint8_t                    SetMTID;                         //App设置的方案ID
	uint16_t                   MotionInfoUpdateFreq;            //运行数据更新频率
    LightMTinfo_Typedef        LightMTinfo;                     //白天监护方案信息
}Monitor_Template_Typedef;

typedef struct
{
	uint32_t                   		 SampleFreq;                //异常监测模板采集频率
	uint16_t                  		 SampleLastTime;            //异常监测模板采集持续时间
	uint16_t                  		 SampleID;                  //异常监测模板采集参数ID
	SampleExcetionTemplate_Typedef   SampleTimeClock;           //异常监测模板采集时间点
}Excetion_Monitor_Template_Typedef;


/* Exported Variables */
extern Monitor_Template_Typedef       g_MonitorTemplate;


/* Exported functions --------------------------------------------------------*/
void unpack_monitor_template(const uint8_t *p_MonitorTemplate,uint16_t Len);
void monitor_template_task_handler(void *pvParameters);




#endif /* __MONITER_TEMPLATE_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

