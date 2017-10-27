/**
* @file				cc_alg_xcorr_czt_transf.h
* @brief 			xcorr and czt transfer routines
* @date				2016/08/15
* @author			yun.zhu@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/

#ifndef __CC_ALG_XCORR_CZT_TRANSF_H
#define __CC_ALG_XCORR_CZT_TRANSF_H

#include "stdio.h"
#include "stdbool.h"
#include "cc_alg_types.h"

/**
* @brief 		init the xcorr and czt instance 
* @param[in]	none
* @retval		none
*/
void cc_alg_xcorr_czt_transf_init(void);


/**
  * @brief 		  run the instance to process
  * @param[in]	uDatLen 	loaded data length
  * @param[out]	piScaleFactor 	pointer to the scale factor during the process
  * @param[out]	xcorrPeakInf_pst 	pointer to the ratio of xcorr peak buffer
  * @param[out]	piDatSpectrum	pointer to the internal frequency spectrum data
  * @param[out]	PEAK_POS_VAL_INFO_T  pPeakBuf
  * @retval 		FUNC_RETURN_TYPE
  */
FUNC_RETURN_TYPE cc_alg_xcorr_czt_transf_proc(
	uint16_t 					uDatLen,
	int16_t 				* 	piScaleFactor,
	PEAK_POS_VAL_INFO_T 	* 	xcorrPeakInf_pst,
	PEAK_POS_VAL_INFO_T 	* 	peakBuf_pst,
	int32_t 				** 	piDatSpectrum
	);
  
/**
  * @brief 		load data into buffer, int16_t format
  * @param[in]	piDatSrc	 pointer to the data source
  * @param[out]	piDatSpectrum
  * 	-# 0 --- no down sample
  * 	-# 1 --- 1/2 sample
  * 	-# 2 --- 1/4 sample
  * @retval		none
  */
void cc_alg_xcorr_czt_load_q15(int16_t * piDatSrc, uint16_t uDatLen, uint8_t uDownSampleCoef, uint8_t uLeftShiftBit);

/**
  * @brief 		load data into buffer, int32 format
  * @param[in]	piDatSrc	 pointer to the data source
  * @param[out]	piDatSpectrum
  * 	-# 0 --- no down sample
  * 	-# 1 --- 1/2 sample
  * 	-# 2 --- 1/4 sample
  * @retval		none
  */
void cc_alg_xcorr_czt_load_q31(int32_t * piDatSrc, uint16_t uDatLen, uint8_t uDownSampleCoef, uint8_t uLeftShiftBit);


#endif //end file
