/**
* @file				cc_alg_spo2_calc.h
* @brief 			cc_alg_spo2_calc routines
* @date				2016/08/11
* @author			yun.zhu@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/
#ifndef __CC_ALG_SPO2_CALC_H
#define __CC_ALG_SPO2_CALC_H

#include <stdio.h>
#include <stdbool.h>
#include "arm_math.h"
#include "cc_alg_types.h"

/**
* @typedef time zone spo2 calc input structure
*/
typedef struct{
	int32_t 	m_iPulsePpValIr;
	int32_t 	m_iPulsePpValRed;
} TZ_SPO2_CALC_IN_T;

/**
* @typedef time zone spo2 calc output structure
*/
typedef struct{
	float32_t 	m_fRvalTimeZone;
	bool 		m_bIsTzSpO2Valid;
} TZ_SPO2_CALC_OUT_T;


/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE cc_alg_time_zone_spo2_calc(TZ_SPO2_CALC_IN_T * tzSpO2CalcIn_pst, TZ_SPO2_CALC_OUT_T * tzSpO2CalcOut_pst);

#endif

