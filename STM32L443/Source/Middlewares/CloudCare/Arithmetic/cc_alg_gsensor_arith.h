/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_gsensor_arith.h
* Descprition:              the functions to handle the gsensor data
* Created Date:             2016/03/17
* Author:                   yun.zhu@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef _CC_ALG_GSENSOR_ARITH_H
#define _CC_ALG_GSENSOR_ARITH_H

#include <stdint.h>
#include "cc_alg_types.h"


#define GSEN_BOSH250E_COMB_DAT_SRS 			(5) 	/* 	Right shift bits for combined data */

#define SENSOR_DATA_NUM_PER_PROC 		    (50)
#define SENSOR_DATA_BUF_LEN 			      (100) 		//50hz odr, 2s interval

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE cc_gSensor_dat_combine(int16_t * piSensorDatComb, uint16_t uiDatLen, uint8_t uiBitShift);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
bool cc_gSensor_PreSpO2_MoveCheck(uint16_t * pStdVal, int32_t *pStdValArr, uint8_t StdValArrOffset, uint32_t uiMotionCheckThres);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
bool cc_gSensor_move_check(int16_t * sensorDatBufC, uint16_t dataLen, uint32_t moveThreshold, uint16_t * pValStd);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void cc_gSensor_data_buf_get(int16_t **piDataX,int16_t **piDataY,int16_t **piDataZ);
/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void cc_alg_gSensor_dat_combine(int16_t * sensorDatBufC, uint16_t uDatLen);

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void cc_gSensor_data_copy_to_buf(void);

#endif //_CC_ALG_GSENSOR_ARITH_H
