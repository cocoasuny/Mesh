/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_anc_lms.h
* Descprition:              the cc_alg_anc_lms.h files for algorithm process
* Created Date:             2016/06/15
* Author:                   guoliang.li@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef _CC_ALG_ANC_LMS_Q31_
#define _CC_ALG_ANC_LMS_Q31_

#ifdef __cplusplus
extern "C"{
#endif

#include "cc_alg_types.h"


#define LMS_ORDER					(64)
#define LMS_STEP_SIZE				(1)

#define LMS_WEIGHT_DECIMAL_BIT			(16)
#define LMS_MU_BIT						(36)


#define LMS_NOISE_GAIN				(100)


typedef struct
{
	uint8_t m_ui8Order;
	int32_t pi32_Weight[LMS_ORDER];
	int32_t m_ui32StepSize;
	int32_t pi32_DataBuf[LMS_ORDER];

}st_ANC_LMS_Q31_Typedef;



FUNC_RETURN_TYPE cc_alg_anc_lms_q31_init(st_ANC_LMS_Q31_Typedef *pst_ANC_LMS_q31);
FUNC_RETURN_TYPE cc_alg_anc_lms_q31_run(st_ANC_LMS_Q31_Typedef *pst_ANC_LMS_q31,int32_t *pDataSignal,int32_t *pDataNoise,int32_t *pDataOut,int32_t *pDataError,uint32_t len);



#ifdef __cplusplus
}
#endif


#endif
