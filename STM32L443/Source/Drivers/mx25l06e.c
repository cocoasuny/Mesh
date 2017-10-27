/**
  ****************************************************************************************
  * @file    mx25l06e.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-20
  * @brief   driver for mx25l06e
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "mx25l06e.h"


/* Private variables ---------------------------------------------------------*/

 
/* Private function prototypes -----------------------------------------------*/
static void mx25_gpio_init(void);
static void mx25_gpio_deinit(void);
static void mx25_cs_low(void);
static void mx25_cs_high(void);
static HAL_StatusTypeDef mx25_spi_dma_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len);
static uint8_t mx25_status_read(void);
static void mx25_busy_wait(void);
static bool mx25_check_busy(void);
static void mx25_write_enable(void);

/**
  * @brief  flash power control
  * @note   open/close the flash power
  * @param  power: ON/OFF
  * @retval None
  */
void flash_powerCtl(uint8_t power)
{
	GPIO_InitTypeDef        GPIO_InitStructure;

    /* GPIO Ports Clock Enable */
    FLASH_POWER_PORT_CLK_ENABLE();
    
	if(power == ON)
	{
        /* GPIO Pin Configure */
        GPIO_InitStructure.Pin = GPIO_PIN_FLASH_POWER;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIO_PORT_FLASH_POWER, &GPIO_InitStructure);

        /* output 0 to open the flash power */
        HAL_GPIO_WritePin(GPIO_PORT_FLASH_POWER,GPIO_PIN_FLASH_POWER,GPIO_PIN_RESET); 
	}
	else
	{
        /*  Configure GPIO Pin in AN Mode to close the flash power */
        GPIO_InitStructure.Pin = GPIO_PIN_FLASH_POWER;
        GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStructure.Pull = GPIO_PULLDOWN;
        HAL_GPIO_Init(GPIO_PORT_FLASH_POWER, &GPIO_InitStructure);
	}
}
/**
  * @brief  flash spi init 
  * @note   mx25l06e spi init
  * @param  None
  * @retval status
  */
HAL_StatusTypeDef mx25_spi_init(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    /* flash power on */
    flash_powerCtl(ON);
    
    /* mx25l06e spi cs pin init */
    mx25_gpio_init();
    
    /* mx25l06e和nrf51822共用SPI3 */
    ret = MX_SPI3_Init();
    
    return ret;
}
/**
  * @brief  flash spi deinit 
  * @note   mx25l06e spi deinit
  * @param  None
  * @retval status
  */
HAL_StatusTypeDef mx25_spi_deinit(void)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    
    mx25_gpio_deinit();
    
    return ret;
}

/**
  * @brief  mx25_chip_id_get
  * @note   mx25l06e chip id get
  * @param  uint32_t *id
  * @retval status
  */
HAL_StatusTypeDef mx25_chip_id_get(uint32_t *id)
{
    HAL_StatusTypeDef       ret = HAL_ERROR;
    uint8_t                 wrdataArr[4] = {0};
    uint8_t                 rdDataArr[4] = {0};
        
    wrdataArr[0] = FLASH_CMD_RDID;
    wrdataArr[1] = 0x00;
    wrdataArr[2] = 0x00;
    wrdataArr[3] = 0x00;
        
    /* mx25l06e spi init */
    mx25_spi_init();
    
    /* flash spi chip select */
    mx25_cs_low(); 
    
    /* spi read write data */
    memset(rdDataArr,0,sizeof(rdDataArr));
    
    ret = mx25_spi_dma_write_read(wrdataArr,rdDataArr,sizeof(wrdataArr));
    
    /* flash spi chip release */
    mx25_cs_high();    
        
    *id = (uint32_t)((rdDataArr[1] << 16) | (rdDataArr[2] << 8) | (rdDataArr[3]));
        
    return ret;
}

/**
  * @brief  mx25_sector_read
  * @note   sector read
  * @param  uint32_t nSector, uint8_t* pBuffer
  * @retval None
  */
