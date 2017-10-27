/**
  ****************************************************************************************
  * @file    bma250e.c
  * @author  上海团队
  * @version V1.0.0
  * @date    $$$$-$$-$$
  * @brief   driver for bma250e
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
#include "bma250e.h"

/* Private variables ---------------------------------------------------------*/
static struct bma2x2_t s_bma250e_st;

/* Private function prototypes -----------------------------------------------*/
static void bma250e_gpio_init(void);
static void bma250e_cs_high(void);
static void bma250e_cs_low(void);
static HAL_StatusTypeDef bma250e_spi_init(void);
static HAL_StatusTypeDef bma250e_spi_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len);
static int8_t drv_bma250e_spi_write(uint8_t v_uDevAddr,uint8_t v_uRegisterAddr,uint8_t *v_puRegisterData,uint8_t v_uWriteLen);
static int8_t drv_bma250e_spi_read(uint8_t v_uDevAddr,uint8_t v_RregisterAddr,uint8_t *v_puRegisterData,uint8_t v_uReadLen);
static void drv_bma250e_delay(uint32_t v_uMsc);



/**
  * @brief  bma250e_init
  * @note   bma250e init
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef bma250e_init(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
	s_bma250e_st.bus_write = drv_bma250e_spi_write;
	s_bma250e_st.bus_read  = drv_bma250e_spi_read;
	s_bma250e_st.delay_msec = drv_bma250e_delay;

	bma2x2_init(&s_bma250e_st);
    
    if(BMA250E_CHIP_ID != s_bma250e_st.chip_id)
    {
        printf("bmaID:0x%x\r\n",s_bma250e_st.chip_id);
    }
    else
    {
        ret = HAL_OK;
    }
	
	bma2x2_soft_rst();
	
	/*set the ranges(g values) of the sensor*/
	bma2x2_set_range(BMA2x2_RANGE_2G);
	
	/*set the bandwidth of the sensor*/
	bma2x2_set_bw(BMA2x2_BW_250HZ);

	bma2x2_set_offset_target(BMA2x2_CUT_OFF,0);
	bma2x2_set_slow_comp(BMA2x2_SLOW_COMP_X,1);
	bma2x2_set_slow_comp(BMA2x2_SLOW_COMP_Y,1);
	bma2x2_set_slow_comp(BMA2x2_SLOW_COMP_Z,1);

	bma2x2_set_mode_value(BMA2x2_MODE_NORMAL); 

	/*set shadow dis(MSB is Locked)*/
	bma2x2_set_shadow_dis(0);
    
    return ret;
}
/**
  * @brief  accel_data_get
  * @note   get the data of accelate
  * @param  int16_t *v_piXdata,int16_t *v_piYdata,int16_t *v_piZdata
  * @retval None
  */
void accel_data_get(int16_t *v_piXdata,int16_t *v_piYdata,int16_t *v_piZdata)
{
    struct bma2x2_accel_data accel_data;
    
    bma2x2_read_accel_xyz(&accel_data);
    
    *v_piXdata = accel_data.x;
    *v_piYdata = accel_data.y;
    *v_piZdata = accel_data.z;
}

/**
*@brief write bma250e register data
*@param[in] v_uDevAddr:reserve
*@param[in] v_uRegisterAddr:bma250e register address
*@param[in] v_puRegisterData:data to be wrote to bma250e register
*@param[in] v_uWriteLen:write data len
*@retval always return 1
*/
static int8_t drv_bma250e_spi_write(uint8_t v_uDevAddr,uint8_t v_uRegisterAddr,uint8_t *v_puRegisterData,uint8_t v_uWriteLen)
{
	uint8_t         v_uWriteVal = 0;
	uint16_t        i = 0;
	uint8_t         v_uWriteData[v_uWriteLen+1];
    uint8_t         v_uRecvData[v_uWriteLen+1];

    /* clear buffer */
    memset(v_uWriteData,0,(v_uWriteLen+1));
    memset(v_uRecvData,0,(v_uWriteLen+1));
	
    /* bma250e spi init */
    bma250e_spi_init();
    
	/* write the register */
	v_uWriteVal = v_uRegisterAddr | BMA250E_SPI_WRITE;
	
	/* bma250e spi chip select */
	bma250e_cs_low();

    /* fill the write date */
	v_uWriteData[0] = v_uWriteVal;
	for(i = 0; i < v_uWriteLen; i++)
	{
		v_uWriteData[i + 1] = v_puRegisterData[i];
	}
    
    /* spi write */
	bma250e_spi_write_read(v_uWriteData,v_uRecvData,v_uWriteLen+1);

	/* bam250e spi chip unselect */
	bma250e_cs_high();
    
	return 1;
}

