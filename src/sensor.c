#include "sensor.h"

#include <bme68x.h>

#ifndef TEST
#include <stm32f4xx_hal.h>
#define STATIC static
#else
#include "mock_stm32f4x.h"
#define STATIC
#endif // TEST

#define BME68X_I2C_TIMEOUT (1000u)
#define BME68X_I2C_MEM_ADDR_SIZE (1u)
    
#define BME68X_AMBIANT_TEMPERATURE (25u)

STATIC I2C_HandleTypeDef i2c_handle = {0};
STATIC struct bme68x_dev sensor = {0};

STATIC const uint8_t BME68X_DEV_ADDR = BME68X_I2C_ADDR_LOW;

STATIC sensor_returnCode_e convertBme68xToSensorReturnCode(int8_t bme68x_return_code) {
    sensor_returnCode_e sensor_return_code;
    switch (bme68x_return_code) {
    case BME68X_OK:
        sensor_return_code = SENSOR_OK;
        break;
    case BME68X_E_NULL_PTR:
        sensor_return_code = SENSOR_NULL_POINTER;
        break;
    case BME68X_E_COM_FAIL:
        sensor_return_code = SENSOR_I2C_FAILURE;
        break;
    case BME68X_E_DEV_NOT_FOUND:
        sensor_return_code = SENSOR_NOT_FOUND;
        break;
    case BME68X_E_INVALID_LENGTH:
        sensor_return_code = SENSOR_INVALID_PARAM;
        break;
    case BME68X_E_SELF_TEST:
        sensor_return_code = SENSOR_SELF_TEST_FAILURE;
        break;
    default:
        sensor_return_code = SENSOR_MISC_FAILURE;
        break;
    }
    return sensor_return_code;
}

STATIC int8_t bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    if((reg_data == NULL) || (intf_ptr == NULL)) {
        return BME68X_E_NULL_PTR;
    }
    uint16_t *DevAddress = (uint16_t*)intf_ptr;
    HAL_StatusTypeDef result =  HAL_I2C_Mem_Read(&i2c_handle, *DevAddress << 1, (uint16_t )reg_addr, BME68X_I2C_MEM_ADDR_SIZE, (uint8_t*)reg_data, len, BME68X_I2C_TIMEOUT);
    if(result != HAL_OK) {
        return BME68X_E_COM_FAIL;
    }
    return BME68X_OK;
}

STATIC int8_t bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    if((reg_data == NULL) || (intf_ptr == NULL)) {
        return BME68X_E_NULL_PTR;
    }
    uint16_t *DevAddress = (uint16_t*)intf_ptr;
    HAL_StatusTypeDef result =  HAL_I2C_Mem_Write(&i2c_handle, *DevAddress << 1, (uint16_t )reg_addr, BME68X_I2C_MEM_ADDR_SIZE, (uint8_t*)reg_data, len, BME68X_I2C_TIMEOUT);
    if(result != HAL_OK) {
        return BME68X_E_COM_FAIL;
    }
    return BME68X_OK;
}

STATIC void bme68x_delay_us(uint32_t period, void *intf_ptr) {
    (void)intf_ptr;
    uint32_t delay_in_ms = period / 1000u;
    HAL_Delay(delay_in_ms);
}

sensor_returnCode_e sensor_init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /* I2C1 Configuration
        PB8 ------> I2C1_SCL
        PB9 ------> I2C1_SDA
    */
    GPIO_InitTypeDef gpio_init = {
        .Pin = GPIO_PIN_8 | GPIO_PIN_9,
        .Mode = GPIO_MODE_AF_OD,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF4_I2C1,
    };
    HAL_GPIO_Init(GPIOB, &gpio_init);

    __HAL_RCC_I2C1_CLK_ENABLE();
    i2c_handle.Instance = I2C1;
    i2c_handle.Init.ClockSpeed = 100000;
    i2c_handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
    i2c_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    i2c_handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_StatusTypeDef hal_rslt = HAL_I2C_Init(&i2c_handle);
    if(hal_rslt != HAL_OK) {
        return SENSOR_I2C_FAILURE;
    }

    sensor.intf = BME68X_I2C_INTF;
    sensor.intf_ptr = (uint8_t*)&BME68X_DEV_ADDR;
    sensor.delay_us = bme68x_delay_us;
    sensor.read = bme68x_i2c_read;
    sensor.write = bme68x_i2c_write;
    sensor.amb_temp = BME68X_AMBIANT_TEMPERATURE;
    int8_t bme68x_rslt = bme68x_init(&sensor);
    return convertBme68xToSensorReturnCode(bme68x_rslt);
}

sensor_returnCode_e sensor_selfTest(void) {
    int8_t bme68x_rslt = bme68x_selftest_check(&sensor);
    return convertBme68xToSensorReturnCode(bme68x_rslt);
}

sensor_returnCode_e sensor_getData(sensor_data_s *data, uint32_t *number_of_data) {
    if((data == NULL) || (number_of_data == NULL)) {
        return SENSOR_NULL_POINTER;
    }

    struct bme68x_conf conf = {
        .os_temp = BME68X_OS_2X,
        .os_hum = BME68X_OS_16X,
        .os_pres = BME68X_OS_1X,
        .odr = BME68X_ODR_NONE,
        .filter = BME68X_FILTER_OFF,
    };
    int8_t bme68x_rslt = bme68x_set_conf(&conf, &sensor);
    if(bme68x_rslt != BME68X_OK) {
        return convertBme68xToSensorReturnCode(bme68x_rslt);
    }

    struct bme68x_heatr_conf heatr_conf = {
        .enable = BME68X_ENABLE,
        .heatr_temp = 300,
        .heatr_dur = 100,
    };
    bme68x_rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &sensor);
    if(bme68x_rslt != BME68X_OK) {
        return convertBme68xToSensorReturnCode(bme68x_rslt);
    }

    bme68x_rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &sensor);
    if(bme68x_rslt != BME68X_OK) {
        return convertBme68xToSensorReturnCode(bme68x_rslt);
    }

    uint32_t delay_in_us = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &sensor);
    sensor.delay_us(delay_in_us, (uint8_t*)&BME68X_DEV_ADDR);

    struct bme68x_data bme68x_data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint8_t bme68x_n_data = 0;
    bme68x_rslt = bme68x_get_data(BME68X_FORCED_MODE, bme68x_data, &bme68x_n_data, &sensor);
    if(bme68x_rslt == BME68X_OK) {
        *number_of_data = bme68x_n_data;
        for(uint32_t index=0; index<bme68x_n_data; index++) {
            data[index].temperature_in_deg = bme68x_data[index].temperature;
            data[index].pressure_in_pascal = bme68x_data[index].pressure;
            data[index].humidity_in_per100 = bme68x_data[index].humidity;
            data[index].gas_resistance_in_ohms = bme68x_data[index].gas_resistance;
        }
    }
    return convertBme68xToSensorReturnCode(bme68x_rslt);
}
