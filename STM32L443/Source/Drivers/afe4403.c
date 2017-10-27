/**
  ****************************************************************************************
  * @file    afe4403.c
  * @author  上海团队
  * @version V1.0.0
  * @date    $$$$-$$-$$
  * @brief   driver for afe4403
  ****************************************************************************************
  * @version V1.1.0
  * @author  Jason
  * @date    2016-10-10
  * @brief   update the driver for YWK16-002
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/  
#include "afe4403.h"

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
static HAL_StatusTypeDef afe4403_spi_init(void);
static HAL_StatusTypeDef afe4403_spi_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len);
static void afe4403_cs_low(void);
static void afe4403_cs_high(void);
static void afe4403_ctl_gpio_init(void);
static void drv_afe4403_reg_get(uint8_t v_uRegAdr,uint32_t *v_puRegVal);
static void drv_afe4403_reg_set(uint8_t v_uRegAdr,uint32_t v_uRegVal);
static void afe4403_rdy_gpio_init(void);



/**
  *@brief	afe4403 register init
  *@param[in]	none
  *@retval	none
  */
void drv_afe4403_init(void)
{
    /* afe4403 power on */
    drv_afe4403_power(ON);
    
    /* afe4403 control gpio init */
    afe4403_ctl_gpio_init();
    
	/*reset and powerdown AFE chipset*/
    HAL_GPIO_WritePin(AFE4403_PDNZ_PORT,AFE4403_PDNZ_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AFE4403_RESET_PORT,AFE4403_RESET_PIN,GPIO_PIN_RESET);
    
	/*Delay*/
	HAL_Delay(20);
    
	/*clear the reset & power down*/
	HAL_GPIO_WritePin(AFE4403_PDNZ_PORT,AFE4403_PDNZ_PIN,GPIO_PIN_SET);					//Disable power down
	HAL_GPIO_WritePin(AFE4403_RESET_PORT,AFE4403_RESET_PIN,GPIO_PIN_SET);;		        //Disable reset    
    
    /* Init DRDY Interrupt*/
    afe4403_rdy_gpio_init();
    
    /* afe4403 spi init */
    afe4403_spi_init();
    
	drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0);          
	drv_afe4403_reg_set(AFE4403_LED2STC_ADDRESS,AFE4403_LED2STC);          
	drv_afe4403_reg_set(AFE4403_LED2ENDC_ADDRESS,AFE4403_LED2ENDC);        
	drv_afe4403_reg_set(AFE4403_LED2LEDSTC_ADDRESS,AFE4403_LED2LEDSTC);    
	drv_afe4403_reg_set(AFE4403_LED2LEDENDC_ADDRESS,AFE4403_LED2LEDENDC);  
	drv_afe4403_reg_set(AFE4403_ALED2STC_ADDRESS,AFE4403_ALED2STC);        
	drv_afe4403_reg_set(AFE4403_ALED2ENDC_ADDRESS,AFE4403_ALED2ENDC);      
	drv_afe4403_reg_set(AFE4403_LED1STC_ADDRESS,AFE4403_LED1STC);          
	drv_afe4403_reg_set(AFE4403_LED1ENDC_ADDRESS,AFE4403_LED1ENDC);        
	drv_afe4403_reg_set(AFE4403_LED1LEDSTC_ADDRESS,AFE4403_LED1LEDSTC);    
	drv_afe4403_reg_set(AFE4403_LED1LEDENDC_ADDRESS,AFE4403_LED1LEDENDC);  
	drv_afe4403_reg_set(AFE4403_ALED1STC_ADDRESS,AFE4403_ALED1STC);        
	drv_afe4403_reg_set(AFE4403_ALED1ENDC_ADDRESS,AFE4403_ALED1ENDC);      
	drv_afe4403_reg_set(AFE4403_LED2CONVST_ADDRESS,AFE4403_LED2CONVST);    
	drv_afe4403_reg_set(AFE4403_LED2CONVEND_ADDRESS,AFE4403_LED2CONVEND);  
	drv_afe4403_reg_set(AFE4403_ALED2CONVST_ADDRESS,AFE4403_ALED2CONVST);  
	drv_afe4403_reg_set(AFE4403_ALED2CONVEND_ADDRESS,AFE4403_ALED2CONVEND);
	drv_afe4403_reg_set(AFE4403_LED1CONVST_ADDRESS,AFE4403_LED1CONVST);    
	drv_afe4403_reg_set(AFE4403_LED1CONVEND_ADDRESS,AFE4403_LED1CONVEND);  
	drv_afe4403_reg_set(AFE4403_ALED1CONVST_ADDRESS,AFE4403_ALED1CONVST);  
	drv_afe4403_reg_set(AFE4403_ALED1CONVEND_ADDRESS,AFE4403_ALED1CONVEND);
	drv_afe4403_reg_set(AFE4403_ADCRSTSTCT0_ADDRESS,AFE4403_ADCRSTSTCT0);  
	drv_afe4403_reg_set(AFE4403_ADCRSTENDCT0_ADDRESS,AFE4403_ADCRSTENDCT0);
	drv_afe4403_reg_set(AFE4403_ADCRSTSTCT1_ADDRESS,AFE4403_ADCRSTSTCT1);  
	drv_afe4403_reg_set(AFE4403_ADCRSTENDCT1_ADDRESS,AFE4403_ADCRSTENDCT1);
	drv_afe4403_reg_set(AFE4403_ADCRSTSTCT2_ADDRESS,AFE4403_ADCRSTSTCT2);  
	drv_afe4403_reg_set(AFE4403_ADCRSTENDCT2_ADDRESS,AFE4403_ADCRSTENDCT2);
	drv_afe4403_reg_set(AFE4403_ADCRSTSTCT3_ADDRESS,AFE4403_ADCRSTSTCT3);  
	drv_afe4403_reg_set(AFE4403_ADCRSTENDCT3_ADDRESS,AFE4403_ADCRSTENDCT3);
	drv_afe4403_reg_set(AFE4403_PRPCOUNT_ADDRESS,AFE4403_PRPCOUNT);        
	drv_afe4403_reg_set(AFE4403_CONTROL1_ADDRESS,AFE4403_CONTROL1);                 
	drv_afe4403_reg_set(AFE4403_TIAGAIN_ADDRESS,AFE4403_TIAGAIN);          
	drv_afe4403_reg_set(AFE4403_TIA_AMB_GAIN_ADDRESS,AFE4403_TIA_AMB_GAIN);
	drv_afe4403_reg_set(AFE4403_LEDCNTRL_ADDRESS,AFE4403_LEDCNTRL);        
	drv_afe4403_reg_set(AFE4403_CONTROL2_ADDRESS,AFE4403_CONTROL2); 
	drv_afe4403_reg_set(AFE4403_CONTROL3_ADDRESS,AFE4403_CONTROL3); 
	drv_afe4403_reg_set(AFE4403_PDNCYCLESTC_ADDRESS,AFE4403_PDNCYCLESTC);
	drv_afe4403_reg_set(AFE4403_PDNCYCLEENDC_ADDRESS,AFE4403_PDNCYCLEENDC);
	
