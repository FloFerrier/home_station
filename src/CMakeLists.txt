set(BUILD_TARGET weather_sensors)

message("OPENOCD_TARGET_BOARD=${OPENOCD_TARGET_BOARD}")

set(FIRMWARE_FILE ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TARGET}.elf)
message("FIRMWARE_FILE=${FIRMWARE_FILE}")

add_executable(${BUILD_TARGET}
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
)

add_custom_target(flash
    COMMAND openocd -f config/openocd.cfg -c "setup ${OPENOCD_TARGET_BOARD}" -c "program_release ${FIRMWARE_FILE}"
    DEPENDS ${FIRMWARE_FILE}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)