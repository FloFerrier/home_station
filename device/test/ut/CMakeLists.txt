macro(add_unit_test unitestname)
    message(NOTICE "Create test executable: ${ARGV0}")
    add_executable(${ARGV0})
    target_include_directories(${ARGV0} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/console
        ${CMAKE_SOURCE_DIR}/src/commands
        ${CMAKE_SOURCE_DIR}/src/sensor
        ${CMAKE_SOURCE_DIR}/src/shell
        ${CMAKE_SOURCE_DIR}/src/led
        ${CMAKE_SOURCE_DIR}/src/fsm
        ${CMAKE_SOURCE_DIR}/src/protocol
        ${CMAKE_SOURCE_DIR}/test/mock
    )
    target_sources(${ARGV0} PRIVATE ${ARGN})
    target_compile_definitions(${ARGV0} PRIVATE -DTEST)
    #target_compile_options(${ARGV0} PRIVATE -std=c99 -Og -g -Wall -W -Wextra -Werror)
    target_link_libraries(${ARGV0} PRIVATE cmocka-static bme68x)
    add_test(NAME ${ARGV0} COMMAND $<TARGET_FILE:${ARGV0}>)
endmacro()

add_unit_test(test_shell
    ${CMAKE_SOURCE_DIR}/src/shell/shell.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_console.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_commands.c
    ${CMAKE_SOURCE_DIR}/test/ut/test_shell.c
)

add_unit_test(test_bme680
${CMAKE_SOURCE_DIR}/test/ut/test_bme680.c
)

add_unit_test(test_sensor
    ${CMAKE_SOURCE_DIR}/src/sensor/sensor.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_bme68x.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_stm32f4x.c
    ${CMAKE_SOURCE_DIR}/test/ut/test_sensor.c
)

add_unit_test(test_commands
    ${CMAKE_SOURCE_DIR}/src/commands/commands.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_sensor.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_protocol.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_freertos.c
    ${CMAKE_SOURCE_DIR}/test/ut/test_commands.c
)

add_unit_test(test_console
    ${CMAKE_SOURCE_DIR}/src/console/console.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_stm32f4x.c
    ${CMAKE_SOURCE_DIR}/test/ut/test_console.c
)

add_unit_test(test_led
    ${CMAKE_SOURCE_DIR}/src/led/led.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_stm32f4x.c
    ${CMAKE_SOURCE_DIR}/test/ut/test_led.c
)

add_unit_test(test_fsm
    ${CMAKE_SOURCE_DIR}/src/fsm/fsm.c
    ${CMAKE_SOURCE_DIR}/test/ut/test_fsm.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_freertos.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_stm32f4x.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_console.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_led.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_sensor.c
    ${CMAKE_SOURCE_DIR}/test/mock/mock_shell.c
)

add_unit_test(test_protocol
    ${CMAKE_SOURCE_DIR}/src/protocol/protocol.c
    ${CMAKE_SOURCE_DIR}/test/ut/test_protocol.c
)