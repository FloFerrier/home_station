#include "unity.h"

#include "led.h"

#include "mock_stm32f4xx_hal_gpio.h"
#include "mock_stm32f4xx_hal_rcc.h"

void test_led_should_be_initialized(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_Init_IgnoreAndReturn(HAL_OK);
    for (uint8_t i = 0; i < 3; i++) {
        HAL_GPIO_WritePin_Ignore();
    }

    led_init();
}

void test_led_should_be_set_to_off(void) {
    HAL_GPIO_WritePin_Ignore();

    bool result = led_setState(LED_ID_BLUE, LED_STATE_OFF);

    TEST_ASSERT_TRUE(result);
}

void test_led_should_be_set_to_on(void) {
    HAL_GPIO_WritePin_Ignore();

    bool result = led_setState(LED_ID_BLUE, LED_STATE_ON);

    TEST_ASSERT_TRUE(result);
}

void test_led_should_be_set_with_id_out_of_bounds(void) {
    bool result = led_setState(0xFF, LED_STATE_ON);

    TEST_ASSERT_FALSE(result);
}

void test_led_should_be_set_with_state_out_of_bounds(void) {
    bool result = led_setState(LED_ID_BLUE, 0xFF);

    TEST_ASSERT_FALSE(result);
}