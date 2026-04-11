add_library(fsm STATIC)

target_sources(fsm PRIVATE fsm.c)

target_link_libraries(fsm PRIVATE
    ble
    console
    freertos_kernel
    led
    sampling
    shell
    stm32_hal_driver
)

target_include_directories(fsm PUBLIC .)