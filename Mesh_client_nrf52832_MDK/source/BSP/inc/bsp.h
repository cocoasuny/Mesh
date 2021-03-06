/**
  ****************************************************************************************
  * @file    bsp.h
  * @author  Jason
  * @version V1.0.0
  * @date    2019-5-10
  * @brief   the header of bsp.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#ifndef __BSP__
#define __BSP__

#include "stdint.h"
#include <stdarg.h>

/* function declares */
void clock_init(void);
void log_init_entity(void);
void log_callback_default(uint32_t dbg_level, const char * p_filename, uint16_t line,	\
		uint32_t timestamp, const char * format, va_list arguments);


#endif  /* __BSP__ */

/************************ (C) COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



