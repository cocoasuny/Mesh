/**
* @name	led.c
* @brief 	led驱动文件，实现led驱动
* @rev		v0.1
* @author	keke.sun@ywkang.com
* @date		2016/8/22
*/

/* Includes ------------------------------------------------------------------*/
#include "led.h"



/**
* @brief 		Led init
* @param[in]	led
* @param[out]	none
* @retval		none
*/
void LEDx_Init(LED_T ledColour)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    switch(ledColour)
    {
        case LED_RED:
        {
            /* Enable the Led GPIO CLK */
            LED_RED_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in output push-pull mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull  = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_RED;
            HAL_GPIO_Init(GPIO_PORT_LED_RED, &GPIO_InitStruct);            
        }              
        break;
        case LED_GREEN:
        {
            /* Enable the Led GPIO CLK */
            LED_GREEN_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in output push-pull mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull  = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_GREEN;
            HAL_GPIO_Init(GPIO_PORT_LED_GREEN, &GPIO_InitStruct);              
        } 
        break;
        case LED_ORANGE:
        {
            /* Enable the Led GPIO CLK */
            LED_RED_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in output push-pull mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull  = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_RED;
            HAL_GPIO_Init(GPIO_PORT_LED_RED, &GPIO_InitStruct);

            /* Enable the Led GPIO CLK */
            LED_GREEN_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in output push-pull mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull  = GPIO_PULLUP;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_GREEN;
            HAL_GPIO_Init(GPIO_PORT_LED_GREEN, &GPIO_InitStruct);              
        }            
        break;
        default:break;            
    }
}

/**
* @brief 		LEDx_ON
* @param[in]	led
* @param[out]	none
* @retval		none
*/
void LEDx_ON(LED_T ledColour)
{
    switch(ledColour)
    {
        case LED_RED:
        {
            HAL_GPIO_WritePin(GPIO_PORT_LED_RED,GPIO_PIN_LED_RED,GPIO_PIN_SET);
        }
        break;
        case LED_GREEN:
        {
            HAL_GPIO_WritePin(GPIO_PORT_LED_GREEN,GPIO_PIN_LED_GREEN,GPIO_PIN_SET);
        }
        break;
        case LED_ORANGE:
        {
            HAL_GPIO_WritePin(GPIO_PORT_LED_RED,GPIO_PIN_LED_RED,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIO_PORT_LED_GREEN,GPIO_PIN_LED_GREEN,GPIO_PIN_SET);
        }
        break;
        default: break;
    }
}

/**
* @brief 		LEDx_OFF
* @param[in]	led
* @param[out]	none
* @retval		none
*/
void LEDx_OFF(LED_T ledColour)
{
    switch(ledColour)
    {
        case LED_RED:
        {
            HAL_GPIO_WritePin(GPIO_PORT_LED_RED,GPIO_PIN_LED_RED,GPIO_PIN_RESET);
        }
        break;
        case LED_GREEN:
        {
            HAL_GPIO_WritePin(GPIO_PORT_LED_GREEN,GPIO_PIN_LED_GREEN,GPIO_PIN_RESET);
        }
        break;
        case LED_ORANGE:
        {
            HAL_GPIO_WritePin(GPIO_PORT_LED_RED,GPIO_PIN_LED_RED,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIO_PORT_LED_GREEN,GPIO_PIN_LED_GREEN,GPIO_PIN_RESET);
        }
        break;
        default: break;
    }
}
/**
* @brief 		LEDx_Toggle
* @param[in]	led
* @param[out]	none
* @retval		none
*/
void LEDx_Toggle(LED_T ledColour)
{
    switch(ledColour)
    {
        case LED_RED:
        {
            HAL_GPIO_TogglePin(GPIO_PORT_LED_RED,GPIO_PIN_LED_RED);
        }
        break;
        case LED_GREEN:
        {
            HAL_GPIO_TogglePin(GPIO_PORT_LED_GREEN,GPIO_PIN_LED_GREEN);
        }
        break;
        case LED_ORANGE:
        {
            HAL_GPIO_TogglePin(GPIO_PORT_LED_RED,GPIO_PIN_LED_RED);
            HAL_GPIO_TogglePin(GPIO_PORT_LED_GREEN,GPIO_PIN_LED_GREEN);
        }
        break;
        default: break;
    }
}
/**
* @brief 		LEDx_DeInit
* @param[in]	led
* @param[out]	none
* @retval		none
*/
void LEDx_DeInit(LED_T ledColour)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    switch(ledColour)
    {
        case LED_RED:
        {
            /* Enable the Led GPIO CLK */
            LED_RED_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in Lower mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull  = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_RED;
            HAL_GPIO_Init(GPIO_PORT_LED_RED, &GPIO_InitStruct);            
        }              
        break;
        case LED_GREEN:
        {
            /* Enable the Led GPIO CLK */
            LED_GREEN_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in Lower mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull  = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_GREEN;
            HAL_GPIO_Init(GPIO_PORT_LED_GREEN, &GPIO_InitStruct);              
        } 
        break;
        case LED_ORANGE:
        {
            /* Enable the Led GPIO CLK */
            LED_RED_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in Lower mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull  = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_RED;
            HAL_GPIO_Init(GPIO_PORT_LED_RED, &GPIO_InitStruct);

            /* Enable the Led GPIO CLK */
            LED_GREEN_GPIO_CLK_ENABLE();
            
            /* Comfigure the Led Pin in Lower mode */
            GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull  = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            GPIO_InitStruct.Pin = GPIO_PIN_LED_GREEN;
            HAL_GPIO_Init(GPIO_PORT_LED_GREEN, &GPIO_InitStruct);              
        }            
        break;
        default:break;            
    }
}

