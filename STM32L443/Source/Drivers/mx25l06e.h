/**
  ****************************************************************************************
  * @file    mx25l06e.h
  * @author  Jason
  * @version V1.0.0
  * @date    2016-9-20
  * @brief   header file of mx25l06e.c
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 Chengdu CloudCare Healthcare Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MX25L06E_H_
#define __MX25L06E_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "main.h"

/*
  Flash Related Parameter Define
*/

#define    FLASH_ID             0xc22015

#define    FlashSize        	0x200000    // 2 MB
#define    BLOCK_SIZE       	0x10000     // 64K Block size
#define    BLOCK32K_SIZE    	0x8000      // 32K Block size
#define    SECTOR_SIZE      	0x1000      // 4K Sector size
#define    PAGE_SIZE			0x0100      // 256 Byte Page size
#define    PAGE32_SIZE      	0x0020      // 32 Byte Page size (some products have smaller page size)
#define    BLOCK_NUM          	(FlashSize / BLOCK_SIZE)
#define    SECTOR_NUM          	(FlashSize / SECTOR_SIZE)
#define    PAGE_NUM          	(FlashSize / PAGE_SIZE)

#define MX25_FLASH_PAGE_SIZE            PAGE_SIZE
#define MX25_FLASH_SECTOR_SIZE 	        SECTOR_SIZE
#define	MX25_FLASH_SECTOR_COUNT         SECTOR_NUM  
#define MX25_FLASH_BLOCK_SIZE   	    BLOCK_SIZE 
#define MX25_FLASH_PAGES_PER_SECTOR	    (MX25_FLASH_SECTOR_SIZE / MX25_FLASH_PAGE_SIZE)


/* Exported macros -----------------------------------------------------------*/
//ID comands
#define    FLASH_CMD_RDID      				0x9F    //RDID (Read Identification)
#define    FLASH_CMD_RES       				0xAB    //RES (Read Electronic ID)
#define    FLASH_CMD_REMS      				0x90    //REMS (Read Electronic & Device ID)

//Register comands
#define    FLASH_CMD_WRSR      				0x01    //WRSR (Write Status Register)
#define    FLASH_CMD_RDSR      				0x05    //RDSR (Read Status Register)
#define    FLASH_CMD_WRSCUR    				0x2F    //WRSCUR (Write Security Register)
#define    FLASH_CMD_RDSCUR    				0x2B    //RDSCUR (Read Security Register)

//READ comands
#define    FLASH_CMD_READ        			0x03    //READ (1 x I/O)
#define    FLASH_CMD_FASTREAD    			0x0B    //FAST READ (Fast read data)
#define    FLASH_CMD_DREAD       			0x3B    //DREAD (1In/2 Out fast read)
#define    FLASH_CMD_RDSFDP      			0x5A    //RDSFDP (Read SFDP)

//Program comands
#define    FLASH_CMD_WREN     				0x06    //WREN (Write Enable)
#define    FLASH_CMD_WRDI     				0x04    //WRDI (Write Disable)
#define    FLASH_CMD_PP       				0x02    //PP (page program)

//Erase comands
#define    FLASH_CMD_SE       				0x20    //SE (Sector Erase)
#define    FLASH_CMD_BE       				0xD8    //BE (Block Erase)
#define    FLASH_CMD_CE       				0x60    //CE (Chip Erase) hex code: 60 or C7

//Mode setting comands
#define    FLASH_CMD_DP       				0xB9    //DP (Deep Power Down)
#define    FLASH_CMD_RDP      				0xAB    //RDP (Release form Deep Power Down)
#define    FLASH_CMD_ENSO     				0xB1    //ENSO (Enter Secured OTP)
#define    FLASH_CMD_EXSO     				0xC1    //EXSO  (Exit Secured OTP)

// Flash control register mask define
// status register
#define    FLASH_WIP_MASK         0x01
#define    FLASH_LDSO_MASK        0x02
#define    FLASH_QE_MASK          0x40
// security register
#define    FLASH_OTPLOCK_MASK     0x03
#define    FLASH_4BYTE_MASK       0x04
#define    FLASH_WPSEL_MASK       0x80
// configuration reigster
#define    FLASH_DC_MASK          0x80
#define    FLASH_DC_2BIT_MASK     0xC0
// other
#define    BLOCK_PROTECT_MASK     0xff
#define    BLOCK_LOCK_MASK        0x01


/* Exported functions --------------------------------------------------------*/
void flash_powerCtl(uint8_t power);
HAL_StatusTypeDef mx25_spi_init(void);
HAL_StatusTypeDef mx25_spi_deinit(void);
HAL_StatusTypeDef mx25_chip_id_get(uint32_t *id);
void mx25_sector_read(uint32_t nSector, uint8_t* pBuffer);
void mx25_sector_write(uint32_t nSector, uint8_t* pBuffer);
void mx25_sector_erase(DWORD v_uSector);
void mx25_chip_erase(void);


#endif /* __MX25L06E_H_ */

/************************ (C) COPYRIGHT Chengdu CloudCare Healthcare Co., Ltd. *****END OF FILE****/

