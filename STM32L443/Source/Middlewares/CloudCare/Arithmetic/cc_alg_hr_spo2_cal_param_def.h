/*
* Copyright(c) 2015, CloudCare Healthcare Co.,Ltd
* All rights reserved
* File Name:                cc_alg_hr_spo2_cal_param_def.h
* Descprition:              the _CC_ALG_HR_SPO2_CAL_PARAM_DEF_H.h files for translate layer
* Created Date:             2016/03/14
* Author:                   yun.zhu@ywkang.com
* Current Reversion:        V1.0
* Reversion History:
*   Rev *.*
*     Author:
*     Change Date:
*     Change List:
*/

#ifndef ARITHMETIC_CC_ALG_HR_SPO2_CAL_PARAM_DEF_H_
#define ARITHMETIC_CC_ALG_HR_SPO2_CAL_PARAM_DEF_H_

/** 
* @def CZT_N	The CZT length
* @details 		This value will be used as #if ** == **, 
* 				So, don't need a branch
*/
#define  CZT_N 						256

#define     DATA_BUF_MAX_MEA				(100)

/*  Define the Max/Min HR value     */
#define MAX_HR_VAL      (240)
#define MIN_HR_VAL      (30)

#define FREQ_PEAK_BUF_LEN			(10)
#define FREQ_PEAK_MATCH_BUF_LEN         (6)
#define PEAK_MATCH_BUF_NUM							(3) 	//MUST less or equal to FREQ_PEAK_MATCH_BUF_LEN

/**
* @def 	define the position relationship
*/
#if CZT_N == 512
	#define FREQ_PEAK_POS_UP_LIMIT 			(475)
	#define FREQ_PEAK_POS_LP_LIMIT 			(25)
	#define CZT_FREQ_OFFSET					(38)
	#define PEAK_MATCH_THRES				(12)
	#define HR_NO_SECOND_PEAK_THRE_POS      (210)
	#define 	XCORR_PEAK_MIN_DISTANCE		(12)
	#define 	XCORR_PEAK_MAX_DISTANCE		(100)
#elif CZT_N == 256
	#define FREQ_PEAK_POS_UP_LIMIT 			(250)
	#define FREQ_PEAK_POS_LP_LIMIT 			(13)
	#define CZT_FREQ_OFFSET					(19)
	#define PEAK_MATCH_THRES				(6)
	#define HR_NO_SECOND_PEAK_THRE_POS      (125)
	#define 	XCORR_PEAK_MIN_DISTANCE		(6)
	#define 	XCORR_PEAK_MAX_DISTANCE		(50)
#endif

#define HR_HI_CONF_LO					(50)
#define HR_HI_CONF_HI					(120)


#endif /* ARITHMETIC_CC_ALG_HR_SPO2_CAL_PARAM_DEF_H_ */
