#ifndef INA219_H
#define INA219_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    INA219_SUCCESS = 0,
    INA219_FAILURE,
} ina219_status_e;

typedef struct
{
    int (*write)(uint8_t addr, const uint8_t *data, size_t len);
    int (*read) (uint8_t addr,       uint8_t *data, size_t len);
    float current_lsb;
} ina219_t;

typedef enum
{
    INA219_BUS_VOLTAGE_RANGE_16V = 0,
    INA219_BUS_VOLTAGE_RANGE_32V,
} ina219_bus_voltage_range_e;

typedef enum
{
    INA219_PROGRAMMING_GAIN_1 = 0, // Range +/-40mV
    INA219_PROGRAMMING_GAIN_2,     // Range +/-80mV
    INA219_PROGRAMMING_GAIN_4,     // Range +/-160mV
    INA219_PROGRAMMING_GAIN_8,     // Range +/-320mV
} ina219_programming_gain_e;

typedef enum
{
    INA219_ADC_RESOLUTION_9_BIT = 0,   // Conversion Time 84 µs
    INA219_ADC_RESOLUTION_10_BIT,      // Conversion Time 148 µs
    INA219_ADC_RESOLUTION_11_BIT,      // Conversion Time 276 µs
    INA219_ADC_RESOLUTION_12_BIT,      // Conversion Time 532 µs
    INA219_ADC_RESOLUTION_2_SAMPLES,   // Conversion Time 1.06 ms
    INA219_ADC_RESOLUTION_4_SAMPLES,   // Conversion Time 2.13 ms
    INA219_ADC_RESOLUTION_8_SAMPLES,   // Conversion Time 4.26 ms
    INA219_ADC_RESOLUTION_16_SAMPLES,  // Conversion Time 8.51 ms
    INA219_ADC_RESOLUTION_32_SAMPLES,  // Conversion Time 17.02 ms
    INA219_ADC_RESOLUTION_64_SAMPLES,  // Conversion Time 34.05 ms
    INA219_ADC_RESOLUTION_128_SAMPLES, // Conversion Time 68.10 ms
} ina219_adc_resolution_e;

typedef enum
{
    INA219_MODE_POWER_DOWN = 0,
    INA219_MODE_SHUNT_VOLTAGE_TRIGGERED,
    INA219_MODE_BUS_VOLTAGE_TRIGGERED,
    INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED,
    INA219_MODE_ADC_DISABLED,
    INA219_MODE_SHUNT_CONTINUOUS,
    INA219_MODE_BUS_CONTINUOUS,
    INA219_MODE_SHUNT_BUS_CONTINUOUS,
} ina219_operating_mode_e;

typedef struct
{
    ina219_adc_resolution_e bus;
    ina219_adc_resolution_e shunt;
} ina219_resolution_t;

typedef struct
{
    ina219_bus_voltage_range_e range;
    ina219_programming_gain_e gain;
    ina219_resolution_t resolution;
    ina219_operating_mode_e mode;
} ina219_config_t;

ina219_status_e ina219_get_configuration(ina219_t *dev, ina219_config_t *config);
ina219_status_e ina219_set_configuration(ina219_t *dev, ina219_config_t *config);

ina219_status_e ina219_set_calibration(ina219_t *dev, float max_expected_current, float res_shunt);

ina219_status_e ina219_read_bus_voltage(ina219_t *dev, uint32_t *volt);

ina219_status_e ina219_read_shunt_voltage(ina219_t *dev, int32_t *millivolt);

ina219_status_e ina219_read_current(ina219_t *dev, float *current);

ina219_status_e ina219_read_power(ina219_t *dev, float *power);

#ifdef __cplusplus
}
#endif

#endif /* INA219_H */
