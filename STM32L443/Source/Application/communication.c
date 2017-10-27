/**
  ****************************************************************************************
  * @file    communication.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-27
  * @brief   通讯应用层处理
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "communication.h"


/* Private variables ---------------------------------------------------------*/


 
/* Private function prototypes -----------------------------------------------*/
static void cmu_data_handle(CMU_DATA_READ_BUFFER_T *buffer);
static void cmu_device_info_transmit(void);
static void cmu_bat_level_transmit(void);
static void cmu_immediately_measure_result_transmit(uint8_t ID);


/**
  * @brief  CMU_Task_Handler
  * @note   communication with nRF51822 task handle
  * @param  *pvParameters
  * @retval None
  */
void CMU_Task_Handler(void *pvParameters)
{
    const TickType_t                xTicksToWait = 100 / portTICK_PERIOD_MS; /* 最大等待时间100ms */
    CMU_DATA_READ_BUFFER_T          cmu_data_buf;
    CMU_MSG_T                       cmuQueueMsgValue;
    BaseType_t                      xResult = pdFALSE;
    
    /* clear buffer */
    cmu_data_buf.len = 0;
    memset(cmu_data_buf.buf,0,cmu_data_buf.len);
    /* CMU event queue init */
    cmuQueueMsgValue.eventID = CMU_EVENT_NULL;
     
    while(1)
    {
        //{code block} handle with Queue 
        {
            /* Wait for a message */
            xResult = xQueueReceive(cmuEventQueue,&cmuQueueMsgValue,xTicksToWait);
            if(xResult == pdPASS)
            {    
                xResult = pdFALSE;
                switch(cmuQueueMsgValue.eventID)
                {
                    case CMU_EVENT_RX:  //receive datas event
                    {
                        /* clear buffer */
                        cmu_data_buf.len = 0;
                        memset(cmu_data_buf.buf,0,cmu_data_buf.len);
                        
                        /* Get Data from CMU */
                        get_data_from_CMU(&cmu_data_buf);
                        
                        /* Handle the Data from CMU */
                        cmu_data_handle(&cmu_data_buf);
                    }
                    break;
                    case CMU_EVENT_HR_TX: //HR immediately measure translate event
                    {
                        /* cmu send hr immediately measure result */
                        if(gSubFunc_Stat_Get(HR_RealTimeSample_State) != OFF)
                        {
                            cmu_immediately_measure_result_transmit(HR_ID);
                        }
                    }
                    break;
                    case CMU_EVENT_SPO2_TX: //SpO2 immediately measure translate event
                    {                        
                        /* cmu send spo2 immediately measure result */
                        if(gSubFunc_Stat_Get(SpO2_RealTimeSample_State) != OFF)
                        {
                            cmu_immediately_measure_result_transmit(SPO2_ID); 
                        }                    
                    }
                    break;
                    default:break;
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
  * @brief  cmu_data_handle
  * @note   解析CMU接收到的数据
  * @param  CMU_DATA_READ_BUFFER_T *buffer
  * @retval None
  */
static void cmu_data_handle(CMU_DATA_READ_BUFFER_T *buffer)
{
    uint16_t                        i = 0;
    uint16_t                        bufLen = 0;  //数据包长度
    uint8_t                         buf[CMU_RX_BUF_MAXLEN] = {0}; //数据包
    uint8_t                         FrameTyp = 0; //数据包标志
    RTC_DateTypeDef                 date_s;
    RTC_TimeTypeDef                 rtc_time;
    uint16_t                        MonitorTemplateLen = 0; //接收监测模板数据长度
    uint8_t                         *p_MonitorTemplate = NULL; //保存接收到的监测模板数据
    uint16_t                        CheckUpParamID = 0;   //立即测量采集参数ID
	IMMEDIATELY_MEASURE_MSG_T		immediateltMeasureQueueMsgValue;
	const TickType_t 				xMaxBlockTime = pdMS_TO_TICKS(100); /* 设置最大等待时间为 100ms */
	
	/* immediatelt measure event queue init */
    immediateltMeasureQueueMsgValue.eventID = IMMEDIATELY_MEASURE_EVENT_NULL;	
    
    /* Copy the data Buf */
    bufLen = buffer->len;
    memcpy(buf,buffer->buf,buffer->len); 
    
    #ifdef CMU_INTERNAL_DEBUG
        printf("Rx Data: ");
        for(i=0;i<bufLen;i++)
        {
            printf("0x%02x,",buf[i]);
        }
        printf("\r\n");
    #endif
        
    FrameTyp = buf[0]; //数据包标志 
    /**		Process Body Based on Each Frame ID 		**/        
    switch(FrameTyp)
    {
        case FRAME_DEVICE_INFO_REQ:     //请求设备信息
        {
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Frame Device info reguire\r\n");
            #endif
            cmu_device_info_transmit();
        }
        break;
        case FRAME_CONNECT_STATE:
        {
            if(buf[1] == 0x00) //蓝牙断开连接
            {
                #ifdef CMU_TOP_LEVEL_DEBUG
                    printf("Disconnected\r\n");
                #endif
                g_PassKey.Connect_Status = false;
                g_DeviceInfomation.isBleConnect = false; //Ble disconnect
                
                /* Stop Immediately measure when disconnect from App */
                if(gSubFunc_Stat_Get(SPO2_HR_REALTIME_STATE) != OFF)
                {
					immediateltMeasureQueueMsgValue.eventID = IMMEDIATELY_MEASURE_EVENT_STOP_MEASURE;
                    xQueueSend(immediatelyMeasureEventQueue,(void *)&immediateltMeasureQueueMsgValue,xMaxBlockTime);
                }
            }
            else
            {
                #ifdef CMU_TOP_LEVEL_DEBUG
                    printf("Connected\r\n");
                #endif
                g_DeviceInfomation.isBleConnect = true;  //Ble connected
            }
        }
        break;   
        case FRAME_PASSKEY:       //密码验证
        {
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Passkey Verfy\r\n");
            #endif
            g_PassKey.Status = true;                //是否收到验证密码：true：收到，false：没有收到
            g_PassKey.Connect_Status  = true;       //收到密码时蓝牙连接状态：true：连接，false：断开
            g_PassKey.ConfirmStatus = buf[1];       //密码验证状态：true：通过，false：失败
            g_PassKey.len = 4;                      //buf[2]密码长度

            #ifdef PASSKEY_DEBUG
                printf("Confirm Status=0x%x,\r\n",g_PassKey.ConfirmStatus);
            #endif
            for(i=0;i<g_PassKey.len;i++)
            {
                g_PassKey.p_contex[i]=buf[i+3]-0x30; //保存密码
                #ifdef PASSKEY_DEBUG
                    printf("0x%x, ",g_PassKey.p_contex[i]);
                #endif
            }

            if(g_PassKey.ConfirmStatus == false)  //密码验证失败
            {

            }
            else  //密码验证通过
            {

            }                 
        }
        break;
        case FRAME_TIME_SYNC:     //同步时间信息
        {
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Frame Sync time\r\n");
            #endif
            date_s.Year = (uint8_t)(buf[2]);
            date_s.Month = (uint8_t)buf[3];
            date_s.Date = (uint8_t)buf[4];
            
            rtc_time.Hours = (uint8_t)buf[5];
            rtc_time.Minutes = (uint8_t)buf[6];
            rtc_time.Seconds = (uint8_t)buf[7];
           
            calendar_set(&date_s,&rtc_time);            
        }
        break;
        case FRAME_LIMIT_SET:       //参数门限设置
        {
            //参数门限设置，已取消，暂时未用
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Frame Limit Set\r\n");
            #endif            
        }
        break;
        case FRAME_MONITER_TEMPLATE_SET:    //监护方案设置
        {
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Frame Moniter Template Set\r\n");
            #endif
            /* 	Get data length 		*/
            MonitorTemplateLen = (((uint16_t)buf[1]) << 8u) | (0x00ff & buf[2]);  
            
            /* 	Process only when length is valid		*/
            if(MonitorTemplateLen < MAX_MT_DATA_LEN)
            {
                p_MonitorTemplate = buf + 3;
                if((p_MonitorTemplate[0] == '#') && (p_MonitorTemplate[MonitorTemplateLen-1] == '#'))
                {
                    unpack_monitor_template(p_MonitorTemplate,MonitorTemplateLen);
                }
            }            
        }
        break;
        case FRAME_BASIC_INFO:      //用户基本信息
        {
            //用户基本信息设置，已取消，暂时未用
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Frame user info set\r\n");
            #endif          
        }
        break;
        case FRAME_SYNC_DATA_SWITCH:  //触发数据同步
        {
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Frame Sync Data Switch\r\n");
            #endif         
        }
        break; 
        case FRAME_IMMEDIATELY_MEASURE_PARAM:  //发起立即测量
        {
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("Frame Check Up\r\n");
            #endif
            /*
            Frame format:
                        [1]	-- Parameter ID MSB, reserved for future
                        [2]	-- Parameter ID LSB
                        [3]	-- Number of sample, 0--continous sample, else -- reserved for future

            */
            CheckUpParamID = (((uint16_t)buf[1]) << 8u) | (0x00ff & buf[2]);
            if(g_DeviceInfomation.isBleConnect == true) //响应App发起的立即测量
            {
                /* HR immediately measure */
                if(CheckUpParamID & SAMPLE_ID_HR)
                {
					immediateltMeasureQueueMsgValue.eventID = IMMEDIATELY_MEASURE_EVENT_START_HR_MEASURE;
                    xQueueSend(immediatelyMeasureEventQueue,(void *)&immediateltMeasureQueueMsgValue,xMaxBlockTime);					
                }                
                /* SpO2 immediately measure */
                if(CheckUpParamID & SAMPLE_ID_SPO2)
                {
					immediateltMeasureQueueMsgValue.eventID = IMMEDIATELY_MEASURE_EVENT_START_SPO2_MEASURE;
                    xQueueSend(immediatelyMeasureEventQueue,(void *)&immediateltMeasureQueueMsgValue,xMaxBlockTime);					
                }
                /* stop all immediately measure */
                if(CheckUpParamID == SAMPLE_ID_STOP)
                {
					immediateltMeasureQueueMsgValue.eventID = IMMEDIATELY_MEASURE_EVENT_STOP_MEASURE;
                    xQueueSend(immediatelyMeasureEventQueue,(void *)&immediateltMeasureQueueMsgValue,xMaxBlockTime);					
                }
            }
            else  //蓝牙断开连接
            {
            
            }
            CheckUpParamID = 0;
        }
        break;
        default :
        {
            #ifdef CMU_TOP_LEVEL_DEBUG
                printf("CMU Frame Err...");
                for(i=0;i<bufLen;i++)
                {
                    printf("0x%02x,",buf[i]);
                }
                printf("\r\n"); 
            #endif     
        }
        break;
    }
}
/**
  * @brief  cmu_device_info_transmit
  * @note   发送设备详情信息
  * @param  None
  * @retval None
  */
static void cmu_device_info_transmit(void)
{
		uint8_t 	Senddata[24] = {0};

		Senddata[0] = 0xAA;
		Senddata[1] = 0x54;
		Senddata[2] = FRAME_DEVICEINFORM;

		Senddata[3] = g_DeviceInfomation.DevSupportCap;
		Senddata[4] = g_DeviceInfomation.pDevREV[0];
		Senddata[5] = g_DeviceInfomation.pDevREV[1];
		Senddata[6] = g_DeviceInfomation.pDevREV[2];
		Senddata[7] = g_DeviceInfomation.pFWREV[0];
		Senddata[8] = g_DeviceInfomation.pFWREV[1];
		Senddata[9] = g_DeviceInfomation.pFWREV[2];
//		Senddata[10] = g_DeviceInfomation.pSeriNum[0];
//		Senddata[11] = g_DeviceInfomation.pSeriNum[1];
//		Senddata[12] = g_DeviceInfomation.pSeriNum[2];
//		Senddata[13] = g_DeviceInfomation.pSeriNum[3];
        Senddata[10] = 0x3B;
		Senddata[11] = 0x9A;
		Senddata[12] = 0xCA;
		Senddata[13] = 0x01;
//		Senddata[14] = g_DeviceInfomation.pBootREV[0];
//		Senddata[15] = g_DeviceInfomation.pBootREV[1];
//		Senddata[16] = g_DeviceInfomation.pBootREV[2];
		Senddata[14] = 0x01;
		Senddata[15] = 0x00;
		Senddata[16] = 0x00;
        Senddata[17] = 0x02;                               //协议版本号

		cmu_data_send(Senddata, 24);
        #ifdef FWUpdate_Debug
		printf("SPI_DeviceInformationTransmit...\r\n");
		#endif

		g_DeviceInfomation.DeviceInfo_Status = 1;    
}
/**
  * @brief  cmu_bat_level_transmit
  * @note   发送电池电量等级
  * @param  None
  * @retval None
  */
static void cmu_bat_level_transmit(void)
{
		uint8_t 	Senddata[24]={0};

		Senddata[0] = 0xAA;
		Senddata[1] = 0x54;
		Senddata[2] = FRAME_BAT_LEVEL_TRANSLATE;

		//Senddata[3] = GetBatScaleLevel();   /* 	Get the battery level 	*/
        Senddata[3] = 4;   /* 	Get the battery level 	*/

        cmu_data_send(Senddata, 24);    
}
/**
  * @brief  cmu_immediately_measure_result_transmit
  * @note   immediately measure result transmit by cmu
  * @param  None
  * @retval None
  */
static void cmu_immediately_measure_result_transmit(uint8_t ID)
{
    uint8_t v_uWriteData_arr[24] = {0}; 
    
    switch(ID)
    {
        case TEMPERATURE_ID:
        {
        
        }
        break;
        case HR_ID:
        {
            #ifdef IMMEDIATELY_MEASURE_DEBUG
                printf("cmu hr send\r\n");
            #endif
            /* Get the HR Dara */
            spo2_hr_post_handle();
            
            v_uWriteData_arr[0] = 0xAA;
            v_uWriteData_arr[1] = 0x54;
            v_uWriteData_arr[2] = FRAME_IMMEDIATELY_MEASURE_DATA;

            v_uWriteData_arr[3] = 0x00;  //the HSB of data length
            v_uWriteData_arr[4] = 0x0E;  //the LSB of data length

            v_uWriteData_arr[5] = 0x14;
            v_uWriteData_arr[6] = g_HrSpo2Val_st.m_RtcDate_st.Year;
            v_uWriteData_arr[7] = g_HrSpo2Val_st.m_RtcDate_st.Month;
            v_uWriteData_arr[8] = g_HrSpo2Val_st.m_RtcDate_st.Date;
            v_uWriteData_arr[9] = g_HrSpo2Val_st.m_RtcTime_st.Hours;
            v_uWriteData_arr[10] = g_HrSpo2Val_st.m_RtcTime_st.Minutes;
            v_uWriteData_arr[11] = g_HrSpo2Val_st.m_RtcTime_st.Seconds;

            v_uWriteData_arr[12] = HR_ID;  //the data ID
            v_uWriteData_arr[13] = 0xFF;   //a fixed data flag, reserved for future
            v_uWriteData_arr[14] = ((g_HrSpo2Val_st.m_HrSpo2DataRep_pst)->m_ui8HrVal / 100) + 0x30;
            v_uWriteData_arr[15] = (((g_HrSpo2Val_st.m_HrSpo2DataRep_pst)->m_ui8HrVal % 100)/10) + 0x30;
            v_uWriteData_arr[16] = ((g_HrSpo2Val_st.m_HrSpo2DataRep_pst)->m_ui8HrVal % 10) + 0x30;
            v_uWriteData_arr[17] = 0x23;

            v_uWriteData_arr[23]=0xDD;

            cmu_data_send(v_uWriteData_arr, 24);      
        }
        break;
        case SPO2_ID:
        {
            #ifdef IMMEDIATELY_MEASURE_DEBUG
                printf("cmu spo2 send\r\n");
            #endif
            /* Get the SpO2 Data */
            spo2_hr_post_handle();
            v_uWriteData_arr[0] = 0xAA;
            v_uWriteData_arr[1] = 0x54;
            v_uWriteData_arr[2] = FRAME_IMMEDIATELY_MEASURE_DATA;

            v_uWriteData_arr[3] = 0x00;  //the HSB of data length
            v_uWriteData_arr[4] = 0x0E;  //the LSB of data length

            v_uWriteData_arr[5] = 0x14;
            v_uWriteData_arr[6] = g_HrSpo2Val_st.m_RtcDate_st.Year;
            v_uWriteData_arr[7] = g_HrSpo2Val_st.m_RtcDate_st.Month;
            v_uWriteData_arr[8] = g_HrSpo2Val_st.m_RtcDate_st.Date;
            v_uWriteData_arr[9] = g_HrSpo2Val_st.m_RtcTime_st.Hours;
            v_uWriteData_arr[10] = g_HrSpo2Val_st.m_RtcTime_st.Minutes;
            v_uWriteData_arr[11] = g_HrSpo2Val_st.m_RtcTime_st.Seconds;

            v_uWriteData_arr[12] = SPO2_ID; //the data ID
            v_uWriteData_arr[13] = 0xFF;    //a fixed data flag, reserved for future

            v_uWriteData_arr[14] = ((g_HrSpo2Val_st.m_HrSpo2DataRep_pst)->m_ui8SpO2Val / 100) + 0x30;
            v_uWriteData_arr[15] = ((g_HrSpo2Val_st.m_HrSpo2DataRep_pst)->m_ui8SpO2Val % 100)/10 + 0x30;
            v_uWriteData_arr[16] = ((g_HrSpo2Val_st.m_HrSpo2DataRep_pst)->m_ui8SpO2Val % 10) + 0x30;
            v_uWriteData_arr[17] = 0x23;

            v_uWriteData_arr[23]=0xDD;

            cmu_data_send(v_uWriteData_arr, 24);
        }
        break;
        default:break;
    }
}

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/



