#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cmocka.h>

#include "mock_bme68x.h"
#include "mock_console.h"

#include "sensor.h"

static void test_sensor_should_failed_initialization(void **state) {
    (void)state;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_E_COM_FAIL);

    bool rslt = sensor_init();

    assert_int_equal(rslt, false);
}

static void test_sensor_should_be_initialized(void **state) {
    (void)state;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);

    bool rslt = sensor_init();

    assert_int_equal(rslt, true);
}

static void test_sensor_should_failed_selftest(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_E_SELF_TEST);

    bool rslt = sensor_selfTest();

    assert_int_equal(rslt, false);
}

static void test_sensor_should_be_selftested(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);

    bool rslt = sensor_selfTest();

    assert_int_equal(rslt, true);
}

static void test_sensor_should_failed_set_conf(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_I_PARAM_CORR);

    bool rslt = sensor_getData();

    assert_int_equal(rslt, false);
}

static void test_sensor_should_failed_set_heatr_conf(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_I_PARAM_CORR);

    bool rslt = sensor_getData();

    assert_int_equal(rslt, false);
}

static void test_sensor_should_failed_set_forced_mode(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_I_PARAM_CORR);

    bool rslt = sensor_getData();

    assert_int_equal(rslt, false);
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
    mock_assert_call_console_send("(sensor)> Wait 10000 us before get data\r\n");
    struct bme68x_data data = {0};
    uint8_t n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &data, n_data, &dev, BME68X_W_NO_NEW_DATA);
    mock_assert_call_console_send("(sensor)> No new data available\r\n");

    bool rslt = sensor_getData();

    assert_int_equal(rslt, false);
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
    mock_assert_call_console_send("(sensor)> Wait 10000 us before get data\r\n");
    struct bme68x_data data = {0};
    uint8_t n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &data, n_data, &dev, BME68X_E_COM_FAIL);
    mock_assert_call_console_send("(sensor)> Error to get data -2\r\n");

    bool rslt = sensor_getData();

    assert_int_equal(rslt, false);
}

static void test_sensor_should_get_some_data(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, 10000u);
    mock_assert_call_console_send("(sensor)> Wait 10000 us before get data\r\n");
    struct bme68x_data data = {
        .temperature = 25.0f,
        .pressure = 98000.0f,
        .humidity = 50.0f,
    };
    uint8_t n_data = 1;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &data, n_data, &dev, BME68X_OK);
    mock_assert_call_console_send("(sensor)> 1 data available\r\n");
    mock_assert_call_console_send("(sensor)> 25.0°C 50.0%Rh 980.0hPa\r\n");

    bool rslt = sensor_getData();

    assert_int_equal(rslt, true);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sensor_should_failed_initialization),
        cmocka_unit_test(test_sensor_should_be_initialized),
        cmocka_unit_test(test_sensor_should_failed_selftest),
        cmocka_unit_test(test_sensor_should_be_selftested),
        cmocka_unit_test(test_sensor_should_failed_set_conf),
        cmocka_unit_test(test_sensor_should_failed_set_heatr_conf),
        cmocka_unit_test(test_sensor_should_failed_set_forced_mode),
        cmocka_unit_test(test_sensor_should_no_available_data),
        cmocka_unit_test(test_sensor_should_failed_get_data),
        cmocka_unit_test(test_sensor_should_get_some_data),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}