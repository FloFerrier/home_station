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
#include "mock_stm32f4x.h"

#include "sensor.h"

#define BME68X_AMBIANT_TEMPERATURE (25u)

extern const uint8_t BME68X_DEV_ADDR;

extern int8_t bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) ;
extern int8_t bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);
extern void bme68x_delay_us(uint32_t period, void *intf_ptr);

static void test_delay_interface_should_worked(void **state) {
    (void) state;

    const uint32_t DELAY_IN_MS = 10u;
    mock_assert_call_HAL_Delay(DELAY_IN_MS);

    const uint32_t DELAY_IN_US = DELAY_IN_MS * 1000u;
    bme68x_delay_us(DELAY_IN_US, NULL);
}

static void test_i2c_interface_should_failed_on_read(void **state) {
    (void) state;

    const uint16_t expected_dev_addr = (0x76 << 1);
    const uint16_t mem_addr = 0x0a;
    const uint16_t mem_addr_len = 1u;
    const uint16_t read_data_len = 4u;
    const uint8_t expected_read_data[] = {0xaa, 0xbb, 0xcc, 0xdd};
    const uint32_t expected_i2c_timeout = 1000u;
    mock_assert_call_HAL_I2C_Mem_Read(expected_dev_addr, mem_addr, mem_addr_len, expected_read_data, read_data_len, expected_i2c_timeout, HAL_ERROR);

    uint8_t obtained_read_data[4] = {0};
    int8_t result = bme68x_i2c_read(mem_addr, obtained_read_data, read_data_len, (uint8_t*)&BME68X_DEV_ADDR); 

    assert_int_equal(result, BME68X_E_COM_FAIL);
}

static void test_i2c_interface_should_read_several_data(void **state) {
    (void) state;

    const uint16_t expected_dev_addr = (0x76 << 1);
    const uint16_t mem_addr = 0x0a;
    const uint16_t mem_addr_len = 1u;
    const uint16_t read_data_len = 4u;
    const uint8_t expected_read_data[] = {0xaa, 0xbb, 0xcc, 0xdd};
    const uint32_t expected_i2c_timeout = 1000u;
    mock_assert_call_HAL_I2C_Mem_Read(expected_dev_addr, mem_addr, mem_addr_len, expected_read_data, read_data_len, expected_i2c_timeout, HAL_OK);

    uint8_t obtained_read_data[4] = {0};
    int8_t result = bme68x_i2c_read(mem_addr, obtained_read_data, read_data_len, (uint8_t*)&BME68X_DEV_ADDR); 

    assert_int_equal(result, BME68X_OK);
    assert_memory_equal(obtained_read_data, expected_read_data, read_data_len);
}

static void test_i2c_interface_should_failed_on_write(void **state) {
    (void) state;

    const uint16_t expected_dev_addr = (0x76 << 1);
    const uint16_t mem_addr = 0x0a;
    const uint16_t mem_addr_len = 1u;
    const uint16_t write_data_len = 4u;
    const uint8_t expected_write_data[] = {0xaa, 0xbb, 0xcc, 0xdd};
    const uint32_t expected_i2c_timeout = 1000u;
    mock_assert_call_HAL_I2C_Mem_Write(expected_dev_addr, mem_addr, mem_addr_len, expected_write_data, write_data_len, expected_i2c_timeout, HAL_ERROR);

    const uint8_t write_data[] = {0xaa, 0xbb, 0xcc, 0xdd};
    int8_t result = bme68x_i2c_write(mem_addr, write_data, write_data_len, (uint8_t*)&BME68X_DEV_ADDR);

    assert_int_equal(result, BME68X_E_COM_FAIL);
}

static void test_i2c_interface_should_write_several_data(void **state) {
    (void) state;

    const uint16_t expected_dev_addr = (0x76 << 1);
    const uint16_t mem_addr = 0x0a;
    const uint16_t mem_addr_len = 1u;
    const uint16_t write_data_len = 4u;
    const uint8_t expected_write_data[] = {0xaa, 0xbb, 0xcc, 0xdd};
    const uint32_t expected_i2c_timeout = 1000u;
    mock_assert_call_HAL_I2C_Mem_Write(expected_dev_addr, mem_addr, mem_addr_len, expected_write_data, write_data_len, expected_i2c_timeout, HAL_OK);

    const uint8_t write_data[] = {0xaa, 0xbb, 0xcc, 0xdd};
    int8_t result = bme68x_i2c_write(mem_addr, write_data, write_data_len, (uint8_t*)&BME68X_DEV_ADDR);

    assert_int_equal(result, BME68X_OK);
}

static void test_sensor_should_failed_on_i2c_init(void **state) {
    (void)state;

    mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE();
    mock_assert_call_HAL_GPIO_Init();
    mock_assert_call_HAL_RCC_I2C1_CLK_ENABLE();
    mock_assert_call_HAL_I2C_Init(HAL_ERROR);

    sensor_returnCode_e rslt = sensor_init();

    assert_int_equal(rslt, SENSOR_I2C_FAILURE);
}

