add_library(ble STATIC)

target_sources(ble PRIVATE ble.c)

target_link_libraries(ble PRIVATE
    console
    freertos_kernel
    stm32_hal_driver
)

target_include_directories(ble PUBLIC .)