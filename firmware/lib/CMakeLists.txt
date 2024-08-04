add_subdirectory(driver)

if(NOT CMAKE_BUILD_TYPE STREQUAL test)
	add_subdirectory(hal)
    add_subdirectory(rtos)
endif()