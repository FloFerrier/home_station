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
#include "mock_console.h"
#include "mock_stm32f4x.h"
#include "commands.h"

#define MESSAGE_LEN_MAX (255u)

static void test_command_get_index_should_be_help() {
    command_index_e command_is_available = command_getIndex("help");

    assert_int_equal(command_is_available, COMMAND_HELP);
}

static void test_command_get_index_should_be_unknown() {
    command_index_e command_is_available = command_getIndex("");

    assert_int_equal(command_is_available, COMMAND_UNKNOWN);
}

static void test_command_should_avoid_incorrect_parameter() {
    command_index_e command_is_available = command_getIndex(NULL);

    assert_int_equal(command_is_available, COMMAND_UNKNOWN);
}

static void test_command_execute_should_avoid_incorrect_parameter() {
    command_execute(-1, 0, NULL);
}

static void test_command_execute_should_be_unknown_command() {
    #define RESPONSE_LEN_MAX (255u)
    char response[RESPONSE_LEN_MAX+1] = "";

    command_execute(COMMAND_UNKNOWN, RESPONSE_LEN_MAX, response);

    assert_string_equal(response, "{\"code\":\"FAILURE\", \"message\":\"Command unknown\", \"response\":\"Tap help to display all available command.\"}\r\n");
}

static void test_command_execute_should_display_all_available_command() {
    #define RESPONSE_LEN_MAX (255u)
    char response[RESPONSE_LEN_MAX+1] = "";

    command_execute(COMMAND_HELP, RESPONSE_LEN_MAX, response);

    assert_string_equal(response, "{\"code\":\"SUCCESS\", \"message\":\"\", \"response\":\"sensor_selfTest: Performing a sensor self-test, sensor_getData: Request a sensor to get data\"}\r\n");
}

static void test_command_execute_should_reboot_the_system() {
    #define RESPONSE_LEN_MAX (255u)
    char response[RESPONSE_LEN_MAX+1] = "";
    mock_assert_call_console_send("{\"code\":\"SUCCESS\", \"message\":\"\", \"response\":\"\"}\r\n", true);
    mock_assert_call_NVIC_SystemReset();

    command_execute(COMMAND_REBOOT, RESPONSE_LEN_MAX, response);
}

static void test_command_execute_should_failed_sensor_selftest() {
    #define RESPONSE_LEN_MAX (255u)
    char response[RESPONSE_LEN_MAX+1] = "";
    mock_assert_call_sensor_selfTest(SENSOR_SELF_TEST_FAILURE);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_SELF_TEST_FAILURE, "Sensor self test failure");

    command_execute(COMMAND_SENSOR_SELF_TEST, RESPONSE_LEN_MAX, response);

    assert_string_equal(response, "{\"code\":\"FAILURE\", \"message\":\"Sensor self test failure\", \"response\":\"\"}\r\n");
}

static void test_command_execute_should_passed_sensor_selftest() {
    #define RESPONSE_LEN_MAX (255u)
    char response[RESPONSE_LEN_MAX+1] = "";
    mock_assert_call_sensor_selfTest(SENSOR_OK);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_OK, "Sensor ok");

    command_execute(COMMAND_SENSOR_SELF_TEST, RESPONSE_LEN_MAX, response);

    assert_string_equal(response, "{\"code\":\"SUCCESS\", \"message\":\"Sensor ok\", \"response\":\"\"}\r\n");
}

static void test_command_execute_should_failed_sensor_get_data() {
    #define RESPONSE_LEN_MAX (255u)
    char response[RESPONSE_LEN_MAX+1] = "";
    sensor_data_s expected_data = {0};
    uint32_t expected_number_of_data = 0;
    mock_assert_call_sensor_getData(&expected_data, expected_number_of_data, SENSOR_MISC_FAILURE);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_MISC_FAILURE, "Sensor misc failure");

    command_execute(COMMAND_SENSOR_GET_DATA, RESPONSE_LEN_MAX, response);

    assert_string_equal(response, "{\"code\":\"FAILURE\", \"message\":\"Sensor misc failure\", \"response\":\"\"}\r\n");
}

static void test_command_execute_should_passed_sensor_get_data() {
    #define RESPONSE_LEN_MAX (255u)
    char response[RESPONSE_LEN_MAX+1] = "";
    sensor_data_s expected_data = {
        .temperature_in_deg = 15.0f,
        .pressure_in_pascal = 98000.0f,
        .humidity_in_per100 = 50.0f,
        .gas_resistance_in_ohms = 0.0f,
    };

    mock_assert_call_sensor_getData(&expected_data, 1u, SENSOR_OK);
    mock_assert_call_sensor_returnCodeAsString(SENSOR_OK, "Sensor ok");

    command_execute(COMMAND_SENSOR_GET_DATA, RESPONSE_LEN_MAX, response);

    assert_string_equal(response, "{\"code\":\"SUCCESS\", \"message\":\"Sensor ok\", \"response\":\"15.0 deg, 980.0 hPa 50.0 rH\"}\r\n");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_command_get_index_should_be_help),
        cmocka_unit_test(test_command_get_index_should_be_unknown),
        cmocka_unit_test(test_command_should_avoid_incorrect_parameter),
        cmocka_unit_test(test_command_execute_should_avoid_incorrect_parameter),
        cmocka_unit_test(test_command_execute_should_be_unknown_command),
        cmocka_unit_test(test_command_execute_should_display_all_available_command),
        cmocka_unit_test(test_command_execute_should_reboot_the_system),
        cmocka_unit_test(test_command_execute_should_failed_sensor_selftest),
        cmocka_unit_test(test_command_execute_should_passed_sensor_selftest),
        cmocka_unit_test(test_command_execute_should_failed_sensor_get_data),
        cmocka_unit_test(test_command_execute_should_passed_sensor_get_data),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}