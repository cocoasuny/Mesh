/**
  ****************************************************************************************
  * @file    bsp_communication.c
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-26
  * @brief   communication with nRF51822 By SPI
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "bsp_communication.h"


/* Private variables ---------------------------------------------------------*/


 
/* Private function prototypes -----------------------------------------------*/
static void cmu_ctl_gpio_init(void);
static void cmu_ctl_gpio_deinit(void);
static void cmu_spi_init(void);
static void cmu_spi_deinit(void);
static void cmu_spi_cs_low(void);
static void cmu_spi_cs_high(void);
static HAL_StatusTypeDef cmu_spi_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len);



/**
  * @brief  communication init 
  * @note   communication with nRF51822, SPI and control GPIO init
  * @param  None
  * @retval status
  */
HAL_StatusTypeDef cmu_init(void)
{
    HAL_StatusTypeDef           ret = HAL_ERROR;
    
    /* Cmu control GPIO Pin(SYNC/REQN) Init */
    cmu_ctl_gpio_init();
    
    return  ret;
}
/**
  * @brief  communication deinit 
  * @note   communication with nRF51822, SPI and control GPIO deinit
  * @param  None
  * @retval status
  */
HAL_StatusTypeDef cmu_deinit(void)
{
    HAL_StatusTypeDef           ret = HAL_ERROR;
    
    /* CMU control GPIO Pin(SYNC/REQN) deInit */
    cmu_ctl_gpio_deinit();
    
    return  ret;
}

/**
  * @brief  get_data_from_CMU
  * @note   get data from CMU by SPI
  * @param  CMU_DATA_READ_BUFFER_T *buffer
  * @retval None
  */
void get_data_from_CMU(CMU_DATA_READ_BUFFER_T *buffer)
{
	uint8_t             FrameHeader1=0x00;  //帧头第一个字节
	uint8_t             FrameHeader2=0x00;  //帧头第二个字节
    uint8_t             SPIBusyTry_Cnt=0;    //SPI数据传输Busy时，try计数
    uint8_t             SPI_RX_Buf[CMU_RX_BUF_MAXLEN]={0};
    uint8_t             SPI_TX_Buf[CMU_RX_BUF_MAXLEN]={0};
    uint16_t            SPI_RX_buf_len = 0;  //SPI接收数据长度
    uint16_t            i = 0;

    /* Clear Buffer */
    memset(SPI_RX_Buf,0,CMU_RX_BUF_MAXLEN);
    memset(SPI_TX_Buf,0,CMU_RX_BUF_MAXLEN);
    
    /* cmu spi init */
    cmu_spi_init();
    
    do
    {
        cmu_spi_cs_low();
        cmu_spi_write_read(SPI_TX_Buf,SPI_RX_Buf,1); //读取数据帧头1
        FrameHeader1 = SPI_RX_Buf[0];
        if(FrameHeader1 == 0xAB)
        {
            //nRF51822 SPI Busy
            cmu_spi_cs_high();
        }
        SPIBusyTry_Cnt++;
    }while((FrameHeader1 == 0xAB) && (SPIBusyTry_Cnt < 20));
    
    cmu_spi_write_read(SPI_TX_Buf,SPI_RX_Buf,1); //读取数据帧头2
    FrameHeader2 = SPI_RX_Buf[0];
    
    if((FrameHeader1 == 0xAA) && (FrameHeader2 == 0x55))
    {
        FrameHeader1 = 0x00;
        FrameHeader2 = 0x00;
        cmu_spi_write_read(SPI_TX_Buf,SPI_RX_Buf,3); //读取3个字节，第一个为FrameType，第二个为数据帧长度高8位
                                                     //第三个字节为数据帧长度低8位
        buffer->buf[0] = SPI_RX_Buf[0];  //组包第一个字节
        SPI_RX_buf_len = (SPI_RX_Buf[1] << 8) + SPI_RX_Buf[2];
        
        cmu_spi_write_read(SPI_TX_Buf,SPI_RX_Buf,SPI_RX_buf_len); //接收有效数据长度
        
        cmu_spi_cs_high();  //数据接收完成
        for(i=0;i<SPI_RX_buf_len;i++)
        {
            buffer->buf[i+1] = SPI_RX_Buf[i];
        }
        
        buffer->len = SPI_RX_buf_len + 1;
    }
    
}

