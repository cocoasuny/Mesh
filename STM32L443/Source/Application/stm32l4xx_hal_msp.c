/**
  ******************************************************************************
  * File Name          : stm32l4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

extern void Error_Handler(void);
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
    HAL_StatusTypeDef         ret = HAL_ERROR;
     
    if(hrtc->Instance==RTC)
    {
        /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
        /* To change the source clock of the RTC feature (LSE, LSI), You have to:
            - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
            - Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
            configure the RTC clock source (to be done once after reset).
            - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
            __HAL_RCC_BACKUPRESET_RELEASE().
            - Configure the needed RTc clock source 
        */
        __HAL_RCC_PWR_CLK_ENABLE();
        HAL_PWR_EnableBkUpAccess();


        /*##-2- Configue LSE as RTC clock soucre ###################################*/
        RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
        RCC_OscInitStruct.LSEState = RCC_LSE_ON;
        ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
        if(ret != HAL_OK)
        { 
            App_Error_Check(ret);
        }

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
        ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
        if(ret != HAL_OK)
        { 
            App_Error_Check(ret);
        }

        /*##-3- Enable RTC peripheral Clocks #######################################*/
        /* Enable RTC Clock */
        __HAL_RCC_RTC_ENABLE();
        
        /*##-4- Configure the NVIC for RTC Alarm ###################################*/
        HAL_NVIC_SetPriority(RTC_Alarm_IRQn, RTC_ALARM_EXIT_PREPRIORITY, RTC_ALARM_EXIT_SUBPRIORITY);
        HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);        
    }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
    if(hrtc->Instance==RTC)
    {
      /*##-1- Reset peripherals ##################################################*/
      __HAL_RCC_RTC_DISABLE();

      /*##-2- Disables the PWR Clock and Disables access to the backup domain ###################################*/
      HAL_PWR_DisableBkUpAccess();
      __HAL_RCC_PWR_CLK_DISABLE();
    }
}
/**
  * @brief  HAL_UART_MspInit 
  * @note   初始化usart3串口GPIO
  * @param  UART_HandleTypeDef* huart
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOD_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN USART3_MspInit 1 */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE END USART3_MspInit 1 */
  }

}
/**
  * @brief  HAL_UART_MspDeInit
  * @note   usart3 GPIO deinit
  * @param  UART_HandleTypeDef* huart
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PD8     ------> USART3_TX
    PD9     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9);

  }
  /* USER CODE BEGIN USART3_MspDeInit 1 */
  HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE END USART3_MspDeInit 1 */

}

