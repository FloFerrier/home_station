/**
 * @file
 */

#include "led.h"

#include <stdbool.h>
#include <stdint.h>

#ifndef TEST
#include <stm32f4xx_hal.h>
#define STATIC static
#else
#include "mock_stm32f4x.h"
#define STATIC
#endif  // TEST

typedef struct {
    GPIO_TypeDef *port;
    uint32_t pin;
} gpio_conf_s;

STATIC const gpio_conf_s GPIO_CONF[] = {
    [LED_ID_BLUE] =
        {
            .port = GPIOB,
            .pin = GPIO_PIN_0,
        },
    [LED_ID_GREEN] =
        {
            .port = GPIOC,
            .pin = GPIO_PIN_1,
        },
    [LED_ID_RED] =
        {
            .port = GPIOC,
            .pin = GPIO_PIN_0,
        },
};

STATIC GPIO_PinState led_transformLedStateToPinState(led_state_e state) {
    return ((state == LED_STATE_ON) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void led_init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    for (led_id_e id = LED_ID_BLUE; id <= LED_ID_RED; id++) {
        GPIO_InitTypeDef gpio_init = {
            .Pin = GPIO_CONF[id].pin,
            .Mode = GPIO_MODE_OUTPUT_OD,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
        };
        HAL_GPIO_Init(GPIO_CONF[id].port, &gpio_init);
        GPIO_PinState pin_state =
            led_transformLedStateToPinState(LED_STATE_OFF);
        HAL_GPIO_WritePin(GPIO_CONF[id].port, GPIO_CONF[id].pin, pin_state);
    }
}

STATIC bool led_idIsAvailable(led_id_e id) {
    return ((id > LED_ID_RED) ? false : true);
}

STATIC bool led_stateIsAvailable(led_state_e state) {
    return ((state > LED_STATE_ON) ? false : true);
}

bool led_setState(led_id_e id, led_state_e state) {
    if ((led_idIsAvailable(id) == false) ||
        led_stateIsAvailable(state) == false) {
        return false;
    }
    GPIO_PinState pin_state = led_transformLedStateToPinState(state);
    HAL_GPIO_WritePin(GPIO_CONF[id].port, GPIO_CONF[id].pin, pin_state);
    return true;
}