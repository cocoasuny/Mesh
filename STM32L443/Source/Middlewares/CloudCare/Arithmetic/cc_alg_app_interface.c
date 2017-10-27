/**
* @file				cc_alg_app_interface.c
* @brief 			application level interface
* @details			The functions and implementation of algorithm module to the application layer
* @date				2016/03/14
* @author			yun.zhu@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/

#include "cc_alg_app_interface.h"


/*default Icancel*/
#define TX_ICANCEL_CUR_VAL 			    (4<<16) 	//4uA Cancel current

#define IR_LED_CUR_DEFAULT_VAL			(0x1F)		/**< current = 31/255 * 100mA = 12.1569mA	*/
#define RED_LED_CUR_DEFAULT_VAL			(0x1F)      /**< current = 31/255 * 100mA = 12.1569mA	*/
#define IR_LED_CUR_MIN_VAL				(0x10)	    /**< current = 16/255 * 100mA = 6.2mA	*/
#define RED_LED_CUR_MIN_VAL				(0x10)	    /**< current = 16/255 * 100mA = 6.2mA	*/
#define IR_LED_CUR_MAX_VAL				(0xf0)		/**< current= 224/255 * 50mA = 87.8mA	*/
#define RED_LED_CUR_MAX_VAL				(0xf0)		/**< current= 224/255 * 50mA = 87.8mA	*/



#define SPOT_CHECK_BUF_LEN              (16)		/**< Spotcheck functions' data buffer length	*/
#define THRES_SPOT_CHECK_25K            (96119)		/**< 25K is enabled       */
#define THRES_SPOT_CHECK_10K            (188743)	/**<  10K only            */
#define THRES_SPOT_CHECK_50K            (48059)		/**< 50K is enabled      */
#define THRES_SPOT_CHECK_100K           (24029)	    /**<  100K is enabled      */
#define INT_CNT_OVERTIME_THRES 			(30000)		/**< OverTime counter, equals to 100 * 60 * 5(MIN) 	*/
#define DATA_BUF_MAX_ADJ			    (16)		/**< max number for signal average when in adj mode	*/
#define STAGE_ONE_BUF_LEN               (4)
#define SPOT_CHECK_DIFF_THRE            (20000)

#define TARGET_SIGNAL_LEVEL_MAX		    (524288)	//0.3V
#define TARGET_SIGNAL_LEVEL_MIN			(-524288)	//-0.3v
#define TARGET_SIGNAL_LEVEL				(0)			//0V

#define	TARGET_SIGNAL_LEVEL_MAX_RUN		(1048576)   //0.6v
#define TARGET_SIGNAL_LEVEL_MIN_RUN		(-1048576)  //-0.6v

#define FULLSCALE_THRESHOLD 			(2096921)
#define ADJ_GOOD_CNT_THRES				(8)			/**< Adj good counter limit, must (ADJ_GOOD_CNT_THRES)*(DATA_BUF_MAX_ADJ) > 100 	*/
#define ADJ_BAD_CNT_THRES 				(30)		/**< Adj bad counter limit, will trig error event 	*/
#define CURRENT_CANCEL_COMPENSATION  	(2796202.7)

/** @def Enable or Disable AFE4403 stage two amplifier	*/
#define IS_AFE_STAGE2_AMP_ENABLED		true
  
/** @brief  EQUAL TARGET_SIGNAL_LEVEL_MIN*Ri/Rg + Ic*Ri*2^22/1.2    */
#define TARGET_MIN_DC_STAGEONE_4UA          (1135957)
#define	TARGET_MIN_DC_LEVEL  		TARGET_MIN_DC_STAGEONE_4UA


#if MOD_CTRL_IS_DATA_DOUBLE_CHECK_IN_MONITOR_MODE == true
	static uint8_t                          s_ui8HrSpO2MonitorStableCnt = 0;
	static st_Arr_ui8_Typedef               s_HrMonitorStore_st;
	static st_Arr_ui8_Typedef               s_SpO2MonitorStore_st;
	static uint8_t 							s_HrCheckBuf_arr[DATA_DOUBLE_CHECK_NUM];
	static uint8_t 							s_SpO2CheckBuf_arr[DATA_DOUBLE_CHECK_NUM];
#endif


static HR_SPO2_INIT_PARA_Typedef s_SpO2HrInitParam;

HR_SPO2_PARAMETERS_T g_HrSpo2Val_st;

