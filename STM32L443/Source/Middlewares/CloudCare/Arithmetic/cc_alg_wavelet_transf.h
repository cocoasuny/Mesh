/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_wavelet_transf.h
* Descprition:              the functions for wavelet transform
* Created Date:             2016/03/12
* Author:                   yun.zhu@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
* Rev *.*    
* Author:     
* Change Date:
* Change List:
*/

#ifndef __CC_ALG_WAVELET_TRANS
#define __CC_ALG_WAVELET_TRANS

#include "cc_alg_types.h"

typedef enum {
  WAVELET_FLUSH  = 0,
  WAVELET_NORMAL = 1
}WaveletRunMode_en;

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE wavelet_trans_init_func(void);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE wavelet_trans_flush_func(
	int32_t i32DatForFlushIr,
	int32_t i32DatForFlushRed,
	int16_t iDatForFlushAxes,
	uint16_t ui16NumFlush
);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE wavelet_trans_process_func(
  int32_t * pi32DataInRed, \
  int32_t * pi32DataInIr, \
  int32_t * pi32DataOutRed, \
  int32_t * pi32DataOutIr, \
	int16_t * piDatInAxes,\
	int16_t * piDatOutAxes,\
  uint8_t ui8DatLen \
);

#endif //_CC_ALG_WAVELET_TRANS

//end file
