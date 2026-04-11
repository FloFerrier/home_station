#include "unity.h"

#include "console.h"

#include "mock_stm32f4xx_hal_gpio.h"
#include "mock_stm32f4xx_hal_rcc.h"
#include "mock_stm32f4xx_hal_uart.h"

void test_console_should_be_initialized(void) {
    HAL_GPIO_Init_IgnoreAndReturn(HAL_OK);
    HAL_UART_Init_IgnoreAndReturn(HAL_OK);

    bool ret = console_init();

    TEST_ASSERT_TRUE(ret);
}

void test_console_should_failed_on_initialization(void) {
    HAL_GPIO_Init_IgnoreAndReturn(HAL_OK);
    HAL_UART_Init_IgnoreAndReturn(HAL_ERROR);

    bool ret = console_init();

    TEST_ASSERT_FALSE(ret);
}

void test_console_should_send_a_message(void) {
    HAL_UART_Transmit_IgnoreAndReturn(HAL_OK);

    bool ret = console_send("Hello world !\r\n");

    TEST_ASSERT_TRUE(ret);
}

void test_console_should_failed_to_send_a_message(void) {
    HAL_UART_Transmit_IgnoreAndReturn(HAL_ERROR);

    bool ret = console_send("");

    TEST_ASSERT_FALSE(ret);
}

void test_console_should_received_a_character(void) {
    HAL_UART_Receive_IgnoreAndReturn(HAL_OK);

    char character;
    bool ret = console_receive(&character);

    TEST_ASSERT_TRUE(ret);
}

void test_console_should_failed_to_receive_a_character(void) {
    HAL_UART_Receive_IgnoreAndReturn(HAL_ERROR);

    char character;
    bool ret = console_receive(&character);

    TEST_ASSERT_FALSE(ret);
}