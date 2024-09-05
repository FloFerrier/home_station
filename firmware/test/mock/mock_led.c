#include "mock_led.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

void mock_assert_call_led_init(void) {
    expect_function_call(led_init);
}
void led_init(void) {
    function_called();
}

void mock_assert_call_led_setState(led_id_e id, led_state_e state, bool ret) {
    expect_function_call(led_setState);
    expect_value(led_setState, id, id);
    expect_value(led_setState, state, state);
    will_return(led_setState, ret);
}
bool led_setState(led_id_e id, led_state_e state) {
    function_called();
    check_expected(id);
    check_expected(state);
    return mock();
}