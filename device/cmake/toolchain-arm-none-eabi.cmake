set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Executables must be on PATH environment variable
find_program(ARM-NONE-EABI-AR_Executable Name arm-none-eabi-ar REQUIRED)
set(CMAKE_AR ${ARM-NONE-EABI-AR_Executable})

find_program(ARM-NONE-EABI-GCC_Executable Name arm-none-eabi-gcc REQUIRED)
set(CMAKE_ASM_COMPILER ${ARM-NONE-EABI-GCC_Executable})
set(CMAKE_C_COMPILER ${ARM-NONE-EABI-GCC_Executable})

find_program(ARM-NONE-EABI-G++_Executable Name arm-none-eabi-g++ REQUIRED)
set(CMAKE_CXX_COMPILER ${ARM-NONE-EABI-G++_Executable})

find_program(ARM-NONE-EABI-LINKER_Executable Name arm-none-eabi-ld REQUIRED)
set(CMAKE_LINKER ${ARM-NONE-EABI-LINKER_Executable})

find_program(ARM-NONE-EABI-OBJCOPY_Executable Name arm-none-eabi-objcopy REQUIRED)
set(CMAKE_OBJCOPY ${ARM-NONE-EABI-OBJCOPY_Executable})

find_program(ARM-NONE-EABI-RANLIB_Executable Name arm-none-eabi-ranlib REQUIRED)
set(CMAKE_RANLIB ${ARM-NONE-EABI-RANLIB_Executable})

find_program(ARM-NONE-EABI-SIZE_Executable Name arm-none-eabi-size REQUIRED)
set(CMAKE_SIZE ${ARM-NONE-EABI-SIZE_Executable})

find_program(ARM-NONE-EABI-STRIP_Executable Name arm-none-eabi-strip REQUIRED)
set(CMAKE_STRIP ${ARM-NONE-EABI-STRIP_Executable})

# General compilation flags
set(CMAKE_C_FLAGS "-mthumb -fdata-sections -ffunction-sections -Wl,--gc-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions")

set(CMAKE_C_FLAGS_DEBUG "-Og -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")

set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)