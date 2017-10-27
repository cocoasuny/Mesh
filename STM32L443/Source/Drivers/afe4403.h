/**
  ****************************************************************************************
  * @file    afe4403.h
  * @author  上海团队
  * @version V1.0.0
  * @date    $$$$-$$-$$
  * @brief   header file of afe4403.c
  ****************************************************************************************
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AFE4403_H
#define __AFE4403_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------*/
#include "main.h"
     
/* debug switch --------------------------------------------------------------*/ 
#define AFE4403_DEBUG     
	 
enum
{
	AFE4403_CONTROL0_ADDRESS 				= 0, 			// 0 - CONTROL0
	AFE4403_LED2STC_ADDRESS	 				= 1,			// 1 - LED2STC
	AFE4403_LED2ENDC_ADDRESS 				= 2,			// 2 - LED2ENDC 
	AFE4403_LED2LEDSTC_ADDRESS			= 3,			// 3 - LED2LEDSTC
	AFE4403_LED2LEDENDC_ADDRESS			= 4,			// 4 - LED2LEDENDC
	AFE4403_ALED2STC_ADDRESS				= 5,			// 5 - ALED2STC
	AFE4403_ALED2ENDC_ADDRESS				= 6,			// 6 - ALED2ENDC
	AFE4403_LED1STC_ADDRESS					= 7,			// 7 - LED1STC
	AFE4403_LED1ENDC_ADDRESS				= 8,			// 8 - LED1ENDC
	AFE4403_LED1LEDSTC_ADDRESS			= 9,			// 9 - LED1LEDSTC
	AFE4403_LED1LEDENDC_ADDRESS			= 10,			// 10- LED1LEDENDC
	AFE4403_ALED1STC_ADDRESS				= 11,			// 11- ALED1STC
	AFE4403_ALED1ENDC_ADDRESS				= 12,			// 12- ALED1ENDC
	AFE4403_LED2CONVST_ADDRESS			= 13,			// 13- LED2CONVST
	AFE4403_LED2CONVEND_ADDRESS			= 14,			// 14- LED2CONVEND
	AFE4403_ALED2CONVST_ADDRESS			= 15,			// 15- ALED2CONVST
	AFE4403_ALED2CONVEND_ADDRESS		= 16,			// 16- ALED2CONVEND
	AFE4403_LED1CONVST_ADDRESS			= 17,			// 17- LED1CONVST
	AFE4403_LED1CONVEND_ADDRESS			= 18,			// 18- LED1CONVEND
	AFE4403_ALED1CONVST_ADDRESS			= 19,			// 19- ALED1CONVST
	AFE4403_ALED1CONVEND_ADDRESS		= 20,			// 20- ALED1CONVEND
	AFE4403_ADCRSTSTCT0_ADDRESS			= 21,			// 21- ADCRSTSTCT0
	AFE4403_ADCRSTENDCT0_ADDRESS		= 22,			// 22- ADCRSTENDCT0
	AFE4403_ADCRSTSTCT1_ADDRESS			= 23,			// 23- ADCRSTSTCT1
	AFE4403_ADCRSTENDCT1_ADDRESS		= 24,			// 24- ADCRSTENDCT1
	AFE4403_ADCRSTSTCT2_ADDRESS			= 25,			// 25- ADCRSTSTCT2
	AFE4403_ADCRSTENDCT2_ADDRESS		= 26,			// 26- ADCRSTENDCT2
	AFE4403_ADCRSTSTCT3_ADDRESS			= 27,			// 27- ADCRSTSTCT3
	AFE4403_ADCRSTENDCT3_ADDRESS		= 28,			// 28- ADCRSTENDCT3
	AFE4403_PRPCOUNT_ADDRESS				= 29,			// 29- PRPCOUNT
	AFE4403_CONTROL1_ADDRESS				= 30,			// 30- CONTROL1
	AFE4403_SPARE1_ADDRESS					= 31,			// 31- SPARE1, N/A
	AFE4403_TIAGAIN_ADDRESS					= 32,			// 32- TIAGAIN
	AFE4403_TIA_AMB_GAIN_ADDRESS		= 33,			// 33- TIA_AMB_GAIN
	AFE4403_LEDCNTRL_ADDRESS				= 34,			// 34- LEDCNTRL
	AFE4403_CONTROL2_ADDRESS				= 35,			// 35- CONTROL2
	AFE4403_SPARE2_ADDRESS					= 36,			// 36- SPARE2, N/A
	AFE4403_SPARE3_ADDRESS					= 37,			// 37- SPARE3, BOOST RESERVED
	AFE4403_SPARE4_ADDRESS					= 38,			// 38- SPARE4, N/A
	AFE4403_RESERVED1_ADDRESS				= 39,			// 39- RESERVED1
	AFE4403_RESERVED2_ADDRESS				= 40,			// 40- RESERVED2
	AFE4403_ALARM_ADDRESS						= 41,			// 41- ALARM
	AFE4403_LED2VAL_ADDRESS					= 42,			// 42- LED2VAL, LED2 MEASUREMENT VALUE
	AFE4403_ALED2VAL_ADDRESS				= 43,			// 43- ALED2VAL, LED2 AMBIENT MEASUREMENT VALUE
	AFE4403_LED1VAL_ADDRESS					= 44,			// 44- LED1VAL, LED1 MEASUREMENT VALUE
	AFE4403_ALED1VAL_ADDRESS				= 45,			// 45- ALED1VAL, AMBIENT LED1 MEASUREMENT VALUE
	AFE4403_LED2ALED2VAL_ADDRESS		= 46,			// 46- LED2ALED2VAL, LED2 - ALED2 VALUE
	AFE4403_LED1ALED1VAL_ADDRESS		= 47,			// 47- LED1ALED1VAL, LED1 - ALED1 VALUE
	AFE4403_DIAG_ADDRESS            = 48,     // 48- Diag register
	AFE4403_CONTROL3_ADDRESS        = 49,
	AFE4403_PDNCYCLESTC_ADDRESS     = 50,
	AFE4403_PDNCYCLEENDC_ADDRESS    = 51
};

