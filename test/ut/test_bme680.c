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

static void test_bme680_device_is_plugged_and_found(void **state) {
    (void)state;

    uint8_t soft_reset_cmd[] = {BME68X_SOFT_RESET_CMD};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, soft_reset_cmd, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
    uint8_t chip_id[] = {BME68X_CHIP_ID};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, chip_id, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_VARIANT_ID, chip_id, 1, NULL, BME68X_OK);
    uint8_t calib_data_1[BME68X_LEN_COEFF1] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF1, calib_data_1, BME68X_LEN_COEFF1, NULL, BME68X_OK);
    uint8_t calib_data_2[BME68X_LEN_COEFF2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF2, calib_data_2, BME68X_LEN_COEFF2, NULL, BME68X_OK);
    uint8_t calib_data_3[BME68X_LEN_COEFF3] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF3, calib_data_3, BME68X_LEN_COEFF3, NULL, BME68X_OK);
    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    int8_t rslt = bme68x_init(&sensor);

    assert_int_equal(rslt, BME68X_OK);
    assert_int_equal(sensor.chip_id, BME68X_CHIP_ID);
}

static void test_bme680_device_is_not_plugged(void **state) {
    (void)state;

    uint8_t soft_reset_cmd[] = {BME68X_SOFT_RESET_CMD};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, soft_reset_cmd, 1, NULL, BME68X_E_COM_FAIL);
    uint8_t chip_id[] = {BME68X_CHIP_ID};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, chip_id, 1, NULL, BME68X_E_COM_FAIL);
    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    int8_t rslt = bme68x_init(&sensor);

    assert_int_equal(rslt, BME68X_E_COM_FAIL);
}

static void test_bme680_device_is_not_found(void **state) {
    (void)state;

    uint8_t soft_reset_cmd[] = {BME68X_SOFT_RESET_CMD};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, soft_reset_cmd, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
    uint8_t chip_id[] = {0xff};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, chip_id, 1, NULL, BME68X_OK);
    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    int8_t rslt = bme68x_init(&sensor);

    assert_int_equal(rslt, BME68X_E_DEV_NOT_FOUND);
}

static void test_bme680_device_is_configured(void **state) {
    (void)state;

    uint8_t soft_reset_cmd[] = {BME68X_SOFT_RESET_CMD};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, soft_reset_cmd, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
    uint8_t chip_id[] = {BME68X_CHIP_ID};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, chip_id, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_VARIANT_ID, chip_id, 1, NULL, BME68X_OK);
    uint8_t calib_data_1[BME68X_LEN_COEFF1] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF1, calib_data_1, BME68X_LEN_COEFF1, NULL, BME68X_OK);
    uint8_t calib_data_2[BME68X_LEN_COEFF2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF2, calib_data_2, BME68X_LEN_COEFF2, NULL, BME68X_OK);
    uint8_t calib_data_3[BME68X_LEN_COEFF3] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF3, calib_data_3, BME68X_LEN_COEFF3, NULL, BME68X_OK);
    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    int8_t rslt = bme68x_init(&sensor);
    uint8_t ctrl_meas_0[] = {0x0}; // reset state
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_0, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_0, 1, NULL, BME68X_OK);
    uint8_t config[BME68X_LEN_CONFIG] = {0x0}; // reset state
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, config, BME68X_LEN_CONFIG, NULL, BME68X_OK);
    uint8_t data[9] = {0x80, BME68X_REG_CTRL_HUM, 0x05, 0x73, 0x00, BME68X_REG_CTRL_MEAS, 0x44, BME68X_REG_CONFIG, 0x00};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, data, 9, NULL, BME68X_OK);
    struct bme68x_conf conf = {
        .os_temp = BME68X_OS_2X,
        .os_hum = BME68X_OS_16X,
        .os_pres = BME68X_OS_1X,
        .odr = BME68X_ODR_NONE,
        .filter = BME68X_FILTER_OFF,
    };
    rslt = bme68x_set_conf(&conf, &sensor);
    uint8_t ctrl_meas_1[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_1, 1, NULL, BME68X_OK);
    uint8_t res_heat0[] = {0xc7};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, res_heat0, 1, NULL, BME68X_OK);
    uint8_t gas_wait0[] = {0x59};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, gas_wait0, 1, NULL, BME68X_OK);
    uint8_t ctrl_gas_0[] = {0x00, 0x00};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, ctrl_gas_0, 2, NULL, BME68X_OK);
    uint8_t ctrl_gas[] = {0x00, BME68X_REG_CTRL_GAS_1, 0x10};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, ctrl_gas, 3, NULL, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {
        .enable = BME68X_ENABLE,
        .heatr_temp = 300,
        .heatr_dur = 100,
    };
    rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &sensor);

    assert_int_equal(rslt, BME68X_OK);
}

