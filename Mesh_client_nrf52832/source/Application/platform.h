/**
  ****************************************************************************************
  * @file    platform.h
  * @author  Jason
  * @version V1.0.0
  * @date    2017-7-27
  * @brief   the hardware platform define 
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM__
#define __PLATFORM__


/* debug switch define */
#define LOG_CALLBACK_DEFAULT log_callback_stdout

/* gpio define for uart */
#define RX_PIN_NUMBER               23
#define TX_PIN_NUMBER               24
#define CTS_PIN_NUMBER              17
#define RTS_PIN_NUMBER              15
#define HWFC                        false
    
#ifndef UART_TX_BUF_SIZE
    #define UART_TX_BUF_SIZE 1024         /**< UART TX buffer size. */
#endif
#ifndef UART_RX_BUF_SIZE
    #define UART_RX_BUF_SIZE 1            /**< UART RX buffer size. */
#endif
    


#endif // __PLATFORM__


/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/


