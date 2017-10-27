/**
  ****************************************************************************************
  * @file    main.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   header file of main.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H_
#define __MAIN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stdbool.h"
#include "Global_Typedef.h"
#include "fatfs.h"
#include "cmsis_os.h"
#include "platform.h"
#include "shell.h"
#include "bsp.h"
#include "bsp_uart.h"
#include "bsp_rtc.h"
#include "string.h"
#include "mx25l06e.h"
#include "spi.h"
#include "StoreManageFatfs.h"
#include "bsp_communication.h"
#include "led.h"
#include "communication.h"
#include "moniter_template.h"
#include "immediately_measure.h"
#include "bma250e.h"
#include "cc_alg_app_interface.h"
#include "battery_manage.h"
#include "data_memory_manage.h"
#include "ymodem.h"



/* Type Struct define -----------------------------------------------------------*/


/* Queue define */
//CMU Event Group define 
#define CMU_QUEUE_LENGTH                    	(10)
#define IMMEDIATELY_MEASURE_QUEUE_LENGTH		(10)
#define MONITOR_TEMPLATE_QUEUE_LENGTH			(10)
    
//communication event define 	
typedef enum
{
    CMU_EVENT_NULL = 0,
    CMU_EVENT_RX,                        
    CMU_EVENT_HR_TX, 
    CMU_EVENT_SPO2_TX       
}CMU_EVENT_ID_T;

typedef struct
{
    CMU_EVENT_ID_T      eventID;
}CMU_MSG_T;

//immediately measure event define 
typedef enum
{
    IMMEDIATELY_MEASURE_EVENT_NULL = 0,
    IMMEDIATELY_MEASURE_EVENT_START_HR_MEASURE,         
    IMMEDIATELY_MEASURE_EVENT_START_SPO2_MEASURE ,       
    IMMEDIATELY_MEASURE_EVENT_STOP_MEASURE              
}IMMEDIATELY_MEASURE_EVENT_ID_T;    

typedef struct
{
    IMMEDIATELY_MEASURE_EVENT_ID_T    eventID;
}IMMEDIATELY_MEASURE_MSG_T;

//monitor template event group define
typedef enum
{
	MONITOR_TEMPLATE_EVENT_NULL = 0,
	MONITOR_TEMPLATE_EVENT_SET,                          
	MONITOR_TEMPLATE_EVENT_SAMPLE_DATA,                  
	MONITOR_TEMPLATE_EVENT_STORE_DATA                
}MONITOR_TEMPLATE_EVENT_ID_T;

typedef struct
{
	MONITOR_TEMPLATE_EVENT_ID_T eventID;
}MONITOR_TEMPLATE_MSG_T;


/* global variables declare */
extern DEVICE_INFORMATION_T        g_DeviceInfomation;
extern PASSKEY_T                   g_PassKey;
#ifdef SYS_USAGE_DEBUG
extern volatile uint32_t ulHighFrequencyTimerTicks;
#endif

/* global message variables declare */
extern osMessageQId     spo2QueueHandle;
extern osMessageQId     collectionQueueHandle;
extern QueueHandle_t    cmuEventQueue;
extern QueueHandle_t    immediatelyMeasureEventQueue;
extern QueueHandle_t	monitorTemplateEvevtQueue;

/* global semaphore variables declare */
extern SemaphoreHandle_t uartRxBinarySemaphore;

#endif /* __MAIN_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

