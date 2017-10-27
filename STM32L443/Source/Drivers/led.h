/**
* @name	led.h
* @brief 	led.c头文件
* @rev		v0.1
* @author	keke.sun@ywkang.com
* @date		2016/8/22
*/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __led_H
#define __led_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"


/* Exported types ------------------------------------------------------------*/ 
typedef  enum
{
    LED_RED,
    LED_GREEN,
    LED_ORANGE
}LED_T;



/* function declare */
void LEDx_Init(LED_T ledColour);
void LEDx_ON(LED_T ledColour);
void LEDx_OFF(LED_T ledColour);
void LEDx_Toggle(LED_T ledColour);
void LEDx_DeInit(LED_T ledColour);



#endif /* __led_H */


