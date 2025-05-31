cmake_minimum_required(VERSION 3.20)

project(device ASM C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(NOTICE "The selected build type is : ${CMAKE_BUILD_TYPE}.")
if(CMAKE_BUILD_TYPE STREQUAL debug OR CMAKE_BUILD_TYPE STREQUAL release)
	include(cmake/openocd.cmake)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
elseif(CMAKE_BUILD_TYPE STREQUAL test)
	message(NOTICE "All binaries are generating by a native compiler (no cross-compilation).")
	include(CTest)
	enable_testing()
	set(CMAKE_C_FLAGS "-D_POSIX_C_SOURCE=200809L -DTEST")
	add_subdirectory(test)
else()
	message(FATAL_ERROR "Must to specify CMAKE_BUILD_TYPE as debug, release or test")
endif()

add_subdirectory(src)
add_subdirectory(third_party)