static void test_bme680_device_is_operated(void **state) {
    (void)state;

    uint8_t ctrl_meas_1[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_1, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_2[] = {0x1};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, ctrl_meas_2, 1, NULL, BME68X_OK);
    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    int8_t rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &sensor);

    assert_int_equal(rslt, BME68X_OK);
}

/*static void test_bme680_device_is_self_tested(void **state) {
    (void)state;

    uint8_t soft_reset_cmd[] = {BME68X_SOFT_RESET_CMD};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, soft_reset_cmd, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
    uint8_t chip_id[] = {BME68X_CHIP_ID};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, chip_id, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_VARIANT_ID, chip_id, 1, NULL, BME68X_OK);
    uint8_t calib_data_1[BME68X_LEN_COEFF1] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF1, calib_data_1, BME68X_LEN_COEFF1, NULL, BME68X_OK);
    uint8_t calib_data_2[BME68X_LEN_COEFF2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF2, calib_data_2, BME68X_LEN_COEFF2, NULL, BME68X_OK);
    uint8_t calib_data_3[BME68X_LEN_COEFF3] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF3, calib_data_3, BME68X_LEN_COEFF3, NULL, BME68X_OK);

    uint8_t ctrl_meas_1[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_1, 1, NULL, BME68X_OK);
    uint8_t res_heat0[] = {0xda};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, res_heat0, 1, NULL, BME68X_OK);
    uint8_t gas_wait0[] = {0xbe};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, gas_wait0, 1, NULL, BME68X_OK);
    uint8_t ctrl_gas_0[2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, ctrl_gas_0, 2, NULL, BME68X_OK);
    uint8_t ctrl_gas_1[3] = {0x0, BME68X_REG_CTRL_GAS_1, 0x10};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, ctrl_gas_1, 3, NULL, BME68X_OK);
    uint8_t ctrl_meas_2[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_2, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_3[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_3, 1, NULL, BME68X_OK);
    uint8_t config[BME68X_LEN_CONFIG] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, config, BME68X_LEN_CONFIG, NULL, BME68X_OK);
    uint8_t data[9] = {0x0, BME68X_REG_CTRL_HUM, 0x01, 0x73, 0x00, BME68X_REG_CTRL_MEAS, 0x54, BME68X_REG_CONFIG, 0x00};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, data, 9, NULL, BME68X_OK);
    uint8_t ctrl_meas_4[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_4, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_5[] = {0x1};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, ctrl_meas_5, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_HEATR_DUR1_DELAY, NULL);
    uint8_t field0[BME68X_LEN_FIELD] = {0x80, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x0, 0x0, 0x0, 0x0, 0x30, 0x30, 0x30, 0x30};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_FIELD0, field0, BME68X_LEN_FIELD, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_RES_HEAT0, res_heat0, 1, NULL, BME68X_OK);
    uint8_t idac_heat0[] = {0xaa}; // random number
    mock_assert_call_bme68x_i2c_read(BME68X_REG_IDAC_HEAT0, idac_heat0, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_GAS_WAIT0, gas_wait0, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_6[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_6, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, res_heat0, 1, NULL, BME68X_OK);
    uint8_t gas_wait0_1[] = {0xdf};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, gas_wait0_1, 1, NULL, BME68X_OK);
    uint8_t ctrl_gas_2[2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, ctrl_gas_2, 2, NULL, BME68X_OK);
    uint8_t ctrl_gas_3[3] = {0x0, BME68X_REG_CTRL_GAS_1, 0x10};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, ctrl_gas_3, 3, NULL, BME68X_OK);
    uint8_t ctrl_meas_7[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_7, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_8[] = {0x1};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_8, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_9[] = {0x0};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, ctrl_meas_9, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);

    uint8_t ctrl_meas_10[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_10, 1, NULL, BME68X_OK);
    uint8_t config_1[BME68X_LEN_CONFIG] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, config_1, BME68X_LEN_CONFIG, NULL, BME68X_OK);
    uint8_t data_1[9] = {0x0, BME68X_REG_CTRL_HUM, 0x01, 0x73, 0x00, BME68X_REG_CTRL_MEAS, 0x54, BME68X_REG_CONFIG, 0x00};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, data_1, 9, NULL, BME68X_OK);
    uint8_t ctrl_meas_11[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_11, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_12[] = {0x1};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, ctrl_meas_12, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_HEATR_DUR2_DELAY, NULL);
    uint8_t field0_1[BME68X_LEN_FIELD] = {0x80, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x0, 0x0, 0x0, 0x0, 0x30, 0x30, 0x30, 0x30};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_FIELD0, field0_1, BME68X_LEN_FIELD, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_RES_HEAT0, res_heat0, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_IDAC_HEAT0, idac_heat0, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_GAS_WAIT0, gas_wait0, 1, NULL, BME68X_OK);
    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    int8_t rslt = bme68x_selftest_check(&sensor);

    assert_int_equal(rslt, BME68X_OK);
}*/

