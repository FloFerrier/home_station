include(FetchContent)

set(RTOS_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/rtos CACHE STRING "" FORCE)
set(FREERTOS_CONFIG_FILE_DIRECTORY ${RTOS_DIRECTORY} CACHE STRING "" FORCE)
set(FREERTOS_HEAP 4 CACHE STRING "")
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")

FetchContent_Declare(
    freertos_kernel
    GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
    GIT_TAG V10.5.1
    SOURCE_DIR ${RTOS_DIRECTORY}/freertos_kernel
)

message(NOTICE "FreeRTOS v10.5.1 kernel is using.")

FetchContent_MakeAvailable(freertos_kernel)