#define AFE4403_CONTROL0	0x000000
#define AFE4403_LED2STC	0x000050
#define AFE4403_LED2ENDC	0x00031F
#define AFE4403_LED2LEDSTC	0x000000
#define AFE4403_LED2LEDENDC	0x00031F
#define AFE4403_ALED2STC	0x000370
#define AFE4403_ALED2ENDC	0x00063F
#define AFE4403_LED1STC	0x000690
#define AFE4403_LED1ENDC	0x00095F
#define AFE4403_LED1LEDSTC	0x000640
#define AFE4403_LED1LEDENDC	0x00095F
#define AFE4403_ALED1STC	0x0009B0
#define AFE4403_ALED1ENDC	0x000C7F
#define AFE4403_LED2CONVST	0x000328
#define AFE4403_LED2CONVEND	0x001C63
#define AFE4403_ALED2CONVST	0x001C6C
#define AFE4403_ALED2CONVEND	0x0035A7
#define AFE4403_LED1CONVST	0x0035B0
#define AFE4403_LED1CONVEND	0x004EEB
#define AFE4403_ALED1CONVST	0x004EF4
#define AFE4403_ALED1CONVEND	0x00682F
#define AFE4403_ADCRSTSTCT0	0x000321
#define AFE4403_ADCRSTENDCT0	0x000326
#define AFE4403_ADCRSTSTCT1	0x001C65
#define AFE4403_ADCRSTENDCT1	0x001C6A
#define AFE4403_ADCRSTSTCT2	0x0035A9
#define AFE4403_ADCRSTENDCT2	0x0035AE
#define AFE4403_ADCRSTSTCT3	0x004EED
#define AFE4403_ADCRSTENDCT3	0x004EF2
#define AFE4403_PRPCOUNT	0x009C3F
#define AFE4403_CONTROL1	0x00001F
#define AFE4403_SPARE1	0x000000
#define AFE4403_TIAGAIN	0x000000
#define AFE4403_TIA_AMB_GAIN	0x44265
#define AFE4403_LEDCNTRL	0x001F1F
#define AFE4403_CONTROL2	0x124918
#define AFE4403_SPARE2	0x000000
#define AFE4403_SPARE3	0x000000
#define AFE4403_SPARE4	0x000000
#define AFE4403_RESERVED1	0x000000
#define AFE4403_RESERVED2	0x000000
#define AFE4403_ALARM	0x000000
#define AFE4403_LED2VAL	0x000000
#define AFE4403_ALED2VAL	0x000000
#define AFE4403_LED1VAL	0x000000
#define AFE4403_ALED1VAL	0x000000
#define AFE4403_LED2_ALED2VAL	0x000000
#define AFE4403_LED1_ALED1VAL	0x000000
#define AFE4403_DIAG	0x000000
#define AFE4403_CONTROL3	0x000002
#define AFE4403_PDNCYCLESTC	0x006B4E
#define AFE4403_PDNCYCLEENDC	0x00991F



#define AFE4403_CONTROL0_READ	0x000001
#define AFE4403_CONTROL0_WRITE	0x000000


#define AFE44x0_SAMPLE_FREQ         (100)

/****************************************************************/
/* AFE44x0 Register Function Definition*/
/****************************************************************/

/** @defgroup AFE44x0_CLKALMPIN 
  * @{
  */
#define AFE44x0_CLKALMPIN_SLED2_SLED1			((unsigned int)0x00000000)			//PD_ALM(Sample LED2 Pulse) LED_ALM(Sample LED1 Pulse)
#define AFE44x0_CLKALMPIN_LED2_LED1				((unsigned int)0x00000200)			//PD_ALM(LED2 LED Pulse) LED_ALM(LED1 LED Pulse)
#define AFE44x0_CLKALMPIN_SLED2A_SLED1A			((unsigned int)0x00000400)			//PD_ALM(Sample LED2 ambient Pulse) LED_ALM(Sample LED1 ambient Pulse)
#define AFE44x0_CLKALMPIN_LED2C_LED1C			((unsigned int)0x00000600)			//PD_ALM(LED2 Convert) LED_ALM(LED1 Convert)
#define AFE44x0_CLKALMPIN_LED2AC_LED1AC			((unsigned int)0x00000800)			//PD_ALM(LED2 Ambient convert ) LED_ALM(LED1 ambient)
#define AFE44x0_CLKALMPIN_LEDNO_LEDNO			((unsigned int)0x00000a00)| \
												((unsigned int)0x00000c00)| \
												((unsigned int)0x00000e00)			//PD_ALM(no output) LED_ALM(no output)