void mx25_sector_read(uint32_t nSector, uint8_t* pBuffer)
{
    uint8_t wrDataArr[4] = {0};
    
    /* mx25l06e spi init */
    mx25_spi_init();
    
    /* 扇区号转为地址 */
    nSector = nSector * MX25_FLASH_SECTOR_SIZE;
        
    /* flash spi chip select */
    mx25_cs_low(); 
    
    wrDataArr[0] = FLASH_CMD_READ;
    wrDataArr[1] = nSector >> 16;
    wrDataArr[2] = nSector >> 8;
    wrDataArr[3] = nSector;
    
    mx25_spi_dma_write_read(wrDataArr,wrDataArr,sizeof(wrDataArr));
    
    mx25_spi_dma_write_read(pBuffer,pBuffer,MX25_FLASH_SECTOR_SIZE);
    
    /* flash spi chip release */
    mx25_cs_high();   

    /* wait flash to ready */
    mx25_busy_wait();    
}

/**
  * @brief  mx25_sector_write
  * @note   sector write
  * @param  uint32_t nSector, uint8_t* pBuffer
  * @retval None
  */
void mx25_sector_write(uint32_t nSector, uint8_t* pBuffer)
{
	uint32_t i;
	uint8_t v_uWriteData_arr[4];
	uint8_t v_uReceData[MX25_FLASH_PAGE_SIZE];
    
    /* 扇区号转为地址 */
	nSector *= MX25_FLASH_SECTOR_SIZE;

    /* mx25l06e spi init */
    mx25_spi_init();
    
	for(i = 0;i < MX25_FLASH_PAGES_PER_SECTOR; i++)
	{
        /* enable write */
		mx25_write_enable();
		
        /* flash spi chip select */
        mx25_cs_low(); 
		
		v_uWriteData_arr[0] = FLASH_CMD_PP;
		v_uWriteData_arr[1] = nSector >> 16;
		v_uWriteData_arr[2] = nSector >> 8;
		v_uWriteData_arr[3] = 0x00;

		mx25_spi_dma_write_read(v_uWriteData_arr,v_uWriteData_arr,sizeof(v_uWriteData_arr));
		
		mx25_spi_dma_write_read(pBuffer,v_uReceData,MX25_FLASH_PAGE_SIZE);

		pBuffer += MX25_FLASH_PAGE_SIZE;
		nSector += MX25_FLASH_PAGE_SIZE;

        /* flash spi chip release */
        mx25_cs_high(); 

		/* wait flash to ready */
		mx25_busy_wait();	
	}    
}
/**
  * @brief  mx25_sector_erase
  * @note   sector erase
  * @param  DWORD v_uSector
  * @retval None
  */
void mx25_sector_erase(DWORD v_uSector)
{
	uint8_t v_uWriteData_arr[4] = {0};
	
    /* 扇区号转为地址 */
	v_uSector *= MX25_FLASH_SECTOR_SIZE;

    /* mx25l06e spi init */
    mx25_spi_init();
    
    /* enable write */
	mx25_write_enable();
	
    /* flash spi chip select */
    mx25_cs_low(); 
	
	v_uWriteData_arr[0] = FLASH_CMD_SE;
	v_uWriteData_arr[1] = v_uSector >> 16;
	v_uWriteData_arr[2] = v_uSector >> 8;
	v_uWriteData_arr[3] = v_uSector;
	
	mx25_spi_dma_write_read(v_uWriteData_arr,v_uWriteData_arr,sizeof(v_uWriteData_arr));
	
    /* flash spi chip release */
    mx25_cs_high(); 
	
	/* wait flash to ready */
	mx25_busy_wait();	
}
/**
  * @brief  mx25_chip_erase
  * @note   erase chip
  * @param  uint32_t *id
  * @retval status
  */
void mx25_chip_erase(void)
{
	uint8_t v_uWriteData_arr[1] = {0};

    /* mx25l06e spi init */
    mx25_spi_init();
	
    /* enable flash write */
	mx25_write_enable();
	
    /* flash spi chip select */
    mx25_cs_low(); 

	v_uWriteData_arr[0] = FLASH_CMD_CE;
	
	mx25_spi_dma_write_read(v_uWriteData_arr,v_uWriteData_arr,sizeof(v_uWriteData_arr));

    /* flash spi chip release */
    mx25_cs_high(); 
	
	/* wait flash to ready */
	mx25_busy_wait();	    
}

