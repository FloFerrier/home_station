#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <cmocka.h>

#include "mock_freertos.h"
#include "mock_stm32f4x.h"
#include "mock_console.h"
#include "mock_led.h"
#include "mock_sensor.h"

#include "fsm.h"

extern fsm_state_e fsm_state;
extern TaskHandle_t fsm_task_handle;

typedef struct {
    fsm_state_e *fsm_state;
} test_fixture_s;

static int setup(void **state) {
    test_fixture_s *fixture = calloc(1, sizeof(test_fixture_s));
    fixture->fsm_state = &fsm_state;
    *fixture->fsm_state = FSM_STATE_INIT;

    *state = fixture;
    return 0;
}

static int teardown(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    free(fixture);
    return 0;
}

static void test_fsm_should_enter_on_running_state_from_init_state(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->fsm_state = FSM_STATE_INIT;
    mock_assert_call_led_init();
    mock_assert_call_led_setState(LED_ID_GREEN, LED_STATE_ON, true);
    mock_assert_call_console_init(true);
    mock_assert_call_sensor_init(SENSOR_OK);
    mock_assert_call_xTaskCreate(pdPASS);
    mock_assert_call_xTaskNotifyGive(fsm_task_handle, 0);
    mock_assert_call_ulTaskNotifyTake(pdTRUE, portMAX_DELAY, 0);

    uint32_t params;
    fsm_task(&params);

    assert_int_equal(*fixture->fsm_state, FSM_STATE_RUNNING);
}

static void test_fsm_should_enter_on_error_state_from_init_state(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->fsm_state = FSM_STATE_INIT;
    mock_assert_call_led_init();
    mock_assert_call_led_setState(LED_ID_GREEN, LED_STATE_ON, true);
    mock_assert_call_console_init(false);
    mock_assert_call_xTaskNotifyGive(fsm_task_handle, 0);
    mock_assert_call_ulTaskNotifyTake(pdTRUE, portMAX_DELAY, 0);

    uint32_t params;
    fsm_task(&params);

    assert_int_equal(*fixture->fsm_state, FSM_STATE_ERROR);
}

static void test_fsm_should_enter_on_error_state_from_running_state(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->fsm_state = FSM_STATE_RUNNING;
    mock_assert_call_xTaskNotifyGive(fsm_task_handle, 0);
    mock_assert_call_led_setState(LED_ID_RED, LED_STATE_ON, true);
    mock_assert_call_led_setState(LED_ID_GREEN, LED_STATE_OFF, true);
    fsm_requestNewState(FSM_STATE_ERROR);
    mock_assert_call_ulTaskNotifyTake(pdTRUE, portMAX_DELAY, 0);

    uint32_t params;
    fsm_task(&params);

    assert_int_equal(*fixture->fsm_state, FSM_STATE_ERROR);
}

static void test_fsm_should_enter_on_rebooting_state_from_running_state(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->fsm_state = FSM_STATE_RUNNING;
    mock_assert_call_xTaskNotifyGive(fsm_task_handle, 0);
    fsm_requestNewState(FSM_STATE_REBOOTING);
    mock_assert_call_NVIC_SystemReset();
    mock_assert_call_ulTaskNotifyTake(pdTRUE, portMAX_DELAY, 0);

    uint32_t params;
    fsm_task(&params);

    assert_int_equal(*fixture->fsm_state, FSM_STATE_REBOOTING);
}

static void test_fsm_should_not_enter_on_rebooting_state_from_init_state(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->fsm_state = FSM_STATE_INIT;
    mock_assert_call_led_init();
    mock_assert_call_led_setState(LED_ID_GREEN, LED_STATE_ON, true);
    mock_assert_call_console_init(true);
    mock_assert_call_sensor_init(SENSOR_OK);
    mock_assert_call_xTaskCreate(pdPASS);
    mock_assert_call_xTaskNotifyGive(fsm_task_handle, 0);
    fsm_requestNewState(FSM_STATE_REBOOTING);
    mock_assert_call_ulTaskNotifyTake(pdTRUE, portMAX_DELAY, 0);

    uint32_t params;
    fsm_task(&params);

    assert_int_not_equal(*fixture->fsm_state, FSM_STATE_REBOOTING);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_fsm_should_enter_on_running_state_from_init_state, setup, teardown),
        cmocka_unit_test_setup_teardown(test_fsm_should_enter_on_error_state_from_init_state, setup, teardown),
        cmocka_unit_test_setup_teardown(test_fsm_should_enter_on_error_state_from_running_state, setup, teardown),
        cmocka_unit_test_setup_teardown(test_fsm_should_enter_on_rebooting_state_from_running_state, setup, teardown),
        cmocka_unit_test_setup_teardown(test_fsm_should_not_enter_on_rebooting_state_from_init_state, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}