#ifdef AFE4403_DEBUG
	{
		uint32_t v_uAfe4403RegData = 0;
		uint8_t i = 1;
		drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);
		for (i = 1; i < 42; i++)
		{
			drv_afe4403_reg_get(i,&v_uAfe4403RegData);
			APP_TRACE_LOG("register#%d  = %x\n\r", i, v_uAfe4403RegData);
		}
	}
#endif
}


/**
  *@brief	start afe4403
  *@param[in]	none
  *@retval	none
  */
void drv_afe4403_running_enable(void)
{
    /* afe4403 spi init */
    afe4403_spi_init();
    
	uint32_t v_uRegVal = 0;
	drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);
	drv_afe4403_reg_get(AFE4403_CONTROL1_ADDRESS,&v_uRegVal);
	v_uRegVal |= (1 << 8);
	drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);
	drv_afe4403_reg_set(AFE4403_CONTROL1_ADDRESS,v_uRegVal);
}
/**
  * @brief  bma250e gpio pin init 
  * @note   bma250e gpio pin init 
  * @param  None
  * @retval None
  */
void drv_afe4403_power(uint8_t status)
{
    GPIO_InitTypeDef        GPIO_InitStruct;
     
    /* afe4403 power pin init */ 
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    AFE4403_POWER_GPIO_CLK_ENABLE();
    
    if(status == ON)
    {
        /*##-2- Configure peripheral GPIO ##########################################*/  
        /* afe4403 power GPIO pin configuration  */    
        GPIO_InitStruct.Pin = AFE4403_POWER_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(AFE4403_POWER_PORT, &GPIO_InitStruct); 

        /* set power pin high to open the power */
        HAL_GPIO_WritePin(AFE4403_POWER_PORT,AFE4403_POWER_PIN,GPIO_PIN_SET);         
    }
    else if(status == OFF)
    {
        /*##-2- Configure peripheral GPIO ##########################################*/  
        /* afe4403 power GPIO pin configuration  */    
        GPIO_InitStruct.Pin = AFE4403_POWER_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(AFE4403_POWER_PORT, &GPIO_InitStruct); 
        
        /* reset power pin low to shut down the power */
        HAL_GPIO_WritePin(AFE4403_POWER_PORT,AFE4403_POWER_PIN,GPIO_PIN_RESET);         
    }
}

