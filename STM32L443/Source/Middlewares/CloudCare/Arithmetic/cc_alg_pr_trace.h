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

#ifndef __CC_ALG_PR_TRACE_H
#define __CC_ALG_PR_TRACE_H

#include "stdint.h"
#include "stdbool.h"

typedef enum{
  CALC_MOD_NULL         = 0, 
  CALC_MOD_HR_PEAK_VALL = 1, 
  CALC_MOD_PEAK_VALL    = 2
}PR_CALC_MODE_T;

/**
  * @brief ######################
  */
typedef struct 
{
  int32_t iSampleFreq;  /* sample frequncy */
  int32_t * pIredData;  /* pointer to Ir data */
  int32_t * pRedData;   /* pointer to Red data */
  int32_t iDataLen;     /* data length */
  PR_CALC_MODE_T m_CalculateMode; /* mode 1:calculate hr,peak and valley 
                                     mode 2:only calculate peak and valley */
}PR_TRACE_INPUT_T;

/**
  * @brief ######################
  */
typedef struct 
{
  int32_t iIrHr;        /* realtime heart rate value */  
  int32_t iIrHrAve;     /* Ir four heart rate mean */
  int32_t iRedHr;       /* realtime heart rate value */
  int32_t iRedHrAve;    /* Red four heart rate mean */
  int32_t iIrPeakVal;   /* Ir peak value */
  int32_t iIrValleyVal; /* Ir valley value */    
  int32_t iRedPeakVal;  /* Red peak value */
  int32_t iRedValleyVal;/* Red valley value */ 
}PR_TRACE_OUTPUT_T;

/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
void cc_alg_pr_trace_init(void);


/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
bool cc_alg_pr_trace_calculate(PR_TRACE_INPUT_T *pInput, PR_TRACE_OUTPUT_T *pOutput);


#endif //end file
