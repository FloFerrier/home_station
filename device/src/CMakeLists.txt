set(BUILD_TARGET ${CMAKE_PROJECT_NAME}_firmware)

set(FIRMWARE_FILENAME ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TARGET}.elf)

add_executable(${BUILD_TARGET})

target_include_directories(${BUILD_TARGET} PRIVATE
    console
    commands
    sensor
    shell
    led
    fsm
    protocol
)

target_sources(${BUILD_TARGET} PRIVATE
    main.c
    syscalls.c
    console/console.c
    commands/commands.c
    sensor/sensor.c
    shell/shell.c
    led/led.c
    fsm/fsm.c
    protocol/protocol.c
)

target_link_libraries(${BUILD_TARGET} PRIVATE
    stm32_hal_driver
    freertos_kernel
    bme68x
)

target_compile_options(${BUILD_TARGET} PUBLIC
    -Wall
    -Wextra
)

target_link_options(${BUILD_TARGET} PUBLIC
    -T${LINKER_FILE}
    -mcpu=${CPU}
    -mfpu=${FPU}
    -mfloat-abi=${FLOAT_ABI}
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,-Map=output.map,--cref
    -Wl,--print-memory-usage
    -Wl,--gc-sections
    -u _printf_float
)

openocd_custom_target_flash("${FIRMWARE_FILENAME}")
openocd_custom_target_debug_session()
