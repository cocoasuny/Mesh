//******************************************************************************
//  Claude ZHU
//  Hardware Team
//  (C) iCareTech Inc., 2013
//  All Rights Reserved.
//  Built with Keil MDK
//
//-------------------------------------------------------------------------------

#ifndef __SPO2HRCALCZT_H
#define __SPO2HRCALCZT_H

#include <stdbool.h>
#include "arm_math.h"
#include "cc_alg_types.h"

//#include "arm_const_structs.h"
//#include "arrayOperaFunction.h"
//#include "cc_alg_hr_cal.h"

/**
  * @brief ######################
  */  
#define SPO2_RVAL_AVE_BUF_LEN               (4)

/**
  * @brief ######################
  */  
#define SENSOR_DATA_BUF_CAL_LEN_ANTI_MOV    (10)

/**
  * @brief Define the buffer slot for "anti motion" function
  */
#define ANTI_MOTION_BUF_SLOT    (2)

/**
  * @brief ######################
  */  
#define DATA_BUF_MAX_MEA				(100)

/**
  * @brief ######################
  */  
#define IIR_Q29_SHIFT_BITS 			(0)

/**
  * @brief ######################
  */
#define P2P_VALUE_THRES		      (1000)

/**
  * @brief ######################
  */
#define P2P_VALUE_SMALL_CNT 	  (10)

/**
  * @brief Define the SpO2 Signal Confidence
  */
#define SPO2_SIG_CONF_FULL_TRUST  (3)
#define SPO2_SIG_CONF_HIGH        (2)
#define SPO2_SIG_CONF_MID         (1)
#define SPO2_SIG_CONF_LOW         (0)

/**
  * @brief ######################
  */
#define Motion_Buffer_For_Kalman_Len      (4)
#define SENSOR_DATA_STD_VAL_MOTION_DIV    (2048) 

/**
  * @brief ######################
  */
#define SPO2_HIGH_CONF_THRES 			(95)

/**
  * @brief ######################
  */
#define DAT_ANTI_ABNORMAL_CNT_THRE 		(3)

/**
  * @brief ######################
  */
#define RUN_TIMEOUT_CNT_THRE			(30)

/**
  * @brief ######################
  */
#define SPO2_HI_CONF_LO					  (94)

/**
  * @brief ######################
  */
#define LF_ANTI_MOTION_SCOPE			(64)
#define LF_ANTI_MOTION_LIMIT			(FREQ_PEAK_POS_LP_LIMIT - 1)

/**
  * @brief Macro for Differential Channel
  *        Span: 0.1 ~ 2.1, Step: 0.02
  *        Format: Q31
  *        Right Shift : 2bit
  */
#define DIF_CH_RATIO_STEP 				(10485760) 			/* 		(2^(-6) + 2^(-8)) x 2^(29), 2^(-6)+2^(-8) = 0.019 		*/
#define DIF_CH_RATIO_START 				(53687091) 			/* 	0.1*2^29	*/
#define DIF_CH_RATIO_RSR				  (2) 					  /* 		Right shift 2 bits to Q31 in all span 					*/
#define DIF_CH_RATIO_LEN 				  (100)

/**
* @typedef 		SPO2_HR_CAL_STATUS_T 
* @details 		Report measure module's running status, the value updates continously, NOT Latch
*/
typedef struct{
	bool 	bIsMotionCheckPass;			  /** -# true motion detected; -# false no motion detected			*/
	bool 	bIsSmallSignalDetected;		/** -# true small signal detected; -# false signal is normal 	*/
	bool 	bIsCalTimeOut;				    /** -# true calculation timeout; -# false no error occurs			*/
} SPO2_HR_CAL_STATUS_T;

/**
* @typedef 		SPO2_HR_CAL_MODE_T 
* @details 		Control the mode of SPO2_HR calculation process
*/
typedef struct{
	bool 	bIsMotionReportOnly;			/**< Only report motion level 			*/
	bool 	bIsFirstCalc;						  /**< The first calculation after led adjust 	*/
} SPO2_HR_CAL_MODE_T;

/**
  * @brief the structure of spo2_CZT_cal_body
  */
typedef struct{
  ccAlgSpO2HrParam_st *       m_ccAlgParamIn_pst; /*  pointer to the input parameters         */
  sampleDataRaw_Typedef *     sampleDataRaw_pst;  /*  Pointer to the new data input buffer    */
    
} CC_ALG_CZT_CALC_INIT_T;

/**
  * @brief ######################
  */
typedef struct{
	int32_t	i32RedDat[ANTI_MOTION_BUF_SLOT*DATA_BUF_MAX_MEA];				
	int32_t	i32IrDat[ANTI_MOTION_BUF_SLOT*DATA_BUF_MAX_MEA];		
}SPO2_RAW_DATA_BUF_T;

/**
  * @brief Typedef for R value output to filter module
  */
typedef struct{
    uint16_t        m_ui16Rval;
    uint16_t        m_ui16Pos2CalR;         /*  the peak position for this R value  */
    uint8_t         m_ui8RvalSigConf;
    uint16_t *      m_pui16RvalFilter;
} SPO2_R_VAL_PARAM_T;

/**
  * @brief typedef @SpO2ProcessParam
  */
