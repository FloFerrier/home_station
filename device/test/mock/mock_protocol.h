#ifndef TEST_MOCK_PROTOCOL_H
#define TEST_MOCK_PROTOCOL_H

#include "protocol.h"

void mock_assert_call_protocol_serialize(protocol_s protocol, const uint32_t string_len_max, const char *string);

#endif  // TEST_MOCK_PROTOCOL_H