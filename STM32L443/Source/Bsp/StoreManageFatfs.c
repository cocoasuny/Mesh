#include "StoreManageFatfs.h"
#include "diskio.h"
#include <time.h>

FIL  File;          /* File object */
FATFS fs;           /* File system object */


/* Private Variables */
static FILE_INFO_T         s_currentWRFile;           //当前写文件
static READ_FILE_INFO_T    s_currentRDFile;           //当前读文件
static PRE_WRITE_INFO_T    s_pre_write_buffer;        //写数据缓存buffer
static uint32_t            s_uTotleValidLength = 0;   //可用数据总长度
static bool                s_pre_write_buffer_clearFlag = false;
static uint8_t             s_readOutDataLen = 0;      //已被读出数据长度

/* Private functions */
//static FRESULT wtite_data_to_currentFile(uint8_t *pbuf,uint16_t len);
static FRESULT scan_files_InPath(char* path, FILE_INPATH_INFO_T * totleFileInfo);
static FRESULT scan_all_files(FILE_INPATH_INFO_T * totleFileInfo);
static FRESULT deleteFileByName(char *filename);
static void getFreeSpace(uint32_t *totleSize,uint32_t *freeSize);
static void avaliableSpaceCheck(void);
static void getFileOrder(FILE_INFO_T *firstFile,FILE_INFO_T *secFile,FILE_INFO_T *lastFile);
static void freeSpaceNotEnoughCB(uint32_t totleSize, uint32_t freeSize);


#define STORAGE_FATFS_DEBUG

void FatFs_Test(void)
{ 
//    FATFS fs;           /* File system object */
    FIL fil;            /* File object */
    FRESULT res;        /* API result code */
    UINT bw;            /* Bytes written */
//    BYTE work[_MAX_SS]; /* Work area (larger is better for process time) */
    char buffer[5] = {0};
    uint16_t i =0;
    uint32_t re=0;
    
    printf("Start Fat test\r\n");
 
    /* Register work area */
    res = f_mount(&fs, "", 0);
	if (res != FR_OK)
	{
		printf("f_mount err: %d\r\n",res);
	}
    
    /* Create FAT volume */
    res = f_mkfs("",1,4096);
	if (res != FR_OK)
	{
		printf("format err: %d\r\n",res);
	}
    else
    {
        printf("format ok\r\n");
    }
    
    /* Create a file as new */
    res = f_open(&fil, "hello.txt", FA_CREATE_NEW | FA_WRITE);
	if (res != FR_OK)
	{
		printf("f_open err: %d\r\n",res);
	}
    else
    {
        printf("f_open ok\r\n");
    }

    /* Write a message */
    res = f_write(&fil, "Hello, World!\r\n", 15, &bw);
    if ((bw != 15) || (res != FR_OK))
    {
        printf("write err:%d\r\n",res);
    }

    for(i = 0;i < 512;i++)
    {
        buffer[i] = 0x00;
    }
    res = f_read(&fil,buffer,15,&re);
    printf("%s",buffer);
    if(res || re == 0)
    {
        printf("read done\r\n");
    }
    
    /* Close the file */
    f_close(&fil);

    /* Unregister work area */
    f_mount(0, "", 0);
    
    printf("End of Fat test\r\n");
}

/**
  * @brief  storage manage init
  * @param  date
  * @retval None
  */
FRESULT store_manage_init(void)
{
    FRESULT res;        /* API result code */
 
    /* Register work area */
    res = f_mount(&fs, "", 1);
    
    /* 格式化一下，测试使用 */
//    res = f_mkfs("",1,4096);
    
    /* Create FAT volume */
	if (res != FR_OK)
	{
        #ifdef STORAGE_FATFS_DEBUG
            printf("fatfs mount err:0x%x\r\n",res);
        #endif
        
		/* Create FAT volume */
        res = f_mkfs("",1,4096);
        
        #ifdef STORAGE_FATFS_DEBUG
            if(res != FR_OK)
            {
                printf("fatfs format err:0x%x",res);
            }
            else
            {
                printf("fatfs format ok\r\n");
            }
        #endif             
	}
    #ifdef STORAGE_FATFS_DEBUG
    else
    {
        printf("Register FS OK\r\n");
    }
    #endif
    
    if(res == FR_OK)    //挂载文件系统成功，完成存储管理初始化工作
    {
        /* 清除当前缓存Buffer */
        s_pre_write_buffer.len = 0;
        memset(s_pre_write_buffer.buf,0,PRE_BUFFER_SIZE);        
        
        /* 清除当前读文件信息 */
        s_currentRDFile.isFileToBeDelete = false;
        s_currentRDFile.pRdLen = 0;    //！！！上电时，需要从EEPROM中获取该长度
        memset(s_currentRDFile.fileInfo.fileName,MAX_FILE_NAME_LEN,0);  //！！！上电时，需要从EEPROM中获取该长度
        s_currentRDFile.fileInfo.size = 0;
        
        /* 查找当前写文件 */
        findCurrentWRFile(&s_currentWRFile);
        
        /* 查看EEPROM存储的文件是否还在 */
        res = f_open(&File,s_currentRDFile.fileInfo.fileName,FA_OPEN_EXISTING | FA_READ);
        
        if(res != FR_OK)  //文件打不开，说明文件已被删除或其他原因
        {
            findCurrentRDFile(&s_currentRDFile.fileInfo);  //查找新的当前读文件
            s_currentRDFile.pRdLen = 0;
            s_currentRDFile.isFileToBeDelete = false;
        }
        else  //文件打开成功，说明文件正常，关闭当前读文件
        {
            f_close(&File);
        }
    }
    #ifdef STORAGE_FATFS_DEBUG
        TestFileDir();
        printf("Current wr file init:%s,%d\r\n",s_currentWRFile.fileName,s_currentWRFile.size);
        printf("Current rd file init:%s,%d\r\n",s_currentRDFile.fileInfo.fileName,s_currentRDFile.fileInfo.size);
    #endif
    return res;
}
/**
  * @brief  创建OSA方案数据存储文件
  * @param  None
  * @retval None
  */
