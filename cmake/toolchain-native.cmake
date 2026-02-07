find_program(CLANG_Executable Name clang REQUIRED)
set(CMAKE_ASM_COMPILER ${CLANG_Executable})
set(CMAKE_C_COMPILER ${CLANG_Executable})

find_program(CLANGXX_Executable Name clang++ REQUIRED)
set(CMAKE_CXX_COMPILER ${CLANGXX_Executable})

set(CMAKE_C_FLAGS "-g -O0 -Wall -Wextra" CACHE STRING "C flags")
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra" CACHE STRING "C++ flags")