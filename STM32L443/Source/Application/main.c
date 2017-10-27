/**
  ****************************************************************************************
  * @file    main.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   Main program body
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"


/* global variables ---------------------------------------------------------*/
osThreadId                  defaultTaskHandle;
xTaskHandle                 xHandleShellCtl;
xTaskHandle                 xHandleCMU;
xTaskHandle                 xHandleSpO2;
xTaskHandle                 xHandlecollection;
xTaskHandle                 xHandleMonitorTemplate;
DEVICE_INFORMATION_T        g_DeviceInfomation;
PASSKEY_T                   g_PassKey;
#ifdef SYS_USAGE_DEBUG
volatile uint32_t ulHighFrequencyTimerTicks = 0UL;
#endif

/* message variables ---------------------------------------------------------*/
osMessageQId    spo2QueueHandle;
osMessageQId    collectionQueueHandle;
QueueHandle_t   cmuEventQueue = NULL;
QueueHandle_t   immediatelyMeasureEventQueue = NULL;
QueueHandle_t	monitorTemplateEvevtQueue = NULL;

/* semaphore variables -------------------------------------------------------*/
SemaphoreHandle_t uartRxBinarySemaphore = NULL;

/* private variables ---------------------------------------------------------*/
const uint8_t DevREV[3]="4.1";  //HW board version
const uint8_t FWDEV[3]="400";   //STM32L FW version

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
void StartDefaultTask(void const * argument);
static void app_event_queue_creat(void);
static void app_semaphore_creat(void);


int main(void)
{
    BaseType_t  ret = pdFAIL;
    /* MCU Configuration----------------------------------------------------------*/
    /* Device Information define */
    g_DeviceInfomation.DevSupportCap = DEVICE_SUPPORT_ABILITY + 0x30;
    g_DeviceInfomation.pDevREV = DevREV;
	g_DeviceInfomation.pFWREV = FWDEV;
    g_DeviceInfomation.isBleConnect = false;

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    bsp_init();
    
    /* Create the Event Queue for tasks */
    app_event_queue_creat();
	
	app_semaphore_creat();
    
    /* Create the thread(s) */
#ifdef SHELL_ENABLE    
    /* Shell Task Creat */
    ret = xTaskCreate(shellCtlTask,"shell",TASK_SHELLCTL_STACK,NULL,TASK_SHELLCTL_PRIORITY,&xHandleShellCtl);
    if(ret != pdPASS)   
    {        
        App_Error_Check(ret); 
    }        
#endif

    /* communication with nRF51822 Task Creat */
    ret = xTaskCreate(CMU_Task_Handler, "CMU",TASK_CMU_STACK,NULL,TASK_CMU_PRIORITY,&xHandleCMU);
    if(ret != pdPASS)   
    {        
        App_Error_Check(ret); 
    }    
    
    /* immediately measure task creat */
    ret = xTaskCreate(immediately_measure_task_handler,"IMD Measure",TASK_IMMEDIATELY_MEASURE_STACK,    
                        NULL,TASK_IMMEDIATELY_MEASURE_PRIORITY,&xHandleCMU
                    );
    if(ret != pdPASS)   
    {        
        App_Error_Check(ret); 
    }    
    
    /* spo2 task creat */
    ret = xTaskCreate(spo2Task,"spo2",TASK_SPO2_STACK,NULL,TASK_SPO2_PRIORITY,&xHandleSpO2);
    if(ret != pdPASS)   
    {        
        App_Error_Check(ret); 
    }  
    
    /* SpO2 original data collection task creat */
    ret = xTaskCreate(collectionTask,"collect",TASK_COLLECTION_STACK,NULL,TASK_COLLECTION_PRIORITY,&xHandlecollection);
    if(ret != pdPASS)   
    {        
        App_Error_Check(ret); 
    }

    /* Monitor template task creat */
    ret = xTaskCreate(monitor_template_task_handler,"MT",TASK_MONITOR_TEMPLATE_STACK,NULL,TASK_MONITOR_TEMPLATE_PRIORITY,
                        &xHandleMonitorTemplate);
    if(ret != pdPASS)   
    {        
        App_Error_Check(ret); 
    }    
    
    /* message creat */
    /* definition and creation of spo2Queue */
    osMessageQDef(spo2Queue, 4, uint16_t);
    spo2QueueHandle = osMessageCreate(osMessageQ(spo2Queue), NULL);

    /* definition and creation of storeQueue */
    osMessageQDef(collectionQueue, 4, uint16_t);
    collectionQueueHandle = osMessageCreate(osMessageQ(collectionQueue), NULL);    
    
    /* Start scheduler */
    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler */
    printf("Scheduler Err\r\n");
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    }
    /* USER CODE END 3 */
}
/**
  * @brief  app_event_queue_creat
  * @note   creat event queue for tasks
  * @param  None
  * @retval None
  */
static void app_event_queue_creat(void)
{
    /* creat event queue for CMU task */
    cmuEventQueue = xQueueCreate(CMU_QUEUE_LENGTH,sizeof(CMU_MSG_T));
    if(cmuEventQueue == NULL)
    {
        App_Error_Check(HAL_ERROR);
    }

    /* creat event queue for immediate measure task */
    immediatelyMeasureEventQueue = xQueueCreate(IMMEDIATELY_MEASURE_QUEUE_LENGTH,sizeof(IMMEDIATELY_MEASURE_MSG_T));
    if(immediatelyMeasureEventQueue == NULL)
    {
        App_Error_Check(HAL_ERROR);
    }
	
	/* creat event queue for monitor template task */
	monitorTemplateEvevtQueue = xQueueCreate(MONITOR_TEMPLATE_QUEUE_LENGTH,sizeof(MONITOR_TEMPLATE_MSG_T));
    if(monitorTemplateEvevtQueue == NULL)
    {
        App_Error_Check(HAL_ERROR);
    }	
}
/**
  * @brief  app_semaphore_creat
  * @note   creat semaphores for tasks
  * @param  None
  * @retval None
  */
static void app_semaphore_creat(void)
{
	/* creat binary semaphore for uart rx */
	uartRxBinarySemaphore = xSemaphoreCreateBinary();
	if(uartRxBinarySemaphore == NULL)
	{
		App_Error_Check(HAL_ERROR);
	}	
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_1);

  __HAL_RCC_PWR_CLK_ENABLE();

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}


/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
    if (htim->Instance == TIM1) 
    {
        HAL_IncTick();
        #ifdef SYS_USAGE_DEBUG
            ulHighFrequencyTimerTicks++; 
        #endif
    }
/* USER CODE BEGIN Callback 1 */

/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/
