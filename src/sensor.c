#include "sensor.h"
#include "console.h"

#include <bme68x.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif // TEST

STATIC struct bme68x_dev dev = {0};

bool sensor_init(void) {
    int8_t bme68x_rslt = bme68x_init(&dev);
    if(bme68x_rslt != BME68X_OK) {
        return false;
    }
    return true;
}

bool sensor_selfTest(void) {
    int8_t bme68x_rslt = bme68x_selftest_check(&dev);
    if(bme68x_rslt != BME68X_OK) {
        return false;
    }
    return true;
}

bool sensor_getData(void) {
    struct bme68x_conf conf = {0};
    int8_t bme68x_rslt = bme68x_set_conf(&conf, &dev);
    if(bme68x_rslt != BME68X_OK) {
        return false;
    }

    struct bme68x_heatr_conf heatr_conf = {0};
    bme68x_rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &dev);
    if(bme68x_rslt != BME68X_OK) {
        return false;
    }

    bme68x_rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &dev);
    if(bme68x_rslt != BME68X_OK) {
        return false;
    }

    uint32_t delay_in_us = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &dev);
    console_send("(sensor)> Wait %d us before get data\r\n", delay_in_us);

    struct bme68x_data data = {0};
    uint8_t n_data = 0;
    bme68x_rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_data, &dev);
    switch (bme68x_rslt)
    {
    case BME68X_OK: {
        console_send("(sensor)> %d data available\r\n", n_data);
        console_send("(sensor)> %.1f°C %.1f%Rh %.1fhPa\r\n", (float)data.temperature, (float)data.humidity, (float)data.pressure/100.0f);
    }
        break;
    case BME68X_W_NO_NEW_DATA : {
        console_send("(sensor)> No new data available\r\n");
        return false;
    }
        break;
    default: {
        console_send("(sensor)> Error to get data %d\r\n", bme68x_rslt);
        return false;
    }
        break;
    }
    return true;
}