#define IS_AFE44x0_CLKALMPIN_MODE(MODE)			(((MODE) == AFE44x0_CLKALMPIN_SLED2_SLED1) || \
                                    			((MODE) == AFE44x0_CLKALMPIN_LED2_LED1) || \
                                    			((MODE) == AFE44x0_CLKALMPIN_SLED2A_SLED1A) || \
                                    			((MODE) == AFE44x0_CLKALMPIN_LED2C_LED1C) || \
                                    			((MODE) == AFE44x0_CLKALMPIN_LED2AC_LED1AC) || \
                                    			((MODE) == AFE44x0_CLKALMPIN_LEDNO_LEDNO))



/** @defgroup AFE44x0_AMBDAC 
  * @{
  */
#define AFE44x0_AMBDAC_Current_0				((unsigned int)0x00000000)			//0uA
#define AFE44x0_AMBDAC_Current_1				((unsigned int)0x00010000)			//1uA
#define AFE44x0_AMBDAC_Current_2				((unsigned int)0x00020000)			//2uA
#define AFE44x0_AMBDAC_Current_3				((unsigned int)0x00030000)			//3uA
#define AFE44x0_AMBDAC_Current_4				((unsigned int)0x00040000)			//4uA
#define AFE44x0_AMBDAC_Current_5				((unsigned int)0x00050000)			//5uA
#define AFE44x0_AMBDAC_Current_6				((unsigned int)0x00060000)			//6uA
#define AFE44x0_AMBDAC_Current_7				((unsigned int)0x00070000)			//7uA
#define AFE44x0_AMBDAC_Current_8				((unsigned int)0x00080000)			//8uA
#define AFE44x0_AMBDAC_Current_9				((unsigned int)0x00090000)			//9uA
#define AFE44x0_AMBDAC_Current_10				((unsigned int)0x000a0000)			//10uA

#define IS_AFE44x0_AMBDAC_Current(Current)		(((Current) == AFE44x0_AMBDAC_Current_0) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_1) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_2) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_3) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_4) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_5) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_6) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_7) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_8) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_9) || \
                                    			((Current) == AFE44x0_AMBDAC_Current_10))


/** @defgroup AFE44x0_STG2GAIN 
  * @{
  */
#define AFE44x0_STG2GAIN_0dB					(0x0<<8)			//0dB, linear 1
#define AFE44x0_STG2GAIN_3_5dB				(0x1<<8)			//3.5dB, linear 1.5
#define AFE44x0_STG2GAIN_6dB					(0x2<<8)			//6dB, linear 2
#define AFE44x0_STG2GAIN_9_5dB				(0x3<<8)			//9.5dB, linear 3
#define AFE44x0_STG2GAIN_12dB					(0x4<<8)			//12dB, linear 4

#define IS_AFE44x0_STG2GAIN(GAIN)		(((GAIN) == AFE44x0_STG2GAIN_0dB) || \
                                    			((GAIN) == AFE44x0_STG2GAIN_3_5dB) || \
                                    			((GAIN) == AFE44x0_STG2GAIN_6dB) || \
                                    			((GAIN) == AFE44x0_STG2GAIN_9_5dB) || \
                                    			((GAIN) == AFE44x0_STG2GAIN_12dB))


/** @defgroup AFE44x0_CF_LED 
  * @{
  */
#define AFE44x0_CF_LED_5pF						0x00000000			//5pF
#define AFE44x0_CF_LED_5pF_Plus_5pF				0x00000008			//5pF+5pF
#define AFE44x0_CF_LED_15pF_Plus_5pF			0x00000010			//15pF+5pF
#define AFE44x0_CF_LED_25pF_Plus_5pF			0x00000020			//25pF+5pF
#define AFE44x0_CF_LED_50pF_Plus_5pF			0x00000040			//50pF+5pF
#define AFE44x0_CF_LED_150pF_Plus_5pF			0x00000080			//150pF+5pF
#define AFE44x0_CF_LED_100pF_Plus_5pF			0x00000078 			//100pF+5pF
#define AFE44x0_CF_LED_200pF_Plus_5pF			0x000000C0			//210pF
#define IS_AFE44x0_CF_LED(Cap)		(((Cap) == AFE44x0_CF_LED_5pF) || \
                                    			((Cap) == AFE44x0_CF_LED_5pF_Plus_5pF) || \
                                    			((Cap) == AFE44x0_CF_LED_15pF_Plus_5pF) || \
                                    			((Cap) == AFE44x0_CF_LED_25pF_Plus_5pF) || \
                                    			((Cap) == AFE44x0_CF_LED_50pF_Plus_5pF) || \
                                    			((Cap) == AFE44x0_CF_LED_150pF_Plus_5pF))


/** @defgroup AFE44x0_RF_LED 
  * @{
  */
