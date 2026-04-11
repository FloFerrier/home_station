add_library(shell STATIC)

target_sources(shell PRIVATE shell.c)

target_link_libraries(shell PRIVATE
    ble
    console
    embedded_cli_lib
    freertos_kernel
    led
)

target_include_directories(shell PUBLIC
    .
    $<TARGET_PROPERTY:freertos_kernel,INTERFACE_INCLUDE_DIRECTORIES>
)