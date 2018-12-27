/**
  ****************************************************************************************
  * @file    bsp_cli.h
  * @author  Jason
  * @version V1.0.0
  * @date    2017-7-10
  * @brief   the header of bsp_cli.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "nrf_cli.h"
#include "nrf_cli_uart.h"


/* function declares */
ret_code_t cli_init(void);
ret_code_t cli_start(void);
void cli_process(void);
void log_callback_cli(uint32_t dbg_level, const char * p_filename, uint16_t line,
    uint32_t timestamp, const char * format, va_list arguments);





/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



