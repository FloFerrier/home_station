add_library(sensor STATIC)

target_sources(sensor PRIVATE sensor.c)

target_link_libraries(sensor PRIVATE
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_stm32f4x,stm32_hal_driver>
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_bme68x,bme68x>
)

target_include_directories(sensor PUBLIC .)