#include "protocol.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif // TEST

#define PROTOCOL_CODE_LEN_MAX (8u)

STATIC const char PROTOCOL_CODE[][PROTOCOL_CODE_LEN_MAX+1] = {
    [PROTOCOL_CODE_FAILURE] = "FAILURE",
    [PROTOCOL_CODE_SUCCESS] = "SUCCESS",
};

void protocol_serialize(protocol_s data, const uint32_t string_len_max, char *string) {
    if((string == NULL) || (string_len_max <=0)) {
        return;
    }

    (void)snprintf(string, string_len_max, "{\"code\":\"%s\", \"message\":\"%s\", \"response\":\"%s\"}\r\n", PROTOCOL_CODE[data.code], data.message, data.response);
}