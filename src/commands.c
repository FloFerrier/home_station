#include "commands.h"
#include "console.h"

#include "sensor.h"
#include "bme68x.h"

#include <stddef.h>

void command_sensorSelfTest(uint32_t argc, char *argv[]) {
    (void)argc;
    (void)argv;

    int8_t result = sensor_selfTest();
    if(result == BME68X_OK) {
        console_send("(sensor)> Self-test passed !\r\n");
    }
    else {
        console_send("(sensor)> Self-test failed ... %d\r\n", result);
    }
}

void command_sensorGetData(uint32_t argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sensor_data_s data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    uint32_t number_of_data = 0;
    int8_t result = sensor_getData(data, &number_of_data);
    if (result != BME68X_OK) {
        console_send("(sensor)> Get data failed ... %d\r\n", result);
    }
    else {
        console_send("(sensor)> Number of data available %d\r\n", number_of_data);
        for(uint32_t index=0; index<number_of_data; index++) {
            console_send("(sensor)> Temperature %.1f deg, Pressure %.1f hPa, Humidity %.1f %rH\r\n",
                data[index].temperature_in_deg, data[index].pressure_in_pascal / 100.0f, data[index].humidity_in_per100);
        }
    }
}