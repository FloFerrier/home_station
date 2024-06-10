#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

#include "mock_commands.h"

void mock_assert_call_shellCommand_sensorSelfTest(void) {
    expect_function_call(shellCommand_sensorSelfTest);
}
void shellCommand_sensorSelfTest(uint32_t argc, char *argv[]) {
    function_called();
    (void)argc;
    (void)argv;
}

void mock_assert_call_shellCommand_sensorGetData(void) {
    expect_function_call(shellCommand_sensorGetData);
}
void shellCommand_sensorGetData(uint32_t argc, char *argv[]) {
    function_called();
    (void)argc;
    (void)argv;
}