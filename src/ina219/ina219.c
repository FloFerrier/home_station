#include "ina219.h"

#include <assert.h>
#include <stdbool.h>

#define INA219_SLAVE_ADDRESS (0x41)

#define INA219_BUS_VOLTAGE_RANGE_MASK (0x1)
#define INA219_BUS_VOLTAGE_RANGE_OFFSET (13)

#define INA219_PROGRAMMING_GAIN_MASK (0x3)
#define INA219_PROGRAMMING_GAIN_OFFSET (11)

#define INA219_ADC_RESOLUTION_MASK (0xF)
#define INA219_ADC_RESOLUTION_BUS_OFFSET (7)
#define INA219_ADC_RESOLUTION_SHUNT_OFFSET (3)

#define INA219_OPERATING_MODE_MASK (0x7)
#define INA219_OPERATING_MODE_OFFSET (0)

typedef enum
{
    INA219_ADDR_REGISTER_CONFIGURATION = 0x00,
    INA219_ADDR_REGISTER_SHUNT_VOLTAGE = 0x01,
    INA219_ADDR_REGISTER_BUS_VOLTAGE   = 0x02,
    INA219_ADDR_REGISTER_POWER         = 0x03,
    INA219_ADDR_REGISTER_CURRENT       = 0x04,
    INA219_ADDR_REGISTER_CALIBRATION   = 0x05,
} ina219_reg_addr_e;

static ina219_programming_gain_e convert_register_to_gain(uint16_t raw)
{
    ina219_programming_gain_e gain;
    switch (raw & (INA219_PROGRAMMING_GAIN_MASK << INA219_PROGRAMMING_GAIN_OFFSET))
    {
    case 0x0000:
        gain = INA219_PROGRAMMING_GAIN_1;
        break;
    case 0x0800:
        gain = INA219_PROGRAMMING_GAIN_2;
        break;
    case 0x1000:
        gain = INA219_PROGRAMMING_GAIN_4;
        break;
    case 0x1800:
        gain = INA219_PROGRAMMING_GAIN_8;
        break;
    default:
        // Not reach here due to mask
        break;
    }
    return gain;
}

static ina219_adc_resolution_e convert_register_to_resolution(uint16_t raw)
{
    ina219_adc_resolution_e resolution;
    switch (raw)
    {
    case 0b0000:
    case 0b0100:
        resolution = INA219_ADC_RESOLUTION_9_BIT;
        break;
    case 0b0001:
    case 0b0101:
        resolution = INA219_ADC_RESOLUTION_10_BIT;
        break;
    case 0b0010:
    case 0b0110:
        resolution = INA219_ADC_RESOLUTION_11_BIT;
        break;
    case 0b0011:
    case 0b0111:
    case 0b1000:
        resolution = INA219_ADC_RESOLUTION_12_BIT;
        break;
    case 0b1001:
        resolution = INA219_ADC_RESOLUTION_2_SAMPLES;
        break;
    case 0b1010:
        resolution = INA219_ADC_RESOLUTION_4_SAMPLES;
        break;
    case 0b1011:
        resolution = INA219_ADC_RESOLUTION_8_SAMPLES;
        break;
    case 0b1100:
        resolution = INA219_ADC_RESOLUTION_16_SAMPLES;
        break;
    case 0b1101:
        resolution = INA219_ADC_RESOLUTION_32_SAMPLES;
        break;
    case 0b1110:
        resolution = INA219_ADC_RESOLUTION_64_SAMPLES;
        break;
    case 0b1111:
        resolution = INA219_ADC_RESOLUTION_128_SAMPLES;
        break;
    default:
        // Not reach here due to mask
        break;
    }
    return resolution;
}

static ina219_adc_resolution_e convert_register_to_bus_resolution(uint16_t raw)
{
    return convert_register_to_resolution((raw & (INA219_ADC_RESOLUTION_MASK << INA219_ADC_RESOLUTION_BUS_OFFSET)) >> INA219_ADC_RESOLUTION_BUS_OFFSET);
}

