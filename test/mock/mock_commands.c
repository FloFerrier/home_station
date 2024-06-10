#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

#include "mock_commands.h"

void mock_assert_call_command_sensorSelfTest(void) {
    expect_function_call(command_sensorSelfTest);
}
void command_sensorSelfTest(uint32_t argc, char *argv[]) {
    function_called();
    (void)argc;
    (void)argv;
}

void mock_assert_call_command_sensorGetData(void) {
    expect_function_call(command_sensorGetData);
}
void command_sensorGetData(uint32_t argc, char *argv[]) {
    function_called();
    (void)argc;
    (void)argv;
}