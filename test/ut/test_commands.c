#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cmocka.h>

#include "mock_sensor.h"
#include "mock_protocol.h"
#include "commands.h"

#define MESSAGE_LEN_MAX (255u)

static void test_command_should_avoid_incorrect_parameter() {
    command_index_e command_is_available = command_getIndex(NULL);

    assert_int_equal(command_is_available, COMMAND_UNKNOWN);
}

static void test_command_execute_should_avoid_incorrect_parameter() {
    command_execute(-1, 0, NULL);
}

static void test_command_execute_should_failed_sensor_selftest() {
    #define RESPONSE_LEN_MAX (256u)
    char response[RESPONSE_LEN_MAX] = "";
    mock_assert_call_sensor_selfTest(SENSOR_SELF_TEST_FAILURE);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_SELF_TEST_FAILURE, "Sensor self test failure");
    protocol_s expected_protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_FAILURE,
            .message = "Sensor self test failure",
        },
    };
    mock_assert_call_protocol_serialize(expected_protocol, RESPONSE_LEN_MAX, response);

    command_execute(COMMAND_SENSOR_SELF_TEST, RESPONSE_LEN_MAX, response);
}

static void test_command_execute_should_passed_sensor_selftest() {
    #define RESPONSE_LEN_MAX (256u)
    char response[RESPONSE_LEN_MAX] = "";
    mock_assert_call_sensor_selfTest(SENSOR_OK);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_OK, "Sensor ok");
    protocol_s expected_protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "Sensor ok",
        },
    };
    mock_assert_call_protocol_serialize(expected_protocol, RESPONSE_LEN_MAX, response);

    command_execute(COMMAND_SENSOR_SELF_TEST, RESPONSE_LEN_MAX, response);
}

static void test_command_execute_should_failed_sensor_get_data() {
    #define RESPONSE_LEN_MAX (256u)
    char response[RESPONSE_LEN_MAX] = "";
    sensor_data_s expected_sensor_data = {0};
    uint32_t expected_number_of_data = 0;
    mock_assert_call_sensor_getData(&expected_sensor_data, expected_number_of_data, SENSOR_MISC_FAILURE);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_MISC_FAILURE, "Sensor misc failure");
    protocol_s expected_protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_FAILURE,
            .message = "Sensor misc failure",
        },
    };
    mock_assert_call_protocol_serialize(expected_protocol, RESPONSE_LEN_MAX, response);

    command_execute(COMMAND_SENSOR_GET_DATA, RESPONSE_LEN_MAX, response);
}

static void test_command_execute_should_passed_sensor_get_data() {
    #define RESPONSE_LEN_MAX (256u)
    char response[RESPONSE_LEN_MAX] = "";
    sensor_data_s expected_sensor_data = {
        .temperature_in_deg = 15.0f,
        .pressure_in_pascal = 98000.0f,
        .humidity_in_per100 = 50.0f,
        .gas_resistance_in_ohms = 0.0f,
    };
    mock_assert_call_sensor_getData(&expected_sensor_data, 1u, SENSOR_OK);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_OK, "Sensor ok");
    protocol_s expected_protocol = {
        .request = {
            .code = PROTOCOL_REQUEST_CODE_SUCCESS,
            .message = "Sensor ok",
        },
        .data_nb = 3,
        .data = {
            [0] = {
                .name = "temperature",
                .value = 15.0f,
                .unit = "degC",
            },
            [1] = {
                .name = "humidity",
                .value = 50.0f,
                .unit = "per100",
            },
            [2] = {
                .name = "pressure",
                .value = 98000.0f,
                .unit = "Pa",
            },
        },
    };
    mock_assert_call_protocol_serialize(expected_protocol, RESPONSE_LEN_MAX, response);

    command_execute(COMMAND_SENSOR_GET_DATA, RESPONSE_LEN_MAX, response);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_command_should_avoid_incorrect_parameter),
        cmocka_unit_test(test_command_execute_should_avoid_incorrect_parameter),
        cmocka_unit_test(test_command_execute_should_failed_sensor_selftest),
        cmocka_unit_test(test_command_execute_should_passed_sensor_selftest),
        cmocka_unit_test(test_command_execute_should_failed_sensor_get_data),
        cmocka_unit_test(test_command_execute_should_passed_sensor_get_data),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}