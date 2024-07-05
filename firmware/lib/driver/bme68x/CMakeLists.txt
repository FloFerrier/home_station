include(${CMAKE_SOURCE_DIR}/cmake/bme68x.cmake)

add_library(bme68x STATIC src/bme68x.c)

target_include_directories(bme68x PUBLIC src)