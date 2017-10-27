/**
  ****************************************************************************************
  * @file    immediately_measure.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-10-9
  * @brief   实现立即测量功能
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "immediately_measure.h"


/* Private variables ---------------------------------------------------------*/
static TimerHandle_t    IMMDM_HR_Timer = NULL;
static TimerHandle_t    IMMDM_SpO2_Timer = NULL;


 
/* Private function prototypes -----------------------------------------------*/
static void vTimerImmediatelyMeasureCB(xTimerHandle pxTimer);



/**
  * @brief  CMU_Task_Handler
  * @note   communication with nRF51822 task handle
  * @param  *pvParameters
  * @retval None
  */
void immediately_measure_task_handler(void *pvParameters)
{
    BaseType_t                      xResult = pdFALSE;
    const TickType_t                xTicksToWait = 100 / portTICK_PERIOD_MS; /* 最大等待时间100ms */
    const TickType_t                xTimesPerTick = 1000 / portTICK_PERIOD_MS;
	IMMEDIATELY_MEASURE_MSG_T		immediateltMeasureQueueMsgValue;
        
	/* immediatelt measure event queue init */
    immediateltMeasureQueueMsgValue.eventID = IMMEDIATELY_MEASURE_EVENT_NULL;
	
    /* creat timers for periodic handle immediately measure result */
    IMMDM_HR_Timer = xTimerCreate("HR Timer",xTimesPerTick,pdTRUE,(void *)0,vTimerImmediatelyMeasureCB);
    if(IMMDM_HR_Timer == NULL)
    {
        App_Error_Check(HAL_ERROR);
    }
    vTimerSetTimerID(IMMDM_HR_Timer,(void *)IMMDM_HR_TIMER_ID);
    
    IMMDM_SpO2_Timer = xTimerCreate("SPO2 Timer",xTimesPerTick,pdTRUE,(void *)0,vTimerImmediatelyMeasureCB);
    if(IMMDM_SpO2_Timer == NULL)
    {
        App_Error_Check(HAL_ERROR);
    }
    vTimerSetTimerID(IMMDM_SpO2_Timer,(void *)IMMDM_SPO2_TIMERID);
    
    while(1)
    {
        //{code block} handle with Queue 
        {
			/* Wait for a message */
            xResult = xQueueReceive(immediatelyMeasureEventQueue,&immediateltMeasureQueueMsgValue,xTicksToWait);
            if(xResult == pdPASS)
            {    
                xResult = pdFALSE;
				switch(immediateltMeasureQueueMsgValue.eventID)
				{
					case IMMEDIATELY_MEASURE_EVENT_START_HR_MEASURE:  //receive hr measure start event
					{
						#ifdef IMMEDIATELY_MEASURE_DEBUG
							printf("start HR immediately measure\r\n");
						#endif
						
						/* Start the immediately measure HR Timer */
						if(xTimerStart(IMMDM_HR_Timer,xTicksToWait) != pdPASS)
						{
							App_Error_Check(HAL_ERROR);
						}
						
						/* app start measure */
						gSubFunc_Stat_Set(HR_RealTimeSample_State,ON);
						apl_collection_event_post(CollectionStart);                
					}
					break;
					case IMMEDIATELY_MEASURE_EVENT_START_SPO2_MEASURE: //spo2 measure start event  
					{
						#ifdef IMMEDIATELY_MEASURE_DEBUG
							printf("start SpO2 immediately measure\r\n");
						#endif
						
						/* Start the immediately measure SpO2 Timer */
						if(xTimerStart(IMMDM_SpO2_Timer,xTicksToWait) != pdPASS)
						{
							App_Error_Check(HAL_ERROR);
						}
							
						/* app start measure */
						gSubFunc_Stat_Set(SpO2_RealTimeSample_State,ON);
						apl_collection_event_post(CollectionStart);
					}
					break;
					case IMMEDIATELY_MEASURE_EVENT_STOP_MEASURE: //stop all immediately measure event
					{
						#ifdef IMMEDIATELY_MEASURE_DEBUG
							printf("stop all immediately measure\r\n");
						#endif
						
						/* stop All timers for immediately measure */
						if(xTimerStop(IMMDM_HR_Timer,xTicksToWait) != pdPASS)
						{
							App_Error_Check(HAL_ERROR);
						}
						if(xTimerStop(IMMDM_SpO2_Timer,xTicksToWait) != pdPASS)
						{
							App_Error_Check(HAL_ERROR);
						} 
						/* app stop measure */
						gSubFunc_Stat_Set(HR_RealTimeSample_State | SpO2_RealTimeSample_State,OFF);
						apl_collection_event_post(CollectionStop);                
					}
					break;
					default:
						break;
				}
			}// end of if(xResult == pdPASS)
        }
        //end of event group handle
        
        //{code block} handle of queues
        {
            
        }
        //end of queues handle  		
    }
}

/**
  * @brief  vTimerImmediatelyMeasureCB
  * @note   immediatelt measure periodic timer call back
  * @param  pxTimer
  * @retval None
  */
static void vTimerImmediatelyMeasureCB(xTimerHandle pxTimer)
{
    uint32_t                        ulTimerID = 0;
    const TickType_t                xTicksToWait = 100 / portTICK_PERIOD_MS; /* 最大等待时间100ms */
    CMU_MSG_T                       cmuQueueMsgValue;
    
    
    configASSERT(pxTimer);
    
    /* get the time ID */
    ulTimerID = (uint32_t)pvTimerGetTimerID(pxTimer);
    if(((uint8_t)ulTimerID)  == IMMDM_HR_TIMER_ID)
    {
        cmuQueueMsgValue.eventID = CMU_EVENT_HR_TX;
        if(xQueueSend(cmuEventQueue,(void *)&cmuQueueMsgValue,xTicksToWait) != pdPASS)
        {
            App_Error_Check(HAL_ERROR);
        }
    }

    if(((uint8_t)ulTimerID) == IMMDM_SPO2_TIMERID)
    {
        cmuQueueMsgValue.eventID = CMU_EVENT_SPO2_TX;
        if(xQueueSend(cmuEventQueue,(void *)&cmuQueueMsgValue,xTicksToWait) != pdPASS)
        {
            App_Error_Check(HAL_ERROR);
        }        
    }    
}


/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

