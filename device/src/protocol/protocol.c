#include "protocol.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif // TEST

#define PROTOCOL_REQUEST_CODE_LEN_MAX (7u)
STATIC const char PROTOCOL_REQUEST_CODE[][PROTOCOL_REQUEST_CODE_LEN_MAX+1] = {
    [PROTOCOL_REQUEST_CODE_FAILURE] = "FAILURE",
    [PROTOCOL_REQUEST_CODE_SUCCESS] = "SUCCESS",
};

#define PROTOCOL_DATA_FIELD_LEN_MAX (6u)
STATIC const char PROTOCOL_DATA_FIELD[][PROTOCOL_DATA_FIELD_LEN_MAX+1] = {
    [PROTOCOL_DATA_FIELD_0] = "field0",
};

#define PROTOCOL_DATA_UNIT_LEN_MAX (5u)
STATIC const char PROTOCOL_DATA_UNIT[][PROTOCOL_DATA_UNIT_LEN_MAX+1] = {
    [PROTOCOL_DATA_UNIT_0] = "unit0",
};

#define PROTOCOL_METADATA_FIELD_LEN_MAX (6u)
STATIC const char PROTOCOL_METADATA_FIELD[][PROTOCOL_METADATA_FIELD_LEN_MAX+1] = {
    [PROTOCOL_METADATA_FIELD_0] = "field0",
};

#define PROTOCOL_METADATA_VALUE_LEN_MAX (6u)
STATIC const char PROTOCOL_METADATA_VALUE[][PROTOCOL_METADATA_VALUE_LEN_MAX+1] = {
    [PROTOCOL_METADATA_VALUE_0] = "value0",
};

void protocol_serialize(protocol_s protocol, const uint32_t string_len_max, char *string) {
    if((string == NULL) || (string_len_max <=0)) {
        return;
    }

    if((protocol.request != NULL) && (protocol.data != NULL) && (protocol.metadata != NULL)) {
        (void)snprintf(string, string_len_max, "{\"request\":{\"code\":\"%s\",\"message\":\"%s\"},\"data\":{\"field\":\"%s\",\"value\":%.1f,\"unit\":\"%s\"},\"metadata\":{\"field\":\"%s\",\"value\":\"%s\"}}\r\n",
            PROTOCOL_REQUEST_CODE[protocol.request->code], protocol.request->message,
            PROTOCOL_DATA_FIELD[protocol.data->field], protocol.data->value, PROTOCOL_DATA_UNIT[protocol.data->unit],
            PROTOCOL_METADATA_FIELD[protocol.metadata->field], PROTOCOL_METADATA_VALUE[protocol.metadata->value]);
    }
    else if((protocol.request != NULL) && (protocol.data != NULL)) {
        (void)snprintf(string, string_len_max, "{\"request\":{\"code\":\"%s\",\"message\":\"%s\"},\"data\":{\"field\":\"%s\",\"value\":%.1f,\"unit\":\"%s\"}}\r\n",
            PROTOCOL_REQUEST_CODE[protocol.request->code], protocol.request->message,
            PROTOCOL_DATA_FIELD[protocol.data->field], protocol.data->value, PROTOCOL_DATA_UNIT[protocol.data->unit]);
    }
    else if((protocol.request != NULL) && (protocol.metadata != NULL)) {
        (void)snprintf(string, string_len_max, "{\"request\":{\"code\":\"%s\",\"message\":\"%s\"},\"metadata\":{\"field\":\"%s\",\"value\":\"%s\"}}\r\n",
            PROTOCOL_REQUEST_CODE[protocol.request->code], protocol.request->message,
            PROTOCOL_METADATA_FIELD[protocol.metadata->field], PROTOCOL_METADATA_VALUE[protocol.metadata->value]);
    }
    else if((protocol.request != NULL)) {
        (void)snprintf(string, string_len_max, "{\"request\":{\"code\":\"%s\",\"message\":\"%s\"}}\r\n",
            PROTOCOL_REQUEST_CODE[protocol.request->code], protocol.request->message);
    }
}