static void spo2_hr_interact_init(void);



static int32_t *s_piSampleRed,*s_piSampleIr;
static int16_t *s_piSampleX,*s_piSampleY,*s_piSampleZ;
static uint8_t s_uAfeDataCnt = 0;
static uint8_t s_uGsensorDataCnt = 0;
static HR_SPO2_DAT_T   HrSpO2DataFinal_st;

/* SpO2 submodule's status */
static ccAlgSpO2HrStatus_en s_Spo2SubModuleStat_en = SPO2_HR_STATUS_STOP;

static uint16_t 	gSubFuncState = OFF;

static uint8_t s_uGoodDcCnt = 0;
static uint8_t s_uBadDcCnt = 0;
static uint8_t s_uSignalValid = 0;


static PULSE_OXI_TX_CHANNEL_T s_txChannelParameter_st;
static PULSE_OXI_RX_CHANNEL_T s_rxChannelParameter_st;

static int32_t s_iRedRawDataAdjust_arr[AFE_ADJUST_LEN] = {0};
static int32_t s_iIrRawDataAdjust_arr[AFE_ADJUST_LEN] = {0};

void gSubFunc_Stat_Set(uint16_t mask, uint8_t newState)
{
	gSubFuncState = (newState == OFF)? gSubFuncState & (~mask) : gSubFuncState | mask;
}

uint16_t gSubFunc_Stat_Get(uint16_t mask)
{
	return(gSubFuncState & mask);
}


/**
* @brief Led current calulate
* @param[in] iTargetVal target signal level
* @param[in] uCurrLed current LED current
* @param[in] iSampleVal current signal level
* @param[in] bIsStage2Enabled stage 2 amplifer enabled or not
* @param[out] none 
* @retval uint8_t return the new LED current, range 0~255
*/
static uint8_t apl_led_curr_est(int32_t iTargetVal, uint8_t uCurrLed, int32_t iSampleVal, bool bIsStage2Enabled)
{
	float32_t 	tmpResult;
	float32_t 	tmpFloat1,tmpFloat2;
	
	/*Get the different between Target and current value */
	if(iSampleVal >= FULLSCALE_THRESHOLD)
	/* almost touch the full scale */
	{
		return(uCurrLed >> 1u);
	}
	
	if(iSampleVal <= (0 - FULLSCALE_THRESHOLD))
	/* almost touch the minus full scale */
	{
		return(uCurrLed + (uCurrLed >> 1u));
	}

	if(bIsStage2Enabled == false)
	{
		tmpResult = ((float32_t)iTargetVal/iSampleVal)*uCurrLed;
			
		return((tmpResult > 255)? 0xff : (uint8_t)tmpResult);
	}
	else
	{
		tmpFloat1 = (float32_t)iTargetVal + (float32_t)CURRENT_CANCEL_COMPENSATION;
		tmpFloat2 = (float32_t)iSampleVal + (float32_t)CURRENT_CANCEL_COMPENSATION;

		tmpResult = (tmpFloat1/tmpFloat2)*uCurrLed;

		return((tmpResult > 255)? 0xff : (uint8_t)tmpResult);
	}
}
/**
* @brief Led signal adjust, calculate process 
* @details
* 	-# check the average value, if fall into acceptable range
*   -# calculate the proper LED value
* 	-# trig following process if needed
* @param[in] iIrAverVal Ir average input
* @param[in] iRedAverVal Red average input
* @param[in] bIsStage2Enabled stage 2 amplifer enabled or not
* @param[in] maxVal acceptable max value
* @param[in] minVal acceptable min value
* @param[in] targetVal acceptable target value
* @param[out] none
* @retval none
*/
static void apl_channel_signal_adjust(int32_t iIrAverVal, int32_t iRedAverVal, 
								bool bIsStage2Enabled, int32_t maxVal, int32_t minVal, int32_t targetVal)
{
	uint8_t uTempCurr;

	/*adjust for infrared channel*/
	if(
        (iIrAverVal > maxVal)
        ||(iIrAverVal < minVal)
    )
	{
		uTempCurr = apl_led_curr_est(targetVal, s_txChannelParameter_st.m_uIrLedCurr, iIrAverVal, bIsStage2Enabled);
    
		//Can add code here to process the situation(less than min allowable current
		if(uTempCurr <= IR_LED_CUR_MIN_VAL)							//less than minmum current value
		{
			s_txChannelParameter_st.m_uIrLedCurr = IR_LED_CUR_MIN_VAL;
		}
		else if(uTempCurr >= IR_LED_CUR_MAX_VAL)
		{
			s_txChannelParameter_st.m_uIrLedCurr = IR_LED_CUR_MAX_VAL;
		}
		else if(uTempCurr == s_txChannelParameter_st.m_uIrLedCurr)
		{
			if(iIrAverVal > targetVal)
			{
				s_txChannelParameter_st.m_uIrLedCurr = uTempCurr - 1;
			}
			else
			{
				s_txChannelParameter_st.m_uIrLedCurr = uTempCurr + 1;
			}
		}
		else
		{
			s_txChannelParameter_st.m_uIrLedCurr = uTempCurr;
		}
    
        APP_TRACE_LOG("ir : %x\r\n",uTempCurr);
	} //end if


	if(
        (iRedAverVal > maxVal)
        ||(iRedAverVal < minVal)
    )	
	{
		uTempCurr = apl_led_curr_est(targetVal, s_txChannelParameter_st.m_uRedLedCurr, iRedAverVal, bIsStage2Enabled);
    
		//Can add code here to process the situation(less than min allowable current
		if(uTempCurr <= RED_LED_CUR_MIN_VAL)							//less than minmum current value
		{
			s_txChannelParameter_st.m_uRedLedCurr = RED_LED_CUR_MIN_VAL;
		}
		else if(uTempCurr >= RED_LED_CUR_MAX_VAL)
		{
			s_txChannelParameter_st.m_uRedLedCurr = RED_LED_CUR_MAX_VAL;
		}
		else if(uTempCurr == s_txChannelParameter_st.m_uRedLedCurr)
		{
			if(iRedAverVal > targetVal)
			{
				s_txChannelParameter_st.m_uRedLedCurr = uTempCurr - 1;
			}
			else
			{
				s_txChannelParameter_st.m_uRedLedCurr = uTempCurr + 1;
			}
		}
		else
		{
			s_txChannelParameter_st.m_uRedLedCurr = uTempCurr;
		}
    
        APP_TRACE_LOG("red : %x\r\n",uTempCurr);
	} //end if
	drv_afe4403_led_curr_set(s_txChannelParameter_st.m_uRedLedCurr,s_txChannelParameter_st.m_uIrLedCurr);
}


