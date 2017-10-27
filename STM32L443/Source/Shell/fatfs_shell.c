/*********************************Copyright (c)*********************************
** File Name:               fatfs_shell.c
** Last modified Date:      2016/7/26
** Last Version:            V1.0
** Description:             文件操作命令解释
** HW_CMU:                  ANSIC
** Libraries:               NONE
** version                  NONE
******************************************************************************/


#include "shell.h"          //Shell
#include "StoreManageFatfs.h"

/*---------------------* 
*     Shell版本判断
*----------------------*/
#ifdef SHELL_VER
  #if (SHELL_VER < 2)
    #error "shell版本太低"
  #endif
#else
    #error "未找到Shell文件，或Shell版本信息"
#endif


#ifdef RTC_SHELL
  extern void Shell_Fatfs_Service(void);
#else
  void Shell_Fatfs_Service(void){;}
#endif

#ifdef FATFS_SHELL
/*---------------------* 
*       
*----------------------*/
//命令帮助文件
const char Fatfs_HelpMsg[] =
	"[Fatfs contorls]\r\n"
	" fil help                            -  help.\r\n"
	" fil cap                             -  the capacity of Disk.\r\n"
	" fil creatOSA                        -  creat a file for OSA.\r\n"
    " fil creatCustom                     -  creat a file for custom.\r\n"
    " fil open <Name>                     -  open a file by name.\r\n"
    " fil close                           -  close the file.\r\n"
    " fil delete <Name>                   -  delete a file by name.\r\n"
    " fil tell                            -  get the wr/rd pointer.\r\n"
    " fil sync                            -  sync the open file.\r\n"
    " fil size                            -  get size of the open file.\r\n"
    " fil wr <data>                       -  write the datas to the file.\r\n"
	" fil dir                             -  display all files in current directory \r\n"
    " fil current wr file                 -  find thr current write file\r\n"
    " fil current rd file                 -  find thr current read file\r\n"
    " fil format                          -  format the disk\r\n"
	" fil upload <Name>                   -  upload the file by name\r\n"
	"\r\n";
	
//static FIL MyFile;  //***!!!!!****会额外多出4KRAM的需求


/****************************************************************************** 
********************************* 函 数 声 明 ********************************* 
******************************************************************************/  
  
