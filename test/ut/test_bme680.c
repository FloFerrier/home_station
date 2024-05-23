#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cmocka.h>

#include <bme68x.h>

static uint8_t dev_addr = BME68X_I2C_ADDR_LOW;

static int8_t bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    (void)reg_addr;
    (void)reg_data;
    (void)len;
    (void)intf_ptr;
    return BME68X_OK;
}

static int8_t bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    (void)reg_addr;
    (void)reg_data;
    (void)len;
    (void)intf_ptr;
    return BME68X_OK;
}

static void bme68x_delay_us(uint32_t period, void *intf_ptr) {
    (void)intf_ptr;
    (void)period;
}

static void test_empty(void **state) {
    (void)state;

    struct bme68x_dev sensor = {
        .chip_id = 0,
        .intf = BME68X_I2C_INTF,
        .intf_ptr = &dev_addr,
        .delay_us = bme68x_delay_us,
        .read = bme68x_i2c_read,
        .write = bme68x_i2c_write,
        .amb_temp = 25,
    };
    assert_int_equal(bme68x_init(&sensor), BME68X_E_DEV_NOT_FOUND);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_empty),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}