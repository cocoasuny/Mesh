/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "mx25l06e.h"
#include <stdint.h>
#include "bsp_rtc.h"
#include <stdio.h>




DWORD get_fattime(void);

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;
    
    /* mx25l06e spi init */
    mx25_spi_init();
    
    stat &= ~STA_NOINIT;  
    
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
    int i;
    
    for(i=0;i<count;i++)
    {
        mx25_sector_read(sector,buff);
        sector ++;
        buff += MX25_FLASH_SECTOR_SIZE;
    }    
    res = RES_OK;

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
    int i;

    for(i=0;i<count;i++)
    {	    
        mx25_sector_erase(sector);
        mx25_sector_write(sector,(uint8_t*)buff);
        sector ++;
        buff += MX25_FLASH_SECTOR_SIZE;
    }
    res = RES_OK;
    
	return res;
}



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
//	DWORD nFrom,nTo;
//	int i;
//	char *buf = buff;
	
	switch(cmd)
	{
		case CTRL_SYNC :
			break;		
		case GET_BLOCK_SIZE:
			*(DWORD*)buff = MX25_FLASH_BLOCK_SIZE;
			break;
	
	
		case GET_SECTOR_SIZE:
			*(DWORD*)buff = MX25_FLASH_SECTOR_SIZE;
			break;
		
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = MX25_FLASH_SECTOR_COUNT;
			break;
			
		default:
			res = RES_PARERR;
			break;
	}
	return res;
}  
DWORD get_fattime(void)
{
    DWORD date = 0;
    RTC_DateTypeDef     date_s;               //RTC
    RTC_TimeTypeDef     rtc_time;             //RTC
    
    calendar_get(&date_s,&rtc_time);
    
    date_s.Year = date_s.Year + 2000 - 1980;  //年份改为1980年起
    date_s.Month = date_s.Month;     //0-11月改为1-12月
    rtc_time.Seconds /= 2;       //将秒数改为0-29
    
    date = 0;
    date = ((date_s.Year+2000) << 25) | (date_s.Month<<21) | (date_s.Date<<16)|\
            (rtc_time.Hours<<11) | (rtc_time.Minutes<<5) | (rtc_time.Seconds);

    return date;

}
