add_library(stm32_hal_driver STATIC)

target_sources(stm32_hal_driver PRIVATE
    cmsis/Device/Source/Templates/system_stm32f4xx.c
    cmsis/Device/Source/Templates/gcc/startup_stm32f446xx.s
    stm32/Src/stm32f4xx_hal.c
    stm32/Src/stm32f4xx_hal_cortex.c
    stm32/Src/stm32f4xx_hal_dma.c
    stm32/Src/stm32f4xx_hal_flash.c
    stm32/Src/stm32f4xx_hal_gpio.c
    stm32/Src/stm32f4xx_hal_i2c.c
    stm32/Src/stm32f4xx_hal_pwr.c
    stm32/Src/stm32f4xx_hal_rcc.c
    stm32/Src/stm32f4xx_hal_rtc.c
    stm32/Src/stm32f4xx_hal_tim.c
    stm32/Src/stm32f4xx_hal_uart.c
    stm32/Src/stm32f4xx_hal_usart.c
)

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
    -DSTM32F446xx
)
