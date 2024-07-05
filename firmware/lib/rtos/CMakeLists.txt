include(${CMAKE_SOURCE_DIR}/cmake/freertos.cmake)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
  INTERFACE
    include
)

target_compile_definitions(freertos_config
  INTERFACE
  projCOVERAGE_TEST=0
)