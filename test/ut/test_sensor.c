#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cmocka.h>

#include "mock_bme68x.h"

#include "sensor.h"

static void test_sensor_should_failed_initialization(void **state) {
    (void)state;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_E_COM_FAIL);

    int8_t rslt = sensor_init();

    assert_int_equal(rslt, BME68X_E_COM_FAIL);
}

static void test_sensor_should_be_initialized(void **state) {
    (void)state;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);

    int8_t rslt = sensor_init();

    assert_int_equal(rslt, BME68X_OK);
}

static void test_sensor_should_failed_selftest(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_E_SELF_TEST);

    int8_t rslt = sensor_selfTest();

    assert_int_equal(rslt, BME68X_E_SELF_TEST);
}

static void test_sensor_should_be_selftested(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);

    int8_t rslt = sensor_selfTest();

    assert_int_equal(rslt, BME68X_OK);
}

static void test_sensor_should_have_invalid_parameters(void **state) {
    (void)state;

    int8_t rslt = sensor_getData(NULL, 0);

    assert_int_equal(rslt, BME68X_E_NULL_PTR);
}

static void test_sensor_should_failed_set_conf(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_I_PARAM_CORR);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, BME68X_I_PARAM_CORR);
}

static void test_sensor_should_failed_set_heatr_conf(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_I_PARAM_CORR);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, BME68X_I_PARAM_CORR);
}

static void test_sensor_should_failed_set_forced_mode(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_I_PARAM_CORR);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, BME68X_I_PARAM_CORR);
}

static void test_sensor_should_no_available_data(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, 10000u);
    struct bme68x_data bme68x_data = {0};
    uint8_t bme68x_n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &bme68x_data, bme68x_n_data, &dev, BME68X_W_NO_NEW_DATA);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, BME68X_W_NO_NEW_DATA);
}

static void test_sensor_should_failed_get_data(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, 10000u);
    struct bme68x_data bme68x_data = {0};
    uint8_t bme68x_n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &bme68x_data, bme68x_n_data, &dev, BME68X_E_COM_FAIL);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, BME68X_E_COM_FAIL);
}

static void test_sensor_should_get_several_data(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, 10000u);
    struct bme68x_data bme68x_data[SENSOR_MAX_DATA_AVAILABLE] = {
        {.temperature = 15.0f, .pressure = 98000.0f, .humidity = 45.0f},
        {.temperature = 20.0f, .pressure = 99000.0f, .humidity = 50.0f},
        {.temperature = 25.0f, .pressure = 100000.0f, .humidity = 55.0f},
        {.temperature = 30.0f, .pressure = 11000.0f, .humidity = 60.0f},
    };
    uint8_t bme68x_n_data = sizeof(bme68x_data) / sizeof(struct bme68x_data);
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, bme68x_data, bme68x_n_data, &dev, BME68X_OK);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, BME68X_OK);
    sensor_data_s expected_data[SENSOR_MAX_DATA_AVAILABLE] = {
        {.temperature_in_deg = 15.0f, .pressure_in_pascal = 98000.0f, .humidity_in_per100 = 45.0f},
        {.temperature_in_deg = 20.0f, .pressure_in_pascal = 99000.0f, .humidity_in_per100 = 50.0f},
        {.temperature_in_deg = 25.0f, .pressure_in_pascal = 100000.0f, .humidity_in_per100 = 55.0f},
        {.temperature_in_deg = 30.0f, .pressure_in_pascal = 11000.0f, .humidity_in_per100 = 60.0f},
    };
    uint32_t expected_nb_of_data = sizeof(expected_data) / sizeof(sensor_data_s);
    assert_int_equal(number_of_data, expected_nb_of_data);
    for(uint32_t index=0; index< expected_nb_of_data; index++) {
        assert_float_equal(data[index].temperature_in_deg, expected_data[index].temperature_in_deg, 1e-1);
        assert_float_equal(data[index].pressure_in_pascal, expected_data[index].pressure_in_pascal, 1e-1);
        assert_float_equal(data[index].humidity_in_per100, expected_data[index].humidity_in_per100, 1e-1);
    }
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sensor_should_failed_initialization),
        cmocka_unit_test(test_sensor_should_be_initialized),
        cmocka_unit_test(test_sensor_should_failed_selftest),
        cmocka_unit_test(test_sensor_should_be_selftested),
        cmocka_unit_test(test_sensor_should_have_invalid_parameters),
        cmocka_unit_test(test_sensor_should_failed_set_conf),
        cmocka_unit_test(test_sensor_should_failed_set_heatr_conf),
        cmocka_unit_test(test_sensor_should_failed_set_forced_mode),
        cmocka_unit_test(test_sensor_should_no_available_data),
        cmocka_unit_test(test_sensor_should_failed_get_data),
        cmocka_unit_test(test_sensor_should_get_several_data),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}