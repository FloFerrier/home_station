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

#define BME68X_LEN_CHIP_ID (1u)
#define BME68X_LEN_SOFT_RESET_CMD (1u)
#define BME68X_LEN_CTRL_MEAS (1u)
#define BME68X_LEN_RES_HEAT0 (1u)
#define BME68X_LEN_GAS_WAIT0 (1u)
#define BME68X_LEN_IDAC_HEAT0 (1u)
#define BME68X_LEN_CTRL_WRITE (9u)
#define BME68X_LEN_CTRL_GAS_WRITE (3u)
#define BME68X_LEN_CTRL_GAS_READ (2u)

#define BME68X_AMBIANT_TEMPERATURE (25u)

const uint8_t bme680_dev_addr = BME68X_I2C_ADDR_LOW;
const uint8_t bme680_soft_reset_cmd[BME68X_LEN_SOFT_RESET_CMD] = {BME68X_SOFT_RESET_CMD};
const uint8_t bme680_chip_id[BME68X_LEN_CHIP_ID] = {BME68X_CHIP_ID};
const uint8_t bme680_calib_data_1[BME68X_LEN_COEFF1] = {
    0x2f, 0x66, 0x3, 0x10, 0x90, 0x91, 0x91, 0xd6,
    0x58, 0x0, 0x93, 0x18, 0xa, 0x0, 0x32, 0x1e,
    0x0, 0x0, 0x95, 0xf1, 0x88, 0xf7, 0x1e,
};
const uint8_t bme680_calib_data_2[BME68X_LEN_COEFF2] = {
    0x3f, 0x95, 0x2e, 0x0, 0x2d, 0x14, 0x78, 0x9c,
    0x44, 0x66, 0x0, 0xdc, 0xec, 0x12,
};
const uint8_t bme680_calib_data_3[BME68X_LEN_COEFF3] = {
    0x33, 0xaa, 0x16, 0xc9, 0xf3,
};
const uint8_t bme680_sleep_mode[BME68X_LEN_CTRL_MEAS] = {0x0};
const uint8_t bme680_forced_mode[BME68X_LEN_CTRL_MEAS] = {0x1};

static void mock_assert_call_bme68x_i2c_read(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr, int8_t rslt) {
    expect_function_call(bme68x_i2c_read);
    expect_value(bme68x_i2c_read, reg_addr, reg_addr);
    expect_value(bme68x_i2c_read, len, len);
    assert_non_null(reg_data);
    will_return(bme68x_i2c_read, reg_data);
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

static void mock_assert_call_bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr, int8_t rslt) {
    expect_function_call(bme68x_i2c_write);
    expect_value(bme68x_i2c_write, reg_addr, reg_addr);
    expect_value(bme68x_i2c_write, len, len);
    assert_non_null(reg_data);
    expect_memory(bme68x_i2c_write, reg_data, reg_data, len);
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

static void mock_assert_call_bme68x_delay_us(uint32_t period, void *intf_ptr){
    expect_function_call(bme68x_delay_us);
    expect_value(bme68x_delay_us, period, period);
    (void)intf_ptr;
}
static void bme68x_delay_us(uint32_t period, void *intf_ptr) {
    function_called();
    check_expected(period);
    (void)intf_ptr;
}

static int setup(void **state) {
    struct bme68x_dev *sensor = calloc(1, sizeof(struct bme68x_dev));
    uint8_t dev_addr = bme680_dev_addr;
    sensor->intf = BME68X_I2C_INTF;
    sensor->intf_ptr = &dev_addr;
    sensor->delay_us = bme68x_delay_us;
    sensor->read = bme68x_i2c_read;
    sensor->write = bme68x_i2c_write;
    sensor->amb_temp = BME68X_AMBIANT_TEMPERATURE;
    *state = sensor;
    return 0;
}
 
static int teardown(void **state) {
    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    free(sensor);
    return 0;
}

static void helper_bme680_init(void) {
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, (uint8_t*)bme680_soft_reset_cmd, BME68X_LEN_SOFT_RESET_CMD, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, (uint8_t*)bme680_chip_id, BME68X_LEN_CHIP_ID, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_VARIANT_ID, (uint8_t*)bme680_chip_id, BME68X_LEN_CHIP_ID, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF1, (uint8_t*)bme680_calib_data_1, BME68X_LEN_COEFF1, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF2, (uint8_t*)bme680_calib_data_2, BME68X_LEN_COEFF2, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_COEFF3, (uint8_t*)bme680_calib_data_3, BME68X_LEN_COEFF3, NULL, BME68X_OK);
}

static void test_bme680_device_should_be_plugged_and_found(void **state) {
    helper_bme680_init();

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_init(sensor);

    assert_int_equal(rslt, BME68X_OK);
    assert_int_equal(sensor->chip_id, BME68X_CHIP_ID);
}

