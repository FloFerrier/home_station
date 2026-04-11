add_library(sampling STATIC)

target_sources(sampling PRIVATE sampling.c)

target_link_libraries(sampling PRIVATE
    console
    freertos_kernel
    sensor
)

target_include_directories(sampling PUBLIC .)