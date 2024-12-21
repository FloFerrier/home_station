find_program(OPENOCD_Executable Name openocd REQUIRED)

set(OPENOCD_TARGET_BOARD stm32f4x)
set(OPENOCD_CFG_FILE ${CMAKE_SOURCE_DIR}/config/openocd.cfg)

message("Firmware is available on the following folder : ${FIRMWARE_FILENAME}")

function(openocd_custom_target_flash FIRMWARE_FILENAME)
    message("A custom target is available to flash firmware: \"cmake --build <build_folder> --target flash\"")
    add_custom_target(flash
        COMMAND ${OPENOCD_Executable} -f ${OPENOCD_CFG_FILE} -c "setup ${OPENOCD_TARGET_BOARD}" -c "program_release ${FIRMWARE_FILENAME}"
        DEPENDS ${FIRMWARE_FILENAME} ${OPENOCD_CFG_FILE}
    )
endfunction()

function(openocd_custom_target_debug_session)
    message("A custom target is available to open a debug session : \"cmake --build <build_folder> --target debug\"")
    add_custom_target(debug
        COMMAND ${OPENOCD_Executable} -f ${OPENOCD_CFG_FILE} -c "setup ${OPENOCD_TARGET_BOARD}"
        DEPENDS ${OPENOCD_CFG_FILE}
    )
endfunction()
