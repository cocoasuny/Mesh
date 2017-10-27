/**
  ******************************************************************************
  * @file    platform.h
  * @author  CL
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_H_
#define __PLATFORM_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"


/* Debug Switch */
#define APP_TRACE
#define SYS_USAGE_DEBUG
#define CMU_TOP_LEVEL_DEBUG
#define PASSKEY_DEBUG
#define MONITOR_TEMPLATE_DEBUG
#define IMMEDIATELY_MEASURE_DEBUG



/* Shell Switch */
#define SHELL_ENABLE    //Shell使能
#define RTC_SHELL
#define FATFS_SHELL



/* Size of Reception buffer for uart*/
#define RXBUFFERSIZE          1
#define ON                    1
#define OFF                   0

/* Defines related to Clock configuration */
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */
#define DEVICE_SUPPORT_ABILITY 	(0x06)


/******** Task define ********************/
/*  (task priority值越大，任务优先级越高)*/ 
/* Shell Task define */
#define TASK_SHELLCTL_STACK                 1800
#define TASK_SHELLCTL_PRIORITY              1
/* CMU Task define */
#define TASK_CMU_STACK                      500
#define TASK_CMU_PRIORITY                   2
/* Immediately measure Task define */
#define TASK_IMMEDIATELY_MEASURE_STACK      100
#define TASK_IMMEDIATELY_MEASURE_PRIORITY   3
/* SpO2 Task define */
#define TASK_SPO2_STACK                     1500
#define TASK_SPO2_PRIORITY                  4
/* SpO2 original data collection task define */
#define TASK_COLLECTION_STACK               256
#define TASK_COLLECTION_PRIORITY            5
/* Monitor Template Task define */
#define TASK_MONITOR_TEMPLATE_STACK         512
#define TASK_MONITOR_TEMPLATE_PRIORITY      6

/* Exit priority define */
#define RTC_ALARM_EXIT_PREPRIORITY               2
#define RTC_ALARM_EXIT_SUBPRIORITY               0
#define RTC_WAKE_UP_EXIT_PREPRIORITY             2
#define RTC_WAKE_UP_EXIT_SUBPRIORITY             0



/* platform interface define -------------------------------------------------*/
/* Led define */
#define LED_RED_GPIO_CLK_ENABLE                 __HAL_RCC_GPIOC_CLK_ENABLE   //Red Led define 
#define GPIO_PIN_LED_RED                        GPIO_PIN_9
#define GPIO_PORT_LED_RED                       GPIOC

#define LED_GREEN_GPIO_CLK_ENABLE               __HAL_RCC_GPIOB_CLK_ENABLE   //Green Led define 
#define GPIO_PIN_LED_GREEN                      GPIO_PIN_6
#define GPIO_PORT_LED_GREEN                     GPIOB

/* peripheral power define */
#define PERI_POWER_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOC_CLK_ENABLE()
#define GPIO_PIN_PERI_POWER                     GPIO_PIN_0
#define GPIO_PORT_PERI_POWER                    GPIOC


/* Flash GPIO define */
// Power Pin define 
#define FLASH_POWER_PORT_CLK_ENABLE()           __HAL_RCC_GPIOE_CLK_ENABLE()
#define FLASH_POWER_PORT_CLK_DISABLE()          __HAL_RCC_GPIOE_CLK_DISABLE()
#define GPIO_PIN_FLASH_POWER                    GPIO_PIN_15
#define GPIO_PORT_FLASH_POWER                   GPIOE

// SPI GPIO define(mx25l06e和nrf51822共用同一个SPI)
#define FLASH_SPI                               SPI3
#define FLASH_SPI_CLK_ENABLE()                  __HAL_RCC_SPI3_CLK_ENABLE()
#define FLASH_SPI_DMA_CLK_ENABLE()              __HAL_RCC_DMA2_CLK_ENABLE()
#define FLASH_SPI_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
#define FLASH_SPI_MISO_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()
#define FLASH_SPI_MOSI_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()

#define FLASH_SPI_FORCE_RESET()                 __HAL_RCC_SPI3_FORCE_RESET()
#define FLASH_SPI_RELEASE_RESET()               __HAL_RCC_SPI3_RELEASE_RESET()

#define FLASH_SPI_SCK_PIN                       GPIO_PIN_10
#define FLASH_SPI_SCK_PORT                      GPIOC
#define FLASH_SPI_SCK_AF                        GPIO_AF6_SPI3
#define FLASH_SPI_MISO_PIN                      GPIO_PIN_11
#define FLASH_SPI_MISO_PORT                     GPIOC
#define FLASH_SPI_MISO_AF                       GPIO_AF6_SPI3
#define FLASH_SPI_MOSI_PIN                      GPIO_PIN_12
#define FLASH_SPI_MOSI_PORT                     GPIOC
#define FLASH_SPI_MOSI_AF                       GPIO_AF6_SPI3

