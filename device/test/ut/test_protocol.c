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

static void test_protocol_should_fillin_request_only(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (512u)
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "test message",
        },
        .data_nb = 0,
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX-1, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"}}\r\n");
}

static void test_protocol_should_fillin_all_data(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (512u)
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "test message",
        },
        .data_nb = 3,
        .data = {
            {
                .field = PROTOCOL_DATA_FIELD_TEMPERATURE,
                .value = 0.0f,
                .unit = PROTOCOL_DATA_UNIT_DEGREE_CELSIUS,
            },
            {
                .field = PROTOCOL_DATA_FIELD_HUMIDITY,
                .value = 0.0f,
                .unit = PROTOCOL_DATA_UNIT_PERCENTAGE,
            },
            {
                .field = PROTOCOL_DATA_FIELD_PRESSURE,
                .value = 0.0f,
                .unit = PROTOCOL_DATA_UNIT_PASCAL,
            },
        },
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX-1, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"},\"data\":[{\"field\":\"temperature\",\"value\":0.0,\"unit\":\"degC\"},{\"field\":\"humidity\",\"value\":0.0,\"unit\":\"per100\"},{\"field\":\"pressure\",\"value\":0.0,\"unit\":\"Pa\"}]}\r\n");
}

static void test_protocol_should_fillin_all_metadata(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (512u)
    char string[PROTOCOL_STRING_LEN_MAX] = "";
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "test message",
        },
        .metadata_nb = 2,
        .metadata = {
            {
                .field = PROTOCOL_METADATA_FIELD_0,
                .value = PROTOCOL_METADATA_VALUE_0,
            },
            {
                .field = PROTOCOL_METADATA_FIELD_1,
                .value = PROTOCOL_METADATA_VALUE_1,
            },
        },
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX-1, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"},\"metadata\":[{\"field\":\"field0\",\"value\":\"value0\"},{\"field\":\"field1\",\"value\":\"value1\"}]}\r\n");
}

static void test_protocol_should_fillin_all(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (512u)
    char string[PROTOCOL_STRING_LEN_MAX] = "";
    protocol_s protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "test message",
        },
        .data_nb = 3,
        .data = {
            {
                .field = PROTOCOL_DATA_FIELD_TEMPERATURE,
                .value = 0.0f,
                .unit = PROTOCOL_DATA_UNIT_DEGREE_CELSIUS,
            },
            {
                .field = PROTOCOL_DATA_FIELD_HUMIDITY,
                .value = 0.0f,
                .unit = PROTOCOL_DATA_UNIT_PERCENTAGE,
            },
            {
                .field = PROTOCOL_DATA_FIELD_PRESSURE,
                .value = 0.0f,
                .unit = PROTOCOL_DATA_UNIT_PASCAL,
            },
        },
        .metadata_nb = 2,
        .metadata = {
            {
                .field = PROTOCOL_METADATA_FIELD_0,
                .value = PROTOCOL_METADATA_VALUE_0,
            },
            {
                .field = PROTOCOL_METADATA_FIELD_1,
                .value = PROTOCOL_METADATA_VALUE_1,
            },
        },
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX-1, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"},\"data\":[{\"field\":\"temperature\",\"value\":0.0,\"unit\":\"degC\"},{\"field\":\"humidity\",\"value\":0.0,\"unit\":\"per100\"},{\"field\":\"pressure\",\"value\":0.0,\"unit\":\"Pa\"}],\"metadata\":[{\"field\":\"field0\",\"value\":\"value0\"},{\"field\":\"field1\",\"value\":\"value1\"}]}\r\n");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_protocol_should_have_incorrect_parameters),
        cmocka_unit_test(test_protocol_should_fillin_request_only),
        cmocka_unit_test(test_protocol_should_fillin_all_data),
        cmocka_unit_test(test_protocol_should_fillin_all_metadata),
        cmocka_unit_test(test_protocol_should_fillin_all),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}