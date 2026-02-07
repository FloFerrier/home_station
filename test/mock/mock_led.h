#ifndef TEST_MOCK_LED_H
#define TEST_MOCK_LED_H

#include "led.h"

void mock_assert_call_led_init(void);
void mock_assert_call_led_setState(led_id_e id, led_state_e state, bool ret);

#endif  // TEST_MOCK_LED_H