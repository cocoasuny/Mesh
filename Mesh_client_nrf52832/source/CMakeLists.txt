set(WEAK_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/nrf_mesh_weak.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/app_error_weak.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/src/assertion_handler_weak.c"
    CACHE INTERNAL "")
add_subdirectory("Application")
add_subdirectory("BSP")
add_subdirectory("MiddleWare")
add_subdirectory("nRF5_SDK_for_mesh")

