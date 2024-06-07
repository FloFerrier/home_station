#ifndef TEST_MOCK_BME68X_H
#define TEST_MOCK_BME68X_H

#include <bme68x.h>

void mock_assert_call_bme68x_init(struct bme68x_dev *dev, int8_t ret);
void mock_assert_call_bme68x_soft_reset(struct bme68x_dev *dev, int8_t ret);
void mock_assert_call_bme68x_set_op_mode(const uint8_t op_mode, struct bme68x_dev *dev, int8_t ret);
void mock_assert_call_bme68x_get_meas_dur(const uint8_t op_mode, struct bme68x_conf *conf, struct bme68x_dev *dev, uint32_t ret);
void mock_assert_call_bme68x_get_data(uint8_t op_mode, struct bme68x_data *data, uint8_t n_data, struct bme68x_dev *dev, int8_t ret);
void mock_assert_call_bme68x_set_conf(struct bme68x_conf *conf, struct bme68x_dev *dev, int8_t ret);
void mock_assert_call_bme68x_set_heatr_conf(uint8_t op_mode, const struct bme68x_heatr_conf *conf, struct bme68x_dev *dev, int8_t ret);
void mock_assert_call_bme68x_selftest_check(const struct bme68x_dev *dev, int8_t ret);

#endif  // TEST_MOCK_BME68X_H