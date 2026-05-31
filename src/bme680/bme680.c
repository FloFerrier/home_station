/**
 * @file
 */

#include "bme680.h"

#include <bme68x.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif  // TEST

#define BME68X_I2C_TIMEOUT (1000u)
#define BME68X_I2C_MEM_ADDR_SIZE (1u)

#define BME68X_AMBIANT_TEMPERATURE (25u)

#define BME680_RETURN_CODE_LEN_MAX (255)

STATIC I2C_HandleTypeDef *i2c_handle = NULL;
STATIC struct bme68x_dev bme680 = {0};

STATIC const uint8_t BME68X_DEV_ADDR = BME68X_I2C_ADDR_LOW;

STATIC const char return_code[][BME680_RETURN_CODE_LEN_MAX + 1] = {
    [BME680_OK] = "BME680 ok",
    [BME680_NULL_POINTER] = "BME680 null pointer",
    [BME680_I2C_FAILURE] = "BME680 i2c failure",
    [BME680_NOT_FOUND] = "BME680 not found",
    [BME680_INVALID_PARAM] = "BME680 invalid param",
    [BME680_SELF_TEST_FAILURE] = "BME680 self test failure",
    [BME680_MISC_FAILURE] = "BME680 misc failure",
};

STATIC bme680_returnCode_e
convertBme68xToSensorReturnCode(int8_t bme68x_return_code) {
    bme680_returnCode_e return_code;
    switch (bme68x_return_code) {
        case BME68X_OK:
            return_code = BME680_OK;
            break;
        case BME68X_E_NULL_PTR:
            return_code = BME680_NULL_POINTER;
            break;
        case BME68X_E_COM_FAIL:
            return_code = BME680_I2C_FAILURE;
            break;
        case BME68X_E_DEV_NOT_FOUND:
            return_code = BME680_NOT_FOUND;
            break;
        case BME68X_E_INVALID_LENGTH:
            return_code = BME680_INVALID_PARAM;
            break;
        case BME68X_E_SELF_TEST:
            return_code = BME680_SELF_TEST_FAILURE;
            break;
        default:
            return_code = BME680_MISC_FAILURE;
            break;
    }
    return return_code;
}

STATIC int8_t bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len,
                              void *intf_ptr) {
    if ((reg_data == NULL) || (intf_ptr == NULL)) {
        return BME68X_E_NULL_PTR;
    }
    uint16_t *DevAddress = (uint16_t *)intf_ptr;
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(
        i2c_handle, *DevAddress << 1, (uint16_t)reg_addr,
        BME68X_I2C_MEM_ADDR_SIZE, (uint8_t *)reg_data, len, BME68X_I2C_TIMEOUT);
    if (result != HAL_OK) {
        return BME68X_E_COM_FAIL;
    }
    return BME68X_OK;
}

STATIC int8_t bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                               uint32_t len, void *intf_ptr) {
    if ((reg_data == NULL) || (intf_ptr == NULL)) {
        return BME68X_E_NULL_PTR;
    }
    uint16_t *DevAddress = (uint16_t *)intf_ptr;
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(
        i2c_handle, *DevAddress << 1, (uint16_t)reg_addr,
        BME68X_I2C_MEM_ADDR_SIZE, (uint8_t *)reg_data, len, BME68X_I2C_TIMEOUT);
    if (result != HAL_OK) {
        return BME68X_E_COM_FAIL;
    }
    return BME68X_OK;
}

STATIC void bme68x_delay_us(uint32_t period, void *intf_ptr) {
    (void)intf_ptr;
    uint32_t delay_in_ms = period / 1000u;
    HAL_Delay(delay_in_ms);
}

bme680_returnCode_e bme680_init(I2C_HandleTypeDef *i2c_handle) {
    i2c_handle = i2c_handle;
    bme680.intf = BME68X_I2C_INTF;
    bme680.intf_ptr = (uint8_t *)&BME68X_DEV_ADDR;
    bme680.delay_us = bme68x_delay_us;
    bme680.read = bme68x_i2c_read;
    bme680.write = bme68x_i2c_write;
    bme680.amb_temp = BME68X_AMBIANT_TEMPERATURE;
    int8_t bme68x_rslt = bme68x_init(&bme680);
    return convertBme68xToSensorReturnCode(bme68x_rslt);
}

bme680_returnCode_e bme680_selfTest(void) {
    int8_t bme68x_rslt = bme68x_selftest_check(&bme680);
    return convertBme68xToSensorReturnCode(bme68x_rslt);
}

bme680_returnCode_e bme680_getData(bme680_data_s *data,
                                   uint32_t *number_of_data) {
    if ((data == NULL) || (number_of_data == NULL)) {
        return BME680_NULL_POINTER;
    }

    struct bme68x_conf conf = {
        .os_temp = BME68X_OS_2X,
        .os_hum = BME68X_OS_16X,
        .os_pres = BME68X_OS_1X,
        .odr = BME68X_ODR_NONE,
        .filter = BME68X_FILTER_OFF,
    };
    int8_t bme68x_rslt = bme68x_set_conf(&conf, &bme680);
    if (bme68x_rslt != BME68X_OK) {
        return convertBme68xToSensorReturnCode(bme68x_rslt);
    }

    struct bme68x_heatr_conf heatr_conf = {
        .enable = BME68X_ENABLE,
        .heatr_temp = 300,
        .heatr_dur = 100,
    };
    bme68x_rslt =
        bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme680);
    if (bme68x_rslt != BME68X_OK) {
        return convertBme68xToSensorReturnCode(bme68x_rslt);
    }

    bme68x_rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme680);
    if (bme68x_rslt != BME68X_OK) {
        return convertBme68xToSensorReturnCode(bme68x_rslt);
    }

    uint32_t delay_in_us =
        bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme680);
    bme680.delay_us(delay_in_us, (uint8_t *)&BME68X_DEV_ADDR);

    struct bme68x_data bme68x_data[BME680_MAX_DATA_AVAILABLE] = {{0}};
    uint8_t bme68x_n_data = 0;
    bme68x_rslt = bme68x_get_data(BME68X_FORCED_MODE, bme68x_data,
                                  &bme68x_n_data, &bme680);
    if (bme68x_rslt == BME68X_OK) {
        *number_of_data = bme68x_n_data;
        for (uint32_t index = 0; index < bme68x_n_data; index++) {
            data[index].temperature_in_deg = bme68x_data[index].temperature;
            data[index].pressure_in_pascal = bme68x_data[index].pressure;
            data[index].humidity_in_per100 = bme68x_data[index].humidity;
            data[index].gas_resistance_in_ohms =
                bme68x_data[index].gas_resistance;
        }
    }
    return convertBme68xToSensorReturnCode(bme68x_rslt);
}

char *bme680_returnCodeAsString(bme680_returnCode_e code) {
    if (code > BME680_MISC_FAILURE) {
        code = BME680_MISC_FAILURE;
    }
    return (char *)return_code[code];
}
