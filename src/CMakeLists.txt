add_subdirectory(commands)
add_subdirectory(console)
add_subdirectory(fsm)
add_subdirectory(led)
add_subdirectory(protocol)
add_subdirectory(sensor)
add_subdirectory(shell)

if(NOT CMAKE_BUILD_TYPE STREQUAL test)
    set(BUILD_TARGET ${CMAKE_PROJECT_NAME}_firmware)

    set(FIRMWARE_FILENAME ${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TARGET}.elf)

    message("Firmware is available on the following folder : ${FIRMWARE_FILENAME}")

    add_executable(${BUILD_TARGET})

    target_sources(${BUILD_TARGET} PRIVATE
        main.c
        syscalls.c
    )

    target_link_libraries(${BUILD_TARGET} PRIVATE
        stm32_hal_driver
        freertos_kernel
        bme68x
        commands
        console
        fsm
        led
        protocol
        sensor
        shell
    )

    target_compile_options(${BUILD_TARGET} PUBLIC
        -Wall
        -Wextra
        -Werror
    )

    set(LINKER_FILE ${CMAKE_SOURCE_DIR}/config/stm32f446retx.ld)
    message("Linker file is : ${LINKER_FILE}")

    target_link_options(${BUILD_TARGET} PUBLIC
        -T${LINKER_FILE}
        -specs=nano.specs
        -specs=nosys.specs
        -Wl,-Map=output.map,--cref
        -Wl,--print-memory-usage
        -Wl,--gc-sections
        -u _printf_float
    )

    openocd_add_custom_target(flash
        NAME flash
        BOARD st_nucleo_f4
        FW "${CMAKE_CURRENT_BINARY_DIR}/${BUILD_TARGET}.elf"
    )

    openocd_add_custom_target(debug
        NAME debug
        BOARD st_nucleo_f4
    )
endif()
