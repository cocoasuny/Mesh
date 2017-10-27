/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_types.h
* Descprition:              the typedef .h files for algorithm process
* Created Date:             2016/03/12
* Author:                   yun.zhu@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef __CC_ALG_TYPES_H
#define __CC_ALG_TYPES_H

#include "stdint.h"
#include "stdbool.h"
#include "main.h"


/** @todo
  * @brief debug purpose only
  */
#define MOD_CTRL_IS_ENABLE_MQRDLSL      true
#define MOD_CTRL_IS_ENABLE_WAVELET      true
#define MOD_CTRL_IS_ENABLE_IIR_BANDPASS true 

/** @todo-1: Move to pulse trace
  * @brief Definition for frequency peak search
  */           
#define FREQ_PEAK_BUF_LEN			          (10)
#define FREQ_PEAK_MATCH_BUF_LEN         (6)
#define PEAK_MATCH_BUF_NUM							(3) 	  //MUST less or equal to FREQ_PEAK_MATCH_BUF_LEN


//#ifndef MACRO_CONST_NUM_ZERO
//	#define 	MACRO_CONST_NUM_ZERO				(0)					/**< define 0 		*/
//#endif

#ifndef MACRO_CONST_NUM_ONE
	#define 	MACRO_CONST_NUM_ONE					(1)					/**< define 1 		*/
#endif

#ifndef MACRO_CONST_NUM_TWO
	#define 	MACRO_CONST_NUM_TWO					(2)				  /**< define 2 		*/
#endif

#ifndef MACRO_CONST_NUM_NEG_ONE
	#define 	MACRO_CONST_NUM_NEG_ONE			(-1)			  /**< define -1 		*/
#endif

#ifndef MACRO_CONST_NUM_EIGHT
	#define 	MACRO_CONST_NUM_EIGHT				(8)				  /**< define 8 		*/
#endif

#ifndef MACRO_CONST_NUM_SIXTEEN
	#define 	MACRO_CONST_NUM_SIXTEEN			(16)			  /**< define 16 		*/
#endif

//#ifndef MACRO_CONST_NUM_FLOAT_ZERO
//	#define 	MACRO_CONST_NUM_FLOAT_ZERO	(0.0)			  /**< define 0.0 		*/
//#endif

/** 
  * @def      CZT_N	The CZT length
  * @details 	This value will be used as #if ** == **, 
  * 				  So, don't need a branch
  */
#define CZT_N 						256

/**
  * @brief Definition of system event function
  */
#define CC_ALG_SYS_EVENT_SEND_PRT void (* task_event_send)(uint8_t, uint32_t, SysEvtDatTypedef *)

/**
  * @brief Definition of system event function
  */
#define CC_ALG_SYS_EVENT_SEND_FUNC(task_id, event_id, pSysEvtDat)\
           task_event_send(task_id, event_id, pSysEvtDat)
  
/**
  * @brief Definition of System critical resource apply routine
  */
#define CC_ALG_SYS_CRITICAL_RESOURCE_APPLY_PRT void (* cc_alg_sys_critical_resource_apply)(void)
  
/**
  * @brief Definition of System critical resource release routine
  */
#define CC_ALG_SYS_CRITICAL_RESOURCE_RELEASE_PRT void (* cc_alg_sys_critical_resource_release)(void)

/**
  * @brief Definition of system delay in million second
  */
#define CC_ALG_SYS_DELAY_MS_PRT void (* cc_alg_sys_delay_ms)(uint16_t)

/**
  * @def 	FFT translation related defintion
  */
#define FLAG_FFT 					          (0)						/*	FFT transfter indicator				*/
#define FLAG_IFFT 					        (1)						/*	FFT Invert transfter indicator		*/
#define FLAG_REV_BIT_ORDER_ENABLE		(1)				    /*	FFT bit order revert enable 		*/
#define FLAG_REV_BIT_ORDER_DISABLE	(0)				    /*	FFT bit order revert disable 		*/

/**
  * @def 	define the position relationship
  */
#if CZT_N == 512
	#define FREQ_PEAK_POS_UP_LIMIT 			(475)
	#define FREQ_PEAK_POS_LP_LIMIT 			(25)
	#define CZT_FREQ_OFFSET					    (38)
	#define PEAK_MATCH_THRES				    (12)
	#define HR_NO_SECOND_PEAK_THRE_POS  (210)
	#define 	XCORR_PEAK_MIN_DISTANCE		(12)
	#define 	XCORR_PEAK_MAX_DISTANCE		(100)