/**
  * @brief  reset flash spi cs pin
  * @note   mx25l06e spi cs pin low
  * @param  None
  * @retval None
  */
static void mx25_cs_low(void)
{
    HAL_GPIO_WritePin(FLASH_SPI_CS_PORT,FLASH_SPI_CS_PIN,GPIO_PIN_RESET); 
}
/**
  * @brief  set flash spi cs pin
  * @note   mx25l06e spi cs pin high
  * @param  None
  * @retval None
  */
void mx25_cs_high(void)
{
    HAL_GPIO_WritePin(FLASH_SPI_CS_PORT,FLASH_SPI_CS_PIN,GPIO_PIN_SET); 
}
/**
  * @brief  mx25_spi_dma_write_read
  * @note   mx25l06e spi write&read data by dma
  * @param  uint8_t *wrData,uint8_t *rdData, uint16_t len
  * @retval status
  */
static HAL_StatusTypeDef mx25_spi_dma_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    
    ret = spi3_dma_write_read(wrData,rdData,len);
           
    return ret;
}


/**
  * @brief  flash spi cs pin init 
  * @note   mx25l06e spi cs pin init 
  * @param  None
  * @retval status
  */
static void mx25_gpio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    FLASH_SPI_CS_GPIO_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI CS GPIO pin configuration  */    
    GPIO_InitStruct.Pin = FLASH_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStruct);    
}
/**
  * @brief  flash spi cs pin deinit 
  * @note   mx25l06e spi cs pin deinit 
  * @param  None
  * @retval status
  */
static void mx25_gpio_deinit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    FLASH_SPI_CS_GPIO_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI CS GPIO pin configuration  */    
    GPIO_InitStruct.Pin = FLASH_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStruct);   

    HAL_GPIO_WritePin(FLASH_SPI_CS_PORT,FLASH_SPI_CS_PIN,GPIO_PIN_SET); 
}

/**
  * @brief  mx25_status_read
  * @note   mx25l06e status get
  * @param  None
  * @retval status
  */
static uint8_t mx25_status_read(void)
{
    uint8_t                 wrdataArr[2] = {0};
    uint8_t                 rdDataArr[2] = {0};
    uint8_t                 status = 0;
        
    wrdataArr[0] = FLASH_CMD_RDSR;
    wrdataArr[1] = 0x00;
    
    /* flash spi chip select */
    mx25_cs_low();    
    
    /* spi read write data */
    memset(rdDataArr,0,sizeof(rdDataArr));
    mx25_spi_dma_write_read(wrdataArr,rdDataArr,sizeof(wrdataArr));
    
    /* flash spi chip release */
    mx25_cs_high();
            
    status = rdDataArr[1];
    
    return status;
}
/**
  * @brief  wait flash ready to read
  * @note   wait when flash is busy
  * @param  None
  * @retval status
  */
static void mx25_busy_wait(void)
{
    uint8_t     status = 0;
    
    status = mx25_status_read();
    
    while(FLASH_WIP_MASK & status)
    {
        status = mx25_status_read();
    }
}

/**
  * @brief  mx25_check_busy
  * @note   check flash is busy
  * @param  None
  * @retval status: true->busy;false->free
  */
static bool mx25_check_busy(void)
{
    uint8_t     status = 0;
    
    status = mx25_status_read();
    
    if(FLASH_WIP_MASK == (FLASH_WIP_MASK & status))
    {
        return true;
    }
    else
    {
        return false;
    }
}
/**
  * @brief  mx25_write_enable
  * @note   enable mx25l06e
  * @param  None
  * @retval None
  */
static void mx25_write_enable(void)
{	

	uint8_t wrData_arr[1] = {0};
  
	wrData_arr[0] = FLASH_CMD_WREN;

    /* flash spi chip select */
    mx25_cs_low();    
	
	mx25_spi_dma_write_read(wrData_arr,wrData_arr,sizeof(wrData_arr));
    
    /* flash spi chip release */
    mx25_cs_high();    
	
	/* wait flash to ready */
	mx25_busy_wait();
}



/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