void creatOSAFile(void)
{
    RTC_DateTypeDef     date_s;               //RTC 日期
    RTC_TimeTypeDef     rtc_time;             //RTC 时间
    uint32_t            date = 0;
    
    calendar_get(&date_s,&rtc_time);
    date = GetTick(date_s.Year+2000,date_s.Month,date_s.Date,rtc_time.Hours,rtc_time.Minutes,rtc_time.Seconds);
    
    File_Creat(date);    
}
/**
  * @brief  穿件个性化方案或管理类方案白天数据存储文件
  * @param  None
  * @retval None
  */
void creatCustomTemplateFile(void)
{
    uint32_t    u32Date = 0;
   
    u32Date = CovernDateto32();
    
    File_Creat(u32Date);
}
/**
  * @brief  OSA结束时调用，将Buffer中缓存的数据写入文件
  * @param  None
  * @retval None
  */
void closeOSAFile(void)
{
    PRE_WRITE_INFO_T        buffer;
    
    /* 将Buffer中缓存的数据写入当前文件中 */
    if(s_pre_write_buffer.len != 0)  //缓存数据Buffer中有数据 
    {
        memset(buffer.buf,0,PRE_BUFFER_SIZE);
        buffer.len = 0;
        
        memcpy(buffer.buf,s_pre_write_buffer.buf,s_pre_write_buffer.len);
        buffer.len = s_pre_write_buffer.len;

        /* 将数据写入当前文件中 */
        wtite_data_to_currentFile(buffer.buf,buffer.len);
        
        /* Buffer中数据已写入文件中，清除缓存Buffer */
        s_pre_write_buffer.len = 0;
        memset(s_pre_write_buffer.buf,0,PRE_BUFFER_SIZE);
    }
}
/**
* @brief  使用日期作为文件名创建文件
  * @param  date
  * @retval None
  */
void File_Creat(uint32_t date)
{
    FRESULT ret = FR_NO_FILE;
    char fileName[MAX_FILE_NAME_LEN] = "a.txt";
    PRE_WRITE_INFO_T buffer;
    
    memset(buffer.buf,0,PRE_BUFFER_SIZE);
    buffer.len = 0;
    
    /* 以日期为文件名创建文件 */
    memset(fileName,0,MAX_FILE_NAME_LEN);
    sprintf(fileName,"%08x.txt",date);     //采用unix时间戳，转换为字符串为8字节
    
    if(memcmp(fileName,s_currentWRFile.fileName,MAX_FILE_NAME_LEN) != 0)  //写入数据日期与当前写文件不一致
    { 
        /* 将Buffer中缓存数据写入当前文件中 */
        if(s_pre_write_buffer.len != 0)  //缓存数据Buffer中有数据
        {
            memset(buffer.buf,0,PRE_BUFFER_SIZE);
            buffer.len = 0;
            
            memcpy(buffer.buf,s_pre_write_buffer.buf,s_pre_write_buffer.len);
            buffer.len = s_pre_write_buffer.len;

            /* 将数据写入当前文件中 */
            wtite_data_to_currentFile(buffer.buf,buffer.len);
        }
        
        
        /* 创建新的文件 */
        /* 以写的方式创建文件，如果该文件已存贮，则删除已存在的文件重新创建 */
        ret = f_open(&File,fileName,FA_CREATE_ALWAYS | FA_WRITE);
        if(ret != FR_OK)
        {
            #ifdef STORAGE_FATFS_DEBUG
                printf("Creat Fil Err:%d\r\n",ret);
            #endif
        }
        else
        {
            f_close(&File);
        }
        
        /* 更新当前写文件 */
        memcpy(s_currentWRFile.fileName,fileName,MAX_FILE_NAME_LEN);
        s_pre_write_buffer.len = 0;
        memset(s_pre_write_buffer.buf,0,PRE_BUFFER_SIZE);
        #ifdef STORAGE_FATFS_DEBUG
            printf("\r\nUpdate files in Creat file\r\n");
            TestFileDir();
            printf("update wr file in Creat file:%s\r\n",s_currentWRFile.fileName);
        #endif
    }
}
/**
  * @brief  将数据写入存储管理中
  * @param  type,pu8Src,u8Length
  * @retval status:0,Success;255:Error
  */
