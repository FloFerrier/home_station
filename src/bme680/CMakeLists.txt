add_library(bme680 STATIC)

target_sources(bme680 PRIVATE bme680.c)

target_link_libraries(bme680 PRIVATE
    bme68x
    stm32_hal_driver
)

target_include_directories(bme680 PUBLIC .)