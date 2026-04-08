add_library(ble STATIC)

target_sources(ble PRIVATE ble.c)

target_link_libraries(ble PRIVATE
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_stm32f4x,stm32_hal_driver>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_console,console>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_freertos,freertos_kernel>
)

target_include_directories(ble PUBLIC .)