static void apl_raw_sample_average(int32_t *piRawIr,int32_t *piRawRed, uint16_t uSlotLen, int32_t *piAveIr, int32_t *piAveRed)
{
    uint16_t i = 0;
    int32_t i32IrSum = 0;
    int32_t i32RedSum = 0;
    for(i = 0; i < uSlotLen; i++)
    {
        i32IrSum += piRawIr[i];
        i32RedSum += piRawRed[i];
    }
    *piAveIr = i32IrSum / uSlotLen;
    *piAveRed = i32RedSum / uSlotLen;
}

/**
  * @brief  adjust afe4403 led
  * @param  none
  * @retval 1:adjust ok; 0:adjust fail
  */
uint8_t apl_pulse_oximetry_signal_adjust_check(void)
{
    //record the sample number for average
    int32_t iIrDataAverage  = 0;
    int32_t iRedDataAverage = 0;
  
    /*	Cal the average of Ir/Red channel 		*/
    apl_raw_sample_average(s_piSampleIr + AFE_DAT_BUF_LEN - DATA_BUF_MAX_ADJ, s_piSampleRed + AFE_DAT_BUF_LEN - DATA_BUF_MAX_ADJ, DATA_BUF_MAX_ADJ, &iIrDataAverage, &iRedDataAverage);

    APP_TRACE_LOG("%d\t%d\r\n",iRedDataAverage,iIrDataAverage);

  /*	Process to adjust the signal according to input signal level 		*/
  if((iIrDataAverage <= TARGET_SIGNAL_LEVEL_MAX)&&(iIrDataAverage >= TARGET_SIGNAL_LEVEL_MIN)
      &&(iRedDataAverage <= TARGET_SIGNAL_LEVEL_MAX)&&(iRedDataAverage >= TARGET_SIGNAL_LEVEL_MIN))
    /* In the fine range*/
  {
    if(1 == s_uSignalValid)
    {
      return 1;
    }
    /* Increase the good counter  */
    s_uGoodDcCnt++;
    APP_TRACE_LOG("good:%u\r\n",s_uGoodDcCnt);
    
    /* Reach the good counter max */
    if(s_uGoodDcCnt >= ADJ_GOOD_CNT_THRES)
      /* Pass adj, enter measurement */
    {
      /*insert new event to start measurement*/       
      s_uGoodDcCnt = 0;
      s_uBadDcCnt = 0;
      s_uSignalValid = 1;
    }
  }
  else
  {
    s_uGoodDcCnt = 0;
    s_uBadDcCnt++;
    
    if(
      (1 == s_uSignalValid)
      && (s_uBadDcCnt <= 3)
    )
    {
      return 1;
    }
    else if(1 == s_uSignalValid)
    {
      s_uSignalValid = 0;
      s_uBadDcCnt = 0;
    }
    
    /* 	Adjust the IR/RED channel's LED current	*/
    apl_channel_signal_adjust(iIrDataAverage, iRedDataAverage, s_rxChannelParameter_st.m_bIsStage2, TARGET_SIGNAL_LEVEL_MAX, TARGET_SIGNAL_LEVEL_MIN, TARGET_SIGNAL_LEVEL);
    /* 	Clear the good counter 	*/

    APP_TRACE_LOG("bad:%u\r\n",s_uBadDcCnt);
    if(s_uBadDcCnt >= ADJ_BAD_CNT_THRES)
    {
      cc_alg_SpO2_Stat_Set(SPO2_HR_STATUS_ERROR);
      apl_collection_event_post(CollectionStart);

      s_uBadDcCnt = 0;
    }     
  }

  return s_uSignalValid;
}

