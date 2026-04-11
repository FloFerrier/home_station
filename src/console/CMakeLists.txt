add_library(console STATIC)

target_sources(console PRIVATE console.c)

target_link_libraries(console PRIVATE
    stm32_hal_driver
)

target_include_directories(console PUBLIC .)