#define AFE44x0_RF_LED_500K						0x00000000			//500K OHM
#define AFE44x0_RF_LED_250K						0x00000001			//250K OHM
#define AFE44x0_RF_LED_100K						0x00000002			//100K OHM
#define AFE44x0_RF_LED_50K						0x00000003			//50K OHM
#define AFE44x0_RF_LED_25K						0x00000004			//25K OHM
#define AFE44x0_RF_LED_10K						0x00000005			//10K OHM
#define AFE44x0_RF_LED_1M						  0x00000006			//1M OHM

#define IS_AFE44x0_RF_LED(Res)		(((Res) == AFE44x0_RF_LED_500K) || \
                                    			((Res) == AFE44x0_RF_LED_250K) || \
                                    			((Res) == AFE44x0_RF_LED_100K) || \
                                    			((Res) == AFE44x0_RF_LED_50K) || \
                                    			((Res) == AFE44x0_RF_LED_25K) || \
                                    			((Res) == AFE44x0_RF_LED_10K) || \
                                    			((Res) == AFE44x0_RF_LED_1M))


/** @defgroup AFE44x0_TXBRGMOD 
  * @{
  */
#define AFE44x0_TXBRGMOD_H_Bridge						0x00000000			//LED driver is configured as H-Bridge
#define AFE44x0_TXBRGMOD_Push_Pull					0x00000800			//LED driver is configured as push pull
#define IS_AFE44x0_TXBRGMOD(MODE)		(((MODE) == AFE44x0_TXBRGMOD_H_Bridge) || \
                                    			((MODE) == AFE44x0_TXBRGMOD_Push_Pull))

/** @defgroup AFE44x0_DIGOUT_TRISTATE 
  * @{
  */
#define AFE44x0_DIGOUT_TRISTATE_Normal						0x00000000			//digital out pin in normal mode when no selected
#define AFE44x0_DIGOUT_TRISTATE_Tristate					0x00000400			//digital out pin in tristate mode when no selected
#define IS_AFE44x0_DIGOUT_TRISTATE(MODE)		(((MODE) == AFE44x0_DIGOUT_TRISTATE_Normal) || \
                                    			((MODE) == AFE44x0_DIGOUT_TRISTATE_Tristate))


/** @defgroup AFE44x0_XTALDIS 
  * @{
  */
#define AFE44x0_XTALDIS_Crystal						0x00000000			//crystal module enable, 8MHz crystal
#define AFE44x0_XTALDIS_External					0x00000200			//crystal module disable, 8MHz external clock source
#define IS_AFE44x0_XTALDIS(MODE)		(((MODE) == AFE44x0_XTALDIS_Crystal) || \
                                    			((MODE) == AFE44x0_XTALDIS_External))

/** @defgroup AFE44x0_TX_POWER 
  * @{
  */
#define AFE44x0_TX_POWER_ON						0x00000000			//TX power on
#define AFE44x0_TX_POWER_OFF					0x00000004			//TX power off
#define IS_AFE44x0_TX_POWER(MODE)		(((MODE) == AFE44x0_TX_POWER_ON) || \
                                    			((MODE) == AFE44x0_TX_POWER_OFF))

/** @defgroup AFE44x0_RX_POWER 
  * @{
  */
#define AFE44x0_RX_POWER_ON						0x00000000			//RX power on
#define AFE44x0_RX_POWER_OFF					0x00000002			//RX power off
#define IS_AFE44x0_RX_POWER(MODE)		(((MODE) == AFE44x0_RX_POWER_ON) || \
                                    			((MODE) == AFE44x0_RX_POWER_OFF))

/** @defgroup AFE44x0_AFE_POWER 
  * @{
  */
#define AFE44x0_AFE_POWER_ON					0x00000000			//RX power on
#define AFE44x0_AFE_POWER_OFF					0x00000001			//RX power off
#define IS_AFE44x0_AFE_POWER(MODE)		(((MODE) == AFE44x0_AFE_POWER_ON) || \
                                    			((MODE) == AFE44x0_AFE_POWER_OFF))

/** @defgroup AFE44x0_ALMPINCLKEN 
  * @{
  */
#define AFE44x0_ALMPINCLKEN_DISABLE			0x00000000			//Alarm pin clock disable
#define AFE44x0_ALMPINCLKEN_ENABLE			0x00000080			//Alarm pin clock enable
#define IS_AFE44x0_ALMPINCLKEN(MODE)		(((MODE) == AFE44x0_ALMPINCLKEN_DISABLE) || \
                                    			((MODE) == AFE44x0_ALMPINCLKEN_ENABLE))


/** @defgroup AFE44x0_LEDCUROFF 
  * @{
  */
#define AFE44x0_LEDCUROFF_ON					0x00000000			//LED Current on
#define AFE44x0_LEDCUROFF_OFF					0x00020000			//LED current off
#define IS_AFE44x0_LEDCUROFF(MODE)		(((MODE) == AFE44x0_LEDCUROFF_ON) || \
                                    			((MODE) == AFE44x0_LEDCUROFF_OFF))


/** @defgroup AFE44x0_STAGE2EN 
  * @{
  */
#define AFE44x0_STAGE2EN_BYPASS				0x00000000			//Stage 2 amplifier bypass
#define AFE44x0_STAGE2EN_ENABLE				0x00004000			//Stage 2 amplifier enable
#define IS_AFE44x0_STAGE2EN(MODE)		(((MODE) == AFE44x0_STAGE2EN_BYPASS) || \
                                    			((MODE) == AFE44x0_STAGE2EN_ENABLE))

