/**
 * @file
 */

#ifndef BME680_H
#define BME680_H

#include <stdint.h>

#define BME680_MAX_DATA_AVAILABLE (4u)

typedef enum {
    BME680_OK = 0,
    BME680_NULL_POINTER = 1,
    BME680_I2C_FAILURE = 2,
    BME680_NOT_FOUND = 3,
    BME680_INVALID_PARAM = 4,
    BME680_SELF_TEST_FAILURE = 5,
    BME680_MISC_FAILURE = 6,
} bme680_returnCode_e;

typedef struct {
    float temperature_in_deg;
    float pressure_in_pascal;
    float humidity_in_per100;
    float gas_resistance_in_ohms;
} bme680_data_s;

bme680_returnCode_e bme680_init(void);
bme680_returnCode_e bme680_selfTest(void);
bme680_returnCode_e bme680_getData(bme680_data_s *data,
                                   uint32_t *number_of_data);
char *bme680_returnCodeAsString(bme680_returnCode_e code);

#endif /* BME680_H */