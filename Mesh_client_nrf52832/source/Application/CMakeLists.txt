set(target "light_switch_client_${PLATFORM}_${SOFTDEVICE}")

#source file defines
set(SRC_LISTS 
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
    "${CMAKE_SOURCE_DIR}/source/nRF5_SDK_for_mesh/mesh/stack/src/mesh_stack.c"
    "${CMAKE_SOURCE_DIR}/source/Application/src/mesh_provisionee.c"
    "${MBTLE_SOURCE_DIR}/source/Application/src/rtt_input.c"
    "${CMAKE_SOURCE_DIR}/source/Application/src/simple_hal.c"
    "${CMAKE_SOURCE_DIR}/source/Application/src/mesh_app_utils.c"
    "${CMAKE_SOURCE_DIR}/source/Application/src/mesh_adv.c"
    "${CMAKE_SOURCE_DIR}/source/Application/src/ble_softdevice_support.c"
    ${BLE_SOFTDEVICE_SUPPORT_SOURCE_FILES}
    ${WEAK_SOURCE_FILES}
    ${MESH_CORE_SOURCE_FILES}
    ${MESH_BEARER_SOURCE_FILES}
    ${MESH_GATT_SOURCE_FILES}
    ${GENERIC_ONOFF_CLIENT_SOURCE_FILES}
    ${CONFIG_SERVER_SOURCE_FILES}
    ${HEALTH_SERVER_SOURCE_FILES}
    ${ACCESS_SOURCE_FILES}
    ${MESH_APP_TIMER_SOURCE_FILES}
    ${PROV_PROVISIONEE_SOURCE_FILES}
    ${PROV_COMMON_SOURCE_FILES}
    ${PROV_BEARER_ADV_SOURCE_FILES}
    ${PROV_BEARER_GATT_SOURCE_FILES}
    ${${PLATFORM}_SOURCE_FILES}
    ${${nRF5_SDK_VERSION}_SOURCE_FILES}
    ${BSP_SOURCE_FILES}
    ${MIDDLE_WARE_SOURCE_FILES}	
	)

add_executable(${target} ${SRC_LISTS})

# include path defines
target_include_directories(${target} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_SOURCE_DIR}/source/MiddleWare/rtt/include"
    ${BLE_SOFTDEVICE_SUPPORT_INCLUDE_DIRS}
    ${GENERIC_ONOFF_CLIENT_INCLUDE_DIRS}
    ${CONFIG_SERVER_INCLUDE_DIRS}
    ${HEALTH_SERVER_INCLUDE_DIRS}
    ${MESH_INCLUDE_DIRS}
    ${MESH_CORE_SOURCE_DIRS}
    ${${SOFTDEVICE}_INCLUDE_DIRS}
    ${${PLATFORM}_INCLUDE_DIRS}
    ${${BOARD}_INCLUDE_DIRS}
    ${${nRF5_SDK_VERSION}_INCLUDE_DIRS}
    ${BSP_INCLUDE_DIRS}
    ${MIDDLE_WARE_INCLUDE_DIRS}
    )

set_target_link_options(${target}
    ${CMAKE_CURRENT_SOURCE_DIR}/linker/${PLATFORM}_${SOFTDEVICE})

target_compile_options(${target} PUBLIC
    ${${ARCH}_DEFINES})

target_compile_definitions(${target} PUBLIC
    ${USER_DEFINITIONS}
    -DUSE_APP_CONFIG
    -DCONFIG_APP_IN_CORE
    ${${PLATFORM}_DEFINES}
    ${${SOFTDEVICE}_DEFINES}
    ${${BOARD}_DEFINES})

target_link_libraries(${target}
    rtt_${PLATFORM}
    uECC_${PLATFORM})

create_hex(${target})
add_flash_target(${target})

get_property(target_include_dirs TARGET ${target} PROPERTY INCLUDE_DIRECTORIES)
add_pc_lint(${target}
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
    "${target_include_dirs}"
    "${${PLATFORM}_DEFINES};${${SOFTDEVICE}_DEFINES};${${BOARD}_DEFINES}")

add_ses_project(${target})

