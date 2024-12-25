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
    #define PROTOCOL_STRING_LEN_MAX (255u)
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";
    protocol_request_s request = {
        .code = PROTOCOL_REQUEST_CODE_SUCCESS,
        .message = "test message",
    };
    protocol_s protocol = {
        .request = &request,
        .data = NULL,
        .metadata = NULL,
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"}}\r\n");
}

static void test_protocol_should_fillin_data(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (255u)
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";
    protocol_request_s request = {
        .code = PROTOCOL_REQUEST_CODE_SUCCESS,
        .message = "test message",
    };
    protocol_data_s data = {
        .field = PROTOCOL_DATA_FIELD_0,
        .value = 0.0f,
        .unit = PROTOCOL_DATA_UNIT_0,
    };
    protocol_s protocol = {
        .request = &request,
        .data = &data,
        .metadata = NULL,
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"},\"data\":{\"field\":\"field0\",\"value\":0.0,\"unit\":\"unit0\"}}\r\n");
}

static void test_protocol_should_fillin_metadata(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (255u)
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";
    protocol_request_s request = {
        .code = PROTOCOL_REQUEST_CODE_SUCCESS,
        .message = "test message",
    };
    protocol_metadata_s metadata = {
        .field = PROTOCOL_METADATA_FIELD_0,
        .value = PROTOCOL_METADATA_VALUE_0,
    };
    protocol_s protocol = {
        .request = &request,
        .data = NULL,
        .metadata = &metadata,
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"},\"metadata\":{\"field\":\"field0\",\"value\":\"value0\"}}\r\n");
}

static void test_protocol_should_fillin_all(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (255u)
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";
    protocol_request_s request = {
        .code = PROTOCOL_REQUEST_CODE_SUCCESS,
        .message = "test message",
    };
    protocol_data_s data = {
        .field = PROTOCOL_DATA_FIELD_0,
        .value = 0.0f,
        .unit = PROTOCOL_DATA_UNIT_0,
    };
    protocol_metadata_s metadata = {
        .field = PROTOCOL_METADATA_FIELD_0,
        .value = PROTOCOL_METADATA_VALUE_0,
    };
    protocol_s protocol = {
        .request = &request,
        .data = &data,
        .metadata = &metadata,
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX, string);

    assert_string_equal(string, "{\"request\":{\"code\":\"SUCCESS\",\"message\":\"test message\"},\"data\":{\"field\":\"field0\",\"value\":0.0,\"unit\":\"unit0\"},\"metadata\":{\"field\":\"field0\",\"value\":\"value0\"}}\r\n");
}

static void test_protocol_should_message_overflow(void **state) {
    (void)state;
    #define PROTOCOL_STRING_LEN_MAX (255u)
    char string[PROTOCOL_STRING_LEN_MAX+1] = "";
    protocol_request_s request = {
        .code = PROTOCOL_REQUEST_CODE_SUCCESS,
    };
    (void)memset(request.message, 'a', PROTOCOL_REQUEST_MESSAGE_LEN_MAX);
    protocol_data_s data = {
        .field = PROTOCOL_DATA_FIELD_0,
        .value = 0.0f,
        .unit = PROTOCOL_DATA_UNIT_0,
    };
    protocol_metadata_s metadata = {
        .field = PROTOCOL_METADATA_FIELD_0,
        .value = PROTOCOL_METADATA_VALUE_0,
    };
    protocol_s protocol = {
        .request = &request,
        .data = &data,
        .metadata = &metadata,
    };

    protocol_serialize(protocol, PROTOCOL_STRING_LEN_MAX, string);

    skip();
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_protocol_should_have_incorrect_parameters),
        cmocka_unit_test(test_protocol_should_fillin_request_only),
        cmocka_unit_test(test_protocol_should_fillin_data),
        cmocka_unit_test(test_protocol_should_fillin_metadata),
        cmocka_unit_test(test_protocol_should_fillin_all),
        cmocka_unit_test(test_protocol_should_message_overflow),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}