#elif CZT_N == 256
	#define FREQ_PEAK_POS_UP_LIMIT 			(250)
	#define FREQ_PEAK_POS_LP_LIMIT 			(13)
	#define CZT_FREQ_OFFSET					    (19)
	#define PEAK_MATCH_THRES				    (6)
	#define HR_NO_SECOND_PEAK_THRE_POS  (125)
	#define 	XCORR_PEAK_MIN_DISTANCE		(6)
	#define 	XCORR_PEAK_MAX_DISTANCE		(50)
#endif

/**
  * @brief ######################
  * @todo
  */
typedef struct{
	uint8_t 			m_ui8SpO2Val;
	uint8_t 			m_ui8HrVal;
    uint16_t            m_ui16MotionVal;
	bool 		        m_bIsSpO2Valid;
	bool 		        m_bIsHrValid;
    uint8_t             m_ui8ConfVal;
}HR_SPO2_DAT_T;

/**
  * @brief ######################
  * @todo
  */
/* Define the HR/SpO2  data parameters */
typedef struct
{
	RTC_DateTypeDef         m_RtcDate_st;               //the date of the data
	RTC_TimeTypeDef         m_RtcTime_st;               //the time of the data
	uint8_t                 m_uHrValue;                 //HR Value
	uint8_t                 m_uSpo2Value;               //SpO2 value
	uint8_t	                m_uSpo2Valid;			    /* 		Only for test, is spo2 is newly updated 		*/
	uint16_t                m_uMoveLevelVal;		    //flag to quantify the move level
	uint8_t                 m_uConfidenceLevel;         //the confidence level
	HR_SPO2_DAT_T           *m_HrSpo2DataRep_pst;
}HR_SPO2_PARAMETERS_T;


/**
 * @brief Definition of algorithm module status 
 */
typedef enum{
    SPO2_HR_STATUS_NULL         = 0,            /*  Null for reserved                   */
    SPO2_HR_STATUS_ERROR        = 1,            /*  algorithm mode error                */
    SPO2_HR_STATUS_RUNNING      = 2,            /*  algorithm is running                */
    SPO2_HR_STATUS_CHECK        = 3,            /*  spot check is ongoing               */
    SPO2_HR_STATUS_STOP         = 4
} ccAlgSpO2HrStatus_en;

/**
*   @brief Function Return Code definition
*/
typedef enum{
    RET_VAL_NULL            = (0),
    RET_VAL_SUCCESS         = (1),
    RET_VAL_PARAM_ERROR     = (-1),
    RET_VAL_CAL_ERROR       = (-2),
    RET_VAL_CAL_FAIL        = (-3)      /*  Not finish the calculation as routine   */
} FUNC_RETURN_TYPE;

/**
  * @brief SPO2_SMOOTH_LEVEL
  * LEVEL 1-- fast response time, less average
  * LEVEL 2-- balanced repsonse time
  * LEVEL 3-- stable data output
  */
typedef enum{
    SPO2_SMOOTH_LEVEL1     = (1),
    SPO2_SMOOTH_LEVEL2     = (2),
    SPO2_SMOOTH_LEVEL3     = (3),
} SPO2_SMOOTH_LEVEL_TYPE;  

/**
  * @brief Definition of SPO2/HR running steps
  */
typedef enum{
    SPO2_HR_RUN_STEP_NULL			    = 0,            /*  Null for reserved                   */
    SPO2_HR_RUN_STEP_HW_ERROR		  = 1,            /*  hw error detected	                */
    SPO2_HR_RUN_STEP_SPOT_CHECK		= 2,            /*  spot check is ongoing				*/
    SPO2_HR_RUN_STEP_BASELINE_ADJ	= 3,            /*  baseline adj is ongoing				*/
    SPO2_HR_RUN_STEP_CAL_ONGOING	= 4				      /*	spo2 hr calculation is ongoing		*/
} ALG_RUN_STEP_T;


/**
  * @brief Definition of 24bits IR/RED sample data 
  */
