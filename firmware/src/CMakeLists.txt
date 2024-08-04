set(BUILD_TARGET weather_sensors)

set(FIRMWARE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TARGET}.elf)
message(NOTICE "Firmware will be available on the following folder: ${FIRMWARE_FILE}")

add_executable(${BUILD_TARGET})

target_sources(${BUILD_TARGET} PRIVATE
    main.c
    sysmem.c
    console.c
    shell.c
    sensor.c
    commands.c
)

target_compile_options(${BUILD_TARGET} PRIVATE
    -Wall
    -Wextra
)

target_link_libraries(${BUILD_TARGET} PRIVATE
    stm32_hal_driver
    freertos_kernel
    bme68x
)

target_link_options(${BUILD_TARGET} PUBLIC
    -T${LINKER_FILE}
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,-Map=output.map,--cref
    -Wl,--print-memory-usage
    -u _printf_float
)
