# stm32f446retx

include(FetchContent)

set(CPU cortex-m4 CACHE STRING "")
set(FPU fpv4-sp-d16 CACHE STRING "")
set(FLOAT_ABI hard CACHE STRING "")
set(FREERTOS_HEAP 4 CACHE STRING "")
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")
set(OPENOCD_TARGET_BOARD stm32f4x CACHE STRING "")

set(SPECIFIC_BOARD_HAL STM32F446xx)
set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/config/stm32f446retx.ld)
set(STARTUP_FILE startup_stm32f446xx.s)

FetchContent_Declare(
  STM32_HAL_Driver
  GIT_REPOSITORY https://github.com/STMicroelectronics/stm32f4xx_hal_driver.git
  GIT_TAG v1.8.1
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/hal/stm32
)

message(NOTICE "stm32f4xx hal driver v1.8.1 is using.")

FetchContent_MakeAvailable(STM32_HAL_Driver)

FetchContent_Declare(
  STM32_CMSIS_Device
  GIT_REPOSITORY https://github.com/STMicroelectronics/cmsis_device_f4.git
  GIT_TAG v2.6.8
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/hal/cmsis/Device
)

message(NOTICE "cmsis device f4 v2.6.8 is using.")

FetchContent_MakeAvailable(STM32_CMSIS_Device)