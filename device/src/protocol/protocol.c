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

#define PROTOCOL_DATA_FIELD_LEN_MAX (11u)
STATIC const char PROTOCOL_DATA_FIELD[][PROTOCOL_DATA_FIELD_LEN_MAX+1] = {
    [PROTOCOL_DATA_FIELD_TEMPERATURE] = "temperature",
    [PROTOCOL_DATA_FIELD_HUMIDITY] = "humidity",
    [PROTOCOL_DATA_FIELD_PRESSURE] = "pressure",
};

#define PROTOCOL_DATA_UNIT_LEN_MAX (6u)
STATIC const char PROTOCOL_DATA_UNIT[][PROTOCOL_DATA_UNIT_LEN_MAX+1] = {
    [PROTOCOL_DATA_UNIT_DEGREE_CELSIUS] = "degC",
    [PROTOCOL_DATA_UNIT_PERCENTAGE] = "per100",
    [PROTOCOL_DATA_UNIT_PASCAL] = "Pa",
};

#define PROTOCOL_METADATA_FIELD_LEN_MAX (6u)
STATIC const char PROTOCOL_METADATA_FIELD[][PROTOCOL_METADATA_FIELD_LEN_MAX+1] = {
    [PROTOCOL_METADATA_FIELD_0] = "field0",
    [PROTOCOL_METADATA_FIELD_1] = "field1",
};

#define PROTOCOL_METADATA_VALUE_LEN_MAX (6u)
STATIC const char PROTOCOL_METADATA_VALUE[][PROTOCOL_METADATA_VALUE_LEN_MAX+1] = {
    [PROTOCOL_METADATA_VALUE_0] = "value0",
    [PROTOCOL_METADATA_VALUE_1] = "value1",
};

#define STRING_REQUEST_LEN_MAX (128u)
STATIC char string_request[STRING_REQUEST_LEN_MAX] = "";

#define STRING_LIST_DATA_LEN_MAX (256u)
STATIC char string_list_data[STRING_LIST_DATA_LEN_MAX] = "";

#define STRING_DATA_LEN_MAX (64u)
STATIC char string_data[STRING_DATA_LEN_MAX] = "";

#define STRING_LIST_METADATA_LEN_MAX (128u)
STATIC char string_list_metadata[STRING_LIST_METADATA_LEN_MAX] = "";

#define STRING_METADATA_LEN_MAX (64u)
STATIC char string_metadata[STRING_METADATA_LEN_MAX] = "";

void protocol_serialize(protocol_s protocol, const uint32_t string_len_max, char *string) {
    if((string == NULL) || (string_len_max <=0)) {
        return;
    }

    /* Serialize request field */
    (void)snprintf(string_request, STRING_REQUEST_LEN_MAX-1, "\"request\":{\"code\":\"%s\",\"message\":\"%s\"}", PROTOCOL_REQUEST_CODE[protocol.request.code], protocol.request.message);

    /* Serialize data field */
    if(protocol.data_nb > 0) {
        (void)memset(string_list_data, 0, STRING_LIST_DATA_LEN_MAX);
        for(uint8_t i=0; i<protocol.data_nb;i++) {
            (void)snprintf(string_data, STRING_DATA_LEN_MAX-1, "{\"field\":\"%s\",\"value\":%.1f,\"unit\":\"%s\"},",
                PROTOCOL_DATA_FIELD[protocol.data[i].field], protocol.data[i].value, PROTOCOL_DATA_UNIT[protocol.data[i].unit]);
            (void)strncat(string_list_data, string_data, STRING_LIST_DATA_LEN_MAX-1);
        }
    }

    /* Serialize metadata field */
    if(protocol.metadata_nb > 0) {
        (void)memset(string_list_metadata, 0, STRING_LIST_METADATA_LEN_MAX);
        for(uint8_t i=0; i<protocol.metadata_nb;i++) {
            (void)snprintf(string_metadata, STRING_METADATA_LEN_MAX-1, "{\"field\":\"%s\",\"value\":\"%s\"},",
                PROTOCOL_METADATA_FIELD[protocol.metadata[i].field], PROTOCOL_METADATA_VALUE[protocol.metadata[i].value]);
            (void)strncat(string_list_metadata, string_metadata, STRING_LIST_METADATA_LEN_MAX-1);
        }
    }

    /* Only request field */
    if((protocol.data_nb == 0) && (protocol.metadata_nb == 0)) {
        (void)snprintf(string, string_len_max, "{%s}\r\n", string_request);
    }
    /* Request and data field */
    else if(protocol.metadata_nb == 0) {
        (void)snprintf(string, string_len_max, "{%s,\"data\":[%s]}\r\n", string_request, string_list_data);
    }
    /* Request and metadata field */
    else if(protocol.data_nb == 0) {
        (void)snprintf(string, string_len_max, "{%s,\"metadata\":[%s]}\r\n", string_request, string_list_metadata);
    }
    /* Request, data and metadata field */
    else {
        (void)snprintf(string, string_len_max, "{%s,\"data\":[%s],\"metadata\":[%s]}\r\n", string_request, string_list_data, string_list_metadata);
    }
}