uint8_t write_data_to_store_manage(uint8_t type,uint8_t * pu8Src,uint8_t u8Length)
{
    uint16_t                MaxDataLen = 0;
    PRE_WRITE_INFO_T        buffer;
    uint8_t                 ret = 0;   
#ifdef STORAGE_FATFS_DEBUG
    uint8_t     i = 0;
#endif    
    
    
    type = type;  //暂时未使用

    MaxDataLen = (PRE_BUFFER_SIZE/u8Length) * u8Length;
    
    #ifdef STORAGE_FATFS_DEBUG
		printf("\r\n[FLASH] ExtFLASH_ExtWrite\r\n");
		printf("	[FLASH] INCOMING DATA : ");
		for(i = 0;i < u8Length;i ++)
		{
			printf("0x%02x, ",*(pu8Src + i));
		}
		printf("\r\n");    
    #endif

    if((u8Length + s_pre_write_buffer.len) <= MaxDataLen) //未填满缓存buffer，在此之前填满了该buffer是不对的
    {
        memcpy((s_pre_write_buffer.buf+s_pre_write_buffer.len),pu8Src,u8Length);
        s_pre_write_buffer.len = s_pre_write_buffer.len + u8Length;
        s_uTotleValidLength = s_uTotleValidLength + u8Length;
        
        ret = 0;
    }
    else
    {
        ret = 255;
    }
    
    if(s_pre_write_buffer.len >= MaxDataLen) //写入数据后，缓存buffer已满
    {
        memset(buffer.buf,0,PRE_BUFFER_SIZE);
        buffer.len = 0;
        
        memcpy(buffer.buf,s_pre_write_buffer.buf,s_pre_write_buffer.len);
        buffer.len = s_pre_write_buffer.len;

        /* 将数据写入当前文件中 */
        wtite_data_to_currentFile(buffer.buf,buffer.len);
        
        /* buffer中数据已写入当前文件中，清除缓存Buffer */
        s_pre_write_buffer.len = 0;
        memset(s_pre_write_buffer.buf,0,PRE_BUFFER_SIZE);
    }
    
    return ret;
}

/**
  * @brief  从存储管理中读取数据
  * @param[in]  type，pu8Src，u8Length(需要读取的长度)
  * @param[out] len(可以读取的长度)
  * @retval status:0,Success;255:Error
  */
