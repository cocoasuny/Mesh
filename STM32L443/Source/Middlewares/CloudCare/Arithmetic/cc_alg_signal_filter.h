/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_signal_filter.h
* Descprition:              the cc_alg_signal_filter.h files for translate layer
* Created Date:             2016/08/08
* Author:                   yun.zhu@ywkang.com
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*
*     Author:
*     Change Date:
*     Change List:
*/

#ifndef __CC_ALG_SIGNAL_FILTER_H
#define __CC_ALG_SIGNAL_FILTER_H

#include "stdint.h"

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void cc_alg_filter_init(void);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void cc_alg_signal_filter_proc(
	int32_t * const pDatSrcIr,
	int32_t * const pDatDesIr,
	int32_t * const pDatSrcRed,
	int32_t * const pDatDesRed,
	uint8_t     preDatShift,
	uint16_t    blockSize
);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void cc_alg_filter_state_init(int32_t iDataFlushIr, int32_t iDataFlushRed);

#endif //end file
