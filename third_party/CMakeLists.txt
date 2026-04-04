add_subdirectory(driver)
add_subdirectory(middleware)

if(NOT CMAKE_BUILD_TYPE STREQUAL test)
	add_subdirectory(hal)
    add_subdirectory(rtos)
endif()