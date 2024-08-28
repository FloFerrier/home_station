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

void mock_assert_call_command_execute(command_index_e command_index, const uint32_t response_len_max, const char *expected_response) {
    expect_function_call(command_execute);
    expect_value(command_execute, command_index, command_index);
    expect_value(command_execute, response_len_max, response_len_max);
    assert_non_null(expected_response);
    will_return(command_execute, expected_response);
}
void command_execute(command_index_e command_index, const uint32_t response_len_max, char *response) {
    function_called();
    check_expected(command_index);
    check_expected(response_len_max);
    assert_non_null(response);
    char *expected_response = mock_ptr_type(char *);
    (void)strncpy(response, expected_response, response_len_max);
}