static ina219_adc_resolution_e convert_register_to_shunt_resolution(uint16_t raw)
{
    return convert_register_to_resolution((raw & (INA219_ADC_RESOLUTION_MASK << INA219_ADC_RESOLUTION_SHUNT_OFFSET)) >> INA219_ADC_RESOLUTION_SHUNT_OFFSET);
}

static ina219_operating_mode_e convert_register_to_mode(uint16_t raw)
{
    ina219_operating_mode_e mode;
    switch (raw & (INA219_OPERATING_MODE_MASK << INA219_OPERATING_MODE_OFFSET))
    {
    case 0x00:
        mode = INA219_MODE_POWER_DOWN;
        break;
    case 0x01:
        mode = INA219_MODE_SHUNT_VOLTAGE_TRIGGERED;
        break;
    case 0x02:
        mode = INA219_MODE_BUS_VOLTAGE_TRIGGERED;
        break;
    case 0x03:
        mode = INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED;
        break;
    case 0x04:
        mode = INA219_MODE_ADC_DISABLED;
        break;
    case 0x05:
        mode = INA219_MODE_SHUNT_CONTINUOUS;
        break;
    case 0x06:
        mode = INA219_MODE_BUS_CONTINUOUS;
        break;
    case 0x07:
        mode = INA219_MODE_SHUNT_BUS_CONTINUOUS;
        break;
    default:
        // Not reach here due to mask
        break;
    }
    return mode;
}

ina219_status_e ina219_get_configuration(ina219_t *dev, ina219_config_t *config)
{
    assert(dev != NULL);

    uint8_t tx[] = {
        INA219_ADDR_REGISTER_CONFIGURATION,
    };
    assert(dev->write != NULL);
    int status = dev->write(INA219_SLAVE_ADDRESS, tx, sizeof(tx)/sizeof(tx[0]));
    assert(status == 0);

    uint8_t rx[2] = { 0 };
    assert(dev->read != NULL);
    status = dev->read(INA219_SLAVE_ADDRESS, rx, sizeof(rx)/sizeof(rx[0]));
    assert(status == 0);

    uint16_t raw = rx[0] << 8 | rx[1] << 0;

    assert(config != NULL);

    config->range = (raw & (INA219_BUS_VOLTAGE_RANGE_MASK << INA219_BUS_VOLTAGE_RANGE_OFFSET) != 0) ? INA219_BUS_VOLTAGE_RANGE_32V : INA219_BUS_VOLTAGE_RANGE_16V;
    config->gain = convert_register_to_gain(raw);
    config->resolution.bus = convert_register_to_bus_resolution(raw);
    config->resolution.shunt = convert_register_to_shunt_resolution(raw);
    config->mode = convert_register_to_mode(raw);

    return INA219_SUCCESS;
}