/**
  *@brief	stop afe4403
  *@param[in]	none
  *@retval	none
  */
void drv_afe4403_running_disable(void)
{
    /* afe4403 spi init */
    afe4403_spi_init();
    
	uint32_t v_uRegVal = 0;
	drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);
	drv_afe4403_reg_get(AFE4403_CONTROL1_ADDRESS,&v_uRegVal);
	v_uRegVal &= ~(1 << 8);
	drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);
	drv_afe4403_reg_set(AFE4403_CONTROL1_ADDRESS,v_uRegVal);
}


/**
  *@brief	get afe4403 data
  *@param[in]	v_piRedValue:red data
  *@param[in]	v_piIredValue:ired data
  *@retval	none
  */
void drv_afe4403_red_ir_get(int32_t *v_piRedValue, int32_t *v_piIredValue)
{
	uint32_t v_uRegValue;
    
    /* afe4403 spi init */
    afe4403_spi_init();
    
	/* set read mode */
	drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);
	/* Get red signal measurement value from register */
	drv_afe4403_reg_get(AFE4403_LED1VAL_ADDRESS,&v_uRegValue);
	/* Converter uint32_t value into int32_t value */
	if (v_uRegValue & 0x00200000)
	{
		/* Negative value */
		*v_piRedValue = v_uRegValue | 0xFFE00000;
	}
	else
	{
		/* Positive value */
		*v_piRedValue = v_uRegValue & 0x001FFFFF;
	}
	/* Get ired signal measurement value from register */
	drv_afe4403_reg_get(AFE4403_LED2VAL_ADDRESS,&v_uRegValue);
	/* Converter uint32_t value into int32_t value */
	if (v_uRegValue & 0x00200000)
	{
		/* Negative value */
		*v_piIredValue = v_uRegValue | 0xFFE00000;
	}
	else
	{
		/* Positive value */
		*v_piIredValue = v_uRegValue & 0x001FFFFF;
	}
}


/**
  *@brief	get afe4403 data
  *@param[in]	v_piRedDiffValue:red diff data
  *@param[in]	v_piIredDiffValue:ired diff data
  *@retval	none
  */
