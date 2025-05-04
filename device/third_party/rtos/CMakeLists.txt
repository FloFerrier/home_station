set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ CACHE STRING "")
set(FREERTOS_HEAP 4 CACHE STRING "")
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(freertos_config INTERFACE
  projCOVERAGE_TEST=0
)

add_subdirectory(freertos_kernel)