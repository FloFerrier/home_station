#ifndef TEST_MOCK_COMMANDS_H
#define TEST_MOCK_COMMANDS_H

#include "commands.h"

void mock_assert_call_command_getIndex(const char *cmd, command_index_e ret);
void mock_assert_call_command_execute(command_index_e command_index, const uint32_t response_len_max, const char *expected_response);

#endif  // TEST_MOCK_COMMANDS_H