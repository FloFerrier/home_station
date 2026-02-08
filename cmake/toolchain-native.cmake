if(CMAKE_HOST_SYSTEM_NAME STREQUAL Linux OR CMAKE_HOST_SYSTEM_NAME STREQUAL Windows)

    find_program(GCC_Executable Name gcc REQUIRED)
    set(CMAKE_C_COMPILER ${GCC_Executable})

    find_program(GXX_Executable Name g++ REQUIRED)
    set(CMAKE_CXX_COMPILER ${GXX_Executable})

else()

    message(FATAL_ERROR "${CMAKE_HOST_SYSTEM_NAME} is not supported yet !")

endif()

set(CMAKE_C_FLAGS "-g -O0 -Wall -Wextra --coverage" CACHE STRING "C flags")
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra --coverage" CACHE STRING "C++ flags")