#include "unity.h"

#include "ina219.h"

typedef enum
{
    TEST_ADDR_REG_CONFIGURATION = 0x00,
    TEST_ADDR_REG_SHUNT_VOLTAGE = 0x01,
    TEST_ADDR_REG_BUS_VOLTAGE   = 0x02,
    TEST_ADDR_REG_POWER         = 0x03,
    TEST_ADDR_REG_CURRENT       = 0x04,
    TEST_ADDR_REG_CALIBRATION   = 0x05,
} test_reg_addr_e;

static uint8_t current_register_addr = 0x00;
static uint16_t configuration_register_value = 0x399F; // Value per default

int fake_write(uint8_t addr, const uint8_t *data, size_t len)
{
    TEST_PRINTF("Slave Address Byte: %X", (uint8_t)addr);
    if(addr != 0x41)
    {
        TEST_MESSAGE("Unknow this slave address ...");
        return -1;
    }

    TEST_MESSAGE("This slave address corresponds to INA219 device");
    if(len == 1 && data[0] == TEST_ADDR_REG_SHUNT_VOLTAGE)
    {
        TEST_PRINTF("@%X is Register Shunt Voltage", data[0]);
        current_register_addr = data[0];
    }
    else if(len == 1 && data[0] == TEST_ADDR_REG_BUS_VOLTAGE)
    {
        TEST_PRINTF("@%X is Register Bus Voltage", data[0]);
        current_register_addr = data[0];
    }
    else if(len == 1 && data[0] == TEST_ADDR_REG_CURRENT)
    {
        TEST_PRINTF("@%X is Register Current", data[0]);
        current_register_addr = data[0];
    }
    else if(len == 1 && data[0] == TEST_ADDR_REG_POWER)
    {
        TEST_PRINTF("@%X is Register Power", data[0]);
        current_register_addr = data[0];
    }
    else if(len == 1 && data[0] == TEST_ADDR_REG_CONFIGURATION)
    {
        TEST_PRINTF("@%X is Register Configuration", data[0]);
        current_register_addr = data[0];
    }
    else if(len == 3 && data[0] == TEST_ADDR_REG_CONFIGURATION)
    {
        TEST_PRINTF("@%X is Register Configuration", data[0]);
        current_register_addr = data[0];
        TEST_PRINTF("MSB=%X LSB=%X", (uint8_t)data[1], (uint8_t)data[2]);
        configuration_register_value = data[1] << 8 | data[2];
    }
    else if(len == 3 && data[0] == TEST_ADDR_REG_CALIBRATION)
    {
        TEST_PRINTF("@%X is Register Calibration", data[0]);
        current_register_addr = data[0];
        TEST_PRINTF("MSB=%X LSB=%X", (uint8_t)data[1], (uint8_t)data[2]);
    }
    else
    {
        TEST_PRINTF("@%X is unsupported register", data[0]);
    }

    return 0;
}

int fake_read(uint8_t addr, uint8_t *data, size_t len)
{
    TEST_ASSERT_NOT_NULL(data);
    TEST_ASSERT_NOT_EQUAL(0, len);

    TEST_PRINTF("Slave Address Byte: %X", (uint8_t)addr);
    if(addr != 0x41)
    {
        TEST_MESSAGE("Unknow this slave address ...");
        return -1;
    }

    TEST_MESSAGE("This slave address corresponds to INA219 device");
    if(len == 2 && current_register_addr == TEST_ADDR_REG_SHUNT_VOLTAGE)
    {
        TEST_MESSAGE("Read Register Shunt Voltage");
        // [15] Sign
        // [0:14] PGA /8 Shunt Voltage
        data[0] = 0x83; // Data MSByte
        data[1] = 0x00; // Data LSByte
        TEST_PRINTF("MSB=%X LSB=%X", (uint8_t)data[0], (uint8_t)data[1]);
    }
    else if(len == 2 && current_register_addr == TEST_ADDR_REG_BUS_VOLTAGE)
    {
        TEST_MESSAGE("Read Register Bus Voltage");
        // [0] OVF : Set Math Overflow Flag
        // [1] CNVR : Set Conversion Ready
        // [2] Set Reserved
        // [3:15] Set Bus Voltage to 32V
        data[0] = 0xFA; // Data MSByte
        data[1] = 0x03;// Data LSByte
        TEST_PRINTF("MSB=%X LSB=%X", (uint8_t)data[0], (uint8_t)data[1]);
    }
    else if(len == 2 && current_register_addr == TEST_ADDR_REG_CURRENT)
    {
        TEST_MESSAGE("Read Register Current");
        data[0] = 0x19; // Data MSByte
        data[1] = 0x88; // Data LSByte
        TEST_PRINTF("MSB=%X LSB=%X", (uint8_t)data[0], (uint8_t)data[1]);
    }
    else if(len == 2 && current_register_addr == TEST_ADDR_REG_POWER)
    {
        TEST_MESSAGE("Read Register Power");
        data[0] = 0x01; // Data MSByte
        data[1] = 0x47; // Data LSByte
        TEST_PRINTF("MSB=%X LSB=%X", (uint8_t)data[0], (uint8_t)data[1]);
    }
    else if(len == 2 && current_register_addr == TEST_ADDR_REG_CONFIGURATION)
    {
        TEST_MESSAGE("Read Register Configuration");
        data[0] = (configuration_register_value & 0xFF00) >> 8; // Data MSByte
        data[1] = (configuration_register_value & 0x00FF) >> 0; // Data LSByte
        TEST_PRINTF("MSB=%X LSB=%X", (uint8_t)data[0], (uint8_t)data[1]);
    }
    else
    {
        TEST_PRINTF("@%X is unsupported register", data[0]);
    }

    return 0;
}

