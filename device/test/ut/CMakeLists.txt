add_cmocka_test(test_bme680
    SOURCES test_bme680.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka bme68x
)

add_cmocka_test(test_commands
    SOURCES test_commands.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka commands mock_freertos mock_sensor mock_protocol
)

add_cmocka_test(test_console
    SOURCES test_console.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka console mock_stm32f4x
)

add_cmocka_test(test_fsm
    SOURCES test_fsm.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka fsm mock_freertos mock_console mock_led mock_sensor mock_shell mock_stm32f4x
)

add_cmocka_test(test_led
    SOURCES test_led.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka led mock_stm32f4x
)

add_cmocka_test(test_protocol
    SOURCES test_protocol.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka protocol
)

add_cmocka_test(test_sensor
    SOURCES test_sensor.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka sensor mock_stm32f4x mock_bme68x
)

add_cmocka_test(test_shell
    SOURCES test_shell.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror
    LINK_LIBRARIES cmocka shell mock_console mock_commands
)