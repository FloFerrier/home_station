#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cmocka.h>

#include <bme68x.h>

static uint8_t dev_addr = BME68X_I2C_ADDR_LOW;

static void mock_assert_call_bme68x_i2c_read(uint8_t reg_addr_expt, uint8_t *reg_data_mock, uint32_t len_expt, void *intf_ptr, int8_t rslt) {
    expect_function_call(bme68x_i2c_read);
    expect_value(bme68x_i2c_read, reg_addr, reg_addr_expt);
    expect_value(bme68x_i2c_read, len, len_expt);
    assert_non_null(reg_data_mock);
    will_return(bme68x_i2c_read, reg_data_mock);
    (void)intf_ptr;
    will_return(bme68x_i2c_read, rslt);
}
static int8_t bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    function_called();
    check_expected(reg_addr);
    check_expected(len);
    uint8_t *reg_data_mock = mock_ptr_type(uint8_t *);
    memcpy(reg_data, reg_data_mock, len);
    (void)intf_ptr;
    return mock();
}

static void mock_assert_call_bme68x_i2c_write(uint8_t reg_addr_expt, const uint8_t *reg_data_expt, uint32_t len_expt, void *intf_ptr, int8_t rslt) {
    expect_function_call(bme68x_i2c_write);
    expect_value(bme68x_i2c_write, reg_addr, reg_addr_expt);
    expect_value(bme68x_i2c_write, len, len_expt);
    assert_non_null(reg_data_expt);
    expect_memory(bme68x_i2c_write, reg_data, reg_data_expt, len_expt);
    (void)intf_ptr;
    will_return(bme68x_i2c_write, rslt);
}
static int8_t bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    function_called();
    check_expected(reg_addr);
    check_expected(len);
    check_expected(reg_data);
    (void)intf_ptr;
    return mock();
}

static void mock_assert_call_bme68x_delay_us(uint32_t period_expt, void *intf_ptr){
    expect_function_call(bme68x_delay_us);
    expect_value(bme68x_delay_us, period, period_expt);
    (void)intf_ptr;
}
static void bme68x_delay_us(uint32_t period, void *intf_ptr) {
    function_called();
    check_expected(period);
    (void)intf_ptr;
}

static void test_bme680_device_is_plugged(void **state) {
    (void)state;

    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    uint8_t reg_data_expt[] = {BME68X_SOFT_RESET_CMD};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, reg_data_expt, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(10000, NULL);
    uint8_t reg_data_rslt[] = {BME68X_CHIP_ID};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, reg_data_rslt, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_VARIANT_ID, reg_data_rslt, 1, NULL, BME68X_OK);
    uint8_t calib_data_1[BME68X_LEN_COEFF1] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF1, calib_data_1, BME68X_LEN_COEFF1, NULL, BME68X_OK);
    uint8_t calib_data_2[BME68X_LEN_COEFF2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF2, calib_data_2, BME68X_LEN_COEFF2, NULL, BME68X_OK);
    uint8_t calib_data_3[BME68X_LEN_COEFF3] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF3, calib_data_3, BME68X_LEN_COEFF3, NULL, BME68X_OK);
    int8_t rslt = bme68x_init(&sensor);

    assert_int_equal(rslt, BME68X_OK);
    assert_int_equal(sensor.chip_id, BME68X_CHIP_ID);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bme680_device_is_plugged),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}