/** @defgroup AFE44x0_TIMEREN 
  * @{
  */
#define AFE44x0_TIMEREN_DISABLE				0x00000000			//timer module disable
#define AFE44x0_TIMEREN_ENABLE				0x00000100			//timer module enable
#define IS_AFE44x0_TIMEREN(MODE)		(((MODE) == AFE44x0_TIMEREN_ENABLE) || \
                                    			((MODE) == AFE44x0_TIMEREN_DISABLE))


/** @defgroup flag when diagnose 
  * @{
  */
#define AFE44x0_DIAG_FLAG_INPSCLED		0x00000001             //
#define AFE44x0_DIAG_FLAG_INNSCLED		0x00000002             //
#define AFE44x0_DIAG_FLAG_INPSCGND		0x00000004             //
#define AFE44x0_DIAG_FLAG_INNSCGND		0x00000008             //
#define AFE44x0_DIAG_FLAG_PDSC			0x00000010             //
#define AFE44x0_DIAG_FLAG_PDOC			0x00000020             //
#define AFE44x0_DIAG_FLAG_OUTNSHGND		0x00000040             //
#define AFE44x0_DIAG_FLAG_OUTPSHGND		0x00000080             //
#define AFE44x0_DIAG_FLAG_LEDSC			0x00000100             //
#define AFE44x0_DIAG_FLAG_LED2OPEN		0x00000200             //
#define AFE44x0_DIAG_FLAG_LED1OPEN		0x00000400             //
#define AFE44x0_DIAG_FLAG_LED_ALM		0x00000800             //
#define AFE44x0_DIAG_FLAG_PD_ALM		0x00001000             //
#define IS_AFE44x0_DIAG_FLAG(FLAG)		(((FLAG) == AFE44x0_DIAG_FLAG_INPSCLED) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_INNSCLED) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_INPSCGND) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_INNSCGND) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_PDSC) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_PDOC) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_OUTNSHGND) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_OUTPSHGND) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_LEDSC) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_LED2OPEN) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_LED1OPEN) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_LED_ALM) || \
                                    			((FLAG) == AFE44x0_DIAG_FLAG_PD_ALM))

/*  Added register bit definition for AFE4403*/
/** @defgroup AFE4403_REG_TX3_MODE
	* @{
	*/
#define AFE44x0_TX3_MODE_ENABLE 		0x00008000  		//Enable LED3
#define AFE44x0_TX3_MODE_DISABLE 		0x00000000  		//Disable LED3

/** @defgroup AFE4403_REG_SOMI_TRI
	* @{
	*/
#define AFE44x0_SOMI_TRI_ENABLE 		0x00000010  		//Place SOMI in tri state
#define AFE44x0_SOMI_TRI_DISABLE 		0x00000000  		//Place SOMI in active mode

/** @defgroup AFE4403_REG_CLKOUT_TRI
	* @{
	*/
#define AFE44x0_CLKOUT_TRI_ENABLE 		0x00000008  		//Place CLKOUT in tri state
#define AFE44x0_CLKOUT_TRI_DISABLE 		0x00000000  		//Place CLKOUT in active mode


/** @defgroup AFE44x0_AFE_TX_REF 
	* @{
	*/
#define AFE44x0_AFE_TX_REF_025V					0x00000000			//0.25V
#define AFE44x0_AFE_TX_REF_05V					0x00020000			//0.5V
#define AFE44x0_AFE_TX_REF_10V					0x00040000			//1.0V
#define AFE44x0_AFE_TX_REF_075V					0x00060000			//0.75V


/** @defgroup AFE4403_REG_CLKDIV
	* @{
	*/
#define AFE44x0_CLKDIV_2 		0x00000000  		//DIV 2, CLOCK RANGE 8~12MHz
#define AFE44x0_CLKDIV_4 		0x00000002  		//DIV 4, CLOCK RANGE 16~24MHz
#define AFE44x0_CLKDIV_6 		0x00000003  		//DIV 6, CLOCK RANGE 24~36MHz
#define AFE44x0_CLKDIV_8 		0x00000004  		//DIV 8, CLOCK RANGE 32~48MHz
#define AFE44x0_CLKDIV_12 		0x00000005  		//DIV 12, CLOCK RANGE 48~60MHz
#define AFE44x0_CLKDIV_1 		0x00000007  		//DIV 1, CLOCK RANGE 4~6MHz
#define IS_AFE44x0_CLKDIV(MODE)		(((MODE) == AFE44x0_CLKDIV_2) || \
																		((MODE) == AFE44x0_CLKDIV_4) || \
																		((MODE) == AFE44x0_CLKDIV_6) || \
																		((MODE) == AFE44x0_CLKDIV_8) || \
																		((MODE) == AFE44x0_CLKDIV_12) || \
																		((MODE) == AFE44x0_CLKDIV_1)) 
/** @defgroup AFE44x0_CONTROL2_EN_SLOW_DIAG
	* @{
	*/
