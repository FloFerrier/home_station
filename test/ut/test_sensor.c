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

extern int loopCnt;

static void test_sensor_should_failed_init(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_E_COM_FAIL);

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_E_COM_FAIL);
}

static void test_sensor_should_failed_selftest(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_E_SELF_TEST);

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_E_SELF_TEST);
}

static void test_sensor_should_failed_set_conf(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);
    struct bme68x_conf conf;
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_I_PARAM_CORR);

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_I_PARAM_CORR);
}

static void test_sensor_should_failed_set_heatr_conf(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);
    struct bme68x_conf conf;
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf;
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_I_PARAM_CORR);

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_I_PARAM_CORR);
}

static void test_sensor_should_failed_set_forced_mode(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);
    struct bme68x_conf conf;
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf;
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_I_PARAM_CORR);

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_I_PARAM_CORR);
}

static void test_sensor_should_no_available_data(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);
    struct bme68x_conf conf;
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf;
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, 10000u);
    mock_assert_call_console_send("(sensor)> Wait 10000 us before get data\r\n");
    struct bme68x_data data;
    uint8_t n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &data, n_data, &dev, BME68X_W_NO_NEW_DATA);
    mock_assert_call_console_send("(sensor)> No new data available\r\n");

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_W_NO_NEW_DATA);
}

static void test_sensor_should_failed_get_data(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);
    struct bme68x_conf conf;
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf;
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, 10000u);
    mock_assert_call_console_send("(sensor)> Wait 10000 us before get data\r\n");
    struct bme68x_data data;
    uint8_t n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &data, n_data, &dev, BME68X_E_COM_FAIL);
    mock_assert_call_console_send("(sensor)> Error to get data -2\r\n");

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_E_COM_FAIL);
}

static void test_sensor_should_get_some_data(void **state) {
    (void)state;
    loopCnt = 0;

    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);
    struct bme68x_conf conf;
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf;
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

    int8_t bme68x_rslt;
    sensor_task(&bme68x_rslt);

    assert_int_equal(bme68x_rslt, BME68X_OK);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sensor_should_failed_init),
        cmocka_unit_test(test_sensor_should_failed_selftest),
        cmocka_unit_test(test_sensor_should_failed_set_conf),
        cmocka_unit_test(test_sensor_should_failed_set_heatr_conf),
        cmocka_unit_test(test_sensor_should_failed_set_forced_mode),
        cmocka_unit_test(test_sensor_should_no_available_data),
        cmocka_unit_test(test_sensor_should_failed_get_data),
        cmocka_unit_test(test_sensor_should_get_some_data),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}