/**
* @file				cc_alg_anc_mqrdlsl_f32.h
* @brief 			the cc_alg_anc_mqrdlsl_f32.h files for algorithm process
* @date				2016/07/18
* @author			guoliang.li@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/

#ifndef __CC_ALG_ANC_MQRDLSL_F32
#define __CC_ALG_ANC_MQRDLSL_F32

#ifdef __cplusplus
extern "C"{
#endif


#include "arm_math.h" 
#include "cc_alg_types.h"

/**
  * @brief ######################
  */
#define MQRDLSL_M					              (32)
#define MQRDLSL_DELTA				            (1)
#define MQRDLSL_LAMBDA			            (0.999f)
#define MQRDLSL_FLOAT_MIN		            (1e-10f)
  
/**
  * @brief ######################
  */  
#define LSL_DATA_BUF_LEN 				        (50)
#define LSL_PROC_DATA_DOWNSAMPLE_RATE		(1)
#define LSL_CZT_DATA_DOWNSAMPLE_RATE		(1)


typedef struct {
	uint8_t M;
	float32_t lambda;	              /*	the value of the forgetting factor	*/
	float32_t delta;	              /*	the value for the prediction matrices	*/
	float32_t gamma;	              /*	modified conversion factor	*/

	float32_t eb;		                /*	modified angle-normalized backward prediction error	*/
	float32_t eb_arr[MQRDLSL_M];
	float32_t ef;		                /*	modified angle-normalized forward prediction error	*/
	float32_t ef_nextM;
	float32_t err_ch1;	            /*	modified angle-normalized joint-process estimation error of channel I	*/
	float32_t err_ch2;	            /*	modified angle-normalized joint-process estimation error of channel II	*/

	
	float32_t Betaf_nextM;	        /*	new parameters used for forward square-root freeGivens rotations	*/
	float32_t Betaf_arr[MQRDLSL_M];
	float32_t Betab;	              /*	new parameters used for backward square-root freeGivens rotations	*/

	float32_t Fswsqr_arr[MQRDLSL_M];/*	sum of weighted forward prediction error squares	*/
	float32_t Bswsqr_arr[MQRDLSL_M];/*	sum of weighted backward prediction error squares	*/

	/*	modified joint-process auxiliary parameter	*/
	float32_t Pfc_arr[MQRDLSL_M];
	float32_t Pbc_arr[MQRDLSL_M];
	float32_t Pc_ch1_arr[MQRDLSL_M];
	float32_t Pc_ch2_arr[MQRDLSL_M];

}ANC_MQRDLSL_INST_T;


/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE cc_alg_anc_mqrdlsl_f32_init(ANC_MQRDLSL_INST_T *AncMQrdLslF32_pst);


/**
  * @brief ######################
  * @param ######################
  * @retval
  * @todo
  */
FUNC_RETURN_TYPE cc_alg_anc_mqrdlsl_f32_run(ANC_MQRDLSL_INST_T *AncMQrdLslF32_pst,
  int32_t *pi32_DataSignal1,
  int32_t *pi32_DataSignal2,
  int16_t *pi16_DataNoise,
  int32_t *pi32_DataError1,
  int32_t *pi32_DataError2,
  uint32_t ui32_DataLen
);

#ifdef __cplusplus
}
#endif


#endif