uint8_t read_data_from_store_manage(uint8_t type,uint8_t * pu8Src,uint8_t u8Length,uint8_t *avalibLen)
{
    uint8_t             res = 0;
    FRESULT             ret = FR_NO_FILE;
    uint32_t            readByte = 0;
    uint8_t             len = 0;
    uint8_t             totleFileNum = 0;     
    
    type = type;  //暂时未用
    
    #ifdef STORAGE_FATFS_DEBUG
        printf("read file:%s\r\n",s_currentRDFile.fileInfo.fileName);
    #endif

    ret = f_open(&File,s_currentRDFile.fileInfo.fileName,FA_OPEN_EXISTING | FA_READ);
    
    if(ret != FR_OK)  //文件打不开，说明文件已被删除或其他原因
    {
        getTotleValidFileNum(&totleFileNum);
        
        if(totleFileNum != 0)  //还有文件没有被读走
        {
            findCurrentRDFile(&s_currentRDFile.fileInfo);  //查找新的当前读文件
            s_currentRDFile.pRdLen = 0;
            s_currentRDFile.isFileToBeDelete = false;
            
            ret = f_open (&File,s_currentRDFile.fileInfo.fileName,FA_OPEN_EXISTING | FA_READ);
            
            f_lseek(&File,s_currentRDFile.pRdLen);
            
            /* 当前读文件已被打卡 */
            ret = f_read(&File,pu8Src,u8Length,&readByte);
            if(ret == FR_OK)   //读文件成功
            {
                len = (uint8_t)readByte;
                
                if(f_eof(&File) != 0)
                {
                    //当前读文件已读完，需要读下一个文件，在收到ACK后可以删除当前读文件
                    s_currentRDFile.isFileToBeDelete = true;        
                }
            }
            else  //需要考虑读取文件失败时的处理
            {
                readByte = 0;
                #ifdef STORAGE_FATFS_DEBUG
                    printf("read file err:%d\r\n",ret);
                #endif
                
            }
            
            /* 关闭当前读文件 */
            f_close(&File);            
        }
        else  //所有文件已被读走，读取buffer中的数据
        {
            //读取buffer中缓存的数据
            len = (uint8_t)s_pre_write_buffer.len;
            memcpy(pu8Src,s_pre_write_buffer.buf,s_pre_write_buffer.len);
            
            s_pre_write_buffer_clearFlag = true;
            
            res = 1;
        }
    }
    else  //继续读取文件
    {
        f_lseek(&File,s_currentRDFile.pRdLen);
        
        /* 当前读文件已被打开 */
        ret = f_read(&File,pu8Src,u8Length,&readByte);
        if(ret == FR_OK)   //读取文件成功
        {
            len = (uint8_t)readByte;
            
            if(f_eof(&File) != 0)
            {
                //当前读文件已读完，需要读取下一个文件，在收到ACK后可以删除当前读文件
                s_currentRDFile.isFileToBeDelete = true;        
            }
        }
        else  //需要考虑读取文件失败时的处理
        {
            readByte = 0;
            #ifdef STORAGE_FATFS_DEBUG
                printf("read file err:%d\r\n",ret);
            #endif
            
        }
        
        /* 关闭当前读文件 */
        f_close(&File);
    }
   
    s_readOutDataLen = len;
    *avalibLen = len;
    
    if((ret == FR_OK) || (res == 1))
    {
        res = 1;
    }
    else
    {
        res = 255;
    }
       
    #ifdef STORAGE_FATFS_DEBUG
        printf("is file to be deleted:%d\r\n",s_currentRDFile.isFileToBeDelete);
        printf("avalibe len:%d\r\n",*avalibLen);
    #endif
    
    return res;
}
/**
  * @brief  响应数据同步ACK
  * @param  len
  * @retval status:0,Success;255:Error
  */
uint8_t ack_datalen_to_store_manage(uint8_t len)
{
    uint8_t res = 0;
    
    if(len == s_readOutDataLen)  //收到的ACK长度等于之前被读走的数据长度
    {
        s_readOutDataLen = 0;
        s_uTotleValidLength = s_uTotleValidLength - len;

        if(s_currentRDFile.isFileToBeDelete == true) //当前读文件已读完，需要删除
        {
            deleteFileByName(s_currentRDFile.fileInfo.fileName);
            
            /* 查找下一个当前读文件 */
            findCurrentRDFile(&s_currentRDFile.fileInfo);  //查找新的当前读文件
            s_currentRDFile.pRdLen = 0;
            s_currentRDFile.isFileToBeDelete = false;            
        }
        else  //当前读文件还未读完，偏移当前读文件读指针
        {
            s_currentRDFile.pRdLen = s_currentRDFile.pRdLen + len;
        }
        
        if(s_pre_write_buffer_clearFlag == true)  //清除Buffer中缓存的数据
        {
            s_pre_write_buffer_clearFlag = false;
            s_pre_write_buffer.len = 0;
            memset(s_pre_write_buffer.buf,0,PRE_BUFFER_SIZE);
        }        
    }
    else
    {
        res = 255;
    }
    
    return res;
}
/**
  * @brief  调整读出的数据，主要用于在读取一定长度后，进行数据转换，退回的长度
  * @param  len
  * @retval none
  */
void adjustReadOutDataLen(uint8_t len)
{
    s_readOutDataLen = len;
    #ifdef STORAGE_FATFS_DEBUG
        printf("Adj Read Out Len:%d\r\n",len);
    #endif
}       

/**
  * @brief  写入数据至当前写文件
  * @param  pbuf,len
  * @retval result
  */
