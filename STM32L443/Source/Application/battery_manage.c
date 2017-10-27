/**
  ****************************************************************************************
  * @file    battery_manage.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-10-14
  * @brief   battery manage
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "battery_manage.h"
static void bat_charge_ctl(FunctionalState staus);

 
/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  battery charge control
  * @note   Enable or Disable battery charge
  * @param  FunctionalState: ENABLE/DISABLE
  * @retval None
  */
static void bat_charge_ctl(FunctionalState status)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    BAT_CHARGE_CTL_GPIO_CLK_ENABLE();
    
    if(status == ENABLE)
    {
        GPIO_InitStruct.Pin = BAT_CHARGE_CTL_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(BAT_CHARGE_CTL_PORT, &GPIO_InitStruct);
        
        /* set power pin high to open the power */
        HAL_GPIO_WritePin(BAT_CHARGE_CTL_PORT,BAT_CHARGE_CTL_PIN,GPIO_PIN_RESET); 
    }
    else if(status == DISABLE)
    {
        GPIO_InitStruct.Pin = BAT_CHARGE_CTL_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(BAT_CHARGE_CTL_PORT, &GPIO_InitStruct);
        
        /* reset power pin low to shut down the power */
        HAL_GPIO_WritePin(BAT_CHARGE_CTL_PORT,BAT_CHARGE_CTL_PIN,GPIO_PIN_SET);         
    }
}

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

