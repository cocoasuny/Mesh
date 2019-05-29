/**
  ****************************************************************************************
  * @file    app_mesh_implement.c
  * @author  Jason
  * @version V1.0.0
  * @date    2019-5-10
  * @brief   the top level implement of ble mesh
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "app_freertos.h"




/**@brief Thread for handling the ble mesh.
 *
 * @details This thread is responsible for processing top level ble mesh
 *
 * @param[in]   arg   Pointer used for passing some arbitrary information (context) from the
 *                    osThreadCreate() call to the thread.
 */
void ble_mesh_top_thread(void * arg)
{
    UNUSED_PARAMETER(arg);
	
	
	while(1)
	{
	
	}
}



















/************************ (C) COPYRIGHT 2019 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/




