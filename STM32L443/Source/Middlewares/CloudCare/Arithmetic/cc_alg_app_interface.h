/**
* @brief	Interface between APP and algorithm
* @date 	2016/03/12
* @author 	yun.zhu@ywkang.com  
* @rev      V1.0
*/

#ifndef __CC_ALG_APP_INTERFACE
#define __CC_ALG_APP_INTERFACE

#include "stdint.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "SpO2HrCalCZT.h" // @todo: combined with cc_alg_types.h
#include "cc_alg_types.h"
#include "cc_alg_gsensor_arith.h"
#include "main.h"
#include "afe4403.h"


/* Event define */
typedef enum
{
  SpO2EventMeasureInit   = 1,
  SpO2EventAdjust        = 2,
  SpO2EventMeasureCal    = 3,
}SpO2Event;

typedef enum
{
  CollectionStart     = 1,
  CollectionRunning   = 2,
  CollectionStop      = 3,
}CollectionEvent;


/*    Motion Detect Counter Up Limit */
#define MOTION_DETECT_LIMIT 		(30)  

/**
* @def 		RING OR REDHARE PLATFORM	
*/
#define 	APP_IF_WITH_KEY_OLED			false

/**
* @def		The spot check cycle number for normal mode test
* @details 	Current, set to 5 cycle, 500 count per cycle
*/
#define     SPOT_CHECK_CYCLE_NUM_LIMIT      (5*100*5)

/**
* @def 		Enable or Disable IN_Hospital Mode
* @details 	IN_Hospital Mode, will not response ERROR status, measure keeps continues until STOP command
*/
#define 	MOD_CTRL_IS_FORCE_RUNNING_ENABLE 		true

/**
* @def 		Enable or Disable 4 minutes exist function
* @details 	In normal mode, require 4 minutes exist if not in Hospital mode or not in FreeRun mode
*/

#define 	MOD_CTRL_IS_TIME_EXPIRE_EXIT_ENABLE 		true


/**
* @def 		Enable or Disablr Data Double Check Function
* @details 	Enable -- Will double check the data in Monitor Mode
*					  If the SpO2/Hr is abnormal, will restart the measure and get the average
*			Disable -- Not Check the data
*/
#define 	MOD_CTRL_IS_DATA_DOUBLE_CHECK_IN_MONITOR_MODE 	false

/**
* @def 		DATA_DOUBLE_CHECK_NUM
* @details	the limit of data double check
*/
#define 	DATA_DOUBLE_CHECK_NUM 				(2)


/**
* @brief Enable or Disable 100% SpO2 vaule 
*/
#define MOD_CTRL_IS_BLOCK_HUNDRED_SPO2		true

/**
* @def 		SET the motion Check threshold 
* @details 	the corresponding data format is uint32_t
*			To disable this function, can set this value to 0x7fffffff
*/
#define SENSOR_MOTION_THRESHOLD_SPO2		(10240)
/**
* @def 		SET the clear-while-motion threshold 
* @details 	the corresponding data format is uint32_t
*			To disable this function, can set this value to 0x7fffffff
*/
#define SENSOR_DATA_ANTI_MOV_THRES          (0x7fffffff)

#define   TRUE     							(1)
#define   FALSE    							(0)

/* 		Define sub-function status 		*/
#define 	SpO2_MonitorTemplate_State 		(1 << 0)
#define 	HR_MonitorTemplate_State 			(1 << 1)

#define 	SpO2_SingleWork_State 				(1 << 2)
#define 	HR_SingleWork_State 					(1 << 3)

#define 	SpO2_RealTimeSample_State 		(1 << 4)
#define 	HR_RealTimeSample_State 			(1 << 5)

#define 	SPO2_HR_MONITOR_STATE				(SpO2_MonitorTemplate_State | HR_MonitorTemplate_State)
#define 	SPO2_HR_SINGLEWORK_STATE 		(SpO2_SingleWork_State | HR_SingleWork_State)
#define 	SPO2_HR_REALTIME_STATE 			(SpO2_RealTimeSample_State | HR_RealTimeSample_State)

