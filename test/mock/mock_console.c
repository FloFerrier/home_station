#include "mock_console.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

void mock_assert_call_console_init(void) {
    expect_function_call(console_init);
}
void console_init(void) {
    function_called();
}

void mock_assert_call_console_send(const char *string) {
    expect_function_call(console_send);
    assert_non_null(string);
    will_return(console_send, string);
}
void console_send(const char* format, ...) {
    function_called();
    va_list va;
    va_start(va, format);
    const int BUFFER_LEN_MAX = 256u;
    char *buffer_string = (char*)malloc(sizeof(char) * BUFFER_LEN_MAX);
    int buffer_len = vsnprintf(buffer_string, BUFFER_LEN_MAX, format, va);
    va_end(va);
    char *expected_string = mock_ptr_type(char *);
    int expected_len = strlen(expected_string);
    assert_string_equal(buffer_string, expected_string);
    assert_int_equal(buffer_len, expected_len);
    free(buffer_string);
}

void mock_assert_call_console_receive(char character, bool ret) {
    expect_function_call(console_receive);
    will_return(console_receive, character);
    will_return(console_receive, ret);
}
bool console_receive(char *character) {
    function_called();
    *character = mock();
    return mock();
}