static void test_bme680_device_is_not_self_tested(void **state) {
    (void)state;

    uint8_t soft_reset_cmd[] = {BME68X_SOFT_RESET_CMD};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, soft_reset_cmd, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
    uint8_t chip_id[] = {BME68X_CHIP_ID};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, chip_id, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_VARIANT_ID, chip_id, 1, NULL, BME68X_OK);
    uint8_t calib_data_1[BME68X_LEN_COEFF1] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF1, calib_data_1, BME68X_LEN_COEFF1, NULL, BME68X_OK);
    uint8_t calib_data_2[BME68X_LEN_COEFF2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF2, calib_data_2, BME68X_LEN_COEFF2, NULL, BME68X_OK);
    uint8_t calib_data_3[BME68X_LEN_COEFF3] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF3, calib_data_3, BME68X_LEN_COEFF3, NULL, BME68X_OK);
    uint8_t ctrl_meas_1[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_1, 1, NULL, BME68X_OK);
    uint8_t res_heat_0[] = {0xda};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, res_heat_0, 1, NULL, BME68X_OK);
    uint8_t gas_wait_0[] = {0xbe};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, gas_wait_0, 1, NULL, BME68X_OK);
    uint8_t ctrl_gas_0[2] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, ctrl_gas_0, 2, NULL, BME68X_OK);
    uint8_t ctrl_gas[3] = {0x0, BME68X_REG_CTRL_GAS_1, 0x10};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, ctrl_gas, 3, NULL, BME68X_OK);
    uint8_t ctrl_meas_2[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_2, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_3[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_3, 1, NULL, BME68X_OK);
    uint8_t config[BME68X_LEN_CONFIG] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, config, BME68X_LEN_CONFIG, NULL, BME68X_OK);
    uint8_t data[9] = {0x0, BME68X_REG_CTRL_HUM, 0x01, 0x73, 0x00, BME68X_REG_CTRL_MEAS, 0x54, BME68X_REG_CONFIG, 0x00};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, data, 9, NULL, BME68X_OK);
    uint8_t ctrl_meas_4[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, ctrl_meas_4, 1, NULL, BME68X_OK);
    uint8_t ctrl_meas_5[] = {0x1};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, ctrl_meas_5, 1, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_HEATR_DUR1_DELAY, NULL);
    uint8_t field0[BME68X_LEN_FIELD] = {0x80};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_FIELD0, field0, BME68X_LEN_FIELD, NULL, BME68X_OK);
    uint8_t res_heat0[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_RES_HEAT0, res_heat0, 1, NULL, BME68X_OK);
    uint8_t idac_heat0[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_IDAC_HEAT0, idac_heat0, 1, NULL, BME68X_OK);
    uint8_t gas_wait0[] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_GAS_WAIT0, gas_wait0, 1, NULL, BME68X_OK);
    struct bme68x_dev sensor = {
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    int8_t rslt = bme68x_selftest_check(&sensor);

    assert_int_equal(rslt, BME68X_E_SELF_TEST);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_bme680_device_is_plugged_and_found),
        cmocka_unit_test(test_bme680_device_is_not_plugged),
        cmocka_unit_test(test_bme680_device_is_not_found),
        cmocka_unit_test(test_bme680_device_is_configured),
        cmocka_unit_test(test_bme680_device_is_operated),
        //cmocka_unit_test(test_bme680_device_is_self_tested),
        cmocka_unit_test(test_bme680_device_is_not_self_tested),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}