#define AFE44x0_SLOW_DIAG_MODE 		0x00000000			//enable fast mode, 8ms
#define AFE44x0_FAST_DIAG_MODE		0x00000100 			//enable slow mode, 16ms

/** @defgroup AFE44x0_CONTROL2_DYNAMIC1/2/3/4
	* @{
	*/
#define AFE44x0_DYNAMIC1_ENABLE 		0x00100000			//enable 
#define AFE44x0_DYNAMIC1_DISABLE		0x00000000 			//disable

#define AFE44x0_DYNAMIC2_ENABLE 		0x00004000			//enable 
#define AFE44x0_DYNAMIC2_DISABLE		0x00000000 			//disable

#define AFE44x0_DYNAMIC3_ENABLE 		0x00000010			//enable 
#define AFE44x0_DYNAMIC3_DISABLE		0x00000000 			//disable

#define AFE44x0_DYNAMIC4_ENABLE 		0x00000008			//enable 
#define AFE44x0_DYNAMIC4_DISABLE		0x00000000 			//disable



/*******************  Bit definition for AFE44x0_REG_CONTROL0 register  ********************/
#define  AFE44x0_CONTROL0_SW_RST            ((unsigned int)0x00000008)           //soft reset
#define  AFE44x0_CONTROL0_DIAG_EN           ((unsigned int)0x00000004)           //diagnose enable
#define  AFE44x0_CONTROL0_TIM_COUNT_RST     ((unsigned int)0x00000002)           //timer counter reset
#define  AFE44x0_CONTROL0_SPI_READ          ((unsigned int)0x00000001)           //SPI read enable

/*******************  Bit definition for AFE44x0_REG_CONTROL1 register  ********************/

#define  AFE44x0_CONTROL1_TIMEREN           ((unsigned int)0x00000100)           //Timer Enable
#define  AFE44x0_CONTROL1_NUMAVG			((unsigned int)0x000000ff)           //Average number


/*******************  Bit definition for AFE44x0_REG_TIA_AMB_GAIN register  ********************/
#define  AFE44x0_TIA_AMB_GAIN_AMBDAC        ((unsigned int)0x000F0000)           //Ambient DAC value
#define  AFE44x0_TIA_AMB_GAIN_STAGE2EN      ((unsigned int)0x00004000)           //Stage 2 enable
#define  AFE44x0_TIA_AMB_GAIN_STG2GAIN      ((unsigned int)0x00000700)           //Stage 2 gain setting
#define  AFE44x0_TIA_AMB_GAIN_CF_LED        ((unsigned int)0x000000F8)           //Program CF for LEDs
#define  AFE44x0_TIA_AMB_GAIN_RF_LED        ((unsigned int)0x00000007)           //Program RF for LEDs

#define AFE44x0_TIA_AMB_GAIN_FLTRCNRSEL	((unsinged int)0x00008000) 			//must be 0


/*******************  Bit definition for AFE44x0_REG_TIAGAIN register  ********************/

#define  AFE44x0_TIAGAIN_ENSEPGAN        ((unsigned int)0x00008000)           //1--Enable seperate RF/CF for LED1/2
#define  AFE44x0_TIAGAIN_STAGE2EN1       ((unsigned int)0x00004000)           //Stage 2 enable
#define  AFE44x0_TIAGAIN_STG2GAIN1       ((unsigned int)0x00000700)           //Stage 2 gain setting
#define  AFE44x0_TIAGAIN_CF_LED1         ((unsigned int)0x000000F8)           //Program CF for LEDs
#define  AFE44x0_TIAGAIN_RF_LED1         ((unsigned int)0x00000007)           //Program RF for LEDs


/*******************  Bit definition for AFE44x0_REG_LEDCNTRL register  ********************/
#define  AFE44x0_LEDCNTRL_LEDRANGE         ((unsigned int)0x00030000)           //config the fullscale of LED output current


#define  AFE44x0_LEDCNTRL_LED1              ((unsigned int)0x0000FF00)           //Program LED current for LED1 signal
#define  AFE44x0_LEDCNTRL_LED2              ((unsigned int)0x000000FF)           //Program LED current for LED2 signal

/*******************  Bit definition for AFE44x0_REG_CONTROL2 register  ********************/
//AFE4403 DEBUG CHANGE
#define  AFE44x0_CONTROL2_DIGOUT_TRISTATE  ((unsigned int)0x00000400)           //Digital output 3-state mode


#define  AFE44x0_CONTROL2_DYNAMIC1		((unsigned int)0x00100000)
#define  AFE44x0_CONTROL2_DYNAMIC2		((unsigned int)0x00004000)
#define  AFE44x0_CONTROL2_DYNAMIC3		((unsigned int)0x00000010)
#define  AFE44x0_CONTROL2_DYNAMIC4		((unsigned int)0x00000008)
#define  AFE44x0_CONTROL2_TX_REF		((unsigned int)0x00060000)
#define  AFE44x0_CONTROL2_EN_SLOW_DIAG	((unsigned int)0x00000100)


