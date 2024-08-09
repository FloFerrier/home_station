#ifndef TEST_MOCK_SENSOR_H
#define TEST_MOCK_SENSOR_H

#include "sensor.h"

void mock_assert_call_sensor_init(sensor_returnCode_e ret);
void mock_assert_call_sensor_selfTest(sensor_returnCode_e ret);
void mock_assert_call_sensor_getData(sensor_data_s *data, uint32_t number_of_data, sensor_returnCode_e ret);
void mock_assert_call_sensor_returnCodeAsString(sensor_returnCode_e code, char *ret);

#endif  // TEST_MOCK_SENSOR_H