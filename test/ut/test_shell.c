#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmocka.h>

#include "mock_console.h"
#include "mock_commands.h"
#include "shell.h"

extern int loopCnt;
extern shell_s shell;
extern void shell_task(void *params);

typedef struct {
    int *loopCnt;
    shell_s *shell;
} test_fixture_s;

static int setup(void **state) {
    test_fixture_s *fixture = calloc(1, sizeof(test_fixture_s));
    fixture->loopCnt = &loopCnt;
    fixture->shell = &shell;
    (void)memset(fixture->loopCnt, 0, sizeof(int));
    fixture->shell->index = 0;
    (void)memset(fixture->shell->buffer, '\0', RING_BUFFER_LEN_MAX);

    *state = fixture;
    return 0;
}

static int teardown(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    free(fixture);
    return 0;
}

static void test_shell_should_failed_to_receive_a_character(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->loopCnt = 0;

    mock_assert_call_console_receive('a', false);

    uint32_t params;
    shell_task(&params);
}

static void test_shell_should_received_a_character(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->loopCnt = 0;

    mock_assert_call_console_receive('a', true);

    uint32_t params;
    shell_task(&params);
}

static void test_shell_should_received_an_unknown_command(void **state) {
    const char TEST_DATA[] = "Hello world\n";
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->loopCnt = strlen(TEST_DATA) - 1;

    for(int index=0; index < (int)(strlen(TEST_DATA)); index++) {
        mock_assert_call_console_receive(TEST_DATA[index], true);
    }
    mock_assert_call_console_send("> Unknown command\r\n", true);

    uint32_t params;
    shell_task(&params);
}

static void test_shell_should_received_command_sensor_selfTest(void **state) {
    const char TEST_DATA[] = "sensor_selfTest\n";
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->loopCnt = strlen(TEST_DATA) - 1;

    for(int index=0; index < (int)(strlen(TEST_DATA)); index++) {
        mock_assert_call_console_receive(TEST_DATA[index], true);
    }
    mock_assert_call_command_sensorSelfTest();

    uint32_t params;
    shell_task(&params);
}

static void test_shell_should_received_command_sensor_getData(void **state) {
    const char TEST_DATA[] = "sensor_getData\n";
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->loopCnt = strlen(TEST_DATA) - 1;

    for(int index=0; index < (int)(strlen(TEST_DATA)); index++) {
        mock_assert_call_console_receive(TEST_DATA[index], true);
    }
    mock_assert_call_command_sensorGetData();

    uint32_t params;
    shell_task(&params);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_shell_should_failed_to_receive_a_character, setup, teardown),
        cmocka_unit_test_setup_teardown(test_shell_should_received_a_character, setup, teardown),
        cmocka_unit_test_setup_teardown(test_shell_should_received_an_unknown_command, setup, teardown),
        cmocka_unit_test_setup_teardown(test_shell_should_received_command_sensor_selfTest, setup, teardown),
        cmocka_unit_test_setup_teardown(test_shell_should_received_command_sensor_getData, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}