/**
  * @brief  HAL_SPI_MspInit
  * @note   SPI GPIO Init
  * @param  SPI_HandleTypeDef* spiHandle
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    if(spiHandle->Instance == FLASH_SPI)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        FLASH_SPI_SCK_GPIO_CLK_ENABLE();
        FLASH_SPI_MISO_GPIO_CLK_ENABLE();
        FLASH_SPI_MOSI_GPIO_CLK_ENABLE();
        
        /* Enable FLASH SPI clock */
        FLASH_SPI_CLK_ENABLE();
        
        /* Enable DMA clock */
        FLASH_SPI_DMA_CLK_ENABLE();

        /**SPI3 GPIO Configuration    
            PC12     ------> SPI3_MOSI
            PC10     ------> SPI3_SCK
            PC11     ------> SPI3_MISO 
        */
        
        /*##-2- Configure peripheral GPIO ##########################################*/  
        /* SPI SCK GPIO pin configuration  */
        GPIO_InitStruct.Pin = FLASH_SPI_SCK_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = FLASH_SPI_SCK_AF;
        HAL_GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStruct);
        
        /* SPI MISO GPIO pin configuration  */
        GPIO_InitStruct.Pin = FLASH_SPI_MISO_PIN;
        GPIO_InitStruct.Alternate = FLASH_SPI_MISO_AF;
        HAL_GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStruct);

        /* SPI MOSI GPIO pin configuration  */
        GPIO_InitStruct.Pin = FLASH_SPI_MOSI_PIN;
        GPIO_InitStruct.Alternate = FLASH_SPI_MOSI_AF;
        HAL_GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStruct);        

        /*##-3- Configure the DMA ##################################################*/
        /* Configure the DMA handler for Transmission process */
        hdma_spi3_rx.Instance                   = FLASH_SPI_RX_DMA_CHANNEL;
        hdma_spi3_rx.Init.Request               = FLASH_SPI_RX_DMA_REQUEST;
        hdma_spi3_rx.Init.Direction             = DMA_PERIPH_TO_MEMORY;
        hdma_spi3_rx.Init.PeriphInc             = DMA_PINC_DISABLE;
        hdma_spi3_rx.Init.MemInc                = DMA_MINC_ENABLE;
        hdma_spi3_rx.Init.PeriphDataAlignment   = DMA_PDATAALIGN_BYTE;
        hdma_spi3_rx.Init.MemDataAlignment      = DMA_MDATAALIGN_BYTE;
        hdma_spi3_rx.Init.Mode                  = DMA_NORMAL;
        hdma_spi3_rx.Init.Priority              = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_spi3_rx) != HAL_OK)
        {
            App_Error_Check(HAL_ERROR);
        }
        /* Associate the initialized DMA handle to the the SPI handle */
        __HAL_LINKDMA(spiHandle,hdmarx,hdma_spi3_rx);

        hdma_spi3_tx.Instance                   = FLASH_SPI_TX_DMA_CHANNEL;
        hdma_spi3_tx.Init.Request               = FLASH_SPI_TX_DMA_REQUEST;
        hdma_spi3_tx.Init.Direction             = DMA_MEMORY_TO_PERIPH;
        hdma_spi3_tx.Init.PeriphInc             = DMA_PINC_DISABLE;
        hdma_spi3_tx.Init.MemInc                = DMA_MINC_ENABLE;
        hdma_spi3_tx.Init.PeriphDataAlignment   = DMA_PDATAALIGN_BYTE;
        hdma_spi3_tx.Init.MemDataAlignment      = DMA_MDATAALIGN_BYTE;
        hdma_spi3_tx.Init.Mode                  = DMA_NORMAL;
        hdma_spi3_tx.Init.Priority              = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_spi3_tx) != HAL_OK)
        {
            App_Error_Check(HAL_ERROR);
        }
        /* Associate the initialized DMA handle to the the SPI handle */
        __HAL_LINKDMA(spiHandle,hdmatx,hdma_spi3_tx);

        /*##-4- Configure the NVIC for DMA #########################################*/ 
        /* NVIC configuration for DMA transfer complete interrupt (SPI3_TX) */
        HAL_NVIC_SetPriority(FLASH_SPI_DMA_TX_IRQn, FLASH_SPI_DMA_TX_PREPRIORITY, FLASH_SPI_DMA_TX_SUBPRIORITY);
        HAL_NVIC_EnableIRQ(FLASH_SPI_DMA_TX_IRQn);
        
        /* NVIC configuration for DMA transfer complete interrupt (SPI1_RX) */
        HAL_NVIC_SetPriority(FLASH_SPI_DMA_RX_IRQn, FLASH_SPI_DMA_RX_PREPRIORITY, FLASH_SPI_DMA_RX_SUBPRIORITY);
        HAL_NVIC_EnableIRQ(FLASH_SPI_DMA_RX_IRQn);
    }
    
    if(spiHandle->Instance == BMA250E_SPI)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        BMA250E_SPI_SCK_GPIO_CLK_ENABLE();
        BMA250E_SPI_MISO_GPIO_CLK_ENABLE();
        BMA250E_SPI_MOSI_GPIO_CLK_ENABLE();
        
        /* Enable BMA250E SPI clock */
        BMA250E_SPI_CLK_ENABLE();
        
        /**SPI1 GPIO Configuration    
            PA7     ------> SPI1_MOSI
            PA5     ------> SPI1_SCK
            PA6     ------> SPI1_MISO 
        */
        
        /*##-2- Configure peripheral GPIO ##########################################*/  
        /* SPI SCK GPIO pin configuration  */
        GPIO_InitStruct.Pin = BMA250E_SPI_SCK_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = BMA250E_SPI_SCK_AF;
        HAL_GPIO_Init(BMA250E_SPI_SCK_PORT, &GPIO_InitStruct);
        
        /* SPI MISO GPIO pin configuration  */
        GPIO_InitStruct.Pin = BMA250E_SPI_MISO_PIN;
        GPIO_InitStruct.Alternate = BMA250E_SPI_MISO_AF;
        HAL_GPIO_Init(BMA250E_SPI_MISO_PORT, &GPIO_InitStruct);

        /* SPI MOSI GPIO pin configuration  */
        GPIO_InitStruct.Pin = BMA250E_SPI_MOSI_PIN;
        GPIO_InitStruct.Alternate = BMA250E_SPI_MOSI_AF;
        HAL_GPIO_Init(BMA250E_SPI_MOSI_PORT, &GPIO_InitStruct);        
    }   
    if(spiHandle->Instance == AFE4403_SPI)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        AFE4403_SPI_SCK_GPIO_CLK_ENABLE();
        AFE4403_SPI_MISO_GPIO_CLK_ENABLE();
        AFE4403_SPI_MOSI_GPIO_CLK_ENABLE();
        
        /* Enable AFE4403 SPI clock */
        AFE4403_SPI_CLK_ENABLE();
        
        /**SPI1 GPIO Configuration    
            PA7     ------> SPI1_MOSI
            PA5     ------> SPI1_SCK
            PA6     ------> SPI1_MISO 
        */
        
        /*##-2- Configure peripheral GPIO ##########################################*/  
        /* SPI SCK GPIO pin configuration  */
        GPIO_InitStruct.Pin = AFE4403_SPI_SCK_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = AFE4403_SPI_SCK_AF;
        HAL_GPIO_Init(AFE4403_SPI_SCK_PORT, &GPIO_InitStruct);
        
        /* SPI MISO GPIO pin configuration  */
        GPIO_InitStruct.Pin = AFE4403_SPI_MISO_PIN;
        GPIO_InitStruct.Alternate = AFE4403_SPI_MISO_AF;
        HAL_GPIO_Init(AFE4403_SPI_MISO_PORT, &GPIO_InitStruct);

        /* SPI MOSI GPIO pin configuration  */
        GPIO_InitStruct.Pin = AFE4403_SPI_MOSI_PIN;
        GPIO_InitStruct.Alternate = AFE4403_SPI_MOSI_AF;
        HAL_GPIO_Init(AFE4403_SPI_MOSI_PORT, &GPIO_InitStruct);        
    }        
}
/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == FLASH_SPI)
    {   
        /*##-1- Reset peripherals ##################################################*/
        FLASH_SPI_FORCE_RESET();
        FLASH_SPI_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Deconfigure SPI SCK */
        HAL_GPIO_DeInit(FLASH_SPI_SCK_PORT, FLASH_SPI_SCK_PIN);
        /* Deconfigure SPI MISO */
        HAL_GPIO_DeInit(FLASH_SPI_MISO_PORT, FLASH_SPI_MISO_PIN);
        /* Deconfigure SPI MOSI */
        HAL_GPIO_DeInit(FLASH_SPI_MOSI_PORT, FLASH_SPI_MOSI_PIN);

        /*##-3- Disable the DMA ####################################################*/
        /* De-Initialize the DMA associated to transmission process */
        HAL_DMA_DeInit(&hdma_spi3_tx);
        /* De-Initialize the DMA associated to reception process */
        HAL_DMA_DeInit(&hdma_spi3_rx);

        /*##-4- Disable the NVIC for DMA ###########################################*/
        HAL_NVIC_DisableIRQ(FLASH_SPI_DMA_TX_IRQn);
        HAL_NVIC_DisableIRQ(FLASH_SPI_DMA_RX_IRQn);
    }
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
