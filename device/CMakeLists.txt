cmake_minimum_required(VERSION 3.20)

project(weather_sensors ASM C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

message(NOTICE "The selected build type is : ${CMAKE_BUILD_TYPE}.")
if(CMAKE_BUILD_TYPE STREQUAL debug OR CMAKE_BUILD_TYPE STREQUAL release)
	include(cmake/stm32-board.cmake)
	include(cmake/toolchain-arm-none-eabi.cmake)
	add_subdirectory(src)
elseif(CMAKE_BUILD_TYPE STREQUAL test)
	message(NOTICE "All binaries are generating by a native compiler (no cross-compilation).")
	include(cmake/toolchain-native.cmake)
	include(cmake/testing.cmake)
    include(cmake/cmocka.cmake)
	add_subdirectory(test)
else()
	message(FATAL_ERROR "Must to specify CMAKE_BUILD_TYPE as debug, release or test")
endif()

add_subdirectory(lib)