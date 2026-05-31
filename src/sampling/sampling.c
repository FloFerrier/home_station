/**
 * @file
 */

#include "sampling.h"

#include "FreeRTOS.h"
#include "console.h"
#include "bme680.h"
#include "task.h"

void sampling_task(void *params) {
    (void)params;

    bme680_returnCode_e status = bme680_init();
    console_send("[Sampling] Init BME680 : %s\r\n",
                 bme680_returnCodeAsString(status));

    status =
        bme680_selfTest();  // Wait the end of self-test, can be a little longer
    console_send("[Sampling] Self-test on BME680 : %s\r\n",
                 bme680_returnCodeAsString(status));

    static bme680_data_s bme680_data[BME680_MAX_DATA_AVAILABLE] = {{0}};
    static uint32_t bme680_data_number = 0;

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));  // 1 second
        status = bme680_getData(bme680_data, &bme680_data_number);
        // console_send("[Sampling] BME680 get data : %s\r\n",
        // bme680_returnCodeAsString(status));
        if ((status == BME680_OK) && (bme680_data_number > 0)) {
            // console_send("[Sampling] Temperature %.2f degC\r\n",
            // bme680_data[0].temperature_in_deg); console_send("[Sampling]
            // Relative humidity %.0f per100\r\n",
            // bme680_data[0].humidity_in_per100);
        }
    }
}