static int32_t s_iRedRawSampleData[100];
static int32_t s_iIrRwaSampleData[100];

void spo2_hr_cal_mod_init(void)
{
    /*  Init the interaction module */
    //  s_piSampleRed = s_DataSampleCopy_st.i32RedDat + DATA_BUF_MAX_MEA;
    //  s_piSampleIr = s_DataSampleCopy_st.i32IrDat + DATA_BUF_MAX_MEA;

    s_piSampleRed = s_iRedRawSampleData;
    s_piSampleIr = s_iIrRwaSampleData;

    spo2_hr_interact_init();

    cc_gSensor_data_buf_get(&s_piSampleX,&s_piSampleY,&s_piSampleZ);

    g_HrSpo2Val_st.m_HrSpo2DataRep_pst = &HrSpO2DataFinal_st;
}


static void spo2_hr_interact_init(void)
{
#if MOD_CTRL_IS_DATA_DOUBLE_CHECK_IN_MONITOR_MODE	== true
	memset(&s_HrMonitorStore_st,0,sizeof(result_datBuf_Typedef));
	s_HrMonitorStore_st.m_ui16DataLen = DATA_DOUBLE_CHECK_NUM;
	s_HrMonitorStore_st.m_pui8Data = s_HrCheckBuf_arr;
	memset(&s_SpO2MonitorStore_st,0,sizeof(result_datBuf_Typedef));
	s_SpO2MonitorStore_st.m_ui16DataLen = DATA_DOUBLE_CHECK_NUM;
	s_SpO2MonitorStore_st.m_pui8Data = s_SpO2CheckBuf_arr;
#endif
}



/******************************************************************
*                        spo2_hr_post_handle                      *
******************************************************************/
void spo2_hr_post_handle(void)
{
	#if MOD_CTRL_IS_DATA_DOUBLE_CHECK_IN_MONITOR_MODE == true
		uint8_t 	valTmp;
	#endif
	
	HR_SPO2_DAT_T * SpO2HrFinal_pst;
	
	RTC_DateTypeDef	rtc_date_st;
	RTC_TimeTypeDef	rtc_time_st;
	
	calendar_get(&rtc_date_st,&rtc_time_st);
	
	g_HrSpo2Val_st.m_RtcDate_st.Year = rtc_date_st.Year;
	g_HrSpo2Val_st.m_RtcDate_st.Month = rtc_date_st.Month;
	g_HrSpo2Val_st.m_RtcDate_st.Date = rtc_date_st.Date;
	g_HrSpo2Val_st.m_RtcTime_st.Hours = rtc_time_st.Hours;
	g_HrSpo2Val_st.m_RtcTime_st.Minutes = rtc_time_st.Minutes;
	g_HrSpo2Val_st.m_RtcTime_st.Seconds = rtc_time_st.Seconds;
	
	SpO2HrFinal_pst = g_HrSpo2Val_st.m_HrSpo2DataRep_pst;
	
#if MOD_CTRL_IS_BLOCK_HUNDRED_SPO2
		if(SpO2HrFinal_pst->m_ui8SpO2Val == 100)
		{
			SpO2HrFinal_pst->m_ui8SpO2Val = 99;
		}
#endif

   APP_TRACE_LOG("##################### hr:%u spo2:%u\r\n",SpO2HrFinal_pst->m_ui8HrVal,SpO2HrFinal_pst->m_ui8SpO2Val);	
}



