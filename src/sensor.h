#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

#define SENSOR_MAX_DATA_AVAILABLE (4u)

typedef struct {
    float temperature_in_deg;
    float pressure_in_pascal;
    float humidity_in_per100;
    float gas_resistance_in_ohms;
} sensor_data_s;

int8_t sensor_init(void);
int8_t sensor_selfTest(void);
int8_t sensor_getData(sensor_data_s *data, uint32_t *number_of_data);

#endif  /* SENSOR_H */