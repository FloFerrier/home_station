add_library(led STATIC)

target_sources(led PRIVATE led.c)

target_link_libraries(led PRIVATE
    stm32_hal_driver
)

target_include_directories(led PUBLIC .)