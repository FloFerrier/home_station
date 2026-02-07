add_cmocka_test(test_bme680
    SOURCES test_bme680.c
    LINK_LIBRARIES cmocka-static bme68x
)

add_cmocka_test(test_commands
    SOURCES test_commands.c
    LINK_LIBRARIES cmocka-static commands mock_freertos mock_sensor mock_protocol
)

add_cmocka_test(test_console
    SOURCES test_console.c
    LINK_LIBRARIES cmocka-static console mock_stm32f4x
)

add_cmocka_test(test_fsm
    SOURCES test_fsm.c
    LINK_LIBRARIES cmocka-static fsm mock_freertos mock_console mock_led mock_sensor mock_shell mock_stm32f4x
)

add_cmocka_test(test_led
    SOURCES test_led.c
    LINK_LIBRARIES cmocka-static led mock_stm32f4x
)

add_cmocka_test(test_protocol
    SOURCES test_protocol.c
    LINK_LIBRARIES cmocka-static protocol
)

add_cmocka_test(test_sensor
    SOURCES test_sensor.c
    LINK_LIBRARIES cmocka-static sensor mock_stm32f4x mock_bme68x
)

add_cmocka_test(test_shell
    SOURCES test_shell.c
    LINK_LIBRARIES cmocka-static shell mock_console mock_commands
)