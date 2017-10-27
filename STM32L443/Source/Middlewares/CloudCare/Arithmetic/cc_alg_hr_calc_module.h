/**
* @file				cc_alg_hr_calc_module.h
* @brief 			hr calcualtion module routines
* @date				2016/08/15
* @author			yun.zhu@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/

#ifndef __CC_ALG_HR_CALC_MODULE_H
#define __CC_ALG_HR_CALC_MODULE_H

#include <stdint.h>
#include <stdbool.h>
#include "cc_alg_types.h"

/**
* @brief 	definition of structure of time zone information
*/
#define MIN_FREQUENCY       (0.3f)
#define MAX_FREQUENCY       (4.0f)
#define HR_DIFF_THRESHOLD   (10)
#define DEFAULT_HR          (68)

/**
* @brief 	definition of structure of time zone information
*/
typedef struct{
	uint16_t 			uHrValRefTz;					/**<	hr value in time zone						*/
	bool 				  bIsTzHrValid;					/**< 	flag to indicate the time zone hr value 	*/
} HR_CALC_TIME_ZONE_INFO_T;

/**
  * @brief 	signal quality information input structure
  */
typedef struct{
	uint16_t    m_uXcorrRatioIr;					/**< 	Ir channel xcorr ration, = round(ratio x 100)				*/
	uint16_t    m_uXcorrRatioIrLsl;				/**< 	LSL channel xcorr ration, = round(ratio x 100)				*/
	uint16_t 	  m_uiXcorrRatioAxes;				/**< 	Gsensor channel xcorr ration, = round(ratio x 100)			*/
	uint16_t	  m_uiMotionLevel;					/**< 	motion level												*/
	uint32_t 	  m_uCalcRunTimeCnt;				/**<	calculation counter, maybe helpful for first several input 	*/
} HR_CAL_SIGNAL_QUALITY_INFO_T;

/**
  * @brief 	definition of HR result information
  */
typedef struct{
	uint16_t 			uCurrHrPos;					/**<	current Hr position in spectrum					*/
	uint16_t 			uCurrHrVal;					/**< 	current Hr value, 30~240						*/
	bool 				  bIsCurrHrValid;			/**< 	flag to indicate current hr value 				*/
	uint16_t 			uMeanHrPos;					/**<	averaged Hr position in spectrum				*/
	uint16_t 			uMeanHrVal;					/**< 	averaged Hr value, 30~240						*/
	bool 				  bIsMeanHrValid;			/**< 	flag to indicate averaged hr value 				*/
} HR_CALC_RESULT_INFO_T;

/**
  * @brief 		  Init the hr calculation module if needed 
  * @param[in]	None
  * @param[out]	None
  * @retval 		None
  */
void cc_alg_hr_calc_module_init(void);


/**
  * @brief 		  hr calcuation process routine
  * @param[in]	hrSpecInfoIr_pst 	PEAK_POS_VAL_INFO_T		Ir channel specturm input
  * @param[in]	hrSpecInfoIrLsl_pst PEAK_POS_VAL_INFO_T		Ir LSL channel spectrum input
  * @param[in]	hrSpecInfoAxes_pst 	PEAK_POS_VAL_INFO_T		gSensor channel spectrum input
  * @param[in]	hrTzInfo_pst 		HR_CALC_TIME_ZONE_INFO_T	time zone hr information input
  * @param[in]	signalQuality_pst 	HR_CAL_SIGNAL_QUALITY_INFO_T	signal quality information input
  * @param[out]	hrCalcOut_pst		HR_CALC_RESULT_INFO_T		hr calcuation output
  * @retval 		None
  */
void cc_alg_hr_calc_module_proc(
	PEAK_POS_VAL_INFO_T 		* hrSpecInfoIr_pst,
	PEAK_POS_VAL_INFO_T 		* hrSpecInfoIrLsl_pst,
	PEAK_POS_VAL_INFO_T 		* hrSpecInfoAxes_pst,
	HR_CALC_TIME_ZONE_INFO_T 		* hrTzInfo_pst,
	HR_CAL_SIGNAL_QUALITY_INFO_T 	* signalQuality_pst,
	HR_CALC_RESULT_INFO_T			* hrCalcOut_pst
);


#endif // end file