FRESULT wtite_data_to_currentFile(uint8_t *pbuf,uint16_t len)
{
    FRESULT         ret = FR_NO_FILE;
    FSIZE_t         filesize = 0;
    uint32_t        bw = 0;
    
    #ifdef STORAGE_FATFS_DEBUG
        uint16_t i = 0;
        printf("write date to file:%s\r\n",s_currentWRFile.fileName);
    #endif
    
    /* 写入文件前判断剩余可用空间大小 */
    avaliableSpaceCheck();
    
    /* 打开当前写文件 */ //!!!!!!!!!可以修改，打开当前文件失败时，重新查找新的当前写文件
    ret = f_open(&File,s_currentWRFile.fileName,FA_OPEN_EXISTING | FA_WRITE);
    if(ret == FR_OK)
    {
        filesize = f_size(&File);
        ret = f_lseek(&File,filesize);
        #ifdef STORAGE_FATFS_DEBUG
            if(ret != FR_OK)
            {
                printf("f_lseek err:%d\r\n",ret);
            }            
        #endif

        ret = f_write(&File,pbuf,len,&bw);

        #ifdef STORAGE_FATFS_DEBUG
            if((bw == 0) || (ret != FR_OK))
            {
                printf("write data err:%d\r\n",ret);
            }
        #endif

        ret = f_close(&File);

        #ifdef STORAGE_FATFS_DEBUG
            if(ret != FR_OK)
            {
                printf("f_close err:%d\r\n",ret);
            }            
        #endif            
    }
    #ifdef STORAGE_FATFS_DEBUG
    else
    {
        printf("f_open err:%d\r\n",ret);
    }
    #endif
    
    #ifdef STORAGE_FATFS_DEBUG
        if(ret == FR_OK)
        {
            for(i=0;i<len;i++)
            {
                printf("0x%02x,",pbuf[i]);
            }
            printf("write file OK\r\n");
        }
        else
        {
            printf("write file Err:%d\r\n",ret);
        }
    #endif
    
    return ret;
}
/**
  * @brief      查找当前写文件
  * @param[in]  file
  * @retval     None
  */
void findCurrentWRFile(FILE_INFO_T *file)
{
    FILE_INPATH_INFO_T      totlefile;
    FILE_INFO_T             crtfile;
    uint8_t i = 0;
    uint32_t                totleLength = 0;   //所有可用总长度        

    /* 从文件系统中获取所有的文件 */    
    scan_all_files(&totlefile);
    
    if(totlefile.fileTotleNum > 0)    //存储管理中有文件，把第一个文件作为当前写文件查找初始文件
    {
        memcpy(crtfile.fileName,totlefile.info[0].fname,MAX_FILE_NAME_LEN);
        crtfile.size = totlefile.info[0].fsize;
    }
    else   //存储管理中没有文件，将当前写文件名置0
    {
        memset(crtfile.fileName,0,MAX_FILE_NAME_LEN);
        crtfile.size = 0;
    }
    
//    printf("totle file num:%d\r\n",totlefile.fileTotleNum);
//    printf("init current file:%s\r\n",crtfile.fileName);
 
    /* 对所有的文件进行排序，文件名(日期最大的)为当前写文件 */    
    for(i=0;i<totlefile.fileTotleNum;i++)
    {
//        printf("file name:%s,  ",totlefile.info[i].fname);
//        printf("file size:%d\r\n",totlefile.info[i].fsize);
        
        if(strcmp(crtfile.fileName,totlefile.info[i].fname) < 0)
        {
//            printf("copy\r\n");
            memcpy(crtfile.fileName,totlefile.info[i].fname,MAX_FILE_NAME_LEN);
            crtfile.size = totlefile.info[i].fsize;
        }
//        else
//        {
//            printf("Not Copy\r\n");
//        }
    }
//    printf("current wr file:%s\r\n",crtfile.fileName);
    /* 找到当前读文件，更新当前写文件信息 */
    memcpy(file->fileName,crtfile.fileName,MAX_FILE_NAME_LEN);
    file->size = crtfile.size;
    
    /* 计算所有文件总长度，计算可用总长度 */
    for(i=0;i<totlefile.fileTotleNum;i++)
    {
        //将每个文件大小相加
        totleLength = totleLength + totlefile.info[i].fsize;
    }
    totleLength = totleLength + s_pre_write_buffer.len;  //所有文件大小加上缓存buffer中数据大小
    s_uTotleValidLength = totleLength;
}
/**
  * @brief      查找当前读文件
  * @param[in]  file
  * @retval     None
  */