void drv_afe4403_red_ir_diff_get(int32_t *v_piRedDiffValue, int32_t *v_piIredDiffValue)
{
	uint32_t v_uRegValue;

    /* afe4403 spi init */
    afe4403_spi_init();
    
	/* set read mode */
	drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);
	/* Get red signal measurement value from register */
	drv_afe4403_reg_get(AFE4403_LED2ALED2VAL_ADDRESS,&v_uRegValue);
	/* Converter uint32_t value into int32_t value */
	if (v_uRegValue & 0x00200000)
	{
		/* Negative value */
		*v_piRedDiffValue = v_uRegValue | 0xFFE00000;
	}
	else
	{
		/* Positive value */
		*v_piRedDiffValue = v_uRegValue & 0x001FFFFF;
	}
	/* Get ired signal measurement value from register */
	drv_afe4403_reg_get(AFE4403_LED1ALED1VAL_ADDRESS,&v_uRegValue);
	/* Converter uint32_t value into int32_t value */
	if (v_uRegValue & 0x00200000)
	{
		/* Negative value */
		*v_piIredDiffValue = v_uRegValue | 0xFFE00000;
	}
	else
	{
		/* Positive value */
		*v_piIredDiffValue = v_uRegValue & 0x001FFFFF;
	}
}

void drv_afe4403_cf_set(uint32_t v_uCfVal)
{
    uint32_t v_uRetVal;

    /* afe4403 spi init */
    afe4403_spi_init();    

    /* read mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);

    drv_afe4403_reg_get(AFE4403_TIA_AMB_GAIN_ADDRESS,&v_uRetVal);

    /* write mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);

    drv_afe4403_reg_set(AFE4403_TIA_AMB_GAIN_ADDRESS,(v_uRetVal & (~AFE44x0_TIA_AMB_GAIN_CF_LED)) | 
                        (v_uCfVal & AFE44x0_TIA_AMB_GAIN_CF_LED) );
}

void drv_afe4403_rf_set(uint32_t uRfVal)
{
    uint32_t v_uRetVal;

    /* afe4403 spi init */
    afe4403_spi_init();

    /* read mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);

    drv_afe4403_reg_get(AFE4403_TIA_AMB_GAIN_ADDRESS,&v_uRetVal);
    /* write mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);

    drv_afe4403_reg_set(AFE4403_TIA_AMB_GAIN_ADDRESS,(v_uRetVal & (~AFE44x0_TIA_AMB_GAIN_RF_LED)) | 
                        (uRfVal & AFE44x0_TIA_AMB_GAIN_RF_LED) ); 
}


void drv_afe4403_dac_set(uint32_t uDacVal)
{
    uint32_t v_uRetVal;

    /* afe4403 spi init */
    afe4403_spi_init();

    /* read mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);

    drv_afe4403_reg_get(AFE4403_TIA_AMB_GAIN_ADDRESS,&v_uRetVal);
    /* write mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);

    drv_afe4403_reg_set(AFE4403_TIA_AMB_GAIN_ADDRESS,(v_uRetVal & (~AFE44x0_TIA_AMB_GAIN_AMBDAC)) | 
                        (uDacVal & AFE44x0_TIA_AMB_GAIN_AMBDAC) ); 
}


void drv_afe4403_cur_cancel_enable(void)
{
    uint32_t v_uRetVal;

    /* afe4403 spi init */
    afe4403_spi_init();

    /* read mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);

    drv_afe4403_reg_get(AFE4403_TIA_AMB_GAIN_ADDRESS,&v_uRetVal);
    /* write mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);

    drv_afe4403_reg_set(AFE4403_TIA_AMB_GAIN_ADDRESS,(v_uRetVal & (~AFE44x0_TIA_AMB_GAIN_STAGE2EN)) | 
                        (AFE44x0_STAGE2EN_ENABLE & AFE44x0_TIA_AMB_GAIN_STAGE2EN) ); 
}

void drv_afe4403_cur_cancel_disable(void)
{
    uint32_t v_uRetVal;

    /* afe4403 spi init */
    afe4403_spi_init();

    /* read mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);

    drv_afe4403_reg_get(AFE4403_TIA_AMB_GAIN_ADDRESS,&v_uRetVal);
    /* write mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);

    drv_afe4403_reg_set(AFE4403_TIA_AMB_GAIN_ADDRESS,(v_uRetVal & (~AFE44x0_TIA_AMB_GAIN_STAGE2EN)) | 
                        (AFE44x0_STAGE2EN_BYPASS & AFE44x0_TIA_AMB_GAIN_STAGE2EN) ); 
}


void drv_afe4403_sec_gain_set(uint32_t uSecGainVal)
{
    uint32_t v_uRetVal;

    /* afe4403 spi init */
    afe4403_spi_init();

    /* read mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);

    drv_afe4403_reg_get(AFE4403_TIA_AMB_GAIN_ADDRESS,&v_uRetVal);
    /* write mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);

    drv_afe4403_reg_set(AFE4403_TIA_AMB_GAIN_ADDRESS,(v_uRetVal & (~AFE44x0_TIA_AMB_GAIN_STG2GAIN)) | 
                        (uSecGainVal & AFE44x0_TIA_AMB_GAIN_STG2GAIN) ); 
}

void drv_afe4403_led_curr_set(uint8_t uLed1Cur, uint8_t uLed2Cur)
{
    uint32_t v_uAfeWrMask = 0;
    uint32_t v_uAfeWrData = 0;
    uint32_t v_uRetVal;

    /* afe4403 spi init */
    afe4403_spi_init();    

    v_uAfeWrMask = AFE44x0_LEDCNTRL_LED1 | AFE44x0_LEDCNTRL_LED2;

    //  v_uAfeWrData = ((uLed2Cur) << 8) | (uLed1Cur);
    v_uAfeWrData = ((uLed1Cur) << 8) | (uLed2Cur);

    /* read mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_READ);

    drv_afe4403_reg_get(AFE4403_LEDCNTRL_ADDRESS,&v_uRetVal);

    v_uAfeWrData = (v_uRetVal & (~v_uAfeWrMask)) | (v_uAfeWrData & v_uAfeWrMask);

    /* write mode */
    drv_afe4403_reg_set(AFE4403_CONTROL0_ADDRESS,AFE4403_CONTROL0_WRITE);

    drv_afe4403_reg_set(AFE4403_LEDCNTRL_ADDRESS,v_uAfeWrData);
}