static void test_bme680_device_should_be_not_plugged(void **state) {
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, (uint8_t*)bme680_soft_reset_cmd, BME68X_LEN_SOFT_RESET_CMD, NULL, BME68X_E_COM_FAIL);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, (uint8_t*)bme680_chip_id, BME68X_LEN_CHIP_ID, NULL, BME68X_E_COM_FAIL);

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_init(sensor);

    assert_int_equal(rslt, BME68X_E_COM_FAIL);
}

static void test_bme680_device_should_be_not_found(void **state) {
    mock_assert_call_bme68x_i2c_write(BME68X_REG_SOFT_RESET, (uint8_t*)bme680_soft_reset_cmd, BME68X_LEN_SOFT_RESET_CMD, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
    uint8_t chip_id[] = {0xff};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CHIP_ID, (uint8_t*)chip_id, BME68X_LEN_CHIP_ID, NULL, BME68X_OK);

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_init(sensor);

    assert_int_equal(rslt, BME68X_E_DEV_NOT_FOUND);
}

static void test_bme680_device_should_be_configured(void **state) {
    helper_bme680_init();

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_init(sensor);

    assert_int_equal(rslt, BME68X_OK);

    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    const uint8_t bme680_config[BME68X_LEN_CONFIG] = {0x0}; // Reset state
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, bme680_config, BME68X_LEN_CONFIG, NULL, BME68X_OK);
    const uint8_t bme680_ctrl[BME68X_LEN_CTRL_WRITE] = {0x80, BME68X_REG_CTRL_HUM, 0x05, 0x73, 0x00, BME68X_REG_CTRL_MEAS, 0x44, BME68X_REG_CONFIG, 0x00};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, bme680_ctrl, BME68X_LEN_CTRL_WRITE, NULL, BME68X_OK);

    struct bme68x_conf conf = {
        .os_temp = BME68X_OS_2X,
        .os_hum = BME68X_OS_16X,
        .os_pres = BME68X_OS_1X,
        .odr = BME68X_ODR_NONE,
        .filter = BME68X_FILTER_OFF,
    };
    rslt = bme68x_set_conf(&conf, sensor);

    assert_int_equal(rslt, BME68X_OK);

    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    const uint8_t bme680_res_heat0[BME68X_LEN_RES_HEAT0] = {0x70};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, bme680_res_heat0, BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
    const uint8_t bme680_gas_wait0[BME68X_LEN_GAS_WAIT0] = {0x59};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, bme680_gas_wait0, BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);
    const uint8_t bme680_ctrl_gas_read[BME68X_LEN_CTRL_GAS_READ] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_read, BME68X_LEN_CTRL_GAS_READ, NULL, BME68X_OK);
    const uint8_t bme680_ctrl_gas_write[BME68X_LEN_CTRL_GAS_WRITE] = {0x0, BME68X_REG_CTRL_GAS_1, 0x10};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_write, BME68X_LEN_CTRL_GAS_WRITE, NULL, BME68X_OK);

    struct bme68x_heatr_conf heatr_conf = {
        .enable = BME68X_ENABLE,
        .heatr_temp = 300,
        .heatr_dur = 100,
    };
    rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, sensor);

    assert_int_equal(rslt, BME68X_OK);
}

static void test_bme680_device_should_be_operated(void **state) {
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_forced_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, sensor);

    assert_int_equal(rslt, BME68X_OK);

    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_forced_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);

    uint8_t op_mode;
    rslt = bme68x_get_op_mode(&op_mode, sensor);

    assert_int_equal(rslt, BME68X_OK);
    assert_int_equal(op_mode, BME68X_FORCED_MODE);
}