void findCurrentRDFile(FILE_INFO_T *file)
{
    FILE_INPATH_INFO_T      totlefile;
    FILE_INFO_T             crtfile;
    uint8_t i = 0;
    
    /* 从文件系统中获取所有文件 */
    scan_all_files(&totlefile);     
    
    if(totlefile.fileTotleNum > 0)   //存储管理中有文件，把第一个文件作为当前读文件查找初始文件
    {
        memcpy(crtfile.fileName,totlefile.info[0].fname,MAX_FILE_NAME_LEN);
        crtfile.size = totlefile.info[0].fsize;
    }
    else   //存储管理中没有文件，将当前读文件名置0
    {
        memset(crtfile.fileName,0,MAX_FILE_NAME_LEN);
        crtfile.size = 0;
    }
    
//    printf("totle file num:%d\r\n",totlefile.fileTotleNum);
//    printf("init current file:%s\r\n",crtfile.fileName);
    
    /* 对所有的文件进行排序，文件名(日期最小的)为当前读文件 */
    for(i=0;i<totlefile.fileTotleNum;i++)
    {
//        printf("file name:%s,  ",totlefile.info[i].fname);
//        printf("file size:%d\r\n",totlefile.info[i].fsize);
        
        if(strcmp(crtfile.fileName,totlefile.info[i].fname) > 0)
        {
//            printf("copy\r\n");
            memcpy(crtfile.fileName,totlefile.info[i].fname,MAX_FILE_NAME_LEN);
            crtfile.size = totlefile.info[i].fsize;
        }
//        else
//        {
//            printf("Not Copy\r\n");
//        }
    }
//    printf("current rd file:%s\r\n",crtfile.fileName);
    /* 找到当前读文件，更新当前读文件信息 */
    memcpy(file->fileName,crtfile.fileName,MAX_FILE_NAME_LEN);
    file->size = crtfile.size;
}
/**
  * @brief      getTotleValidDataLength，获取可用数据总长度，查找当前读文件时，会更新该总长度
  * @param[in]  *len
  * @retval     None
  */
void getTotleValidDataLength(uint32_t *len)
{
    *len = s_uTotleValidLength;
}
/**
  * @brief      获取当前文件系统中总文件数
  * @param[in]  *num
  * @retval     None
  */
void getTotleValidFileNum(uint8_t *num)
{
    FILE_INPATH_INFO_T      totlefile;
    
    /* 从文件系统中获取所有的文件 */
    scan_all_files(&totlefile); 

    *num = totlefile.fileTotleNum;
}
/**
* @brief      可用存储空间不足回调函数,可用于对可用空间小于设定告警值@ALARM_SIZE时的处理
  * @param[in]  uint32_t totleSize, uint32_t freeSize
  * @retval     None
  */
static void freeSpaceNotEnoughCB(uint32_t totleSize, uint32_t freeSize)
{
    //可用存储空间小于设定的告警值@ALARM_SIZE
    #ifdef STORAGE_FATFS_DEBUG
        /* Print the free space (assuming 4096 bytes/sector) */
        printf("%10lu KiB total drive space.\n%10lu KiB available.\n",totleSize, freeSize); 
    #endif 
    
}

/**
  * @brief      scan_all_files，查找文件系统中所有文件
  * @param[in]  totleFileInfo
  * @retval     FRESULT
  */
static FRESULT scan_all_files(FILE_INPATH_INFO_T * fileInfo)
{
    FRESULT res;
    char buff[10] = {0};
    FILE_INPATH_INFO_T info;
    uint8_t i = 0;
    
    strcpy(buff, "/");
    res = scan_files_InPath(buff,&info);
    
    fileInfo->fileTotleNum = info.fileTotleNum;    
    for(i=0;i<info.fileTotleNum;i++)
    {
        memcpy(fileInfo->info[i].fname,info.info[i].fname,13);
        fileInfo->info[i].fsize = info.info[i].fsize;
    }
    
    return res;
}

/**
  * @brief      scan_files，查找路劲下所有文件
  * @param[in]  char* path, FILE_INPATH_INFO_T * totleFileInfo
  * @retval     FRESULT
  */
static FRESULT scan_files_InPath(char* path, FILE_INPATH_INFO_T * totleFileInfo)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                        /* Open the directory */
    if (res == FR_OK) 
	{
        for (i=0;i<MAX_FILE_TOTLE_NUM;i++) 
		{
            res = f_readdir(&dir, &fno);                /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) 
			{
				break;  								/* Break on error or end of dir */
			}
            if (fno.fattrib & AM_DIR)  					/* It is a directory */
			{                    
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files_InPath(path,NULL);                 /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } 
			else 
			{                                           /* It is a file. */
//                printf("%s/%s,%d\n", path, fno.fname,fno.fsize);
                memcpy(totleFileInfo->info[i].fname,fno.fname,13);
                totleFileInfo->info[i].fsize = fno.fsize;
            }
        }
        f_closedir(&dir);
        
        totleFileInfo->fileTotleNum = i;
    }

    return res;
}
/**
  * @brief      根据文件名删除文件
  * @param[in]  char* filename
  * @retval     FRESULT
  */
static FRESULT deleteFileByName(char *filename)
{
    FRESULT ret = FR_INVALID_PARAMETER;
    
    #ifdef STORAGE_FATFS_DEBUG
        printf("delete file :%s\r\n",filename);
    #endif
    
    /* delate a file by the Name */
    ret = f_unlink(filename);
    
    if(ret != FR_OK)
    {
        /* file Open for write Error */
        #ifdef STORAGE_FATFS_DEBUG
            printf("delete the file:%s Err:%d\r\n",filename,ret); 
        #endif
    }
    else
    {
        #ifdef STORAGE_FATFS_DEBUG
            printf("delete the file:%s OK\r\n",filename); 
        #endif      
    }
    
    return ret;
}
/**
  * @brief       获取空间总大小及剩余可用空间大小
  * @param[out]  uint32_t *totleSize,uint32_t *freeSize
  * @retval      None
  */
