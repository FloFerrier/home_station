/**
 * @file
 */

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

#define SENSOR_MAX_DATA_AVAILABLE (4u)

typedef enum {
    SENSOR_OK = 0,
    SENSOR_NULL_POINTER = 1,
    SENSOR_I2C_FAILURE = 2,
    SENSOR_NOT_FOUND = 3,
    SENSOR_INVALID_PARAM = 4,
    SENSOR_SELF_TEST_FAILURE = 5,
    SENSOR_MISC_FAILURE = 6,
} sensor_returnCode_e;

typedef struct {
    float temperature_in_deg;
    float pressure_in_pascal;
    float humidity_in_per100;
    float gas_resistance_in_ohms;
} sensor_data_s;

sensor_returnCode_e sensor_init(void);
sensor_returnCode_e sensor_selfTest(void);
sensor_returnCode_e sensor_getData(sensor_data_s *data,
                                   uint32_t *number_of_data);
char *sensor_returnCodeAsString(sensor_returnCode_e code);

#endif /* SENSOR_H */