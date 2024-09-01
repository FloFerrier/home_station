#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cmocka.h>

#include "led.h"
#include "mock_stm32f4x.h"

static void test_led_should_be_initialized(void **state) {
    (void)state;
    mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE();
    mock_assert_call_HAL_RCC_GPIOC_CLK_ENABLE();
    for(uint8_t i=0; i< 3; i++) {
        mock_assert_call_HAL_GPIO_Init();
        mock_assert_call_HAL_GPIO_WritePin(GPIO_PIN_SET);
    }

    led_init();
}

static void test_led_should_be_set_to_off(void **state) {
    (void)state;
    mock_assert_call_HAL_GPIO_WritePin(GPIO_PIN_SET);

    bool result = led_setState(LED_ID_BLUE, LED_STATE_OFF);

    assert_int_equal(result, true);
}

static void test_led_should_be_set_to_on(void **state) {
    (void)state;
    mock_assert_call_HAL_GPIO_WritePin(GPIO_PIN_RESET);

    bool result = led_setState(LED_ID_BLUE, LED_STATE_ON);

    assert_int_equal(result, true);
}

static void test_led_should_be_set_with_id_out_of_bounds(void **state) {
    (void)state;

    bool result = led_setState(0xFF, LED_STATE_ON);

    assert_int_equal(result, false);
}

static void test_led_should_be_set_with_state_out_of_bounds(void **state) {
    (void)state;

    bool result = led_setState(LED_ID_BLUE, 0xFF);

    assert_int_equal(result, false);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_led_should_be_initialized),
        cmocka_unit_test(test_led_should_be_set_to_off),
        cmocka_unit_test(test_led_should_be_set_to_on),
        cmocka_unit_test(test_led_should_be_set_with_id_out_of_bounds),
        cmocka_unit_test(test_led_should_be_set_with_state_out_of_bounds),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}