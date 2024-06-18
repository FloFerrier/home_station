#ifndef TEST_MOCK_CONSOLE_H
#define TEST_MOCK_CONSOLE_H

#include "console.h"

void mock_assert_call_console_init(bool ret);
void mock_assert_call_console_send(const char *string, bool ret);
void mock_assert_call_console_receive(char character, bool ret);

#endif  // TEST_MOCK_CONSOLE_H