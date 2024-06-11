#ifndef TEST_MOCK_SENSOR_H
#define TEST_MOCK_SENSOR_H

#include "sensor.h"

void mock_assert_call_sensor_init(int8_t ret);
void mock_assert_call_sensor_selfTest(int8_t ret);
void mock_assert_call_sensor_getData(sensor_data_s *data, uint32_t number_of_data, int8_t ret);

#endif  // TEST_MOCK_SENSOR_H