/******************************************************************************************/

/**
  * @brief  Handle SpO2 algorithm task
  * @param  uint32_t event
  * @retval None
  * @todo
  */
void SpO2_Task_Handler(uint32_t SpO2_Event)
{
    SPO2_HR_CAL_STATUS_T 	SpO2HrCalStatus_st;
    SPO2_HR_CAL_MODE_T		SpO2HrCalMode_st;

    /* @todo: initial structure if necessary */

    /* Init the SpO2 Cal Mode */
    SpO2HrCalMode_st.bIsMotionReportOnly		= 	false;
    SpO2HrCalMode_st.bIsFirstCalc				= 	false;
	
    switch (SpO2_Event)
    {  
        case SpO2EventMeasureInit:
        {
            APP_TRACE_LOG("SPO2 MEASURE INIT\r\n");
            /*	Init the SpO2 measure program		*/
            CZT_Process_Init();

            SpO2HrCalPreDataProcess();

            SpO2HrCalCZTFilterFlush();
            /*	Clear the Spot Check Counter 		*/
            //      cc_spot_check_cnt_clr();

            /*	Run SpO2 measure program			*/
            SpO2HrCalMode_st.bIsFirstCalc 	= true;			/*	The first calc after adjust	*/

            SpO2HrCal_Entry(&SpO2HrCalMode_st, &SpO2HrCalStatus_st, &HrSpO2DataFinal_st);
        }
        break;
        case SpO2EventMeasureCal:
        {
            APP_TRACE_LOG("SPO2 MEASURE CAL\r\n");

            //        if(1 == apl_pulse_oximetry_signal_adjust_check())
            //        {
            /*	Run SpO2 measure program			*/
            SpO2HrCal_Entry(&SpO2HrCalMode_st, &SpO2HrCalStatus_st, &HrSpO2DataFinal_st);
            //        }       
        }
        break;
        case SpO2EventAdjust:
        {
            //APP_TRACE_LOG("SPO2 ADJUST POLL\r\n");
            apl_afe4403_adjust_poll();
        }
        break;

        default:
            APP_TRACE_LOG("SPO2 DEFAULT\r\n");
        break;
    }
}

void apl_afe4403_adjust_poll(void)
{
  int32_t iIrDataAverage  = 0;
	int32_t iRedDataAverage = 0;
  
  if(s_uAfeDataCnt > AFE_ADJUST_LEN)
  {
    apl_raw_sample_average(s_iIrRawDataAdjust_arr, s_iRedRawDataAdjust_arr, AFE_ADJUST_LEN, &iIrDataAverage, &iRedDataAverage);
//    APP_TRACE_LOG("%d\t%d\r\n",iRedDataAverage,iIrDataAverage);
    /*	Process to adjust the signal according to input signal level 		*/
    if(
      (iIrDataAverage <= TARGET_SIGNAL_LEVEL_MAX)
      &&(iIrDataAverage >= TARGET_SIGNAL_LEVEL_MIN)
      &&(iRedDataAverage <= TARGET_SIGNAL_LEVEL_MAX)
      &&(iRedDataAverage >= TARGET_SIGNAL_LEVEL_MIN)
    )
    {
      s_uBadDcCnt = 0;
    }
    else
    {
      s_uBadDcCnt++;     
      s_uAfeDataCnt = 0;
      s_uGsensorDataCnt = 0;
      
      /* 	Adjust the IR/RED channel's LED current	*/
      apl_channel_signal_adjust(iIrDataAverage, iRedDataAverage, s_rxChannelParameter_st.m_bIsStage2, TARGET_SIGNAL_LEVEL_MAX, TARGET_SIGNAL_LEVEL_MIN, TARGET_SIGNAL_LEVEL);
      /* 	Clear the good counter 	*/

      APP_TRACE_LOG("bad:%u\r\n",s_uBadDcCnt);
      if(s_uBadDcCnt >= ADJ_BAD_CNT_THRES)
      {
        cc_alg_SpO2_Stat_Set(SPO2_HR_STATUS_ERROR);
        apl_collection_event_post(CollectionStart);

        s_uBadDcCnt = 0;
      }
    }
  }
}

