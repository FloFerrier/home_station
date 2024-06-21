#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

#include "mock_commands.h"

void mock_assert_call_command_getIndex(const char *cmd, command_index_e ret) {
    expect_function_call(command_getIndex);
    assert_non_null(cmd);
    expect_string(command_getIndex, cmd, cmd);
    will_return(command_getIndex, ret);
}
command_index_e command_getIndex(const char *cmd) {
    function_called();
    check_expected(cmd);
    return mock();
}

void mock_assert_call_command_execute(command_index_e command_index, bool ret) {
    expect_function_call(command_execute);
    expect_value(command_execute, command_index, command_index);
    will_return(command_execute, ret);
}
bool command_execute(command_index_e command_index) {
    function_called();
    check_expected(command_index);
    return mock();
}
