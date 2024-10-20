#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmocka.h>

#include "protocol.h"

static void test_protocol_should_have_incorrect_parameters(void **state) {
    (void)state;
    protocol_s data;

    protocol_serialize(data, 0, NULL);
}

static void test_protocol_should_completed_buffer(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (255u)
    protocol_s data = {
        .code = PROTOCOL_CODE_FAILURE,
        .message = "test message",
        .response = "test response",
    };
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";

    protocol_serialize(data, PROTOCOL_STRING_LEN_MAX, string);

    assert_string_equal(string, "{\"code\":\"FAILURE\", \"message\":\"test message\", \"response\":\"test response\"}\r\n");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_protocol_should_have_incorrect_parameters),
        cmocka_unit_test(test_protocol_should_completed_buffer),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}