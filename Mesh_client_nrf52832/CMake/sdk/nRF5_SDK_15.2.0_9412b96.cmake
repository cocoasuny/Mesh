set(nRF5_SDK_15.2.0_9412b96_SOURCE_FILES
    "${SDK_ROOT}/components/libraries/util/app_error.c"
    "${SDK_ROOT}/components/libraries/uart/app_uart.c"
    "${SDK_ROOT}/components/libraries/util/app_util_platform.c"
    "${SDK_ROOT}/components/libraries/cli/nrf_cli.c" 
    "${SDK_ROOT}/components/libraries/cli/uart/nrf_cli_uart.c" 
    "${SDK_ROOT}/components/libraries/queue/nrf_queue.c" 
    "${SDK_ROOT}/components/libraries/memobj/nrf_memobj.c"
    "${SDK_ROOT}/components/libraries/balloc/nrf_balloc.c"
    "${SDK_ROOT}/integration/nrfx/legacy/nrf_drv_uart.c"
    "${SDK_ROOT}/modules/nrfx/drivers/src/nrfx_uart.c"
    "${SDK_ROOT}/modules/nrfx/drivers/src/nrfx_uarte.c"
    "${SDK_ROOT}/modules/nrfx/drivers/src/prs/nrfx_prs.c"
    "${SDK_ROOT}/components/libraries/ringbuf/nrf_ringbuf.c"
    )

set(nRF5_SDK_15.2.0_9412b96_INCLUDE_DIRS
    "${SDK_ROOT}/modules/nrfx/mdk"
    "${SDK_ROOT}/modules/nrfx/templates/nRF52832"
    "${SDK_ROOT}/modules/nrfx/hal"
    "${SDK_ROOT}/modules/nrfx/drivers"
    "${SDK_ROOT}/modules/nrfx/drivers/include"
    "${SDK_ROOT}/modules/nrfx/drivers/src/prs"
    "${SDK_ROOT}/integration/nrfx"
    "${SDK_ROOT}/integration/nrfx/legacy"
    "${SDK_ROOT}/components/libraries/util"
    "${SDK_ROOT}/components/libraries/timer"
    "${SDK_ROOT}/components/libraries/log"
    "${SDK_ROOT}/components/libraries/log/src"
    "${SDK_ROOT}/components/libraries/uart"  
    "${SDK_ROOT}/components/libraries/cli"    
    "${SDK_ROOT}/components/libraries/cli/uart"
    "${SDK_ROOT}/components/libraries/experimental_section_vars"
    "${SDK_ROOT}/components/softdevice/common"
    "${SDK_ROOT}/components/libraries/delay"
    "${SDK_ROOT}/components/libraries/pwr_mgmt"
    "${SDK_ROOT}/components/libraries/queue"
    "${SDK_ROOT}/components/libraries/memobj"
    "${SDK_ROOT}/components/libraries/balloc"
    "${SDK_ROOT}/components/libraries/ringbuf"
    )

if (TOOLCHAIN STREQUAL "armcc")
    set(nRF5_SDK_15.2.0_9412b96_SOURCE_FILES
        ${nRF5_SDK_15.2.0_9412b96_SOURCE_FILES}
        "${SDK_ROOT}/components/libraries/util/app_error_handler_keil.c")
else ()
    set(nRF5_SDK_15.2.0_9412b96_SOURCE_FILES
        ${nRF5_SDK_15.2.0_9412b96_SOURCE_FILES}
        "${SDK_ROOT}/components/libraries/util/app_error_handler_gcc.c")
endif ()