//static uint8_t s_uAfe4403DataBuf_arr[200];

void apl_afe4403_interrupt_handler(void)
{
  int32_t redData,irData;
  int16_t accelDataX,accelDataY,accelDataZ;
//  static uint8_t MeasureCnt = 0;
//  static uint8_t uPacketCnt = 0;
//  static uint8_t redIndex = 4;
//  static uint8_t irIndex = 64;
//  static uint8_t XIndex = 124;
//  static uint8_t YIndex = 144;
//  static uint8_t ZIndex = 164;

	/*read data from AFE4403*/
	drv_afe4403_red_ir_get(&redData,&irData);
	
//  APP_TRACE_LOG("%d\t%d\r\n",redData,irData);
  
    s_piSampleRed[s_uAfeDataCnt] = redData;
    s_piSampleIr[s_uAfeDataCnt] = irData;

    if(s_uAfeDataCnt <= AFE_ADJUST_LEN)
    {
        s_iRedRawDataAdjust_arr[s_uAfeDataCnt] = redData;
        s_iIrRawDataAdjust_arr[s_uAfeDataCnt] = irData;
    }
  
//  if(MeasureCnt == 0)
//  {
//    s_uAfe4403DataBuf_arr[0] = 0xCA;
//    s_uAfe4403DataBuf_arr[1] = 0xFE;
//    s_uAfe4403DataBuf_arr[2] = uPacketCnt++;
//    s_uAfe4403DataBuf_arr[3] = 0x00;
//  }
//  MeasureCnt++;
//  
//  s_uAfe4403DataBuf_arr[redIndex++] = (irData >> 0) & 0xff;
//  s_uAfe4403DataBuf_arr[redIndex++] = (irData >> 8) & 0xff;
//  s_uAfe4403DataBuf_arr[redIndex++] = (irData >> 16) & 0xff;
//  
//  s_uAfe4403DataBuf_arr[irIndex++] = (redData >> 0) & 0xff;
//  s_uAfe4403DataBuf_arr[irIndex++] = (redData >> 8) & 0xff;
//  s_uAfe4403DataBuf_arr[irIndex++] = (redData >> 16) & 0xff;
  
  
    s_uAfeDataCnt++;
  
    if((s_uAfeDataCnt & 0x01) == 0)
    {
        /* 	Get the raw data 	*/
        accel_data_get(&accelDataX,&accelDataY,&accelDataZ);
        s_piSampleX[s_uGsensorDataCnt] = accelDataX;
        s_piSampleY[s_uGsensorDataCnt] = accelDataY;
        s_piSampleZ[s_uGsensorDataCnt] = accelDataZ;
        s_uGsensorDataCnt++;
        //    s_uAfe4403DataBuf_arr[XIndex++] = (accelDataX >> 0) & 0xff;
        //    s_uAfe4403DataBuf_arr[XIndex++] = (accelDataX >> 0) & 0xff;
        //    
        //    s_uAfe4403DataBuf_arr[YIndex++] = (accelDataY >> 0) & 0xff;
        //    s_uAfe4403DataBuf_arr[YIndex++] = (accelDataY >> 0) & 0xff;
        //    
        //    s_uAfe4403DataBuf_arr[ZIndex++] = (accelDataZ >> 0) & 0xff;
        //    s_uAfe4403DataBuf_arr[ZIndex++] = (accelDataZ >> 0) & 0xff;
    }

//  if(MeasureCnt >= 20)
//  {
//    MeasureCnt = 0;
//    redIndex = 4;
//    irIndex = 64;
//    XIndex = 124;
//    YIndex = 144;
//    ZIndex = 164;
//    
//    s_uAfe4403DataBuf_arr[191] = 0xaa;
//    s_uAfe4403DataBuf_arr[192] = 0xbb;
//    s_uAfe4403DataBuf_arr[193] = 0xcc;
//    s_uAfe4403DataBuf_arr[194] = 0xdd;
//    s_uAfe4403DataBuf_arr[195] = 0xee;
//    s_uAfe4403DataBuf_arr[196] = 0xff;
//    s_uAfe4403DataBuf_arr[197] = 0xff;
//    s_uAfe4403DataBuf_arr[198] = 0xff;
//    s_uAfe4403DataBuf_arr[199] = 0xff;
//    
//    drv_uart_write(s_uAfe4403DataBuf_arr,200);
//  }
  
    if(s_uAfeDataCnt >= AFE_DAT_BUF_LEN)
    {
        s_uAfeDataCnt = 0;
        s_uGsensorDataCnt = 0;

        SpO2HrCalPreDataProcess();

        memcpy(s_DataSampleCopy_st.i32RedDat + AFE_DAT_BUF_LEN,s_piSampleRed,AFE_DAT_BUF_LEN*sizeof(int32_t));
        memcpy(s_DataSampleCopy_st.i32IrDat + AFE_DAT_BUF_LEN,s_piSampleIr,AFE_DAT_BUF_LEN*sizeof(int32_t));

        apl_spo2_event_post(SpO2EventMeasureCal);
    }
    
    if(s_uAfeDataCnt > AFE_ADJUST_LEN)
    {
        apl_spo2_event_post(SpO2EventAdjust);
    }
}


