/**
  ****************************************************************************************
  * @file    platform.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-11-25
  * @brief   platform define 
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_H_
#define __PLATFORM_H_

/* Includes ------------------------------------------------------------------*/
#include "nrf_gpio.h"
#include "log.h"

/* debug switch --------------------------------------------------------------*/
#define LOG_CALLBACK_DEFAULT log_callback_stdout

/* gpio and buffer define for uart */
#define RETARGET_ENABLED	1
#ifndef UART_TX_BUF_SIZE
    #define UART_TX_BUF_SIZE 1024         /**< UART TX buffer size. */
#endif
#ifndef UART_RX_BUF_SIZE
    #define UART_RX_BUF_SIZE 1            /**< UART RX buffer size. */
#endif

#define RX_PIN_NUMBER  				18    // UART RX pin number.
#define TX_PIN_NUMBER  				20    // UART TX pin number.
#define CTS_PIN_NUMBER 				10    // UART Clear To Send pin number. Not used if HWFC is set to false
#define RTS_PIN_NUMBER 				1     // Not used if HWFC is set to false


// LEDs definitions for PCA10028
#define LEDS_NUMBER    4

#define LED_START      21
#define LED_1          21
#define LED_2          22
#define LED_3          23
#define LED_4          24
#define LED_STOP       24

#define LEDS_ACTIVE_STATE 0

#define LEDS_LIST { LED_1, LED_2, LED_3, LED_4 }

#define LEDS_INV_MASK  LEDS_MASK

#define BSP_LED_0      LED_1
#define BSP_LED_1      LED_2
#define BSP_LED_2      LED_3
#define BSP_LED_3      LED_4

#define BUTTONS_NUMBER 4

#define BUTTON_START   17
#define BUTTON_1       17
#define BUTTON_2       18
#define BUTTON_3       19
#define BUTTON_4       20
#define BUTTON_STOP    20
#define BUTTON_PULL    NRF_GPIO_PIN_PULLUP

#define BUTTONS_ACTIVE_STATE 0

#define BUTTONS_LIST { BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4 }

#define BSP_BUTTON_0   BUTTON_1
#define BSP_BUTTON_1   BUTTON_2
#define BSP_BUTTON_2   BUTTON_3
#define BSP_BUTTON_3   BUTTON_4

#endif /* __PLATFORM_H_ */


/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/