// Flash SPI CS Pin define 
#define FLASH_SPI_CS_PORT                       GPIOB
#define FLASH_SPI_CS_PIN                        GPIO_PIN_5
#define FLASH_SPI_CS_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOB_CLK_ENABLE()

// Flash SPI DMA define 
#define FLASH_SPI_TX_DMA_CHANNEL                DMA2_Channel2
#define FLASH_SPI_RX_DMA_CHANNEL                DMA2_Channel1

#define FLASH_SPI_TX_DMA_REQUEST                DMA_REQUEST_3
#define FLASH_SPI_RX_DMA_REQUEST                DMA_REQUEST_3

#define FLASH_SPI_DMA_TX_IRQn                   DMA2_Channel2_IRQn
#define FLASH_SPI_DMA_RX_IRQn                   DMA2_Channel1_IRQn

#define FLASH_SPI_DMA_TX_PREPRIORITY            1
#define FLASH_SPI_DMA_TX_SUBPRIORITY            0

#define FLASH_SPI_DMA_RX_PREPRIORITY            1
#define FLASH_SPI_DMA_RX_SUBPRIORITY            0

/* Bluetooth commmunication GPIO and SPI define */
// BLE CMU Control Pin define 
#define CMU_SYNC_PIN_CLK_ENABLE()               __HAL_RCC_GPIOC_CLK_ENABLE()
#define CMU_SYNC_PIN                            GPIO_PIN_5
#define CMU_SYNC_PORT                           GPIOC
#define CMU_SYNC_EXIT_IRQn                      EXTI9_5_IRQn
#define CMU_SYNC_EXIT_PREPRIORITY               2
#define CMU_SYNC_EXIT_SUBPRIORITY               0

#define CMU_REQN_PIN_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define CMU_REQN_PIN                            GPIO_PIN_15
#define CMU_REQN_PORT                           GPIOA
#define CMU_REQN_EXIT_IRQn                      EXTI15_10_IRQn
#define CMU_REQN_EXIT_PREPRIORITY               2
#define CMU_REQN_EXIT_SUBPRIORITY               0

// BLE CMU SPI CS Pin define (与FLASH共用SPI3)
#define CMU_SPI_CS_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define CMU_SPI_CS_PIN                          GPIO_PIN_3
#define CMU_SPI_CS_PORT                         GPIOB


/**************** bma250e SPI and GPIO define ************************************/
// bma250e SPI CS Pin define 
#define BMA250E_SPI_CS_PORT                     GPIOB
#define BMA250E_SPI_CS_PIN                      GPIO_PIN_8
#define BMA250E_SPI_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()

#define BMA250E_SPI                               SPI1
#define BMA250E_SPI_CLK_ENABLE()                  __HAL_RCC_SPI1_CLK_ENABLE()
#define BMA250E_SPI_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define BMA250E_SPI_MISO_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define BMA250E_SPI_MOSI_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()

#define BMA250E_SPI_FORCE_RESET()                 __HAL_RCC_SPI1_FORCE_RESET()
#define BMA250E_SPI_RELEASE_RESET()               __HAL_RCC_SPI1_RELEASE_RESET()

#define BMA250E_SPI_SCK_PIN                       GPIO_PIN_5
#define BMA250E_SPI_SCK_PORT                      GPIOA
#define BMA250E_SPI_SCK_AF                        GPIO_AF5_SPI1
#define BMA250E_SPI_MISO_PIN                      GPIO_PIN_6
#define BMA250E_SPI_MISO_PORT                     GPIOA
#define BMA250E_SPI_MISO_AF                       GPIO_AF5_SPI1
#define BMA250E_SPI_MOSI_PIN                      GPIO_PIN_7
#define BMA250E_SPI_MOSI_PORT                     GPIOA
#define BMA250E_SPI_MOSI_AF                       GPIO_AF5_SPI1


/**************** afe4403 SPI and GPIO define ************************************/
// afe4403 SPI CS Pin define 
#define AFE4403_SPI_CS_PORT                     GPIOA
#define AFE4403_SPI_CS_PIN                      GPIO_PIN_4
#define AFE4403_SPI_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()

#define AFE4403_SPI                               SPI1
#define AFE4403_SPI_CLK_ENABLE()                  __HAL_RCC_SPI1_CLK_ENABLE()
#define AFE4403_SPI_SCK_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define AFE4403_SPI_MISO_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define AFE4403_SPI_MOSI_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()

#define AFE4403_SPI_FORCE_RESET()                 __HAL_RCC_SPI1_FORCE_RESET()
#define AFE4403_SPI_RELEASE_RESET()               __HAL_RCC_SPI1_RELEASE_RESET()

