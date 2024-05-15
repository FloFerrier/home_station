#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmocka.h>

#include "mock_console.h"
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

    *fixture->loopCnt = 0;
    (void)memset(fixture->shell->buffer, '\0', RING_BUFFER_LEN_MAX);
    fixture->shell->index = 0;

    *state = fixture;
    return 0;
}

static int teardown(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    free(fixture);
    return 0;
}

static void test_fail_to_receive_character(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->loopCnt = 0;

    uint32_t params;
    mock_assert_call_console_init();
    mock_assert_call_console_receive('a', false);

    shell_task(&params);
}

static void test_receive_one_character(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    *fixture->loopCnt = 0;

    uint32_t params;
    mock_assert_call_console_init();
    mock_assert_call_console_receive('a', true);

    shell_task(&params);
}

static void test_receive_command_unknown(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    uint32_t params;
    mock_assert_call_console_init();
    const char TEST_DATA[] = "Hello world\n";
    *fixture->loopCnt = strlen(TEST_DATA) - 1;

    for(int index=0; index < (int)(strlen(TEST_DATA)); index++) {
        mock_assert_call_console_receive(TEST_DATA[index], true);
    }

    mock_assert_call_console_send("> Unknown command\r\n");

    shell_task(&params);
}

static void test_receive_command_dataGet(void **state) {
    test_fixture_s *fixture = (test_fixture_s *)*state;
    uint32_t params;
    mock_assert_call_console_init();
    const char TEST_DATA[] = "data_get\n";
    *fixture->loopCnt = strlen(TEST_DATA) - 1;

    for(int index=0; index < (int)(strlen(TEST_DATA)); index++) {
        mock_assert_call_console_receive(TEST_DATA[index], true);
    }

    mock_assert_call_console_send("> SUCCESS\r\n");

    shell_task(&params);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_fail_to_receive_character, setup, teardown),
        cmocka_unit_test_setup_teardown(test_receive_one_character, setup, teardown),
        cmocka_unit_test_setup_teardown(test_receive_command_unknown, setup, teardown),
        cmocka_unit_test_setup_teardown(test_receive_command_dataGet, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}