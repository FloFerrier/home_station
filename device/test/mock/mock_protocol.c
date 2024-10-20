#include "mock_protocol.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

void mock_assert_call_protocol_serialize(protocol_s data, const uint32_t string_len_max, const char *string) {
    expect_function_call(protocol_serialize);
    expect_memory(protocol_serialize, &data, &data, sizeof(protocol_s));
    expect_value(protocol_serialize, string_len_max, string_len_max);
    assert_non_null(string);
    will_return(protocol_serialize, string);
}
void protocol_serialize(protocol_s data, const uint32_t string_len_max, char *string) {
    function_called();
    check_expected(&data);
    check_expected(string_len_max);
    assert_non_null(string);
    char *expected_string = mock_ptr_type(char *);
    (void)strncpy(string, expected_string, string_len_max);
}