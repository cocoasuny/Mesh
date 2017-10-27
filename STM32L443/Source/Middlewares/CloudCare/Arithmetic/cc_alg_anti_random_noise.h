/**
* @file				cc_alg_anti_random_noise.h
* @brief 			cc_alg_anti_random_noise routines
* @date				2016/08/10
* @author			yun.zhu@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/

#ifndef __CC_ALG_ANTI_RANDOM_NOISE_H
#define __CC_ALG_ANTI_RANDOM_NOISE_H

#include "stdint.h"

/**
  * @brief 		cc_alg_anti_random_noise_init
  * @details 		init the internal static varibles, need call this function before use the module
  * @param[in]	none
  * @param[out]	none
  * @retval 		none
  */
void cc_alg_anti_random_noise_init(void);

/**
  * @brief 		cc_alg_anti_random_noise_proc
  * @details 		
  *           -#	calc the mean value of input peak/valley data
  *	          -# 	calc the mean value of peak-peak value
  *	          -# 	suppress the pulse which bigger than threshold of peak/valley
  *           -# 	output the suppressed data
  * @param[in]	  piDataIr			data input, ir channel
  * @param[in]	  piDataRed			data input, red channel
  * @param[in]	  iCurrPeakValIr		current peak value, ir channel
  * @param[in]	  iCurrPeakValRed		current peak value, red channel
  * @param[in]	  iCurrValleyValIr	current valley value, ir channel
  * @param[in]	  iCurrValleyValRed	current valley value, red channel
  * @param[inout]	iPulsePpValIr		averaged pulse p2p value, ir channel
  * @param[inout]	iPulsePpValRed		averaged pulse p2p value, red channel
  * @retval 		  none
  */
void cc_alg_anti_random_noise_proc(
	int32_t * 	piDataIr,
	int32_t * 	piDataRed,
	int32_t 		iCurrPeakValIr,
	int32_t 		iCurrPeakValRed,
	int32_t 		iCurrValleyValIr,
	int32_t 		iCurrValleyValRed,
	int32_t * 	iPulsePpValIr,
	int32_t * 	iPulsePpValRed
);

#endif //endfile
