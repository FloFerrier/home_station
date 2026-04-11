add_library(sensor STATIC)

target_sources(sensor PRIVATE sensor.c)

target_link_libraries(sensor PRIVATE
    bme68x
    stm32_hal_driver
)

target_include_directories(sensor PUBLIC .)