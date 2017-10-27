/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg.h
* Descprition:              the top .h files for algorithm process
* Created Date:             2016/03/11
* Author:                   yun.zhu@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef __CC_ALG
#define __CC_ALG

#include "cc_alg_types.h"


/**
  * @brief Define the system debug information print function
  *        should be the same format and usage as C standard "printf" function
  * @note  Need to change this for difference implementation
  */
#define     ALG_PRINTF      printf

/**
  * @brief Define the algorithm version number length
  */
#define     ALG_VER_NUM_LEN     (3)

/**
  * @brief function pointer that needs to be appointed at top application layer
  */
typedef struct{
  CC_ALG_SYS_EVENT_SEND_PRT;      //pointer to the system event distributuion function
  CC_ALG_SYS_CRITICAL_RESOURCE_APPLY_PRT;     //pointer to the system critical resource apply
  CC_ALG_SYS_CRITICAL_RESOURCE_RELEASE_PRT;
  CC_ALG_SYS_DELAY_MS_PRT;                    //system delay function
  ccAlgSpO2HrParam_st *       m_ccAlgParamIn_pst; /*  pointer to the input parameters     */ 
// HR_SPO2_DAT_T *       m_HrSpO2DatOutput_pst;  /*  pointer to the output data          */

} ccAlgBody_st;



FUNC_RETURN_TYPE cc_alg_inst_init(ccAlgBody_st * ccAlgBodyInit_pst);
void cc_alg_measure_start(HR_SPO2_INIT_PARA_Typedef * HrSpO2CalInitPara_pst);
void cc_alg_measure_stop(void);
void cc_alg_SpO2_Stat_Set(ccAlgSpO2HrStatus_en newState);
ccAlgSpO2HrStatus_en cc_alg_SpO2_Stat_Get(void);
void cc_alg_get_ver_num(const uint8_t ** pui8VerNumSer, uint8_t * pui8VerNumLen);
FUNC_RETURN_TYPE cc_alg_clr_spo2_hr_result_st(void);

void cc_alg_cal_step_set(ALG_RUN_STEP_T newState);
ALG_RUN_STEP_T cc_alg_cal_step_get(void);


HR_SPO2_DAT_T * cc_alg_get_spo2_hr_result_str(void);

#endif  //end of _CC_ALG
