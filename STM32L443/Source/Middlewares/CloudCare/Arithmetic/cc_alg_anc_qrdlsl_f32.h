/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_anc_qrdlsl_f32.h
* Descprition:              the cc_alg_anc_qrdlsl_f32.h files for algorithm process
* Created Date:             2016/07/11
* Author:                   guoliang.li@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/

#ifndef _CC_ALG_ANC_QRDLSL_F32_
#define _CC_ALG_ANC_QRDLSL_F32_

#ifdef __cplusplus
extern "C"{
#endif


#include "cc_alg_types.h"


#define	QRDLSL_M					(32)
#define QRDLSL_LAMBDA				(0.998)
#define QRDLSL_DELTA				(1)
#define QRDLSL_FLOAT_MIN			(0.00000000093)

//#define QRDLSL_PC_TEST


typedef struct
{
	uint8_t M;
	float32_t lambda;
	float32_t lambda_root;
	float32_t delta;

	float32_t ef_preM;    
    float32_t eb_postM;
    float32_t e;

	float32_t eb_m[QRDLSL_M];
	float32_t pfc_m[QRDLSL_M];
	float32_t pbc_m[QRDLSL_M];
	float32_t pc_m[QRDLSL_M];
	float32_t B_m[QRDLSL_M];
	float32_t F_m[QRDLSL_M];

}st_ANC_QRDLSL_F32_Typedef;

typedef struct
{
	uint8_t M;
	float32_t lambda;
	float32_t lambda_root;
	float32_t delta;

	float32_t ef_preM;    
    float32_t eb_postM;
    float32_t e_channel1;
	float32_t e_channel2;

	float32_t eb_m[QRDLSL_M];
	float32_t pfc_m[QRDLSL_M];
	float32_t pbc_m[QRDLSL_M];
	float32_t pc_m_channel1[QRDLSL_M];
	float32_t pc_m_channel2[QRDLSL_M];
	float32_t B_m[QRDLSL_M];
	float32_t F_m[QRDLSL_M];

}st_ANC_QRDLSL_DCH_F32_Typedef;

FUNC_RETURN_TYPE cc_alg_anc_qrdlsl_f32_init(st_ANC_QRDLSL_F32_Typedef *AncQrdLslF32_pst,float32_t lambda,float32_t delta);
FUNC_RETURN_TYPE cc_alg_anc_qrdlsl_f32_run(st_ANC_QRDLSL_F32_Typedef *AncQrdLslF32_pst,int32_t *pi32_DataSignal,int32_t *pi32_DataNoise,int32_t *pi32_DataOut,int32_t *pi32_DataError,uint32_t ui32_DataLen);

FUNC_RETURN_TYPE cc_alg_anc_qrdlsl_dch_f32_init(st_ANC_QRDLSL_DCH_F32_Typedef *AncQrdLslDchF32_pst,float32_t lambda,float32_t delta);
FUNC_RETURN_TYPE cc_alg_anc_qrdlsl_dch_f32_run(st_ANC_QRDLSL_DCH_F32_Typedef *AncQrdLslDchF32_pst,int32_t *pi32_DataSignal_1,int32_t *pi32_DataSignal_2,int32_t *pi32_DataNoise,int32_t *pi32_DataError_1,int32_t *pi32_DataError_2,uint32_t ui32_DataLen);
#ifdef QRDLSL_PC_TEST
void cc_alg_anc_qrdlsl_f32_test(void);
void cc_alg_anc_qrdlsl_f32_test2(void);
#endif





#ifdef __cplusplus
}
#endif


#endif
