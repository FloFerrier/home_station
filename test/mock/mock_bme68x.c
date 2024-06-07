#include "mock_bme68x.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

void mock_assert_call_bme68x_init(struct bme68x_dev *dev, int8_t ret) {
    expect_function_call(bme68x_init);
    assert_non_null(dev);
    will_return(bme68x_init, ret);
}
int8_t bme68x_init(struct bme68x_dev *dev) {
    function_called();
    assert_non_null(dev);
    return mock();
}

void mock_assert_call_bme68x_soft_reset(struct bme68x_dev *dev, int8_t ret) {
    expect_function_call(bme68x_soft_reset);
    assert_non_null(dev);
    will_return(bme68x_soft_reset, ret);
}
int8_t bme68x_soft_reset(struct bme68x_dev *dev) {
    function_called();
    assert_non_null(dev);
    return mock();
}

void mock_assert_call_bme68x_set_op_mode(const uint8_t op_mode, struct bme68x_dev *dev, int8_t ret) {
    expect_function_call(bme68x_set_op_mode);
    assert_non_null(dev);
    expect_value(bme68x_set_op_mode, op_mode, op_mode);
    will_return(bme68x_set_op_mode, ret);
}
int8_t bme68x_set_op_mode(const uint8_t op_mode, struct bme68x_dev *dev) {
    function_called();
    assert_non_null(dev);
    check_expected(op_mode);
    return mock();
}

void mock_assert_call_bme68x_get_meas_dur(const uint8_t op_mode, struct bme68x_conf *conf, struct bme68x_dev *dev, uint32_t ret) {
    expect_function_call(bme68x_get_meas_dur);
    (void)op_mode;
    assert_non_null(conf);
    assert_non_null(dev);
    expect_value(bme68x_get_meas_dur, op_mode, op_mode);
    will_return(bme68x_get_meas_dur, ret);
}
uint32_t bme68x_get_meas_dur(const uint8_t op_mode, struct bme68x_conf *conf, struct bme68x_dev *dev) {
    function_called();
    assert_non_null(conf);
    assert_non_null(dev);
    check_expected(op_mode);
    return mock();
}

void mock_assert_call_bme68x_get_data(uint8_t op_mode, struct bme68x_data *data, uint8_t n_data, struct bme68x_dev *dev, int8_t ret) {
    expect_function_call(bme68x_get_data);
    expect_value(bme68x_get_data, op_mode, op_mode);
    will_return(bme68x_get_data, n_data);
    assert_non_null(data);
    will_return(bme68x_get_data, data);
    assert_non_null(dev);
    will_return(bme68x_get_data, ret);
}
int8_t bme68x_get_data(uint8_t op_mode, struct bme68x_data *data, uint8_t *n_data, struct bme68x_dev *dev) {
    function_called();
    check_expected(op_mode);
    assert_non_null(n_data);
    *n_data = mock();
    assert_non_null(data);
    struct bme68x_data *expected_data = mock_ptr_type(struct bme68x_data *);
    memcpy(data, expected_data, *n_data * sizeof(struct bme68x_data));
    assert_non_null(dev);
    return mock();
}

void mock_assert_call_bme68x_set_conf(struct bme68x_conf *conf, struct bme68x_dev *dev, int8_t ret) {
    expect_function_call(bme68x_set_conf);
    assert_non_null(conf);
    assert_non_null(dev);
    will_return(bme68x_set_conf, ret);
}
int8_t bme68x_set_conf(struct bme68x_conf *conf, struct bme68x_dev *dev) {
    function_called();
    assert_non_null(conf);
    assert_non_null(dev);
    return mock();
}

void mock_assert_call_bme68x_set_heatr_conf(uint8_t op_mode, const struct bme68x_heatr_conf *conf, struct bme68x_dev *dev, int8_t ret) {
    expect_function_call(bme68x_set_heatr_conf);
    assert_non_null(conf);
    assert_non_null(dev);
    expect_value(bme68x_set_heatr_conf, op_mode, op_mode);
    will_return(bme68x_set_heatr_conf, ret);
}
int8_t bme68x_set_heatr_conf(uint8_t op_mode, const struct bme68x_heatr_conf *conf, struct bme68x_dev *dev) {
    function_called();
    assert_non_null(conf);
    assert_non_null(dev);
    check_expected(op_mode);
    return mock();
}

void mock_assert_call_bme68x_selftest_check(const struct bme68x_dev *dev, int8_t ret) {
    expect_function_call(bme68x_selftest_check);
    assert_non_null(dev);
    will_return(bme68x_selftest_check, ret);
}
int8_t bme68x_selftest_check(const struct bme68x_dev *dev) {
    function_called();
    assert_non_null(dev);
    return mock();
}