void setUp(void)
{
    // Keep this function for Ceedling compilation 
}

void tearDown(void)
{
    // Keep this function for Ceedling compilation
}

void test_ina219_set_calibration(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };

    // When
    float max_expected_current = 10.0f;
    float res_shunt = 1.0f;
    ina219_status_e status = ina219_set_calibration(&dev, max_expected_current, res_shunt);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_FLOAT_WITHIN(0.000001f, 0.000153f, dev.current_lsb);
}

void test_ina219_read_bus_voltage(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };

    // When
    uint32_t volt = 0;
    ina219_status_e status = ina219_read_bus_voltage(&dev, &volt);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_EQUAL_UINT32(32, volt);
}

void test_ina219_read_shunt_voltage(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };

    // When
    int32_t millivolt = 0;
    ina219_status_e status = ina219_read_shunt_voltage(&dev, &millivolt);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_EQUAL_INT32(-320, millivolt);
}

void test_ina219_read_current_without_calibration(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };

    // When
    float current = 0.0f;
    ina219_status_e status = ina219_read_current(&dev, &current);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, current);
}

void test_ina219_read_current_after_calibration(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };
    float max_expected_current = 10.0f;
    float res_shunt = 1.0f;
    (void) ina219_set_calibration(&dev, max_expected_current, res_shunt);

    // When
    float current = 0.0f;
    ina219_status_e status = ina219_read_current(&dev, &current);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, current);
}

void test_ina219_read_power_without_calibration(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };

    // When
    float power = 0.0f;
    ina219_status_e status = ina219_read_power(&dev, &power);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 0.0f, power);
}

void test_ina219_read_power_after_calibration(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };
    float max_expected_current = 10.0f;
    float res_shunt = 1.0f;
    (void) ina219_set_calibration(&dev, max_expected_current, res_shunt);

    // When
    float power = 0.0f;
    ina219_status_e status = ina219_read_power(&dev, &power);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 1.0f, power);
}

void test_ina219_get_default_configuration(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };

    // When
    ina219_config_t config = { 0 };
    ina219_status_e status = ina219_get_configuration(&dev, &config);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_EQUAL(INA219_BUS_VOLTAGE_RANGE_32V, config.range);
    TEST_ASSERT_EQUAL(INA219_PROGRAMMING_GAIN_8, config.gain);
    TEST_ASSERT_EQUAL(INA219_ADC_RESOLUTION_12_BIT, config.resolution.bus);
    TEST_ASSERT_EQUAL(INA219_ADC_RESOLUTION_12_BIT, config.resolution.shunt);
    TEST_ASSERT_EQUAL(INA219_MODE_SHUNT_BUS_CONTINUOUS, config.mode);
}

void test_ina219_set_configuration(void)
{
    // Given
    ina219_t dev = { 
        .read = fake_read,
        .write = fake_write,
        .current_lsb = 0.0f,
    };
    ina219_config_t expected_config = {
        .gain = INA219_PROGRAMMING_GAIN_1,
        .mode = INA219_MODE_ADC_DISABLED,
        .range = INA219_BUS_VOLTAGE_RANGE_16V,
        .resolution = {
            .bus = INA219_ADC_RESOLUTION_128_SAMPLES,
            .shunt = INA219_ADC_RESOLUTION_128_SAMPLES,
        },
    };

    // When
    ina219_status_e status = ina219_set_configuration(&dev, &expected_config);
    ina219_config_t config = { 0 };
    (void) ina219_get_configuration(&dev, &config);

    // Then
    TEST_ASSERT_EQUAL(INA219_SUCCESS, status);
    TEST_ASSERT_EQUAL(INA219_BUS_VOLTAGE_RANGE_16V, config.range);
    TEST_ASSERT_EQUAL(INA219_PROGRAMMING_GAIN_1, config.gain);
    TEST_ASSERT_EQUAL(INA219_ADC_RESOLUTION_128_SAMPLES, config.resolution.bus);
    TEST_ASSERT_EQUAL(INA219_ADC_RESOLUTION_128_SAMPLES, config.resolution.shunt);
    TEST_ASSERT_EQUAL(INA219_MODE_ADC_DISABLED, config.mode);
}
