/**
* @file				cc_alg_signal_quality_module.h
* @brief 			signal quality module routines
* @date				2016/08/19
* @author			yun.zhu@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/

#ifndef _CC_ALG_SIGNAL_QUALITY_MODULE_H_
#define _CC_ALG_SIGNAL_QUALITY_MODULE_H_

#include "stdio.h"
#include "stdbool.h"
#include "cc_alg_types.h"






/**
* @brief 	definition of structure of signal quality module input parameters
*/
typedef struct{
	int32_t				* 	piSpecDataRed;				/**< 	spectrum data, red channel 					*/
	int32_t				* 	piSpecDataIr;				/**< 	spectrum data, ir channel 					*/
	uint16_t 				uSpecDataLen;				/**< 	spectrum data length 						*/
	int32_t 			*	piRawDatRed;				/**< 	raw data of red, from wavelet channel 		*/
	int32_t 			*	piRawDatIr;					/**< 	raw data of ir,  from wavelet channel 		*/
	uint16_t 				uRawIrRedDataLen;			/**< 	raw data length of ir and red channel		*/
	int16_t 			*	piRawDatAxes;				/**< 	raw data of axes,  from wavelet channel 	*/
	uint16_t 				uRawAxesDataLen;			/**< 	raw data length of axes channel				*/
	PEAK_POS_VAL_INFO_T 	*	xcorrPeakInfoRed_pst;		/**< 	xcorr peak information(position, value) in red channel 		*/
	PEAK_POS_VAL_INFO_T 	*	xcorrPeakInfoIr_pst;		/**< 	xcorr peak information(position, value) in ir channel 		*/
	PEAK_POS_VAL_INFO_T 	*	xcorrPeakInfoAxes_pst;		/**< 	xcorr peak information(position, value) in axes channel 	*/
	int32_t 			* 	piRatioPi;					/**<	PI ratio from outside 										*/
	uint16_t				uMeanHrPos;					/**<	averaged hr position, be zero if not valid 					*/
	int32_t 			* 	iMotionThreshold;			/**< 	input motion threshold for internal judgement				*/
} SIG_INPUT_INFO_T;

/**
* @brief 	definition of structure of motion evaluation module output
*/
typedef struct{
	int32_t 			iMotionLevel;					/**< 	motion level indicator				*/
	uint16_t 			uMotionTimeRatio;				/**< 	ratio of motion time in 5 seconds, integer ratio = float ratio * 100			*/
	uint16_t 			uMotionXcorrPeriodRatio;		/**< 	ratio of xcorr main peak and second peak , integer ratio = float ratio * 100	*/
} MOV_EVAL_CHAR_T;


/**
* @brief 	definition of structure of signal quality result 
*/
typedef struct{
	int32_t 				iPiRatio;					/**<	PI ratio, integer ratio = float ratio * 100				*/
	MOV_EVAL_CHAR_T		*	pMotionCharacInfo_pst;		/**< 	motion evaluation module 								*/
	uint16_t 				uRawIrXcorrPeriodRatio;		/**<	ratio to judge if this slice of data is periodical ir channel, integer ratio = float ratio * 100	*/
	uint16_t 				uRawRedXcorrPeriodRatio;	/**<	ratio to judge if this slice of data is periodical red channel, integer ratio = float ratio * 100	*/
	uint16_t 				uSnrCalcResult;				/**< 	ratio of signal SNR in red channel, which is most likely be affected by motion 		*/		
} SIG_QUALITY_RESULT_T;




#endif