typedef struct{
  int32_t m_iIrData;
  int32_t m_iRedData;
}sampleDataRaw_Typedef;


/**
  * @brief Definition of SpO2 Hr module running mode
  */
typedef enum{
    SPO2_HR_MODE_NULL   = 0,            /*  Null for reserved   */
    SPO2_HR_MODE_OSA    = 1,            /*  OSA monitor mode    */
    SPO2_HR_MODE_DEMO   = 2,            /*  demo mode           */
    SPO2_HR_MODE_COPD   = 3,            /*  COPD mode           */
    SPO2_HR_MODE_DETECT = 4             /*  Detect mode, only detect the reflect light  */
} ccAlgSpO2HrRunMode_en;

/**
  * @brief Definition of SpO2Hr_Algorithm_Input_Parameters
  */
typedef struct{
    ccAlgSpO2HrRunMode_en   m_AlgRunMode_en;        /* the running mode of algorithm module */
    uint32_t                m_AlgRunTimeOut_s;      /* the timeout limit from start measuring, 
                                                       unit is second, should not less than 20
                                                       if set to 0, will never stop the measure */
    int32_t                 m_AlgDetThres;          /* the threshold when in detect mode */
} ccAlgSpO2HrParam_st;

/**
  * @brief 	definition of structure of spectrum information, for single channel
  */
typedef struct{
	uint16_t 		*	puPeakPosition;				/**<	pointer to the position buffer of peak in spectrum	*/
	int32_t 		* 	piPeakVal;					/**<	pointer to the value buffer of peak in spectrum		*/
	uint16_t 			uMaxPeakNum;				  /**<	max number of the peaks, should be init 			*/
	uint16_t 			uValidPeakNum;				/**<	valid peak numbers									*/
} PEAK_POS_VAL_INFO_T;

/**
* @brief Definition of system event function's data body
*/
typedef struct{
    uint16_t *   m_pui16SysEvtDat;
} SysEvtDatTypedef;

/**
  * @brief Definition of SpO2/Hr run paramters input structure
  */
typedef struct{
    SPO2_SMOOTH_LEVEL_TYPE  m_DataSmoothLevel;
    ccAlgSpO2HrRunMode_en   m_AlgSpO2HrRunMode;
    bool                    m_bIsFactoryMode;
	  int32_t 				        m_iAfeDcOffset;
	  uint32_t				        m_uiMotionCheckThres;
	  uint32_t 				        m_uiClearWhileMotionThres;
}HR_SPO2_INIT_PARA_Typedef;


///**
//  *   @brief generic data type define here, need to change for different platform
//  */
///* exact-width signed integer types */
//typedef   signed          char      int8_t;
//typedef   signed short     int      int16_t;
//typedef   signed           int      int32_t;
//typedef   signed long long          int64_t;

///* exact-width unsigned integer types */
//typedef unsigned          char      uint8_t;
//typedef unsigned short     int      uint16_t;
//typedef unsigned           int      uint32_t;
//typedef unsigned long long          uint64_t;

///* exact-width float types */
//typedef float                       float32_t;
//typedef double                      float64_t;

///*  Null Definition         */
//#ifndef NULL
//    #define NULL            (0)										/**< define NULL	*/
//#endif

///*  Bool type Definition         */
//#ifndef true
//    #define true    (1)        
//#endif

//#ifndef false
//    #define false   (0)
//#endif
    
///**
//* @brief Definition of system event for spo2 measurement
//*/
//#define gSpO2EventBaselineStart					  ((uint32_t)(1 << 1))
//#define gSpO2EventBaselineCal					  ((uint32_t)(1 << 2))

//#define gSpO2EventMeasureInit					  ((uint32_t)(1 << 3))
//#define gSpO2EventMeasureCal 					  ((uint32_t)(1 << 4))

//#define gSpO2EventError							  ((uint32_t)(1 << 7))

//#define gSpO2EventStartInternal					((uint32_t)(1 << 9))

//#define gSpO2EventTimeExpire					((uint32_t)(1 << 13))

//#define gSpO2EventSignalCheck                   ((uint32_t)(1 << 10))

//#define gSpO2EventSpotCheck                     ((uint32_t)(1 << 11))

//#define gSpO2EventDatReport						((uint32_t)(1 << 12))

#endif  // __CC_ALG_TYPES_H
