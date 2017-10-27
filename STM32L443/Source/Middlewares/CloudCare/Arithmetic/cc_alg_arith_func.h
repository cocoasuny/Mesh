/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_arith_func.h
* Descprition:              the cc_alg_arith_func .h files for algorithm process
* Created Date:             2016/03/15
* Author:                   yun.zhu@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef __CC_ALG_ARITH_FUNC
#define __CC_ALG_ARITH_FUNC

#include "cc_alg_types.h"
#include "arm_math.h"

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE czt_pre_process(
           int32_t * pi32DataBuf,
					 int32_t * pi32FFTRunBuf_arr,
           uint16_t    ui16CztDatLen,
					 const int16_t * pCztAWPar, 
					 int16_t * pScaleFactor
                                );
          
/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE xcorr_cal_func(
           int32_t * pi32DatIn, 
           int32_t * pFFTBuff,
           uint16_t  ui16CztDatLen,
					 const arm_cfft_instance_q31 * cfftInst_pst,
           int16_t *scaleFactor
                                );

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */                
FUNC_RETURN_TYPE XCOR_quality_check(PEAK_POS_VAL_INFO_T * pPeakBuf, uint16_t * pui16SignalRatio, uint16_t uMinDis, uint16_t uMaxDis);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */                
FUNC_RETURN_TYPE cc_alg_diff_motion_cancel(
           int32_t * pDatRed, 
           int32_t * pDatIr, 
           int32_t * pDatMid, 
           int32_t * pDatDes, 
           int32_t   i32DiffRatioStart,
           int32_t   i32DiffRatioStep,
           int32_t   i32DiffRatioLen,
           int32_t   i32DiffRatioRSR,
           uint16_t  ui16DataBufLen,
           bool * pFlagMinFind
                                          );

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE CZT_Anti_LF_Motion(
           int32_t * pBufDatIn, 
           uint16_t  blockSize, 
           uint16_t  ui16AntiLowFreqScope,
           uint16_t  ui16AntiLowFreqLimit,
           uint16_t * LfreqPnt
                                   );       
/**
  * @brief 		  find the peak val and position in a given data buffer 
  * @param[in]	pDatIn	data input, q31 format
  * @param[in]	uSearchStartPos 	the search start position, must greater than 1
  * @param[in]	uSearchEndPos 	the search end position, must less than the buffer length
  * @param[out]	peakInf_pst	peak information output
  * @retval 		FUNC_RETURN_TYPE
  */
FUNC_RETURN_TYPE cc_alg_peak_search(
          int32_t 		* 	pDatIn,
          uint16_t 			uDatLen,
          uint16_t 		 	uSearchStartPos,
          uint16_t 		 	uSearchEndPos,
          PEAK_POS_VAL_INFO_T 	* peakInf_pst
);
                        
#endif //end _CC_ALG_ARITH_FUNC