static void getFreeSpace(uint32_t *totleSize,uint32_t *freeSize)
{
    DWORD fre_clust, fre_sect, tot_sect;
    FATFS *fs;
    uint32_t    tSize = 0;
    uint32_t    fSize = 0; 

    /* Get volume information and free clusters of drive 1 */
    f_getfree("", &fre_clust, &fs);


    /* Get total sectors and free sectors */
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
    
    tSize = tot_sect * 4;  //4Kbyte per sector
    fSize = fre_sect * 4;
    
    *totleSize = tSize;
    *freeSize = fSize;
}
/**
  * @brief       可用存储空间检查,每次写入文件时调用
  * @param       None
  * @retval      None
  */
static void avaliableSpaceCheck(void)
{
    uint32_t    totleSize = 0;
    uint32_t    freeSize = 0; 
    FILE_INFO_T    firstFile;
    FILE_INFO_T    secondFile;
    FILE_INFO_T    lastFile;
    
    getFreeSpace(&totleSize,&freeSize);
       
    if(freeSize < DELETE_SIZE)
    {
        //获取文件系统中第一、第二、最后一个文件
        getFileOrder(&firstFile,&secondFile,&lastFile);
        
        // 删除第一个文件
        if(strcmp(s_currentWRFile.fileName,firstFile.fileName) == 0)
        {
            //当前写文件等于第一个文件，则删除第二个文件
            deleteFileByName(secondFile.fileName);
        }
        else  //当前写文件不等于第一个文件
        {
            deleteFileByName(firstFile.fileName);
        }        
    }
    if(freeSize < ALARM_SIZE)
    {
        //提示空间不足
        freeSpaceNotEnoughCB(totleSize,freeSize);
    }
}
/**
  * @brief       冒泡法对文件排序(从小到大)，获取文件系统中第一、第二、最后一个文件
  * @param       None
  * @retval      None
  */
static void getFileOrder(FILE_INFO_T *firstFile,FILE_INFO_T *secFile,FILE_INFO_T *lastFile)
{  
    int low = 0;   
    int high= 0; //设置变量的初始值
    int j=0;
    char *tempfile; 
    FILE_INPATH_INFO_T   file;  

    /* 从文件系统中获取所有文件 */
    scan_all_files(&file);
    
    if(file.fileTotleNum > MAX_FILE_TOTLE_NUM)
    {
        high= MAX_FILE_TOTLE_NUM -1;
    }
    else
    {
        high= file.fileTotleNum -1;
    }
    
    while(low < high) 
    {  
        for(j= low; j< high; ++j) //正向冒泡，找到最大值
        {
            if(strcmp(file.info[j].fname,file.info[j+1].fname) > 0)
            {
                memcpy(tempfile,file.info[j].fname,13);
                memcpy(file.info[j].fname,file.info[j+1].fname,13);
                memcpy(file.info[j+1].fname,tempfile,13);
            }
        }            
        --high;                   //修改High值，前移一位
        for ( j=high; j>low; --j) //反向冒泡，找到最小值
        {
            if(strcmp(file.info[j].fname,file.info[j-1].fname) < 0)
            {
                memcpy(tempfile,file.info[j].fname,13);
                memcpy(file.info[j].fname,file.info[j-1].fname,13);
                memcpy(file.info[j-1].fname,tempfile,13);
            }                
        }            
        ++low;                    //修改Low值，后移一位
    }

    firstFile->size = file.info[0].fsize;
    memcpy(firstFile->fileName,file.info[0].fname,13);
    
    secFile->size = file.info[1].fsize;
    memcpy(secFile->fileName,file.info[1].fname,13);
    
    lastFile->size = file.info[file.fileTotleNum-1].fsize;
    memcpy(lastFile->fileName,file.info[file.fileTotleNum-1].fname,13);
    
    //for test 
//    uint8_t i = 0;
//    for(i=0;i<file.fileTotleNum;i++)
//    {
//        printf("file name:%s,%d\r\n",file.info[i].fname,i);
//    }
}

/*****************以下为存储管理测试函数*******************************************/
/**
  * @brief  scan_files，查找路劲下所有文件
  * @param  char* path 
  * @retval FRESULT
  */
FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;

    res = f_opendir(&dir, path);                        /* Open the directory */
    if (res == FR_OK) 
	{
        for (;;) 
		{
            res = f_readdir(&dir, &fno);                /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0)
			{
                printf("file find Complete\r\n");
				break;  								/* Break on error or end of dir */
			}
            if (fno.fattrib & AM_DIR)  					/* It is a directory */
			{                    
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                 /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } 
			else 
			{                                           /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}
void TestCreatOSAFile(void)
{
    uint32_t    date = 0;
    
    date = GetTick((2016),9,7,20,02,20);
    
    File_Creat(date);    
    
    date = GetTick((2016),9,8,20,02,20);
    
    File_Creat(date);
    
    date = GetTick((2016),9,20,20,02,20);
    
    File_Creat(date);

    date = GetTick((2016),9,12,20,02,20);
    
    File_Creat(date);
    
    date = GetTick((2016),9,5,20,02,20);
    
    File_Creat(date);    
}

void TestCreatCustom(void)
{
    uint32_t    u32Date = 0;
   
    u32Date  = (uint32_t)((2016 << 16) | (8 << 8) | (10));
    
    File_Creat(u32Date);
    
    u32Date  = (uint32_t)((2016 << 16) | (8 << 8) | (5));
    
    File_Creat(u32Date);

    u32Date  = (uint32_t)((2016 << 16) | (9 << 8) | (10));
    
    File_Creat(u32Date);    
}

void TestFileDir(void)
{
    char buff[20] = {0};
    
    strcpy(buff, "/");
    scan_files(buff);
}

void TestWriteData(void)
{
    uint16_t i = 0,j=0;
    uint8_t buf[256] = "Test123456";
    
    
    for(i=0;i<256;i++)
    {
        buf[i] = i;
    }
    
    for(i=0;i<4000;i++)
    {
        for(j=0;j<256;j++)
        {
            buf[j] = i;
        }
        printf("wr cnt:%d\r\n",i);
        wtite_data_to_currentFile(buf,256);
    }
}
void TestReadFata(void)
{
    FRESULT         ret = FR_NO_FILE;
    FSIZE_t         filesize = 0;
    uint32_t        bw = 0;
    uint16_t         i = 0,j=0;;
    char            buf[256] = {0};
    
    /* 打开当前写文件 */
    ret = f_open(&File,s_currentWRFile.fileName,FA_OPEN_EXISTING | FA_READ);
    if(ret == FR_OK)
    {
        filesize = f_size(&File);
        #ifdef STORAGE_FATFS_DEBUG
                printf("f_read:%s,%d\r\n",s_currentWRFile.fileName,filesize);
        #endif

        for(j=0;j<4000;j++)
        {
            ret = f_read(&File,buf,256,&bw);

            #ifdef STORAGE_FATFS_DEBUG
                if((bw == 0) || (ret != FR_OK))
                {
                    printf("read data err:%d\r\n",ret);
                }
                else
                {
                    printf("Read Data:%d,%d\r\n",bw,j);
                    for(i=0;i<256;i++)
                    {
                        printf("%d,",buf[i]);
                    }
                    printf("\r\n");
                }
                memset(buf,0,256);
            #endif
          }

        ret = f_close(&File);

        #ifdef STORAGE_FATFS_DEBUG
            if(ret != FR_OK)
            {
                printf("f_close err:%d\r\n",ret);
            }            
        #endif            
    }
    #ifdef STORAGE_FATFS_DEBUG
    else
    {
        printf("f_open err:%d\r\n",ret);
    }
    #endif    
}

void TestGetFreeSpace(void)
{
    DWORD fre_clust, fre_sect, tot_sect;
    FATFS *fs;
    uint32_t    totleSize = 0;
    uint32_t    freeSize = 0; 

    /* Get volume information and free clusters of drive 1 */
    f_getfree("", &fre_clust, &fs);


    /* Get total sectors and free sectors */
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    fre_sect = fre_clust * fs->csize;
    
    totleSize = tot_sect * 4;
    freeSize = fre_sect * 4;

    /* Print the free space (assuming 4096 bytes/sector) */
    printf("%10lu KiB total drive space.\n%10lu KiB available.\n",
           totleSize, freeSize);     
}

void TestGetOrder(void)
{
    FILE_INFO_T    firstFileName;
    FILE_INFO_T    secondFileName;
    FILE_INFO_T    lastFileName;
    
    getFileOrder(&firstFileName,&secondFileName,&lastFileName);
    
    printf("first:%s\r\n",firstFileName.fileName);
    printf("sec:%s\r\n",secondFileName.fileName);
    printf("last:%s\r\n",lastFileName.fileName);
}

void TestStoreManage(void)
{    
    TestGetFreeSpace();
//    TestCreatOSAFile();
//    TestCreatCustom();
//    TestFileDir();
//    TestGetOrder();
//    TestWriteData();
//    TestFileDir();
//    TestReadFata();
//    FatFs_Test(); 
}

