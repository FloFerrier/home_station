include(${CMAKE_SOURCE_DIR}/cmake/bme68x.cmake)

add_library(bme68x STATIC)

target_sources(bme68x PRIVATE src/bme68x.c)

target_include_directories(bme68x PUBLIC src)
