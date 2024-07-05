#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

#include "mock_sensor.h"

void mock_assert_call_sensor_init(sensor_returnCode_e ret) {
    expect_function_call(sensor_init);
    will_return(sensor_init, ret);
}
sensor_returnCode_e sensor_init(void) {
    function_called();
    return mock();
}

void mock_assert_call_sensor_selfTest(sensor_returnCode_e ret) {
    expect_function_call(sensor_selfTest);
    will_return(sensor_selfTest, ret);
}
sensor_returnCode_e sensor_selfTest(void) {
    function_called();
    return mock();
}

void mock_assert_call_sensor_getData(sensor_data_s *data, uint32_t number_of_data, sensor_returnCode_e ret) {
    expect_function_call(sensor_getData);
    assert_non_null(data);
    will_return(sensor_getData, number_of_data);
    will_return(sensor_getData, data);
    will_return(sensor_getData, ret);
}
sensor_returnCode_e sensor_getData(sensor_data_s *data, uint32_t *number_of_data) {
    function_called();
    uint32_t expected_nb_of_data = mock();
    assert_non_null(number_of_data);
    *number_of_data = expected_nb_of_data;
    sensor_data_s *expected_data = mock_ptr_type(sensor_data_s *);
    assert_non_null(data);
    memcpy(data, expected_data, expected_nb_of_data * sizeof(sensor_data_s));
    return mock();
}