#define 	SPO2_HR_SUBFUNC_ALL_STATE 	(SPO2_HR_MONITOR_STATE | SPO2_HR_SINGLEWORK_STATE | SPO2_HR_REALTIME_STATE)

#define 	SUBFUNC_ALL_STATE 					(SPO2_HR_SUBFUNC_ALL_STATE)

#define 	SPO2_NEEDED_STATE 			    (SPO2_HR_SUBFUNC_ALL_STATE)



extern HR_SPO2_PARAMETERS_T      g_HrSpo2Val_st;
void spo2_hr_cal_mod_init(void);
void gSubFunc_Stat_Set(uint16_t mask, uint8_t newState);
uint16_t gSubFunc_Stat_Get(uint16_t mask);
void spo2_hr_post_handle(void);


typedef struct{

    /*  function pointer that needs to be appointed at top application layer    */
    CC_ALG_SYS_EVENT_SEND_PRT;      //pointer to the system event distributuion function
    CC_ALG_SYS_CRITICAL_RESOURCE_APPLY_PRT;     //pointer to the system critical resource apply
    CC_ALG_SYS_CRITICAL_RESOURCE_RELEASE_PRT;
    CC_ALG_SYS_DELAY_MS_PRT;                    //system delay function
    /*  pointer to the input parameters     */
    ccAlgSpO2HrParam_st *       m_ccAlgParamIn_pst;
    
    /*  pointer to the output data          */
//    HR_SPO2_DAT_T *       m_HrSpO2DatOutput_pst;

} ccAlgBody_st;

///*Typedef: data sampled from AFE*/
//typedef struct{
//	int32_t m_iRedData;
//	int32_t m_iIrData;
//	int32_t m_iRedDiffData;
//	int32_t m_iIrDiffData;
//}PULSE_OXI_SAMPLE_T;

/*Typedef: TX channel Configuration*/
typedef struct{
	uint8_t m_uLedTurnOn;			//LED status
	uint8_t m_uRedLedCurr;			//red LED current value
	uint8_t m_uIrLedCurr;			//ir LED current value
}PULSE_OXI_TX_CHANNEL_T;

/*Typedef: RX channel Configuration*/
typedef struct{
	uint32_t m_uCfTiaVal;			//CF value
	uint32_t m_uRfTiaVal;			//RF Value
	uint32_t m_uAmbDac;				//Ambient Cancel DAC
	uint32_t m_uGainStage2;			//Stage 2 Gain
	bool 	m_bIsStage2;			//status of stage2 gain
}PULSE_OXI_RX_CHANNEL_T;


#define AFE_DAT_BUF_LEN    (100)
#define AFE_ADJUST_LEN    (16)

/* 	Runtime counter kick func 		*/
//void SpO2_RunTime_Cnt_Kick(void);

/* 	Runtime counter get func 		*/
//uint32_t SpO2_RunTime_Cnt_Get(void);

/* 	Runtime counter set func 		*/
//void SpO2_RunTime_Cnt_Set(uint32_t newValue);

void cc_alg_measure_start(void);
void cc_alg_measure_stop(void);
void cc_alg_SpO2_Stat_Set(ccAlgSpO2HrStatus_en newState);
ccAlgSpO2HrStatus_en cc_alg_SpO2_Stat_Get(void);
void apl_afe4403_interrupt_handler(void);
uint8_t apl_pulse_oximetry_signal_adjust_check(void);
void apl_afe4403_adjust_poll(void);

void apl_spo2_event_post(uint32_t v_uEvent);
uint8_t apl_spo2_event_pend(uint32_t *v_puEvent);

void apl_collection_event_post(uint32_t v_uEvent);
uint8_t apl_collection_event_pend(uint32_t *v_puEvent);

/**
 * @def 	
 * @details
 * @todo
 */
void SpO2_Task_Handler(uint32_t SpO2_Event);
void spo2Task(void *argument);
void collectionTask(void *argument);


#endif
//end file
