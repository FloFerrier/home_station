#include "sensor.h"

#include <bme68x.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif // TEST

STATIC struct bme68x_dev dev = {0};

int8_t sensor_init(void) {
    return (int8_t)bme68x_init(&dev);
}

int8_t sensor_selfTest(void) {
    return (int8_t)bme68x_selftest_check(&dev);
}

int8_t sensor_getData(sensor_data_s *data, uint32_t *number_of_data) {
    int8_t bme68x_rslt = BME68X_OK;
    if((data == NULL) || (number_of_data == NULL)) {
        bme68x_rslt = BME68X_E_NULL_PTR;
        return bme68x_rslt;
    }

    struct bme68x_conf conf = {0};
    bme68x_rslt = bme68x_set_conf(&conf, &dev);
    if(bme68x_rslt != BME68X_OK) {
        return bme68x_rslt;
    }

    struct bme68x_heatr_conf heatr_conf = {0};
    bme68x_rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev);
    if(bme68x_rslt != BME68X_OK) {
        return bme68x_rslt;
    }

    bme68x_rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &dev);
    if(bme68x_rslt != BME68X_OK) {
        return bme68x_rslt;
    }

    uint32_t delay_in_us = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev);
    /* Todo: Add delay function here with API */

    struct bme68x_data bme68x_data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint8_t bme68x_n_data = 0;
    bme68x_rslt = bme68x_get_data(BME68X_FORCED_MODE, bme68x_data, &bme68x_n_data, &dev);
    if(bme68x_rslt == BME68X_OK) {
        *number_of_data = bme68x_n_data;
        for(uint32_t index=0; index<bme68x_n_data; index++) {
            data[index].temperature_in_deg = bme68x_data[index].temperature;
            data[index].pressure_in_pascal = bme68x_data[index].pressure;
            data[index].humidity_in_per100 = bme68x_data[index].humidity;
            data[index].gas_resistance_in_ohms = bme68x_data[index].gas_resistance;
        }
    }

    return bme68x_rslt;
}