ina219_status_e ina219_set_configuration(ina219_t *dev, ina219_config_t *config)
{
    assert(dev != NULL);
    assert(config != NULL);

    uint16_t config_reg = 0x399F;

    config_reg &= ~(INA219_BUS_VOLTAGE_RANGE_MASK << INA219_BUS_VOLTAGE_RANGE_OFFSET);
    if (config->range == INA219_BUS_VOLTAGE_RANGE_32V)
    {
        config_reg |= (INA219_BUS_VOLTAGE_RANGE_MASK << INA219_BUS_VOLTAGE_RANGE_OFFSET);
    }

    config_reg &= ~(INA219_PROGRAMMING_GAIN_MASK << INA219_PROGRAMMING_GAIN_OFFSET);
    switch (config->gain)
    {
    case INA219_PROGRAMMING_GAIN_2:
        config_reg |= 0x0800;
        break;
    case INA219_PROGRAMMING_GAIN_4:
        config_reg |= 0x1000;
        break;
    case INA219_PROGRAMMING_GAIN_8:
        config_reg |= 0x1800;
        break;
    default:
        // Not reach here due to mask
        break;
    }

    config_reg &= ~(INA219_ADC_RESOLUTION_MASK << INA219_ADC_RESOLUTION_BUS_OFFSET);
    switch (config->resolution.bus)
    {
    case INA219_ADC_RESOLUTION_9_BIT:
        config_reg |= (0b0100 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_10_BIT:
        config_reg |= (0b0101 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_11_BIT:
        config_reg |= (0b0110 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_12_BIT:
        config_reg |= (0b0111 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_2_SAMPLES:
        config_reg |= (0b1001 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_4_SAMPLES:
        config_reg |= (0b1010 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_8_SAMPLES:
        config_reg |= (0b1011 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_16_SAMPLES:
        config_reg |= (0b1100 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_32_SAMPLES:
        config_reg |= (0b1101 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_64_SAMPLES:
        config_reg |= (0b1110 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_128_SAMPLES:
        config_reg |= (0b1111 << INA219_ADC_RESOLUTION_BUS_OFFSET);
        break;
    default:
        // Not reach here due to mask
        break;
    }

    config_reg &= ~(INA219_ADC_RESOLUTION_MASK << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
    switch (config->resolution.shunt)
    {
    case INA219_ADC_RESOLUTION_9_BIT:
        config_reg |= (0b0100 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_10_BIT:
        config_reg |= (0b0101 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_11_BIT:
        config_reg |= (0b0110 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_12_BIT:
        config_reg |= (0b0111 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_2_SAMPLES:
        config_reg |= (0b1001 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_4_SAMPLES:
        config_reg |= (0b1010 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_8_SAMPLES:
        config_reg |= (0b1011 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_16_SAMPLES:
        config_reg |= (0b1100 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_32_SAMPLES:
        config_reg |= (0b1101 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_64_SAMPLES:
        config_reg |= (0b1110 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    case INA219_ADC_RESOLUTION_128_SAMPLES:
        config_reg |= (0b1111 << INA219_ADC_RESOLUTION_SHUNT_OFFSET);
        break;
    default:
        // Not reach here due to mask
        break;
    }

    config_reg &= ~(INA219_OPERATING_MODE_MASK << INA219_OPERATING_MODE_OFFSET);
    switch (config->mode)
    {
    case INA219_MODE_SHUNT_VOLTAGE_TRIGGERED:
        config_reg |= (0x01 << INA219_OPERATING_MODE_OFFSET);
        break;
    case INA219_MODE_BUS_VOLTAGE_TRIGGERED:
        config_reg |= (0x02 << INA219_OPERATING_MODE_OFFSET);
        break;
    case INA219_MODE_SHUNT_BUS_VOLTAGE_TRIGGERED:
        config_reg |= (0x03 << INA219_OPERATING_MODE_OFFSET);
        break;
    case INA219_MODE_ADC_DISABLED:
        config_reg |= (0x04 << INA219_OPERATING_MODE_OFFSET);
        break;
    case INA219_MODE_SHUNT_CONTINUOUS:
        config_reg |= (0x05 << INA219_OPERATING_MODE_OFFSET);
        break;
    case INA219_MODE_BUS_CONTINUOUS:
        config_reg |= (0x06 << INA219_OPERATING_MODE_OFFSET);
        break;
    case INA219_MODE_SHUNT_BUS_CONTINUOUS:
        config_reg |= (0x07 << INA219_OPERATING_MODE_OFFSET);
        break;
    default:
        // Not reach here due to mask
        break;
    }

    uint8_t tx[] = {
        INA219_ADDR_REGISTER_CONFIGURATION,
        (config_reg & 0xFF00) >> 8,
        (config_reg & 0x00FF) >> 0,
    };
    assert(dev->write != NULL);
    int status = dev->write(INA219_SLAVE_ADDRESS, tx, sizeof(tx)/sizeof(tx[0]));
    assert(status == 0);

    return INA219_SUCCESS;
}

ina219_status_e ina219_set_calibration(ina219_t *dev, float max_expected_current, float res_shunt)
{
    assert(res_shunt != 0.0f);
    float current_lsb = max_expected_current / (2 << 15);
    float calibration = 0.04096 / (current_lsb * res_shunt);

    assert(dev != NULL);

    uint16_t raw = (uint16_t)calibration;
    uint8_t tx[] = {
        INA219_ADDR_REGISTER_CALIBRATION,
        raw >> 8,
        raw >> 0,
    };
    assert(dev->write != NULL);
    int status = dev->write(INA219_SLAVE_ADDRESS, tx, sizeof(tx)/sizeof(tx[0]));
    assert(status == 0);

    dev->current_lsb = current_lsb;

    return INA219_SUCCESS;
}

ina219_status_e ina219_read_bus_voltage(ina219_t *dev, uint32_t *volt)
{
    assert(dev != NULL);

    uint8_t tx[] = {
        INA219_ADDR_REGISTER_BUS_VOLTAGE,
    };
    assert(dev->write != NULL);
    int status = dev->write(INA219_SLAVE_ADDRESS, tx, sizeof(tx)/sizeof(tx[0]));
    assert(status == 0);

    uint8_t rx[2] = { 0 };
    assert(dev->read != NULL);
    status = dev->read(INA219_SLAVE_ADDRESS, rx, sizeof(rx)/sizeof(rx[0]));
    assert(status == 0);

    assert(volt != NULL);
    uint16_t raw = (rx[0] << 8 | rx[1] << 0) >> 3;
    *volt = (float)(raw) * (float)0.004f;

    return INA219_SUCCESS;
}

ina219_status_e ina219_read_shunt_voltage(ina219_t *dev, int32_t *millivolt)
{
    assert(dev != NULL);
    uint8_t tx[] = {
        INA219_ADDR_REGISTER_SHUNT_VOLTAGE,
    };
    assert(dev->write != NULL);
    int status = dev->write(INA219_SLAVE_ADDRESS, tx, sizeof(tx)/sizeof(tx[0]));
    assert(status == 0);

    uint8_t rx[2] = { 0 };
    assert(dev->read != NULL);
    status = dev->read(INA219_SLAVE_ADDRESS, rx, sizeof(rx)/sizeof(rx[0]));
    assert(status == 0);

    assert(millivolt != NULL);
    uint16_t raw = rx[0] << 8 | rx[1] << 0;
    *millivolt = (int16_t)raw / 100;

    return INA219_SUCCESS;
}

ina219_status_e ina219_read_current(ina219_t *dev, float *current)
{
    assert(dev != NULL);

    // Read Current Register
    uint8_t tx[] = {
        INA219_ADDR_REGISTER_CURRENT,
    };
    assert(dev->write != NULL);
    int status = dev->write(INA219_SLAVE_ADDRESS, tx, sizeof(tx)/sizeof(tx[0]));
    assert(status == 0);

    uint8_t rx[2] = { 0 };
    assert(dev->read != NULL);
    status = dev->read(INA219_SLAVE_ADDRESS, rx, sizeof(rx)/sizeof(rx[0]));
    assert(status == 0);

    uint16_t current_reg = rx[0] << 8 | rx[1] << 0;

    assert(current != NULL);
    *current = current_reg * dev->current_lsb;
    // Current Register = (Shunt Voltage Register * Calibration Register) / 4096

    return INA219_SUCCESS;
}

ina219_status_e ina219_read_power(ina219_t *dev, float *power)
{
    assert(dev != NULL);

    // Read Power Register
    uint8_t tx[] = {
        INA219_ADDR_REGISTER_POWER,
    };
    assert(dev->write != NULL);
    int status = dev->write(INA219_SLAVE_ADDRESS, tx, sizeof(tx)/sizeof(tx[0]));
    assert(status == 0);

    uint8_t rx[2] = { 0 };
    assert(dev->read != NULL);
    status = dev->read(INA219_SLAVE_ADDRESS, rx, sizeof(rx)/sizeof(rx[0]));
    assert(status == 0);

    uint16_t current_power = rx[0] << 8 | rx[1] << 0;

    assert(power != NULL);
    float power_lsb = dev->current_lsb * 20.0f;
    *power = current_power * power_lsb;
    // Power Register = (Current Register * Bus Voltage Register) / 5000

    return INA219_SUCCESS;
}
