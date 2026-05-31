/**
 * @file
 */

#include "sampling.h"

#include "FreeRTOS.h"
#include <stm32f4xx_hal.h>
#include "console.h"
#include "bme680.h"
#include "ina219.h"
#include "task.h"

I2C_HandleTypeDef i2c_handle = {0};

static int ina219_write(uint8_t addr, const uint8_t *data, size_t len) {
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&i2c_handle, addr << 1, (uint8_t*) data, (uint16_t) len, 1000);
    if (status == HAL_OK) {
        return 0;
    }
    return -1;
}

static int ina219_read(uint8_t addr, uint8_t *data, size_t len) {
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&i2c_handle, addr << 1, data, (uint16_t) len, 1000);
    if (status == HAL_OK) {
        return 0;
    }
    return -1;
}

void sampling_task(void *params) {
    (void)params;

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
    (void) HAL_I2C_Init(&i2c_handle);

    bme680_returnCode_e bme680_status = bme680_init(&i2c_handle);
    console_send("[Sampling] Init BME680 : %s\r\n",
                 bme680_returnCodeAsString(bme680_status));

    ina219_t ina219_dev = {
        .read = ina219_read,
        .write = ina219_write,
        .current_lsb = 0.0f,
    };
    /* INA219's configuration is per default */
    ina219_status_e ina219_status = ina219_set_calibration(&ina219_dev, 3.2f, 0.1f);
    console_send("[Sampling] INA219 Set calibration status : 0x%X\r\n", ina219_status);

    if (bme680_status == BME680_OK) {
            bme680_status =
        bme680_selfTest();  // Wait the end of self-test, can be a little longer
        console_send("[Sampling] Self-test on BME680 : %s\r\n",
                    bme680_returnCodeAsString(bme680_status));
    }

    static bme680_data_s bme680_data[BME680_MAX_DATA_AVAILABLE] = {{0}};
    static uint32_t bme680_data_number = 0;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(4000));

        bme680_status = bme680_getData(bme680_data, &bme680_data_number);
        if (bme680_status != BME680_OK) {
            console_send("[Sampling] BME680 Fail to get data : %s\r\n", bme680_returnCodeAsString(bme680_status));
        }

        if ((bme680_status == BME680_OK) && (bme680_data_number > 0)) {
            console_send("[Sampling] BME680 Temperature %.2f degC\r\n", bme680_data[0].temperature_in_deg);
            console_send("[Sampling] BME680 Relative humidity %.0f per100\r\n", bme680_data[0].humidity_in_per100);
        }

        uint32_t volt = 0;
        ina219_status = ina219_read_bus_voltage(&ina219_dev, &volt);
        if (ina219_status != INA219_SUCCESS) {
            console_send("[Sampling] INA219 Fail to read bus voltage : 0x%X\r\n", ina219_status);
        }
        else {
            console_send("[Sampling] INA219 Bus voltage %ld V\r\n", volt);
        }

        float current = 0.0f;
        ina219_status = ina219_read_current(&ina219_dev, &current);
        if (ina219_status != INA219_SUCCESS) {
            console_send("[Sampling] INA219 Fail to read current : 0x%X\r\n", ina219_status);
        }
        else {
            console_send("[Sampling] INA219 Current %.3f A\r\n", current);
        }
    }
}