add_library(commands STATIC)

target_sources(commands PRIVATE commands.c)

target_link_libraries(commands PRIVATE
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_freertos,freertos_kernel>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_sensor,sensor>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_protocol,protocol>
)

target_include_directories(commands PUBLIC .)