#define  AFE44x0_CONTROL2_TXBRGMOD         ((unsigned int)0x00000800)           //Tx bridge mode
#define  AFE44x0_CONTROL2_XTALDIS          ((unsigned int)0x00000200)           //Crystal disable mode
#define  AFE44x0_CONTROL2_PDN_TX           ((unsigned int)0x00000004)           //Tx power-down
#define  AFE44x0_CONTROL2_PDN_RX           ((unsigned int)0x00000002)           //Rx power-down
#define  AFE44x0_CONTROL2_PDN_AFE          ((unsigned int)0x00000001)           //AFE power-down


/*******************  Bit definition for AFE44x0_REG_ALARM register  ********************/

/*******************  Bit definition for Reserved register  ********************/
#define  AFE44x0_SPARE1_SPARE1              ((unsigned int)0x00000000)           //RESERVED 
#define  AFE44x0_TIAGAIN_TIAGAIN            ((unsigned int)0x00000000)           //RESERVED FOR Factory
#define  AFE44x0_SPARE2_SPARE2              ((unsigned int)0x00000000)           //RESERVED 
#define  AFE44x0_SPARE3_SPARE3              ((unsigned int)0x00000000)           //RESERVED 
#define  AFE44x0_SPARE4_SPARE4              ((unsigned int)0x00000000)           //RESERVED 
#define  AFE44x0_RESERVED1_RESERVED1        ((unsigned int)0x00000000)           //RESERVED 
#define  AFE44x0_RESERVED2_RESERVED2        ((unsigned int)0x00000000)           //RESERVED 

/*******************  Bit definition for AFE44x0_REG_DIAG register  ********************/
#define  AFE44x0_DIAG_PD_ALM               ((unsigned int)0x00001000)           //Power-down alarm status diagnostic flag
#define  AFE44x0_DIAG_LED_ALM              ((unsigned int)0x00000800)           //LED alarm status diagnostic flag
#define  AFE44x0_DIAG_LED1OPEN             ((unsigned int)0x00000400)           //LED1 open diagnostic flag
#define  AFE44x0_DIAG_LED2OPEN             ((unsigned int)0x00000200)           //LED2 open diagnostic flag
#define  AFE44x0_DIAG_LEDSC                ((unsigned int)0x00000100)           //LED short diagnostic flag
#define  AFE44x0_DIAG_OUTPSHGND            ((unsigned int)0x00000080)           //OUTP to GND diagnostic flag
#define  AFE44x0_DIAG_OUTNSHGND            ((unsigned int)0x00000040)           //OUTN to GND diagnostic flag
#define  AFE44x0_DIAG_PDOC                 ((unsigned int)0x00000020)           //PD open diagnostic flag
#define  AFE44x0_DIAG_PDSC                 ((unsigned int)0x00000010)           //PD short diagnostic flag
#define  AFE44x0_DIAG_INNSCGND             ((unsigned int)0x00000008)           //INN to GND diagnostic flag
#define  AFE44x0_DIAG_INPSCGND             ((unsigned int)0x00000004)           //INP to GND diagnostic flag
#define  AFE44x0_DIAG_INNSCLED             ((unsigned int)0x00000002)           //INN to LED diagnostic flag
#define  AFE44x0_DIAG_INPSCLED             ((unsigned int)0x00000001)           //INP to LED diagnostic flag

/*******************  Bit definition for AFE44x0_REG_CONTROL3 register  ********************/

#define  AFE44x0_CONTROL3_TX3_MODE		((unsigned int)0x00008000)
#define  AFE44x0_CONTROL3_SOMI_TRI		((unsigned int)0x00000010)
#define  AFE44x0_CONTROL3_CLKOUT_TRI	((unsigned int)0x00000008)
#define  AFE44x0_CONTROL3_CLKDIV		((unsigned int)0x00000007)


/*******************  Bit definition for AFE44x0_REG_PDNCYCLESTC register  ********************/
#define  AFE44x0_PDNCYCLESTC_PDNCYCLESTC		((unsigned int)0x0000FFFF)


/*******************  Bit definition for AFE44x0_REG_PDNCYCLEENDC register  ********************/
#define  AFE44x0_PDNCYCLEENDC_PDNCYCLEENDC		((unsigned int)0x0000FFFF)

