/**
  ****************************************************************************************
  * @file    bsp_cli.c
  * @author  Jason
  * @version V1.0.0
  * @date    2017-7-10
  * @brief   the implement of common line interface
  ****************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd.</center></h2>
  *
  ****************************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_cli.h"
#include "nrf_cli_uart.h"

#if NRF_MODULE_ENABLED(NRF_CLI)     //if cli module enabled in sdk_config.h

/* privarite function declares */
#define CLI_EXAMPLE_LOG_QUEUE_SIZE  (4)
NRF_CLI_UART_DEF(m_cli_uart_transport, 0, 64, 16);
NRF_CLI_DEF(m_cli_uart,
            "uart_cli:~$ ",
            &m_cli_uart_transport.transport,
            '\r',
            CLI_EXAMPLE_LOG_QUEUE_SIZE);


/* privarite variables declares */
static void cmd_print(nrf_cli_t const * p_cli, size_t argc, char **argv);
static void cmd_print_param(nrf_cli_t const * p_cli, size_t argc, char **argv);
static void cmd_print_all(nrf_cli_t const * p_cli, size_t argc, char **argv);
static void cmd_python(nrf_cli_t const * p_cli, size_t argc, char **argv);
static void cmd_ctl(nrf_cli_t const * p_cli, size_t argc, char **argv);
static void cmd_led_ctl_param(nrf_cli_t const * p_cli, size_t argc, char **argv);

/**
 * @brief Command set array
 * */
NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_print)
{
    NRF_CLI_CMD(all,   NULL, "Print all entered parameters.", cmd_print_all),
    NRF_CLI_CMD(param, NULL, "Print each parameter in new line.", cmd_print_param),
    NRF_CLI_SUBCMD_SET_END
};
NRF_CLI_CMD_REGISTER(print, &m_sub_print, "print", cmd_print);

NRF_CLI_CMD_REGISTER(python, NULL, "python", cmd_python);

/* command for led control */
NRF_CLI_CREATE_STATIC_SUBCMD_SET(m_sub_led)
{
    NRF_CLI_CMD(led, NULL, "Control LEDx Status: cmd led 1 on", cmd_led_ctl_param),
    NRF_CLI_SUBCMD_SET_END
};
NRF_CLI_CMD_REGISTER(cmd, &m_sub_led, "cmd", cmd_ctl);


/**
  * @brief  initialize for CLI
  * @param  None
  * @retval Status
  */
ret_code_t cli_init(void)
{
    ret_code_t                  ret = NRF_ERROR_NULL;
    nrf_drv_uart_config_t       uart_config = NRF_DRV_UART_DEFAULT_CONFIG;
    
    uart_config.pseltxd = TX_PIN_NUMBER;
    uart_config.pselrxd = RX_PIN_NUMBER;
    uart_config.hwfc    = NRF_UART_HWFC_DISABLED;
    ret = nrf_cli_init(&m_cli_uart, &uart_config, true, true, NRF_LOG_SEVERITY_INFO);
    APP_ERROR_CHECK(ret);    
        
    return ret;
}
/**
  * @brief  cli start 
  * @param  None
  * @retval Status
  */
ret_code_t cli_start(void)
{
    ret_code_t ret;

    ret = nrf_cli_start(&m_cli_uart);
    APP_ERROR_CHECK(ret);

    return ret;
}
/**
  * @brief  cli process
  * @note   the function should be schedulered priodically to process the cli status
  * @param  None
  * @retval None
  */
void cli_process(void)
{
    nrf_cli_process(&m_cli_uart);
}
/**
  * @brief  log_callback_cli
  * @note   log output by cli module
  * @param  None
  * @retval None
  */
void log_callback_cli(uint32_t dbg_level, const char * p_filename, uint16_t line,
    uint32_t timestamp, const char * format, va_list arguments)
{
    char buf[100] = {0};
    
    nrf_cli_fprintf(&m_cli_uart, NRF_CLI_NORMAL,"<t: %10u>, %s, %4d, ",timestamp, p_filename, line);
    memset(buf,0,sizeof(buf));
    vsprintf(buf,format,arguments);
    nrf_cli_fprintf(&m_cli_uart, NRF_CLI_NORMAL,"%s",buf);
}

/* Command handlers */
static void cmd_print_param(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    for (size_t i = 1; i < argc; i++)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "argv[%d] = %s\r\n", i, argv[i]);
    }
}

static void cmd_print_all(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    for (size_t i = 1; i < argc; i++)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "%s ", argv[i]);
    }
    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "\r\n");
}
static void cmd_print(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    ASSERT(p_cli);
    ASSERT(p_cli->p_ctx && p_cli->p_iface && p_cli->p_name);

    if ((argc == 1) || nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc != 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: bad parameter count\r\n", argv[0]);
        return;
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: unknown parameter: %s\r\n", argv[0], argv[1]);
}

static void cmd_python(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    UNUSED_PARAMETER(argc);
    UNUSED_PARAMETER(argv);

    nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "Nice joke ;)\r\n");
}

static void cmd_ctl(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
    ASSERT(p_cli);
    ASSERT(p_cli->p_ctx && p_cli->p_iface && p_cli->p_name);

    if ((argc == 1) || nrf_cli_help_requested(p_cli))
    {
        nrf_cli_help_print(p_cli, NULL, 0);
        return;
    }

    if (argc != 2)
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: bad parameter count\r\n", argv[0]);
        return;
    }

    nrf_cli_fprintf(p_cli, NRF_CLI_ERROR, "%s: unknown parameter: %s\r\n", argv[0], argv[1]);
}
static void cmd_led_ctl_param(nrf_cli_t const * p_cli, size_t argc, char **argv)
{
//    for (size_t i = 1; i < argc; i++)
//    {
//        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL, "argv[%d] = %s\r\n", i, argv[i]);
//    }
    uint8_t ledNum = 0;
    bool    ledStatus = OFF;
    
    ledNum = atoi(argv[1]);  //convernt to led number
    if((strcmp(argv[2],"on")) == 0)
    {
        ledStatus = ON;
    }
    else if((strcmp(argv[2],"off")) == 0)
    {
        ledStatus = OFF;
    }
        
    if(argc == 3)
    {
        switch(ledNum)
        {
            case 1:
            {
                if(ledStatus == ON)
                {
                    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,"led %d ON\r\n",ledNum);
                    button_event_handler(1);
                }
                else
                {
                    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,"led %d OFF\r\n",ledNum);
                }
            }
            break;
            case 2:
            {
                if(ledStatus == ON)
                {
                     nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,"led %d ON\r\n",ledNum);
                }
                else
                {
                    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,"led %d OFF\r\n",ledNum);
                }
            }
            break;
            case 3:
            {
                if(ledStatus == ON)
                {
                     nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,"led %d ON\r\n",ledNum);
                }
                else
                {
                    nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,"led %d OFF\r\n",ledNum);
                }
            }
            break;            
            default:break;
        }
    }
    else
    {
        nrf_cli_fprintf(p_cli, NRF_CLI_NORMAL,"cmd parameter number err\r\n");
    }
    
}

#endif // NRF_MODULE_ENABLED(NRF_CLI)

/************************ (C) COPYRIGHT 2017 ShenZhen DBGJ Co., Ltd. *****END OF FILE****/



