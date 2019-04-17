set(WEAK_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/nrf_mesh_weak.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/app_error_weak.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/assertion_handler_weak.c"
    CACHE INTERNAL "")

set(BLE_SOFTDEVICE_SUPPORT_SOURCE_FILES
    "${SDK_ROOT}/components/libraries/util/app_util_platform.c"
    "${SDK_ROOT}/components/ble/common/ble_advdata.c"
    "${SDK_ROOT}/components/libraries/experimental_section_vars/nrf_section_iter.c"
    "${SDK_ROOT}/components/softdevice/common/nrf_sdh_soc.c"
    "${SDK_ROOT}/components/softdevice/common/nrf_sdh_ble.c"
    "${SDK_ROOT}/components/softdevice/common/nrf_sdh.c"
    "${SDK_ROOT}/components/ble/common/ble_conn_params.c"
    "${SDK_ROOT}/components/libraries/atomic/nrf_atomic.c"
    "${SDK_ROOT}/components/ble/common/ble_srv_common.c"
    CACHE INTERNAL "")

set(BLE_SOFTDEVICE_SUPPORT_INCLUDE_DIRS
    "${SDK_ROOT}/components/ble/common"
    "${SDK_ROOT}/components/softdevice/common"
    "${SDK_ROOT}/components/libraries/strerror"
    "${SDK_ROOT}/components/libraries/atomic"
    CACHE INTERNAL "")

add_subdirectory("BSP")
add_subdirectory("MiddleWare")
add_subdirectory("nRF5_SDK_for_mesh")
add_subdirectory("Application")