/**
  *@brief	set afe4403 register value
  *@param[in]	v_uRegAdr:the address of register
  *@param[in] v_uRegVal:the value of register to write
  *@retval	none
  */
static void drv_afe4403_reg_set(uint8_t v_uRegAdr,uint32_t v_uRegVal)
{
	uint8_t         v_uSpiWriteData_arr[4];
    uint8_t         v_uSpiReadData_arr[4];
    
    /* clear buffer */
    memset(v_uSpiWriteData_arr,0,4);
    memset(v_uSpiReadData_arr,0,4);

	/* afe4403 spi chip select */
	afe4403_cs_low();

    /* fill the data */
	v_uSpiWriteData_arr[0] = v_uRegAdr;
	v_uSpiWriteData_arr[1] = v_uRegVal >> 16;
	v_uSpiWriteData_arr[2] = v_uRegVal >> 8;
	v_uSpiWriteData_arr[3] = v_uRegVal;
    
    /* spi write */
	afe4403_spi_write_read(v_uSpiWriteData_arr,v_uSpiReadData_arr,4);

	/* afe4403 spi chip unselect */
	afe4403_cs_high();
}


/**
  *@brief	set afe4403 register value
  *@param[in]	v_uRegAdr:the address of register
  *@param[in] v_uRegVal:the value of register
  *@retval	none
  */
static void drv_afe4403_reg_get(uint8_t v_uRegAdr,uint32_t *v_puRegVal)
{
	uint8_t v_uSpiWriteData_arr[4];
    uint8_t v_uSpiReadData_arr[4];
    
    /* clear buffer */
    memset(v_uSpiWriteData_arr,0,4);
    memset(v_uSpiReadData_arr,0,4);
    
	/* afe4403 spi chip select */
	afe4403_cs_low();
	
    /* fill the data */
	v_uSpiWriteData_arr[0] = v_uRegAdr;
    
    /* spi read */
	afe4403_spi_write_read(v_uSpiWriteData_arr,v_uSpiReadData_arr,4);
	*v_puRegVal = (v_uSpiReadData_arr[1] << 16) | (v_uSpiReadData_arr[2] << 8) | (v_uSpiReadData_arr[3]);

	/* afe4403 spi chip unselect */
	afe4403_cs_high();
}