#define AFE4403_SPI_SCK_PIN                       GPIO_PIN_5
#define AFE4403_SPI_SCK_PORT                      GPIOA
#define AFE4403_SPI_SCK_AF                        GPIO_AF5_SPI1
#define AFE4403_SPI_MISO_PIN                      GPIO_PIN_6
#define AFE4403_SPI_MISO_PORT                     GPIOA
#define AFE4403_SPI_MISO_AF                       GPIO_AF5_SPI1
#define AFE4403_SPI_MOSI_PIN                      GPIO_PIN_7
#define AFE4403_SPI_MOSI_PORT                     GPIOA
#define AFE4403_SPI_MOSI_AF                       GPIO_AF5_SPI1

//afe4403 Power Pin define 
#define AFE4403_POWER_PORT                        GPIOA
#define AFE4403_POWER_PIN                         GPIO_PIN_9
#define AFE4403_POWER_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOA_CLK_ENABLE()

//afe4403 RDY Pin define
#define AFE4403_RDY_PORT                          GPIOA
#define AFE4403_RDY_PIN                           GPIO_PIN_11
#define AFE4403_RDY_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()

//afe4403 PDNZ Pin define 
#define AFE4403_PDNZ_PORT                           GPIOD
#define AFE4403_PDNZ_PIN                            GPIO_PIN_5
#define AFE4403_PDNZ_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE()

//afe4403 Reset Pin define 
#define AFE4403_RESET_PORT                          GPIOC
#define AFE4403_RESET_PIN                           GPIO_PIN_6
#define AFE4403_RESET_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()


/**********battery manage gpio define ********************************************/
// battery charge control gpio define 
#define BAT_CHARGE_CTL_PORT                     GPIOE                                                                             
#define BAT_CHARGE_CTL_PIN                      GPIO_PIN_13
#define BAT_CHARGE_CTL_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOE_CLK_ENABLE()

/*********************Stm32 communicate with nRF51822 protocol define*************/
#define FRAME_CONNECT_STATE                 0x01   //设备连接状态标志帧
#define FRAME_DEVICEINFORM                  0x02   //设备详情帧
#define FRAME_IMMEDIATELY_MEASURE_PARAM     0x03   //数据采集帧，标示采集哪些参数
#define FRAME_IMMEDIATELY_MEASURE_DATA      0x04   //立即测量数据实时更新帧
#define FRAME_SYNC_DATA_SWITCH              0x05   //触发数据同步帧
#define FRAME_SYNC_DATA                     0x0D   //同步数据帧
#define FRAME_FW_UPDATE_RESPONSE            0x07   //固件升级结果返回帧
#define FRAME_MONITER_TEMPLATE_SET          0x08   //监护方案设置帧
#define FRAME_MONITER_TEMPLATE_RESULT       0x09   //监护方案设置结果帧
#define FRAME_ALARM                         0x0A   //告警指示帧
#define FRAME_LIMIT_SET                     0x0B   //参数门限设置帧
#define FRAME_TIME_SYNC                     0x0C   //时间同步帧
#define FRAME_PASSKEY                       0x0E   //密码验证帧
#define FRAME_BAT_LEVEL_TRANSLATE           0x0F   //电池电量发送帧
#define FRAME_DEVICE_LOG                    0x10   //日志打印帧
#define FRAME_FW_UPDATE                     0x11   //固件升级帧
#define FRAME_FW_UPDATE_CTLA                0x12   //固件升级数据传输控制帧，STM32To51822
#define FRAME_FW_UPDATE_CTLB                0x13   //固件升级数据传输控制帧，51822ToSTM32
#define FRAME_BASIC_INFO                    0x14   //用户基本信息数据帧
#define FRAME_UPDATE_SPORT_INFO             0x15   //上传用户基本信息数据帧
#define FRAME_DEVICE_INFO_REQ               0x16   //发送设备信息数据帧
#define FRAME_UPDATE_BLE_RATE               0x17   //更新蓝牙速率控制帧
#define FRAME_FACTORY_HW_TEST               0x18   //工厂硬件测试控制帧
#define FRAME_BACK_FACTORY_MODE             0x19   //返回工厂模式控制帧
#define Frame_ANCS_INCOMINGCALL             0x20   //ANCS服务控制帧



/***************************参数ID定义**********************************************/
#define SAMPLE_ID_STOP                     			 0x0000
#define SAMPLE_ID_TEMPERATURE                        0x0001
#define SAMPLE_ID_HR                                 0x0002
#define SAMPLE_ID_SPO2                               0x0004

#define TEMPERATURE_ID                               0x01   //测量参数ID(不同于设备支持能力)
#define HR_ID                                        0x02
#define SPO2_ID                                      0x03


/**********************Data Syn size define **********************************/
#define SYNC_DATA_SIZE_RD                           (230)
#define EEPROMReadMaxData  		                    (230)








#endif /* __PLATFORM_H_ */


