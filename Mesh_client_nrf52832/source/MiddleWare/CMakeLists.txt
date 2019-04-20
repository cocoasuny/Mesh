set(MIDDLE_WARE_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/fprintf/src/nrf_fprintf.c"
  	"${CMAKE_CURRENT_SOURCE_DIR}/fprintf/src/nrf_fprintf_format.c"
    CACHE INTERNAL "")

set(MIDDLE_WARE_INCLUDE_DIRS
	"${CMAKE_CURRENT_SOURCE_DIR}/fprintf/inc"
    CACHE INTERNAL "")


if (NOT BUILD_HOST)
    add_subdirectory("rtt")
    add_subdirectory("micro-ecc")
    add_subdirectory("fprintf")
    add_subdirectory("app_timer")
endif ()