/******************************************
* Name:                 cc_alg_measure_start
* Author:               yun.zhu@ywkang.com 
* Date:                 2016/3/30
* Description:          the entry point to the app level
* Param[in]
*    1. 
* Param[out]:
*    1. 
* Return:
******************************************/
void cc_alg_measure_start(void)
{
    s_uBadDcCnt = 0;
    s_uGoodDcCnt = 0;
    s_uAfeDataCnt = 0;
    s_uSignalValid = 0;
    s_uGsensorDataCnt = 0;

    s_txChannelParameter_st.m_uLedTurnOn = AFE44x0_LEDCUROFF_ON;
    s_txChannelParameter_st.m_uRedLedCurr = RED_LED_CUR_DEFAULT_VAL;
    s_txChannelParameter_st.m_uIrLedCurr = IR_LED_CUR_DEFAULT_VAL;
    s_rxChannelParameter_st.m_bIsStage2 = true;


    s_SpO2HrInitParam.m_DataSmoothLevel = SPO2_SMOOTH_LEVEL1;
              
    s_SpO2HrInitParam.m_bIsFactoryMode = false;

    /*	Get the DC Compensate 		*/
    s_SpO2HrInitParam.m_iAfeDcOffset = (uint32_t)CURRENT_CANCEL_COMPENSATION;

    /*	Set the Motion Check threshold 	*/
    s_SpO2HrInitParam.m_uiMotionCheckThres 		= SENSOR_MOTION_THRESHOLD_SPO2;

    /*	Set the Clear-while_motion threshold 	*/

    s_SpO2HrInitParam.m_uiClearWhileMotionThres 	= SENSOR_DATA_ANTI_MOV_THRES;
                
    /*  init the calculate module's     */
    SpO2HrCal_Init(&s_SpO2HrInitParam);

    /* Set status 						*/
    cc_alg_SpO2_Stat_Set(SPO2_HR_STATUS_RUNNING);

    /*	Clear the SpOt check counter 		*/
    //  cc_spot_check_cnt_clr();
}

/******************************************
* Name:                 cc_alg_measure_stop
* Author:               yun.zhu@ywkang.com 
* Date:                 2016/3/30
* Description:          the entry point to the app level
* Param[in]
*    1. 
* Param[out]:
*    1. 
* Return:
******************************************/
void cc_alg_measure_stop(void)
{
    s_uAfeDataCnt = 0;

    HrSpO2DataFinal_st.m_bIsHrValid				= false;
    HrSpO2DataFinal_st.m_bIsSpO2Valid			= false;
    HrSpO2DataFinal_st.m_ui16MotionVal		= 0;
    HrSpO2DataFinal_st.m_ui8HrVal				  = 0;
    HrSpO2DataFinal_st.m_ui8SpO2Val				= 0;
    HrSpO2DataFinal_st.m_ui8ConfVal				= 0;

    /* set status */
    cc_alg_SpO2_Stat_Set(SPO2_HR_STATUS_STOP);	
}

