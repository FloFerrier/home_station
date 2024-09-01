add_unit_test(test_shell
    ${CMAKE_SOURCE_DIR}/src/shell/shell.c
    mock/mock_console.c
    mock/mock_commands.c
    ut/test_shell.c
)

add_unit_test(test_bme680
    ut/test_bme680.c
)

add_unit_test(test_sensor
    ${CMAKE_SOURCE_DIR}/src/sensor/sensor.c
    mock/mock_bme68x.c
    mock/mock_stm32f4x.c
    ut/test_sensor.c
)

add_unit_test(test_commands
    ${CMAKE_SOURCE_DIR}/src/commands/commands.c
    mock/mock_sensor.c
    ut/test_commands.c
)

add_unit_test(test_console
    ${CMAKE_SOURCE_DIR}/src/console/console.c
    mock/mock_stm32f4x.c
    ut/test_console.c
)

add_unit_test(test_led
    ${CMAKE_SOURCE_DIR}/src/led/led.c
    mock/mock_stm32f4x.c
    ut/test_led.c
)