/**
  * @brief  cmu_data_send
  * @note   cmu send data by SPI
  * @param  uint8_t *pDatSend, uint8_t lenDat
  * @retval None
  */
void cmu_data_send(uint8_t *pDatSend, uint8_t lenDat)
{
    uint8_t             SPI_RX_Buf[CMU_RX_BUF_MAXLEN]={0};
    
    /* Clear Buffer */
    memset(SPI_RX_Buf,0,CMU_RX_BUF_MAXLEN);
    
    /* cmu spi init */
    cmu_spi_init();
    
    cmu_spi_cs_low();
    cmu_spi_write_read(pDatSend,SPI_RX_Buf,lenDat);
    cmu_spi_cs_high();  
}

/**
  * @brief  communication control GPIO init 
  * @note   communication with nRF51822, SYNC and REQN Pin init
  * @param  None
  * @retval None
  */
static void cmu_ctl_gpio_init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    
    /* Enable SYNC/REQN port clock */
    CMU_SYNC_PIN_CLK_ENABLE();
    CMU_REQN_PIN_CLK_ENABLE();
    
    /* Configure SYNC pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = CMU_SYNC_PIN;
    HAL_GPIO_Init(CMU_SYNC_PORT, &GPIO_InitStructure);
    
    /* Configure REQN pin as input floating */
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Pin = CMU_REQN_PIN;
    HAL_GPIO_Init(CMU_REQN_PORT, &GPIO_InitStructure);    

    /* Enable and set SYNC Exit line Interrupt priority */
    HAL_NVIC_SetPriority(CMU_SYNC_EXIT_IRQn, CMU_SYNC_EXIT_PREPRIORITY, CMU_SYNC_EXIT_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(CMU_SYNC_EXIT_IRQn);  
    
    /* Enable and set REQN Exit line Interrupt priority */
    HAL_NVIC_SetPriority(CMU_REQN_EXIT_IRQn, CMU_REQN_EXIT_PREPRIORITY, CMU_REQN_EXIT_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(CMU_REQN_EXIT_IRQn);     
}
/**
  * @brief  communication control GPIO deinit 
  * @note   communication with nRF51822, SYNC and REQN Pin deinit
  * @param  None
  * @retval None
  */
static void cmu_ctl_gpio_deinit(void)
{
    
}
/**
  * @brief  communication control SPI init 
  * @note   communication with nRF51822, SPI Pin deinit
  * @param  None
  * @retval None
  */
static void cmu_spi_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct; 
    
    /* CS Pin Config */ 
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    CMU_SPI_CS_GPIO_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/  
    /* SPI CS GPIO pin configuration  */    
    GPIO_InitStruct.Pin = CMU_SPI_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(CMU_SPI_CS_PORT, &GPIO_InitStruct);        
    
    /* SPI Config */
    MX_SPI3_Init();  //CMU与Flash共用SPI3
}
/**
  * @brief  communication control SPI deinit 
  * @note   communication with nRF51822, SPI Pin deinit
  * @param  None
  * @retval None
  */
static void cmu_spi_deinit(void)
{

}
/**
  * @brief  cmu_spi_write_read
  * @note   cmu spi write&read data by dma
  * @param  uint8_t *wrData,uint8_t *rdData, uint16_t len
  * @retval status
  */
static HAL_StatusTypeDef cmu_spi_write_read(uint8_t *wrData,uint8_t *rdData, uint16_t len)
{
    HAL_StatusTypeDef ret = HAL_ERROR;
    
    ret = spi3_dma_write_read(wrData,rdData,len);
           
    return ret;
}
/**
  * @brief  set cmu spi cs pin
  * @note   cmu spi cs pin high
  * @param  None
  * @retval None
  */
static void cmu_spi_cs_high(void)
{
    HAL_GPIO_WritePin(CMU_SPI_CS_PORT,CMU_SPI_CS_PIN,GPIO_PIN_SET); 
}
/**
  * @brief  reset cmu spi cs pin
  * @note   cmu spi cs pin low
  * @param  None
  * @retval None
  */
static void cmu_spi_cs_low(void)
{
    HAL_GPIO_WritePin(CMU_SPI_CS_PORT,CMU_SPI_CS_PIN,GPIO_PIN_RESET); 
}
/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

