add_cmocka_test(test_shell
    SOURCES test_shell.c
            ${CMAKE_SOURCE_DIR}/src/shell/shell.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka mock
)

add_cmocka_test(test_bme680
    SOURCES test_bme680.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka bme68x
)

add_cmocka_test(test_sensor
    SOURCES test_sensor.c
            ${CMAKE_SOURCE_DIR}/src/sensor/sensor.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka mock
)

add_cmocka_test(test_commands
    SOURCES test_commands.c
            ${CMAKE_SOURCE_DIR}/src/commands/commands.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka mock
)

add_cmocka_test(test_console
    SOURCES test_console.c
            ${CMAKE_SOURCE_DIR}/src/console/console.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka mock
)

add_cmocka_test(test_led
    SOURCES test_led.c
            ${CMAKE_SOURCE_DIR}/src/led/led.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka mock
)

add_cmocka_test(test_fsm
    SOURCES test_fsm.c
            ${CMAKE_SOURCE_DIR}/src/fsm/fsm.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka mock
)

add_cmocka_test(test_protocol
    SOURCES test_protocol.c
            ${CMAKE_SOURCE_DIR}/src/protocol/protocol.c
    COMPILE_OPTIONS -g -Wall -Wextra -Werror -DTEST
    LINK_LIBRARIES cmocka mock
)