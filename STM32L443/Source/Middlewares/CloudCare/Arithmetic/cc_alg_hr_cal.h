/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_hr_cal.h
* Descprition:              the cc_alg_hr_cal.h files for translate layer
* Created Date:             2016/03/14
* Author:                   yun.zhu@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef __CC_ALG_HR_CAL_H
#define __CC_ALG_HR_CAL_H

#include <stdint.h>
#include <stdbool.h>
#include "arm_math.h"
#include "cc_alg_types.h"
#include "arrayOperaFunction.h"
#include "cc_alg_basic_func.h"
#include "cc_alg_signal_quality_cal.h"


#define HR_BASELINE_TIMESPAN		      (120)
#define HR_BASELINE_AVE_FACTOR_SLOW		(32)
#define HR_BASELINE_AVE_FACTOR_FAST		(16)
#define HR_BASELINE_PERIOD_DIFF_MAX 	(150)

#define HR_REALTIME_AVE_FACTOR_MID    (10)
#define HR_REALTIME_AVE_FACTOR_SLOW		(16)
#define HR_REALTIME_AVE_FACTOR_FAST		(2)

#define HR_BASELINE_BUF_LEN			      (10)
#define HR_REALTIME_BUF_LEN			      (2)

#define HR_MAX_FREQ_POS               (CZT_N - 1)


/**
  * @brief The declaration of HR long time reserve structure
  */
typedef struct{
	st_Arr_ui16_Typedef * m_pArrHrPos;
	uint16_t 		m_ui16HrPosBaseline;
	float32_t 		m_f32HrPosBaseline;
	uint16_t 		m_ui16HrBaselineTimespan;
	bool 			m_bIsHrBaselineStable;
	uint8_t 		m_ui8HrBaselineAveFactorFast;
	uint8_t 		m_ui8HrBaselineAveFactorSlow;
	uint16_t 		m_ui16HrBaselineCnt;
	uint8_t 		m_ui8LowConfCnt;
	uint8_t 		m_ui8LowConfCntMax;
	uint16_t 		m_ui16HrChangeSpeedMax;
} st_HrPosBaseline_Typedef;

/**
  * @brief The declaration of Hr position matched structure
  */
typedef struct{
	uint16_t * m_pArrMatchedPos;
	bool			 m_bIsBothMainPeak;
  bool       m_bIsPerfectSignalQuality;
	bool * 		 m_pArrIsBothHighReliable;
} st_HrPosMatch_Typedef;

/**
 * @brief The declaration of HR realtime structure
 */
typedef struct{
	st_Arr_ui16_Typedef * m_pArrHrPos;
	uint16_t 		m_ui16HrRealTime;
	float32_t 	m_f32HrPosRealTime;
	uint16_t 		m_ui16HrRealTimeQuick;
	uint8_t 		m_ui8HrRealTimeFactorFast;
	uint8_t 		m_ui8HrRealTimeFactorSlow;
  uint8_t     m_ui8HrRealTimeFactorMid;
	uint16_t 		m_ui16HrChangeSpeedMax;
  bool        m_bIsRealTimeHrReady;
  bool 			  m_bIsRealTimeHrQuickReady;
  bool 			  m_bIsCurrentRealTimeHrReliable;
  bool        m_bIsCurrentRealTimeHrFullyTrust;
} st_HrPosRealTime_Typedef;

/**
  * @brief ######################
  */  
typedef struct{
    int32_t     m_i32PeakVal[FREQ_PEAK_MATCH_BUF_LEN];
    uint16_t    m_ui16PeakPos[FREQ_PEAK_MATCH_BUF_LEN];
    bool        m_bIsHighConfPeak[FREQ_PEAK_MATCH_BUF_LEN];
    uint8_t     m_ui8PeakLen;
    bool 		    peakTrustFlag;
    bool        m_bIsMainSecondMatch;
}Freq_Peak_Pos_Typedef;


/**
  * @brief define the HR calculate parameters
  */
typedef struct{
	bool 		bIsEnableHrWeight;			/*		Enable or Disable the weighted HR calculation		*/
}	HR_CAL_CTRL_PARAM_T;

/** 
  * @brief define the HR cal input paramter structure
  */
typedef struct{
	PEAK_POS_VAL_INFO_T			* irChWaltPeakInf_pst;	/*		ir channel peak information, from wavelet channel 	*/
	PEAK_POS_VAL_INFO_T			* irChAdpPeakInf_pst;		/*		ir channel peak information, from adaptive channel 	*/
	PEAK_POS_VAL_INFO_T			* movPeakInf_pst;			  /*		motion channel peak information 					*/
	CAL_SIGNAL_QUALITY_T		* signalQuality_pst;		/*		information about current signal quality			*/
	HR_CAL_CTRL_PARAM_T 		* hrCalCtrlParam_pst;		/*		hr calculation control infor input, reserved for further 		*/
} HR_CAL_INFO_T;

/**
  * @brief Function export
  */
void cc_alg_weighted_hr_cal(
	PEAK_POS_VAL_INFO_T * 		pPeakBufIr,
	PEAK_POS_VAL_INFO_T * 		pPeakBufRed,
	PEAK_POS_VAL_INFO_T * 		pPeakBufAxes,
	HR_SPO2_DAT_T * 			    pResultOut,
	CAL_SIGNAL_QUALITY_T *		pSignalCheck_pst,
	st_HrPosBaseline_Typedef *		HrPosBaseline_pst,
	st_HrPosRealTime_Typedef * 		HrPosRealTime_pst,
  uint8_t                       bIsEnableHrWeight,
	uint8_t 						uCurTzPrVal
);


/**
  * @brief Function export
  */
FUNC_RETURN_TYPE get_hr_from_freq_pos(uint16_t ui16FreqPos, uint8_t * pui8HrVal);

/**
  * @brief Function export
  */
FUNC_RETURN_TYPE get_freq_pos_from_hr(uint8_t ui8HrVal, uint16_t * pui16FreqPos);
                    
#endif // _CC_ALG_HR_CAL_H

