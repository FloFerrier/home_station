file(GLOB STM32_HAL_SRC_FILES CONFIGURE_DEPENDS
    stm32/Src/*.c
    cmsis/Device/Source/Templates/*.c
    cmsis/Device/Source/Templates/gcc/${STARTUP_FILE}
)

add_library(stm32_hal_driver STATIC ${STM32_HAL_SRC_FILES})

target_include_directories(stm32_hal_driver PUBLIC
    Inc
    stm32/Inc
    stm32/Inc/Legacy
    cmsis/Device/Include
    cmsis/Include
)

target_compile_definitions(stm32_hal_driver PRIVATE
    -DUSE_HAL_DRIVER
)

target_compile_definitions(stm32_hal_driver PUBLIC
    -D${SPECIFIC_BOARD_HAL}
)