set(FREERTOS_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/source/croutine.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/event_groups.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/freertos_list.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/freertos_queue.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/stream_buffer.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/tasks.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/timers.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/portable/MemMang/heap_1.c"  
    "${CMAKE_CURRENT_SOURCE_DIR}/portable/CMSIS/nrf52/port_cmsis.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/portable/CMSIS/nrf52/port_cmsis_systick.c" 
    "${CMAKE_CURRENT_SOURCE_DIR}/portable/GCC/nrf52/port.c"     
    CACHE INTERNAL "")

set(FREERTOS_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/config"
    "${CMAKE_CURRENT_SOURCE_DIR}/portable/CMSIS/nrf52"
    "${CMAKE_CURRENT_SOURCE_DIR}/portable/GCC/nrf52"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/include"
    CACHE INTERNAL "")
