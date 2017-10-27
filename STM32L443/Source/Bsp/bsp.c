/**
  ****************************************************************************************
  * @file    bsp.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-18
  * @brief   bsp.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Private variables ---------------------------------------------------------*/
const char Dev_Msg[] =
	"/*********************************  This is to be Done *******************************/\r\n"
    "/*                                                                                   */\r\n"
    "/*                                                                                   */\r\n"
    "/*                              BSP Init Complate,Start...                           */\r\n"
#ifdef SHELL_ENABLE
    "/*                              Pls enter help for more usages                       */\r\n"
#endif
	"\r\n";

 
/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  bsp init 
  * @note   This function is called  automatically at the beginning of program 
  * @param  None
  * @retval HAL status
  */
HAL_StatusTypeDef bsp_init(void)
{
    uint8_t                 bspInitStatus = true;
    RTC_DateTypeDef         date_s;
    RTC_TimeTypeDef         rtc_time; 
    uint32_t                flashID = 0;
    
    /* uart & shell init */
    bsp_uart_init();

    /* rtc init */
    if(bsp_rtc_init() != HAL_OK)
    {
        printf("rtc init err\r\n");
        bspInitStatus = false;
    }
    
    /* peri power on */
    peripheral_powerCtl(ON);
    mx25_chip_id_get(&flashID);
    HAL_Delay(100);
    mx25_chip_id_get(&flashID);
    if(flashID != FLASH_ID)
    {
        printf("Flash ID:0x%04x\r\n",flashID);
        bspInitStatus = false;
    }
    /* store manage init */
    store_manage_init();
    
    /* CMU Init */
    cmu_init();
    
    /* bma250e init */
    if(bma250e_init() != HAL_OK)
    {
        bspInitStatus = false;
    }
 
    if(bspInitStatus == false)
    {
        printf("***********WARMING: There are Errs In BSP Init **************************\r\n");
    }
    else
    {
        printf("%s",Dev_Msg);
        calendar_get(&date_s,&rtc_time);
        printf("\r\nThe Current Time:  %02d-%02d-%02d %d %0.2d:%0.2d:%0.2d\r\n",
                                        2000 + date_s.Year,date_s.Month, date_s.Date,date_s.WeekDay, 
                                        rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds); 
    }
    return HAL_OK;
}

/**
  * @brief  mco_outPutLSE 
  * @note   LSE输出控制，控制nRF51822时钟
  * @param  status: ON/OFF
  * @retval None
  */
void mco_outputLSE(bool status)
{

    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    
    if(status == ON)
    {
        /*Configure GPIO pin : PA8 */
        GPIO_InitStruct.Pin = GPIO_PIN_8;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        
    }
    else
    {
        /*Configure GPIO pin : PA8 */
        GPIO_InitStruct.Pin = GPIO_PIN_8;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);          
    }
}
/**
  * @brief  peripheral_powerCtl 
  * @note   peripheral device power contral
  * @param  power: ON/OFF
  * @retval None
  */
void peripheral_powerCtl(uint8_t power)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    PERI_POWER_GPIO_CLK_ENABLE();
    
    if(power == ON)
    {
        GPIO_InitStruct.Pin = GPIO_PIN_PERI_POWER;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIO_PORT_PERI_POWER, &GPIO_InitStruct);
        
        /* set power pin high to open the power */
        HAL_GPIO_WritePin(GPIO_PORT_PERI_POWER,GPIO_PIN_PERI_POWER,GPIO_PIN_SET); 
    }
    else
    {
        GPIO_InitStruct.Pin = GPIO_PIN_PERI_POWER;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIO_PORT_PERI_POWER, &GPIO_InitStruct);
        
        /* reset power pin low to shut down the power */
        HAL_GPIO_WritePin(GPIO_PORT_PERI_POWER,GPIO_PIN_PERI_POWER,GPIO_PIN_RESET);         
    }
}

/**
  * @brief  app_error_handler 
  * @note   uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name
  * @param  None
  * @retval HAL status
  */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
	printf("\r\nErr = 0x%x,file = %s,line = %d.\r\n",error_code,p_file_name,line_num);
}

/**
  * @brief  软件复位系统
  * @param  None
  * @retval None    
  */
void Sys_SoftReset(void)
{
	NVIC_SystemReset();
}

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

