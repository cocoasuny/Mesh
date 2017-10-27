/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_cos_cost_arith.h
* Descprition:              the cc_alg_cos_cost_arith .h files for algorithm process
* Created Date:             2016/05/08
* Author:                   yun.zhu@ywkang.com  
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*    
*     Author:     
*     Change Date:
*     Change List:
*/
#ifndef _CC_ALG_COS_COST_ARITH
#define _CC_ALG_COS_COST_ARITH

#ifdef __cplusplus
extern "C"{
#endif


#include "cc_alg_types.h"

    #define MCDST_LEN           (75)
    #define MCDST_SHIFT_LEN     (50)
    
#ifndef PI
    #define PI					(3.14159265358979f)
#endif
    #define BIT16_MAX			(32767)
    #define BIT_SHIFT_LEN		(8u)
        
    //#define MSDST_INC_STEP_FIXED      (0x51EB85)
    //#define MSDST_INC_STEP_FLOAT      (0.01)
	#define MSDST_INC_STEP_FIXED      (0xA3D70A)	/** equals to 0.02	*/
	#define MSDST_INC_STEP_FLOAT      (0.02)
	#define MSDST_START_VAL_FIXED 		(0x6666664) /** equals to 0.2	*/
	#define MSDST_START_VAL_FLOAT 		(0.2) 		
    #define MSDST_RV_MAX_VAL          (6.9)
    #define ACOS_LUT_LEN		(100)
	#define NOISE_RV_MIN				(90)
	#define NOISE_RV_MAX				(120)
	#define R_BUF_LEN			(3)
/**
*	@brief Define the constraints or defines here 
*/
#define 	CC_DST_MAX_DATA_CAL_LEN 			(512)

#define 	CC_DST_DATA_SHIFT_BITS				(8)



/**
*	@breif Define the ccdst algorithm init data structure
*/
typedef struct
{
	uint16_t 		m_ui16CalDatLen;
	uint8_t 		m_ui8NewDatLen;
} ccDstInitStrTypedef;

/**
* @typedef 
*/
typedef struct
{
	uint16_t 	m_iMinPointIndex;
	int32_t 	m_iMinCentralVal;
//	int32_t 	m_iMinLeftVal;
//	int32_t 	m_iMinRightVal;
}ccPointTypedef;


/**
* @typedef float32_t types R val structure
*/
typedef struct
{
	float32_t 	m_fRatValFir;
	bool		m_bIsRvalFirValid;
	float32_t 	m_fRatValSec;
	bool		m_bIsRvalSecValid;
} MSDST_R_VAL_T;


FUNC_RETURN_TYPE cc_alg_ccdst_init(ccDstInitStrTypedef * ccDstParam_pst,\
								   uint16_t ccDstCalDataLen,\
								   uint8_t ccDstNewDataLen\
								);
FUNC_RETURN_TYPE cc_alg_ccdst_process(ccDstInitStrTypedef * ccDstParam_pst,\
									  const int32_t * 	pi32DatBufIr,\
									  const int32_t * 	pi32DatBufRed,\
									  float32_t 		* 	pf32RvalDst\
									  );



#ifdef __cplusplus
}
#endif


#endif // end of _CC_ALG_COS_COST_ARITH
// end file