/******************   Bit definition for one data 16 bits register      *******************/
#define  AFE44x0_LED2STC_LED2STC                 ((unsigned int)0x0000FFFF)           //Sample LED2 start count
#define  AFE44x0_LED2ENDC_LED2ENDC               ((unsigned int)0x0000FFFF)           //Sample LED2 end count
#define  AFE44x0_LED2LEDSTC_LED2LEDSTC           ((unsigned int)0x0000FFFF)           //LED2 start count
#define  AFE44x0_LED2LEDENDC_LED2LEDENDC         ((unsigned int)0x0000FFFF)           //LED2 end count
#define  AFE44x0_ALED2STC_ALED2STC               ((unsigned int)0x0000FFFF)           //Sample Ambient LED2 start count
#define  AFE44x0_ALED2ENDC_ALED2ENDC             ((unsigned int)0x0000FFFF)           //Sample Ambient LED2 end count
#define  AFE44x0_LED1STC_LED1STC                 ((unsigned int)0x0000FFFF)           //Sample LED1 start count
#define  AFE44x0_LED1ENDC_LED1ENDC               ((unsigned int)0x0000FFFF)           //Sample LED1 end count
#define  AFE44x0_LED1LEDSTC_LED1LEDSTC           ((unsigned int)0x0000FFFF)           //LED1 start count
#define  AFE44x0_LED1LEDENDC_LED1LEDENDC         ((unsigned int)0x0000FFFF)           //LED1 end count
#define  AFE44x0_ALED1STC_ALED1STC               ((unsigned int)0x0000FFFF)           //Sample Ambient LED1 start count
#define  AFE44x0_ALED1ENDC_ALED1ENDC             ((unsigned int)0x0000FFFF)           //Sample Ambient LED1 end count
#define  AFE44x0_LED2CONVST_LED2CONVST           ((unsigned int)0x0000FFFF)           //LED2 convert start count
#define  AFE44x0_LED2CONVEND_LED2CONVEND         ((unsigned int)0x0000FFFF)           //LED2 convert end count
#define  AFE44x0_ALED2CONVST_ALED2CONVST         ((unsigned int)0x0000FFFF)           //LED2 ambient convert start count
#define  AFE44x0_ALED2CONVEND_ALED2CONVEND       ((unsigned int)0x0000FFFF)           //LED2 ambient convert end count
#define  AFE44x0_LED1CONVST_LED1CONVST           ((unsigned int)0x0000FFFF)           //LED1 convert start count
#define  AFE44x0_LED1CONVEND_LED1CONVEND         ((unsigned int)0x0000FFFF)           //LED1 convert end count
#define  AFE44x0_ALED1CONVST_ALED1CONVST         ((unsigned int)0x0000FFFF)           //LED1 ambient convert start count
#define  AFE44x0_ALED1CONVEND_ALED1CONVEND       ((unsigned int)0x0000FFFF)           //LED1 ambient convert end count
#define  AFE44x0_ADCRSTSTCT0_ADCRSTSTCT0         ((unsigned int)0x0000FFFF)           //ADC RESET 0 start count
#define  AFE44x0_ADCRSTENDCT0_ADCRSTENDCT0       ((unsigned int)0x0000FFFF)           //ADC RESET 0 end count
#define  AFE44x0_ADCRSTSTCT1_ADCRSTSTCT1         ((unsigned int)0x0000FFFF)           //ADC RESET 1 start count
#define  AFE44x0_ADCRSTENDCT1_ADCRSTENDCT1       ((unsigned int)0x0000FFFF)           //ADC RESET 1 end count
#define  AFE44x0_ADCRSTSTCT2_ADCRSTSTCT2         ((unsigned int)0x0000FFFF)           //ADC RESET 2 start count
#define  AFE44x0_ADCRSTENDCT2_ADCRSTENDCT2       ((unsigned int)0x0000FFFF)           //ADC RESET 2 end count
#define  AFE44x0_ADCRSTSTCT3_ADCRSTSTCT3         ((unsigned int)0x0000FFFF)           //ADC RESET 3 start count
#define  AFE44x0_ADCRSTENDCT3_ADCRSTENDCT3       ((unsigned int)0x0000FFFF)           //ADC RESET 3 end count
#define  AFE44x0_PRPCOUNT_PRPCOUNT               ((unsigned int)0x0000FFFF)           //Pulse repetition period count

/******************   Bit definition for one data 24 bits register      *******************/
#define AFE44x0_LED2VAL_LED2VAL                       ((unsigned int)0x00FFFFFF)            // 42 LED2VAL[23:0]
#define AFE44x0_ALED2VAL_ALED2VAL                     ((unsigned int)0x00FFFFFF)            // 43 ALED2VAL[23:0]
#define AFE44x0_LED1VAL_LED1VAL                       ((unsigned int)0x00FFFFFF)            // 44 LED1VAL[23:0]
#define AFE44x0_ALED1VAL_ALED1VAL                     ((unsigned int)0x00FFFFFF)            // 45 ALED1VAL[23:0]
#define AFE44x0_LED2_SUB_ALED2VAL_LED2_SUB_ALED2VAL   ((unsigned int)0x00FFFFFF)            // 46 LED2-ALED2VAL[23:0]
#define AFE44x0_LED1_SUB_ALED1VAL_LED1_SUB_ALED1VAL   ((unsigned int)0x00FFFFFF)            // 47 LED1-ALED1VAL[23:0]

void drv_afe4403_init(void);
void drv_afe4403_power(uint8_t status);
void drv_afe4403_running_enable(void);
void drv_afe4403_running_disable(void);
void drv_afe4403_red_ir_get(int32_t *v_piRedValue, int32_t *v_piIredValue);
void drv_afe4403_red_ir_diff_get(int32_t *v_piRedDiffValue, int32_t *v_piIredDiffValue);
void drv_afe4403_cf_set(uint32_t v_uCfVal);
void drv_afe4403_rf_set(uint32_t uRfVal);
void drv_afe4403_dac_set(uint32_t uDacVal);
void drv_afe4403_cur_cancel_enable(void);
void drv_afe4403_cur_cancel_disable(void);
void drv_afe4403_sec_gain_set(uint32_t uSecGainVal);
void drv_afe4403_led_curr_set(uint8_t uLed1Cur, uint8_t uLed2Cur);

#ifdef __cplusplus
}
#endif
#endif /*__AFE4403_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

