/**
 * @file
 */

#include "sampling.h"

#include "FreeRTOS.h"
#include "console.h"
#include "sensor.h"
#include "task.h"

void sampling_task(void *params) {
    (void)params;

    sensor_returnCode_e status = sensor_init();
    console_send("[Sampling] Init sensor : %s\r\n",
                 sensor_returnCodeAsString(status));

    status =
        sensor_selfTest();  // Wait the end of self-test, can be a little longer
    console_send("[Sampling] Self-test on sensor : %s\r\n",
                 sensor_returnCodeAsString(status));

    static sensor_data_s sensor_data[SENSOR_MAX_DATA_AVAILABLE] = {{0}};
    static uint32_t sensor_data_number = 0;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1 second
        status = sensor_getData(sensor_data, &sensor_data_number);
        // console_send("[Sampling] Sensor get data : %s\r\n",
        // sensor_returnCodeAsString(status));
        if ((status == SENSOR_OK) && (sensor_data_number > 0)) {
            // console_send("[Sampling] Temperature %.2f degC\r\n",
            // sensor_data[0].temperature_in_deg); console_send("[Sampling]
            // Relative humidity %.0f per100\r\n",
            // sensor_data[0].humidity_in_per100);
        }
    }
}