typedef struct{
	uint8_t 			u8MoniRestartNum; 		      /* 	restart number if spo2 value below the threshold 	*/
	bool 					flagHRWeightedCalEn;	      /* 	Enable, or disable HR weighted calculation 			*/
	bool          m_bIsSpO2FullRangeSupport;	/* 	support or not support full range spo2 			*/
  bool          m_bIsSpO2CompensateEnable;  /*  enable or disable SpO2 compensate               */
	bool					flagResultKalmanFilterEn;	  /*	Enable or disable HR/SPO2 Result output kalman filter 	*/
//bool					flagSignalAmpCompensateEn;	/* 	Enable or disable amplitude compensation design 	*/
  uint16_t      thresIrXcorrQuality;        /*  Threshold of ir channel xcorr quality judgement     */
  uint16_t      thresRedXcorrQuality;       /*  Threshold of red channel xcorr quality judgement     */
	bool          flagIsFactoryTestMode;      /*  factory test mode, if true, the data will be push out in any condition */
	int32_t 			m_iAfeDcOffset;			        /* dc offset of AFE 		*/
	uint32_t 			m_uiMotionCheckThres;		    /*	threshold for motion check 	*/
	uint32_t 			m_uiClearWhileMotionThres;	/*	threshold for clear-while-motion function	*/
} HR_SPO2_PROC_PARAM_T;

/**
  * @brief kalman filter
  */
typedef struct{
	float32_t 		fPn;
	float32_t 		fPnFoward;
	float32_t 		fKn;
	float32_t 		Rww;
	float32_t 		valPredict;
  uint8_t       valNewDat;
} kalman_dat_str_typedef;

/**
  * @brief ######################
  */
//extern PulseOxi_SampleDatBuf_Typedef s_DataSample_st;
extern SPO2_RAW_DATA_BUF_T s_DataSampleCopy_st;


/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void CZT_Process_Init(void);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void SpO2HrCalPreDataProcess(void);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void SpO2HrCalCZTFilterFlush(void);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE SpO2HrCal_Entry(
	SPO2_HR_CAL_MODE_T 		* pSpO2HrCalMode, 
	SPO2_HR_CAL_STATUS_T 	* pSpO2HrCalStatus,
	HR_SPO2_DAT_T 			* pSpO2HrReport
);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void SpO2HrCal_Init(HR_SPO2_INIT_PARA_Typedef * HrSpO2CalInitPara_pst);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void SpO2HrCal_DeInit(void);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
//FUNC_RETURN_TYPE cc_alg_czt_cal_inst_init(CC_ALG_CZT_CALC_INIT_T *ccAlgCztInstInit_pst);

//#define DATA_BUF_PROCESS				(50)						//Internal process buffer length
//#define PEAK_WEIGHT_NUMBER				(5)
//#define PEAK_2ND_UP_LIMIT				(220)	
//#define PEAK_2ND_SEARCH_OFFSET			(6)	
//#define PEAK_SEARCH_NUM					(3)
//#define PREDICT_BUF_LEN 				(12)
//#define DAT_PRE_PREDICT_BUF_LEN 		(6)
//#define DAT_STABLE_CNT_THRE				(10)
//#define DAT_DIS_STABLE_THRE 			(1)
//#define DAT_LOW_CONFI_BUF_NUM 				DAT_ANTI_ABNORMAL_CNT_THRE
//#define SpO2_RATIO_STEP 		(21474836) 		/* 0.019999 	*/
//#define SpO2_RATIO_LEN 		(100) 		
//#define SpO2_RATIO_START		(107374182) 	/* 0.0999<<30 	*/
//#define SpO2_RATIO_PEAK_BUF_LEN 		(25)
//#define SpO2_RATIO_BUF_LEN 		(30)
//#define SpO2_RATIO_BANDSTOP_LEN 	(25)
//#define SpO2_RATIO_BANDSTOP_LEN_Half (12) 
//#define SpO2_RATIO_BANDSTOP_NUM (5)
//#define GSEN_RATIO_BANDSTOP_LEN 	(51)
//#define GSEN_RATIO_BANDSTOP_LEN_Half 	(25)
//#define HR_VAL_OUT_BUF_LEN 		(6)			/* 	Must less than PREDICT_BUF_LEN */
//#define SPO2_VAL_OUT_BUF_LEN 	(6)		/* 	Must less than PREDICT_BUF_LEN */
//#define HR_SPO2_VAL_OUTPUT_AVE_LEN_SINGLE 		(4)		/* 	Must less than MIN(SPO2_VAL_OUT_BUF_LEN,HR_VAL_OUT_BUF_LEN) */
//#define HR_SPO2_VAL_OUTPUT_AVE_LEN_CONST 		(4)		/* 	Must less than MIN(SPO2_VAL_OUT_BUF_LEN,HR_VAL_OUT_BUF_LEN) */
//#define HR_SPO2_VAL_OUTPUT_AVE_LEN_MONITOR 		(4)		/* 	Must less than MIN(SPO2_VAL_OUT_BUF_LEN,HR_VAL_OUT_BUF_LEN) */
//#define SpO2_PRE_FIXED_VALUE        (96)

//typedef struct{
//	uint8_t 		datBuf[PREDICT_BUF_LEN];
//	uint8_t			bufLen;
//	uint8_t 		bufLenMax;
//} result_datBuf_Typedef;

#endif //end __SPO2HRCALCZT_H