static void test_sensor_should_failed_on_device_init(void **state) {
    (void)state;

    mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE();
    mock_assert_call_HAL_GPIO_Init();
    mock_assert_call_HAL_RCC_I2C1_CLK_ENABLE();
    mock_assert_call_HAL_I2C_Init(HAL_OK);
    struct bme68x_dev dev = {0};
    mock_assert_call_bme68x_init(&dev, BME68X_E_DEV_NOT_FOUND);

    sensor_returnCode_e rslt = sensor_init();

    assert_int_equal(rslt, SENSOR_NOT_FOUND);
}

static void test_sensor_should_be_initialized(void **state) {
    (void)state;

    mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE();
    mock_assert_call_HAL_GPIO_Init();
    mock_assert_call_HAL_RCC_I2C1_CLK_ENABLE();
    mock_assert_call_HAL_I2C_Init(HAL_OK);
    struct bme68x_dev dev;
    mock_assert_call_bme68x_init(&dev, BME68X_OK);

    sensor_returnCode_e rslt = sensor_init();

    assert_int_equal(rslt, SENSOR_OK);
}

static void test_sensor_should_failed_selftest(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_E_SELF_TEST);

    sensor_returnCode_e rslt = sensor_selfTest();

    assert_int_equal(rslt, SENSOR_SELF_TEST_FAILURE);
}

static void test_sensor_should_be_selftested(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    mock_assert_call_bme68x_selftest_check(&dev, BME68X_OK);

    sensor_returnCode_e rslt = sensor_selfTest();

    assert_int_equal(rslt, SENSOR_OK);
}

static void test_sensor_should_have_invalid_parameters(void **state) {
    (void)state;

    sensor_returnCode_e rslt = sensor_getData(NULL, 0);

    assert_int_equal(rslt, SENSOR_NULL_POINTER);
}

static void test_sensor_should_failed_set_conf(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_I_PARAM_CORR);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    sensor_returnCode_e rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, SENSOR_MISC_FAILURE);
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
    sensor_returnCode_e rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, SENSOR_MISC_FAILURE);
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
    sensor_returnCode_e rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, SENSOR_MISC_FAILURE);
}

static void test_sensor_should_no_available_data(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    const uint32_t DELAY_IN_US = 10000u;
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, DELAY_IN_US);
    const uint32_t DELAY_IN_MS = DELAY_IN_US/1000;
    mock_assert_call_HAL_Delay(DELAY_IN_MS);
    struct bme68x_data bme68x_data = {0};
    uint8_t bme68x_n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &bme68x_data, bme68x_n_data, &dev, BME68X_W_NO_NEW_DATA);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    sensor_returnCode_e rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, SENSOR_MISC_FAILURE);
}

static void test_sensor_should_failed_get_data(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    const uint32_t DELAY_IN_US = 10000u;
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, DELAY_IN_US);
    const uint32_t DELAY_IN_MS = DELAY_IN_US/1000;
    mock_assert_call_HAL_Delay(DELAY_IN_MS);
    struct bme68x_data bme68x_data = {0};
    uint8_t bme68x_n_data = 0;
    mock_assert_call_bme68x_get_data(BME68X_FORCED_MODE, &bme68x_data, bme68x_n_data, &dev, BME68X_E_COM_FAIL);

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    sensor_returnCode_e rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, SENSOR_I2C_FAILURE);
}

static void test_sensor_should_get_several_data(void **state) {
    (void)state;

    struct bme68x_dev dev = {0};
    struct bme68x_conf conf = {0};
    mock_assert_call_bme68x_set_conf(&conf, &dev, BME68X_OK);
    struct bme68x_heatr_conf heatr_conf = {0};
    mock_assert_call_bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev, BME68X_OK);
    mock_assert_call_bme68x_set_op_mode(BME68X_FORCED_MODE, &dev, BME68X_OK);
    const uint32_t DELAY_IN_US = 10000u;
    mock_assert_call_bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev, DELAY_IN_US);
    const uint32_t DELAY_IN_MS = DELAY_IN_US/1000;
    mock_assert_call_HAL_Delay(DELAY_IN_MS);
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
    sensor_returnCode_e rslt = sensor_getData(data, &number_of_data);

    assert_int_equal(rslt, SENSOR_OK);
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
        cmocka_unit_test(test_delay_interface_should_worked),
        cmocka_unit_test(test_i2c_interface_should_failed_on_read),
        cmocka_unit_test(test_i2c_interface_should_read_several_data),
        cmocka_unit_test(test_i2c_interface_should_failed_on_write),
        cmocka_unit_test(test_i2c_interface_should_write_several_data),
        cmocka_unit_test(test_sensor_should_failed_on_i2c_init),
        cmocka_unit_test(test_sensor_should_failed_on_device_init),
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