/**
*@brief read bma250e register data
*@param[in] v_uDevAddr:reserve
*@param[in] v_uRegisterAddr:bma250e register address
*@param[in] v_puRegisterData:data to be read from bma250e register
*@param[in] v_uReadLen:read data len
*@retval always return 1
*/
static int8_t drv_bma250e_spi_read(uint8_t v_uDevAddr,uint8_t v_RregisterAddr,uint8_t *v_puRegisterData,uint8_t v_uReadLen)
{
	uint8_t                 v_uWriteVal = 0;
	uint16_t                i = 0;
    uint8_t                 v_uWriteData[v_uReadLen+1];
    uint8_t                 v_uRecvData[v_uReadLen+1];

    /* clear buffer */
    memset(v_uWriteData,0,(v_uReadLen+1));
    memset(v_uRecvData,0,(v_uReadLen+1));
    
    /* bma250e spi init */
    bma250e_spi_init();
    
    /* read the register */
	v_uWriteVal = v_RregisterAddr | BMA250E_SPI_READ;
	
	/* bma250e spi chip select */
	bma250e_cs_low();

    /* fill the write date */
	v_uWriteData[0] = v_uWriteVal;
    
    /* spi read */
	bma250e_spi_write_read(v_uWriteData,v_uRecvData,v_uReadLen+1);
	for(i = 0; i < v_uReadLen; i++)
	{
		*(v_puRegisterData + i) = v_uRecvData[i + 1];
	}

	/* bam250e spi chip unselect */
	bma250e_cs_high();
    
	return 1;
}
/**
  * @brief  delay for write or read bam250e register
  * @note   bma250e spi cs pin init 
  * @param  v_uMsc:millisecond
  * @retval None
  */
static void drv_bma250e_delay(uint32_t v_uMsc)
{
	uint16_t v_uTimeCnt = 0;
	uint16_t i=0;
	
	for(i = 0; i < v_uMsc; i++)
	{
		for(v_uTimeCnt = 0; v_uTimeCnt < 6000; v_uTimeCnt++);
	}
}	

/**
  * @brief  bma250e gpio pin init 
  * @note   bma250e gpio pin init 
  * @param  None
  * @retval None
  */
static void bma250e_gpio_init(void)
{
    
}
/**
  * @brief  set bma250e spi cs pin
  * @note   bma250e spi cs pin high
  * @param  None
  * @retval None
  */
static void bma250e_cs_high(void)
{
    HAL_GPIO_WritePin(BMA250E_SPI_CS_PORT,BMA250E_SPI_CS_PIN,GPIO_PIN_SET);
}
/**
  * @brief  reset bma250e spi cs pin
  * @note   bma250e spi cs pin low
  * @param  None
  * @retval None
  */
static void bma250e_cs_low(void)
{
    HAL_GPIO_WritePin(BMA250E_SPI_CS_PORT,BMA250E_SPI_CS_PIN,GPIO_PIN_RESET);
}
/**
  * @brief  bma250e spi init 
  * @note   bma250e spi init
  * @param  None
  * @retval status
  */
static HAL_StatusTypeDef bma250e_spi_init(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    GPIO_InitTypeDef        GPIO_InitStruct;
     
    /* bma250e spi cs pin init */ 
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    BMA250E_SPI_CS_GPIO_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI CS GPIO pin configuration  */    
    GPIO_InitStruct.Pin = BMA250E_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(BMA250E_SPI_CS_PORT, &GPIO_InitStruct); 
    
    /* bma250e 使用SPI1 */
    ret = MX_SPI1_Init();
    
    return ret;
}
/**
  * @brief  mx25_spi_dma_write_read
  * @note   mx25l06e spi write&read data by dma
  * @param  uint8_t *wrData,uint8_t *rdData, uint16_t len
  * @retval status
  */
static HAL_StatusTypeDef bma250e_spi_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    
    ret = spi1_write_read(wrData,rdData,len);
           
    return ret;
}
/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

