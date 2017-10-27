/**
* @file				sq_spectrum_snr.h
* @brief 			the sq_spectrum_snr.h files for algorithm process
* @date				2015/12/22
* @author			guoliang.li@ywkang.com  
* @version 			V1.0
* @copyright 		CloudCare Healthcare Co.,Ltd,All rights reserved
*/

#ifndef _SQ_SPECTRUM_SNR_H_
#define _SQ_SPECTRUM_SNR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "cc_alg_basic_func.h"

#define     CZT_N                       (256)
#define		CZT_F_START					(0.3)
#define		CZT_F_END					(4.3)
#define		SNR_NOISE_GAIN				(1)
#define		SNR_HARMONIC_ATTEN			(2)
typedef struct {
	float32_t	f32SNR;
	uint16_t	u16SNR_Show;
    uint16_t	u16PeakPosRef;
    int32_t		*pi32SpecDataBuf;
    uint16_t	u16SpecDataLen;
	uint16_t	u16SpecPosStart;
	uint16_t	u16SpecPosDeltaBase;
	uint16_t	u16SpecPosDeltaHarmonic;
	uint16_t	u16SpecBandWidthDefault;

}SQ_SPEC_SNR_T;



/**
* @brief Definition of external functions
*/
FUNC_RETURN_TYPE sq_spec_snr_init(SQ_SPEC_SNR_T * SqCztSnr_pst);
FUNC_RETURN_TYPE sq_spec_snr_run(SQ_SPEC_SNR_T *SqCztSnr_pst);


#ifdef __cplusplus
}
#endif


#endif 

