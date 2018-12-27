set(WEAK_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Application/nrf_mesh_weak.c"
    CACHE INTERNAL "")
add_subdirectory("Application")
add_subdirectory("BSP")
add_subdirectory("MiddleWare")
add_subdirectory("nRF5_SDK_for_52832")

