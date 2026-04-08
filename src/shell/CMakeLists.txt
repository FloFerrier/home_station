add_library(shell STATIC)

target_sources(shell PRIVATE shell.c)

target_link_libraries(shell PRIVATE
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_console,console>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_embedded_cli_lib,embedded_cli_lib>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_led,led>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_ble,ble>
)

target_include_directories(shell PUBLIC .)