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
    protocol_s protocol;

    protocol_serialize(protocol, 0, NULL);
}

static void test_protocol_should_be_have_all_field_empty(void **state) {
    (void)state;
    protocol_s protocol = {0};
    #define PROTOCOL_STRING_SIZE_MAX (512u)
    char string[PROTOCOL_STRING_SIZE_MAX] = "";

    protocol_serialize(protocol, PROTOCOL_STRING_SIZE_MAX, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"UNKNOWN\",\"message\":\"\"},\"data\":[],\"metadata\":[]}\r\n");
}

static void test_protocol_should_fillin_field_request(void **state) {
    (void)state;
    #define PROTOCOL_STRING_SIZE_MAX (512u)
    char string[PROTOCOL_STRING_SIZE_MAX] = "";
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "some line to test this ;)",
        },
    };

    protocol_serialize(protocol, PROTOCOL_STRING_SIZE_MAX, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"some line to test this ;)\"},\"data\":[],\"metadata\":[]}\r\n");
}

static void test_protocol_should_fillin_field_data(void **state) {
    (void)state;
    #define PROTOCOL_STRING_SIZE_MAX (512u)
    char string[PROTOCOL_STRING_SIZE_MAX+1] = "";
    protocol_s protocol = {
        .data_nb = 3,
        .data = {
            {
                .name = "temperature",
                .value = 25.0f,
                .unit = "degC",
            },
            {
                .name = "humidity",
                .value = 100.0f,
                .unit = "per100",
            },
            {
                .name = "pressure",
                .value = 1000.0f,
                .unit = "Pa",
            },
        },
    };

    protocol_serialize(protocol, PROTOCOL_STRING_SIZE_MAX-1, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"UNKNOWN\",\"message\":\"\"},\"data\":[{\"name\":\"temperature\",\"value\":25.0,\"unit\":\"degC\"},{\"name\":\"humidity\",\"value\":100.0,\"unit\":\"per100\"},{\"name\":\"pressure\",\"value\":1000.0,\"unit\":\"Pa\"}],\"metadata\":[]}\r\n");
}

static void test_protocol_should_fillin_field_metadata(void **state) {
    (void)state;
    char string[512] = "";
    protocol_s protocol = {
        .metadata_nb = 2,
        .metadata = {
            {
                .name = "field 0",
                .value = "value 0",
            },
            {
                .name = "field 1",
                .value = "value 1",
            },
        },
    };

    protocol_serialize(protocol, 512-1, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"UNKNOWN\",\"message\":\"\"},\"data\":[],\"metadata\":[{\"name\":\"field 0\",\"value\":\"value 0\"},{\"name\":\"field 1\",\"value\":\"value 1\"}]}\r\n");
}

static void test_protocol_should_fillin_all_field_with_maximum_case(void **state) {
    (void)state;
    char string[2048] = "";
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "this message need 32 character ",
        },
        .data_nb = 3,
        .data = {
            {
                .name = "data name field",
                .value = 0.0f,
                .unit = "data unit field",
            },
            {
                .name = "data name field",
                .value = 0.0f,
                .unit = "data unit field",
            },
            {
                .name = "data name field",
                .value = 0.0f,
                .unit = "data unit field",
            },
        },
        .metadata_nb = 2,
        .metadata = {
            {
                .name = "metadata name field with max ch",
                .value = "metadata value field with max c",
            },
            {
                .name = "metadata name field with max ch",
                .value = "metadata value field with max c",
            },
        },
    };

    protocol_serialize(protocol, 2048-1, string);

    // Note: If a string is too long => it is truncated due to static buffer for each data
    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"this message need 32 character \"},\"data\":[{\"name\":\"data name field\",\"value\":0.0,\"unit\":\"data unit field\"},{\"name\":\"data name field\",\"value\":0.0,\"unit\":\"data unit field\"},{\"name\":\"data name field\",\"value\":0.0,\"unit\":\"data unit field\"}],\"metadata\":[{\"name\":\"metadata name field with max ch\",\"value\":\"metadata value field with max c\"},{\"name\":\"metadata name field with max ch\",\"value\":\"metadata value field with max c\"}]}\r\n");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_protocol_should_have_incorrect_parameters),
        cmocka_unit_test(test_protocol_should_be_have_all_field_empty),
        cmocka_unit_test(test_protocol_should_fillin_field_request),
        cmocka_unit_test(test_protocol_should_fillin_field_data),
        cmocka_unit_test(test_protocol_should_fillin_field_metadata),
        cmocka_unit_test(test_protocol_should_fillin_all_field_with_maximum_case),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}