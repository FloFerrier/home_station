#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cmocka.h>

#include "console.h"
#include "mock_stm32f4x.h"

static void test_console_should_be_initialized(void **state) {
    (void)state;

    mock_assert_call_HAL_RCC_GPIOA_CLK_ENABLE();
    mock_assert_call_HAL_RCC_USART2_CLK_ENABLE();
    mock_assert_call_HAL_GPIO_Init();
    mock_assert_call_HAL_UART_Init(HAL_OK);

    bool ret = console_init();

    assert_int_equal(ret, true);
}

static void test_console_should_failed_on_initialization(void **state) {
    (void)state;

    mock_assert_call_HAL_RCC_GPIOA_CLK_ENABLE();
    mock_assert_call_HAL_RCC_USART2_CLK_ENABLE();
    mock_assert_call_HAL_GPIO_Init();
    mock_assert_call_HAL_UART_Init(HAL_ERROR);

    bool ret = console_init();

    assert_int_equal(ret, false);
}

static void test_console_should_send_a_message(void **state) {
    (void)state;
    const uint8_t expected_message[] = "Hello world !\r\n";
    uint16_t expected_len = strlen((char*)expected_message);
    const uint32_t expected_timeout = 1000u;

    mock_assert_call_HAL_UART_Transmit(expected_message, expected_len, expected_timeout, HAL_OK);

    console_send("Hello world !\r\n");
}

static void test_console_should_failed_to_send_a_message(void **state) {
    (void)state;
    const uint8_t expected_message[] = "";
    uint16_t expected_len = strlen((char*)expected_message);
    const uint32_t expected_timeout = 1000u;

    mock_assert_call_HAL_UART_Transmit(expected_message, expected_len, expected_timeout, HAL_ERROR);

    console_send("");
}

static void test_console_should_received_a_character(void **state) {
    (void)state;
    uint8_t expected_character = 'a';
    const uint16_t expected_len = 1u;
    const uint32_t expected_timeout = 1000u;

    mock_assert_call_HAL_UART_Receive(&expected_character, expected_len, expected_timeout, HAL_OK);

    char character;
    bool ret = console_receive(&character);

    assert_int_equal(ret, true);
    assert_int_equal(character, expected_character);
}

static void test_console_should_failed_to_receive_a_character(void **state) {
    (void)state;
    uint8_t expected_character = '\0';
    const uint16_t expected_len = 1u;
    const uint32_t expected_timeout = 1000u;

    mock_assert_call_HAL_UART_Receive(&expected_character, expected_len, expected_timeout, HAL_ERROR);

    char character;
    bool ret = console_receive(&character);

    assert_int_equal(ret, false);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_console_should_be_initialized),
        cmocka_unit_test(test_console_should_failed_on_initialization),
        cmocka_unit_test(test_console_should_send_a_message),
        cmocka_unit_test(test_console_should_failed_to_send_a_message),
        cmocka_unit_test(test_console_should_received_a_character),
        cmocka_unit_test(test_console_should_failed_to_receive_a_character),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}