static void test_bme680_device_should_be_self_tested(void **state) {
    helper_bme680_init();
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    const uint8_t bme680_res_heat0[][BME68X_LEN_RES_HEAT0] = {{0x7d}, {0x7d}, {0x49}, {0x7d}, {0x49}, {0x7d}, {0x49}};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, (uint8_t*)&(bme680_res_heat0[0]), BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
    const uint8_t bme680_gas_wait0[][BME68X_LEN_GAS_WAIT0] = {{0xbe}, {0xdf}};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, (uint8_t*)&(bme680_gas_wait0[0]), BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);
    const uint8_t bme680_ctrl_gas_read[BME68X_LEN_CTRL_GAS_READ] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_read, BME68X_LEN_CTRL_GAS_READ, NULL, BME68X_OK);
    const uint8_t bme680_ctrl_gas_write[BME68X_LEN_CTRL_GAS_WRITE] = {0x0, BME68X_REG_CTRL_GAS_1, 0x10};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_write, BME68X_LEN_CTRL_GAS_WRITE, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    const uint8_t bme680_config[BME68X_LEN_CONFIG] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, (uint8_t*)bme680_config, BME68X_LEN_CONFIG, NULL, BME68X_OK);
    const uint8_t bme680_ctrl[BME68X_LEN_CTRL_WRITE] = {0x0, BME68X_REG_CTRL_HUM, 0x01, 0x73, 0x00, BME68X_REG_CTRL_MEAS, 0x54, BME68X_REG_CONFIG, 0x00};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, (uint8_t*)bme680_ctrl, BME68X_LEN_CTRL_WRITE, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_forced_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_HEATR_DUR1_DELAY, NULL);
    const uint8_t bme680_field[][BME68X_LEN_FIELD] = {
        {0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x0, 0x0},
        {0x80, 0x0, 0x5a, 0x0, 0x0, 0x7a, 0x0, 0x0, 0x51, 0x00, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0},
        {0x80, 0x0, 0x5a, 0x0, 0x0, 0x7a, 0x0, 0x0, 0x51, 0x00, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0},
        {0x80, 0x0, 0x5a, 0x0, 0x0, 0x7a, 0x0, 0x0, 0x51, 0x00, 0x0, 0x0, 0x0, 0xff, 0x3f, 0x0, 0x0},
        {0x80, 0x0, 0x5a, 0x0, 0x0, 0x7a, 0x0, 0x0, 0x51, 0x00, 0x0, 0x0, 0x0, 0xaa, 0x3f, 0x0, 0x0},
        {0x80, 0x0, 0x5a, 0x0, 0x0, 0x7a, 0x0, 0x0, 0x51, 0x00, 0x0, 0x0, 0x0, 0x11, 0x3f, 0x0, 0x0},
        {0x80, 0x0, 0x5a, 0x0, 0x0, 0x7a, 0x0, 0x0, 0x51, 0x00, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0},
    };
    mock_assert_call_bme68x_i2c_read(BME68X_REG_FIELD0, (uint8_t*)&(bme680_field[0]), BME68X_LEN_FIELD, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_RES_HEAT0, (uint8_t*)&(bme680_res_heat0[0]), BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
    const uint8_t bme680_idac_heat0[BME68X_LEN_IDAC_HEAT0] = {0xaa}; // random number
    mock_assert_call_bme68x_i2c_read(BME68X_REG_IDAC_HEAT0, (uint8_t*)bme680_idac_heat0, BME68X_LEN_IDAC_HEAT0, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_GAS_WAIT0, (uint8_t*)&(bme680_gas_wait0[0]), BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);
    for(uint32_t i=1;i<=BME68X_N_MEAS;i++) {
        mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, (uint8_t*)&(bme680_res_heat0[i]), BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, (uint8_t*)&(bme680_gas_wait0[1]), BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_read, BME68X_LEN_CTRL_GAS_READ, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_write, BME68X_LEN_CTRL_GAS_WRITE, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_forced_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
        mock_assert_call_bme68x_delay_us(BME68X_PERIOD_RESET, NULL);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, (uint8_t*)bme680_config, BME68X_LEN_CONFIG, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, (uint8_t*)bme680_ctrl, BME68X_LEN_CTRL_WRITE, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_forced_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
        mock_assert_call_bme68x_delay_us(BME68X_HEATR_DUR2_DELAY, NULL);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_FIELD0, (uint8_t*)&(bme680_field[i]), BME68X_LEN_FIELD, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_RES_HEAT0, (uint8_t*)&(bme680_res_heat0[i]), BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_IDAC_HEAT0, (uint8_t*)bme680_idac_heat0, BME68X_LEN_IDAC_HEAT0, NULL, BME68X_OK);
        mock_assert_call_bme68x_i2c_read(BME68X_REG_GAS_WAIT0, (uint8_t*)&(bme680_gas_wait0[1]), BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);
    }

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_selftest_check(sensor);

    assert_int_equal(rslt, BME68X_OK);
}