/****************************************************************************** 
/ 函数功能:文件系统Shel指令处理 
/ 修改日期:2016-7-26
/ 输入参数:输入当前的程序版本 
/ 输出参数:none 
/ 使用说明:none 
********************************************************************************/
void Shell_Fatfs_Service(void)
{
    uint8_t     *ptRxd;         //用于接收指令处理  
    uint8_t     i;
    
    RTC_DateTypeDef date_s;
    RTC_TimeTypeDef rtc_time;
    char openfileName[20] = "0";
    uint8_t wrData[256] = "0";
    char rtext[8] = "0";
    uint32_t br = 0;
	char buff[256] = {0};
    FRESULT ret = FR_NO_FILE;
    uint32_t date=0;  
    FILE_INFO_T file_test;
    FIL MyFile;  //***!!!!!****会额外多出4KRAM的需求    

    //指令初级过滤  
    //--------------------------------------------------------------------------  
    //格式:<->[cmd bytes]<CRLF>  即"-[cmd bytes]\r\n"  
    //指令必须以"-"开始, 以"\r\n"结束 
    i = shell_rx_rdy;
    if( (i < 2) || ('\r' != shell_rx_buff[i-2]) || ('\n' != shell_rx_buff[i-1]))return;
    
    //长度和前缀过滤 
    ptRxd = (uint8_t *)shell_rx_buff;
    if( (' ' != shell_rx_buff[3]) || ('f' != shell_rx_buff[0]) || (i < 6) || 
        ('i' != shell_rx_buff[1]) || ('l' != shell_rx_buff[2]) )  return;
       
    //处理指令
    //--------------------------------------------------------------------------
    ptRxd += 4;
    if(StrComp(ptRxd,"cap"))    //按包读取指令
    {
		TestGetFreeSpace();
    }
    else if(StrComp(ptRxd,"creatOSA\r\n"))   //创建OSA文件
    {
		/* 以时间信息为文件名称 */
		calendar_get(&date_s,&rtc_time);

        date = GetTick((date_s.Year+2000),date_s.Month,date_s.Date,rtc_time.Hours,rtc_time.Minutes,rtc_time.Seconds);
        
        File_Creat(date);
    }
    else if(StrComp(ptRxd,"creatCustom"))   //创建个性化方案文件
    {
        /* 以时间信息为文件名称 */
		calendar_get(&date_s,&rtc_time);
        
        date = (uint32_t)(((date_s.Year+2000) << 16) | (date_s.Month << 8) | (date_s.Date));
        
        File_Creat(date);
    }
    else if(StrComp(ptRxd,"dir"))      //显示当前路径下所有文件
    {
        strcpy(buff, "/");
        scan_files(buff);
    }
    else if(StrComp(ptRxd,"current wr file"))  // 查找当前写文件
    {
        memset(file_test.fileName,0,MAX_FILE_NAME_LEN);
        file_test.size = 0;
        findCurrentWRFile(&file_test);
        printf("current wr file:%s,%d\r\n",file_test.fileName,file_test.size);
    }
    else if(StrComp(ptRxd,"current rd file"))  // 查找当前写文件
    {
        memset(file_test.fileName,0,MAX_FILE_NAME_LEN);
        file_test.size = 0;        
        findCurrentRDFile(&file_test);
        printf("current rd file:%s,%d\r\n",file_test.fileName,file_test.size);
    }    
    else if(StrComp(ptRxd,"open"))      //根据文件名称打开文件
    {
		sscanf((void*)shell_rx_buff,"%*s%*s %s",openfileName);  

		/* Open a file by the Name */
        ret = f_open(&MyFile, openfileName, FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
        memset(openfileName,0,sizeof(openfileName));
		if(ret != FR_OK)
		{
			/* file Open for write Error */
            printf("open the file:%s Err:%d\r\n",openfileName,ret); 
		}
		else
		{
             printf("open the file:%s OK\r\n",openfileName);   
        }
    }
    else if(StrComp(ptRxd,"close"))      //关闭当前打开的文件
    {
        /* Close the open text file #################################*/
        ret = f_close(&MyFile);
        if(ret != FR_OK)
        {
            /* file Write or EOF Error */
            printf("f_close data Err:%d\r\n",ret);
        }
        else
        {
            printf("f_close data OK\r\n");
        }          
    }  
    else if(StrComp(ptRxd,"delete"))      //根据文件名称删除文件
    {
		sscanf((void*)shell_rx_buff,"%*s%*s %s",openfileName);  

		/* delate a file by the Name */
        ret = f_unlink(openfileName);
        memset(openfileName,0,sizeof(openfileName));
		if(ret != FR_OK)
		{
			/* file Open for write Error */
            printf("delete the file:%s Err:%d\r\n",openfileName,ret); 
		}
		else
		{
             printf("delete the file:%s OK\r\n",openfileName);      
        }
    } 
    else if(StrComp(ptRxd,"upload"))      //根据文件名称打开文件
    {
		sscanf((void*)shell_rx_buff,"%*s%*s %s",openfileName);  

		ret = f_open(&MyFile, openfileName,FA_OPEN_ALWAYS | FA_READ);
		if(ret != FR_OK)
		{
			/* file Open for write Error */
            printf("open the file:%s Err:%d\r\n",openfileName,ret); 
		}
		else
		{
			printf("open the file:%s OK,,size:%d\r\n",openfileName,MyFile.fsize);   
        }		
		/* upload a file by the Name */
        SerialUpload(&MyFile, (const uint8_t*)openfileName,MyFile.fsize);
		ret = f_close(&MyFile);
		
		if(ret != FR_OK)
		{
			/* file Open for write Error */
            printf("close the file:%s Err:%d\r\n",openfileName,ret); 
		}
		else
		{
             printf("close the file:%s OK\r\n",openfileName);   
        }
    }	
    else if(StrComp(ptRxd,"tell"))      
    {
        FSIZE_t  ptr;
        /* gets the current read/write pointer of a file. */
        ptr = f_tell(&MyFile);
        printf("ptr:0x%x\r\n",ptr);
    }
    else if(StrComp(ptRxd,"size"))   //获取文件大小
    {
        FSIZE_t len = 0;
        len = f_size(&MyFile);

        /* file Write or EOF Error */
        printf("f_size:%d\r\n",len);        
    }
    else if(StrComp(ptRxd,"sync"))
    {
        /*- Sync the open text file #################################*/
        ret = f_sync(&MyFile);
        if(ret != FR_OK)
        {
            /* file Write or EOF Error */
            printf("f_sync data Err:%d\r\n",ret);
        }
        else
        {
             printf("f_sync data OK\r\n");
        }        
    }        
    else if(StrComp(ptRxd,"wr"))      //写入字符串
    {      

        /* 组合写入数据 */
        calendar_get(&date_s,&rtc_time);
        wrData[0] = rtc_time.Hours;
        wrData[1] = rtc_time.Minutes;
        wrData[2] = rtc_time.Seconds;
        wrData[3] = 0x41;
        wrData[4] = 0x42;
        wrData[5] = 0x43;
        wrData[6] = 0x44;
        wrData[7] = 0x45;
        
        uint32_t j = 0;
        
        sscanf((void*)shell_rx_buff,"%*s%*s %s",openfileName); 
        
        for(j=0;j<256;j++)
        {
            wrData[j] = j;
        }
        for(j=0;j<100;j++)
        {
            calendar_get(&date_s,&rtc_time);
            wrData[0] = rtc_time.Hours;
            wrData[1] = rtc_time.Minutes;
            wrData[2] = rtc_time.Seconds;
            wrData[3] = 0x41;
            wrData[4] = 0x42;
            wrData[5] = 0x43;
            wrData[6] = 0x44;
            wrData[7] = 0x45;
            wrData[8] = j;
            
            printf("wr cnt:%d\r\n",j);
            wtite_data_to_currentFile(wrData,256);
            HAL_Delay(200);
        }
    }
    else if(StrComp(ptRxd,"rd\r\n")) 
    {
        /*- Read data from the text file ###########################*/
        ret = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&br);

        if((ret != FR_OK) || br == 0)
        {
            /* file Read or EOF Error */
            printf("f_read Err:%d\r\n",ret);
        }
        else
        {
            /* Success of the demo: no error occurrence */
            printf("read data OK:");
            for(i=0;i<sizeof(rtext);i++)
            {
                printf("%d,",rtext[i]);
            }
            printf("\r\n");
        }
    }
    else if(StrComp(ptRxd,"format"))
    {
        ret = f_mkfs("",1,4096);  
        if(ret != FR_OK)
        {
            printf("format err:%d\r\n",ret);
        }
        else
        {
            printf("format OK\r\n");
        }
    }
    else if(StrComp(ptRxd,"help\r\n"))      //
    {
        printf("%s",Fatfs_HelpMsg);
    }
    else return;
    
    //退出处理
    //--------------------------------------------------------------------------
    shell_rx_rdy = 0;  //shell_rx_rdy为0,表示指令已被处理完毕,否者下个Shell服务继续调用
}
#endif

/****************** End *********************************************************/

