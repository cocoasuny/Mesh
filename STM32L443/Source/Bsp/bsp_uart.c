/**
  ****************************************************************************************
  * @file    bsp_uart.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   bsp_uart.c,实现串口（USART3）输入输出，用于调试使用
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"


/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;
uint8_t  g_aRxBuffer[RXBUFFERSIZE] = {0};


/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  bsp_uart_init 
  * @note   初始化串口，usart3
  * @param  None
  * @retval HAL status
  */
HAL_StatusTypeDef bsp_uart_init(void)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    
    /*##-1- Configure the UART peripheral ######################################*/
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
    ret = HAL_UART_Init(&huart3);
    if (ret != HAL_OK)
    {
        App_Error_Check(ret);
    }  
    
    /*##-2- Put UART peripheral in reception process ###########################*/  
    if(HAL_UART_Receive_IT(&huart3, (uint8_t *)g_aRxBuffer, RXBUFFERSIZE) != HAL_OK)
    {
        App_Error_Check(ret);
    }      
    
    return ret;
}

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);

    if (status != HAL_OK) {
        //while (1);
        return 0;
    }
    return ch;
}


/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

