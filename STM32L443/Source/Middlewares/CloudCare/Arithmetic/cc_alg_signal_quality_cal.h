/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_signal_quality_cal.h
* Descprition:              the functions to support the calculation
* Created Date:             2016/03/13
* Author:                   guoliang.li@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef __CC_ALG_SIGNAL_QUALITY
#define __CC_ALG_SIGNAL_QUALITY

#include <stdint.h>
#include <stdbool.h>
#include "arm_math.h"
#include "cc_alg_types.h"


#define SNR_ARRAY_LEN               (5)
#define SNR_GAIN                    (20)
#define BAND_WIDTH_DEFAULT          (30)
#define	BASE_PEAK_INDEX_DELTA		    (12)
#define HARMONIC_PEAK_INDEX_DELTA	  (20)
#define	UI16_MAX 					          (65535)

typedef struct{
	int32_t     m_i32P2pValueIr;
	int32_t     m_i32P2pValueRed;
	bool		    m_bIsP2PCheckPass;
	uint16_t    m_ui16XcorrRatioIr;
	uint16_t    m_ui16XcorrRatioRed;
	uint16_t    m_ui16XcorrRatioIrLsl;
	uint16_t    m_ui16XcorrRatioRedLsl;
	uint16_t 	  m_uiXcorrRatioAxes;
	bool 		    m_bIsXocrrCheckPass;
	bool 		    m_bIsMotionCheckPass;
	uint8_t		  m_uiMotionLevel;
	uint32_t 	  m_uCalcRunTimeCnt;
} CAL_SIGNAL_QUALITY_T;

typedef struct {
	float32_t SNR;
  uint16_t PeakPosRef;
  float32_t SNR_Array[SNR_ARRAY_LEN];
  uint8_t index_cur;
  uint8_t len_max;
  uint8_t SNR_Show;
  uint8_t SNR_Stability;
}st_Signal_Quality_Typedef;

/**
  * @brief  ######################
  * @param  ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE GetSignalQuality(int32_t *pCZTSpectrum,HR_SPO2_DAT_T * ResultRef,st_Signal_Quality_Typedef *pSignalQuality);

/**
  * @brief Definition of external functions
  */
FUNC_RETURN_TYPE SignalQualityUpdate(st_Signal_Quality_Typedef *pSignalQuality);


/**
  * @brief Definition of external functions
  */
FUNC_RETURN_TYPE signal_quality_cal_init(st_Signal_Quality_Typedef * SigQuality_pst);

#endif //end of _CC_ALG_SIGNAL_QUALITY
