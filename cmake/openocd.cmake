find_program(OPENOCD_Executable Name openocd REQUIRED)

function(openocd_add_custom_target)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "NAME;BOARD;CFG;FW" "")

    if(NOT ARG_BOARD)
        message(FATAL_ERROR "Need to specify a BOARD (ex: \"st_nucleo_f4\" or \"custom\")")
    endif()

    if(ARG_BOARD STREQUAL "custom")
        if(NOT ARG_CFG)
            message(FATAL_ERROR "Need to specify a CFG (filename for openocd config file)")
        endif()
        set(OPENOCD_CFG_FILE ${CFG})
    else()
        set(OPENOCD_CFG_FILE board/${ARG_BOARD}.cfg)
    endif()

    if(ARG_NAME STREQUAL "flash")
        if(NOT ARG_FW)
            message(FATAL_ERROR "Need to specify a FW (filename for firmware binary)")
        endif()
        set(OPENOCD_COMMAND "-c program ${ARG_FW} verify reset exit")
    else()
        set(OPENOCD_COMMAND "")
    endif()

    add_custom_target(${ARG_NAME}
        COMMAND ${OPENOCD_Executable} -f ${OPENOCD_CFG_FILE} ${OPENOCD_COMMAND}
    )
endfunction()