/******************************************************************
*                        SpO2_Stat_Set                            *
* [Yun] set SpO2 Sub module status                                * 
*******************************************************************/
void cc_alg_SpO2_Stat_Set(ccAlgSpO2HrStatus_en newState)
{
	s_Spo2SubModuleStat_en = newState;
}
/******************************************************************
*                        cc_alg_SpO2_Stat_Get                            *
* [Yun] Get SpO2 Sub module status                                * 
*******************************************************************/
ccAlgSpO2HrStatus_en cc_alg_SpO2_Stat_Get(void)
{
	return(s_Spo2SubModuleStat_en);
}



/********************* arithmetic task interface *********************/
/**
  * @brief  Handle SpO2 algorithm task.
  * @param  argument is transferred during task created.
  * @retval None
  * @todo
  */
void spo2Task(void *argument)
{	
    /* Infinite loop */
    for(;;)
    {
        uint32_t v_uEvent;

        if(apl_spo2_event_pend(&v_uEvent))
        {
            SpO2_Task_Handler(v_uEvent);
        }
    }
}

/* collectionTask function */
void collectionTask(void *argument)
{
    /* Infinite loop */
    for(;;)
    {
        uint32_t v_uEvent;

        if(apl_collection_event_pend(&v_uEvent))
        {
            switch(v_uEvent)
            {
                case CollectionStart:
                {
                    APP_TRACE_LOG("SPO2 START\r\n");
                    //        FW_UPDATE_INFO_T FwUpdateInfo_st;
                    //    

                    //        apl_fw_update_cfg_set(FwUpdateInfo_st);
                    //        
                    //        APP_TRACE_LOG("ready to reset\r\n");
                    //        apl_soft_reset();

                    /*  Init the algorithm moduel   */
                    spo2_hr_cal_mod_init();
                    drv_afe4403_init();
                    drv_afe4403_running_enable();
                    
                    /* 	Set the status and following actions 		*/
                    if(
                        (SPO2_HR_STATUS_RUNNING != cc_alg_SpO2_Stat_Get())
                        && (gSubFunc_Stat_Get(SPO2_NEEDED_STATE) != OFF)
                    )
                    {
                        cc_alg_measure_start();
                        apl_spo2_event_post(SpO2EventMeasureInit);
                    }
                }                    
                break;
                case CollectionStop:
                {
                    drv_afe4403_running_disable();
                    
                    if(
                        (SPO2_HR_STATUS_STOP != cc_alg_SpO2_Stat_Get())
                        &&(gSubFunc_Stat_Get(SPO2_NEEDED_STATE) == OFF) 
                    )
                    {
                        cc_alg_measure_stop();      	
                    }
                }
                break;
                case CollectionRunning:
                {
                    apl_afe4403_interrupt_handler();
                }
                break;
                default: 
                break;
            }
        }
    }
    /* USER CODE END collectionTask */
}
/**
*@brief add event to spo2Queue
*@param[in] v_uEvent:event
*@retval none
*/
void apl_spo2_event_post(uint32_t v_uEvent)
{
	if(spo2QueueHandle)
	{
		osMessagePut(spo2QueueHandle,v_uEvent,0);
	}
}
/**
*@brief get event from spo2Queue
*@param[out] *v_puEvent:the event got form spo2Queue
*@retval 1:get event successfully;0:get unsuccessfully
*/
uint8_t apl_spo2_event_pend(uint32_t *v_puEvent)
{
	osEvent v_Event_st;
	v_Event_st = osMessageGet(spo2QueueHandle,osWaitForever);
	if( osEventMessage == v_Event_st.status )
	{
		*v_puEvent = v_Event_st.value.v;
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
*@brief add event to collectQueue
*@param[in] v_uEvent: the event add to collectQueue
*@retval none
*/
void apl_collection_event_post(uint32_t v_uEvent)
{
	if(collectionQueueHandle)
	{
		osMessagePut(collectionQueueHandle,v_uEvent,0);
	}
}
/**
  *@brief get event from collectQueue
  *@param[out] *v_puEvent:the event got form collectQueue
  *@retval 1:get event successfully;0:get unsuccessfully
  */
uint8_t apl_collection_event_pend(uint32_t *v_puEvent)
{
	osEvent v_Event_st;
	v_Event_st = osMessageGet(collectionQueueHandle,osWaitForever);
	if( osEventMessage == v_Event_st.status )
	{
		*v_puEvent = v_Event_st.value.v;
		return 1;
	}
	else
	{
		return 0;
	}
}

// end file
