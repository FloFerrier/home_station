add_library(console STATIC)

target_sources(console PRIVATE console.c)

target_link_libraries(console PRIVATE
    $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},test>,mock_stm32f4x,stm32_hal_driver>
)

target_include_directories(console PUBLIC .)