static void test_bme680_device_should_be_not_self_tested(void **state) {
    helper_bme680_init();
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    const uint8_t bme680_res_heat0[BME68X_LEN_RES_HEAT0] = {0x7d};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_RES_HEAT0, (uint8_t*)bme680_res_heat0, BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
    const uint8_t bme680_gas_wait0[BME68X_LEN_GAS_WAIT0] = {0xbe};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_GAS_WAIT0, (uint8_t*)bme680_gas_wait0, BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);
    const uint8_t bme680_ctrl_gas_read[BME68X_LEN_CTRL_GAS_READ] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_read, BME68X_LEN_CTRL_GAS_READ, NULL, BME68X_OK);
    const uint8_t bme680_ctrl_gas_write[BME68X_LEN_CTRL_GAS_WRITE] = {0x0, BME68X_REG_CTRL_GAS_1, 0x10};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_0, (uint8_t*)bme680_ctrl_gas_write, BME68X_LEN_CTRL_GAS_WRITE, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    const uint8_t bme680_config[BME68X_LEN_CONFIG] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_GAS_1, (uint8_t*)bme680_config, BME68X_LEN_CONFIG, NULL, BME68X_OK);
    const uint8_t bme680_ctrl[BME68X_LEN_CTRL_WRITE] = {0x0, BME68X_REG_CTRL_HUM, 0x01, 0x73, 0x00, BME68X_REG_CTRL_MEAS, 0x54, BME68X_REG_CONFIG, 0x00};
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_GAS_1, (uint8_t*)bme680_ctrl, BME68X_LEN_CTRL_WRITE, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_sleep_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_write(BME68X_REG_CTRL_MEAS, (uint8_t*)bme680_forced_mode, BME68X_LEN_CTRL_MEAS, NULL, BME68X_OK);
    mock_assert_call_bme68x_delay_us(BME68X_HEATR_DUR1_DELAY, NULL);
    const uint8_t bme680_field[BME68X_LEN_FIELD] = {0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_FIELD0, (uint8_t*)bme680_field, BME68X_LEN_FIELD, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_RES_HEAT0, (uint8_t*)bme680_res_heat0, BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
    const uint8_t bme680_idac_heat0[BME68X_LEN_IDAC_HEAT0] = {0xaa}; // random number
    mock_assert_call_bme68x_i2c_read(BME68X_REG_IDAC_HEAT0, (uint8_t*)bme680_idac_heat0, BME68X_LEN_IDAC_HEAT0, NULL, BME68X_OK);
    mock_assert_call_bme68x_i2c_read(BME68X_REG_GAS_WAIT0, (uint8_t*)bme680_gas_wait0, BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_selftest_check(sensor);

    assert_int_equal(rslt, BME68X_E_SELF_TEST);
}

static void test_bme680_device_should_be_measured_after_delay(void **state) {
    helper_bme680_init();

    struct bme68x_dev *sensor = (struct bme68x_dev *)*state;
    int8_t rslt = bme68x_init(sensor);

    assert_int_equal(rslt, BME68X_OK);
    assert_int_equal(sensor->chip_id, BME68X_CHIP_ID);

    struct bme68x_conf conf = {
        .os_temp = BME68X_OS_2X,
        .os_hum = BME68X_OS_16X,
        .os_pres = BME68X_OS_1X,
        .odr = BME68X_ODR_NONE,
        .filter = BME68X_FILTER_OFF,
    };

    uint32_t meas_duration_in_us = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, sensor);

    assert_int_equal(meas_duration_in_us, 42590);

    mock_assert_call_bme68x_delay_us(meas_duration_in_us, NULL);
    sensor->delay_us(meas_duration_in_us, NULL);

    const uint8_t bme680_field[BME68X_LEN_FIELD] = {0x80, 0x0, 0x5a, 0x0, 0x0, 0x7a, 0x0, 0x0, 0x51, 0x00, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_FIELD0, (uint8_t*)bme680_field, BME68X_LEN_FIELD, NULL, BME68X_OK);
    const uint8_t bme680_res_heat0[BME68X_LEN_RES_HEAT0] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_RES_HEAT0, (uint8_t*)bme680_res_heat0, BME68X_LEN_RES_HEAT0, NULL, BME68X_OK);
    const uint8_t bme680_idac_heat0[BME68X_LEN_IDAC_HEAT0] = {0xaa}; // random number
    mock_assert_call_bme68x_i2c_read(BME68X_REG_IDAC_HEAT0, (uint8_t*)bme680_idac_heat0, BME68X_LEN_IDAC_HEAT0, NULL, BME68X_OK);
    const uint8_t bme680_gas_wait0[BME68X_LEN_GAS_WAIT0] = {0x0};
    mock_assert_call_bme68x_i2c_read(BME68X_REG_GAS_WAIT0, (uint8_t*)bme680_gas_wait0, BME68X_LEN_GAS_WAIT0, NULL, BME68X_OK);

    struct bme68x_data data = {0};
    uint8_t data_available = 0;
    rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &data_available, sensor);

    assert_int_equal(rslt, BME68X_OK);
    assert_int_equal(data_available, 1);
    assert_float_equal(data.temperature, 25.210f, 1e-3);
    assert_float_equal(data.pressure, 97092.820f, 1e-3);
    assert_float_equal(data.humidity, 45.749f, 1e-3);
    assert_int_equal(data.gas_resistance, 12929781u);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_plugged_and_found, setup, teardown),
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_not_plugged, setup, teardown),
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_not_found, setup, teardown),
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_configured, setup, teardown),
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_operated, setup, teardown),
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_self_tested, setup, teardown),
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_not_self_tested, setup, teardown),
        cmocka_unit_test_setup_teardown(test_bme680_device_should_be_measured_after_delay, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}