/**
  * @brief  afe4403 spi init 
  * @note   afe4403 spi init
  * @param  None
  * @retval status
  */
static HAL_StatusTypeDef afe4403_spi_init(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    GPIO_InitTypeDef        GPIO_InitStruct;
     
    /* afe4403 spi cs pin init */ 
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    AFE4403_SPI_CS_GPIO_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI CS GPIO pin configuration  */    
    GPIO_InitStruct.Pin = AFE4403_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(AFE4403_SPI_CS_PORT, &GPIO_InitStruct); 
    
    /* AFE4403 使用SPI1 */
    ret = MX_SPI1_Init();
    
    if (ret != HAL_OK)
    {
        printf("AFE 4403 SPI Init Err\r\n");
    }
    
    return ret;    
}
/**
  * @brief  afe4403_spi_write_read
  * @note   afe4403 spi write&read data by dma
  * @param  uint8_t *wrData,uint8_t *rdData, uint16_t len
  * @retval status
  */
static HAL_StatusTypeDef afe4403_spi_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    
    ret = spi1_write_read(wrData,rdData,len);
           
    return ret;
}
/**
  * @brief  afe4403 gpio pin init 
  * @note   afe4403 gpio pin init 
  * @param  None
  * @retval None
  */
static void afe4403_ctl_gpio_init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
        
    /* AFE4403 Reset Pin Config */
    /* Enable Afe4403 Reset port clock */
    AFE4403_RESET_GPIO_CLK_ENABLE();
    
    /* Configure Afe4403 Reset pin */
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Pin = AFE4403_RESET_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(AFE4403_RESET_PORT, &GPIO_InitStructure);     
    
    /* AFE4403 PDNZ Pin Config */
    /* Enable Afe4403 Reset port clock */
    AFE4403_PDNZ_GPIO_CLK_ENABLE();
    
    /* Configure Afe4403 PDNZ pin */
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Pin = AFE4403_PDNZ_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(AFE4403_PDNZ_PORT, &GPIO_InitStructure);      
}
/**
  * @brief  afe4403 rdy gpio pin init 
  * @note   afe4403 rdy gpio pin init 
  * @param  None
  * @retval None
  */
static void afe4403_rdy_gpio_init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure; 
    
    /* AFE4403 RDY Pin Config */
    /* Enable RDY port clock */
    AFE4403_RDY_GPIO_CLK_ENABLE();
    
    /* Configure RDY pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = AFE4403_RDY_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(AFE4403_RDY_PORT, &GPIO_InitStructure);  

    //(afe4403 RDY and REQN use the same exti line 10~15)
    /* Enable and set REQN Exit line Interrupt priority */
    HAL_NVIC_SetPriority(CMU_REQN_EXIT_IRQn, CMU_REQN_EXIT_PREPRIORITY, CMU_REQN_EXIT_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(CMU_REQN_EXIT_IRQn);
}
/**
  * @brief  set afe4403 spi cs pin
  * @note   afe4403 spi cs pin high
  * @param  None
  * @retval None
  */
static void afe4403_cs_high(void)
{
    HAL_GPIO_WritePin(AFE4403_SPI_CS_PORT,AFE4403_SPI_CS_PIN,GPIO_PIN_SET);
}
/**
  * @brief  reset afe4403 spi cs pin
  * @note   afe4403 spi cs pin low
  * @param  None
  * @retval None
  */
static void afe4403_cs_low(void)
{
    HAL_GPIO_WritePin(AFE4403_SPI_CS_PORT,AFE4403_SPI_CS_PIN,GPIO_PIN_RESET);
}

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/



