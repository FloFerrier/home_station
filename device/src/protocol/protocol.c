#include "protocol.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifndef TEST
#define STATIC static
#else
#define STATIC
#endif // TEST

#define PROTOCOL_REQUEST_CODE_SIZE_MAX (8u)
STATIC const char PROTOCOL_REQUEST_CODE[][PROTOCOL_REQUEST_CODE_SIZE_MAX] = {
    [PROTOCOL_REQUEST_CODE_UNKNOWN] = "UNKNOWN",
    [PROTOCOL_REQUEST_CODE_FAILURE] = "FAILURE",
    [PROTOCOL_REQUEST_CODE_SUCCESS] = "SUCCESS",
};

#define STRING_REQUEST_SIZE_MAX (128u)
#define STRING_DATA_LIST_SIZE_MAX (256u)
#define STRING_DATA_ITEM_SIZE_MAX (128u)
#define STRING_METADATA_LIST_SIZE_MAX (512u)
#define STRING_METADATA_ITEM_SIZE_MAX (128u)

void protocol_serialize(protocol_s protocol, const uint32_t string_len_max, char *string) {
    if((string == NULL) || (string_len_max <=0)) {
        return;
    }

    /* Serialize request field */
    char string_request[STRING_REQUEST_SIZE_MAX] = "";
    (void)memset(string_request, 0, STRING_REQUEST_SIZE_MAX);
    (void)snprintf(string_request, STRING_REQUEST_SIZE_MAX-1, "{\"code\":\"%s\",\"message\":\"%s\"}", PROTOCOL_REQUEST_CODE[protocol.request.code], protocol.request.message);

    /* Serialize data field */
    char string_list_data[STRING_DATA_LIST_SIZE_MAX] = "";
    (void)memset(string_list_data, 0, STRING_DATA_LIST_SIZE_MAX);
    if(protocol.data_nb > 0) {
        for(uint8_t i=0; i< protocol.data_nb; i++) {
            char comma = ',';
            /* Remove comma if it's the last element*/
            if(i == (protocol.data_nb - 1)) {
                comma = '\0';
            }
            char string_data[STRING_DATA_ITEM_SIZE_MAX] = "";
            (void)snprintf(string_data, STRING_DATA_ITEM_SIZE_MAX-1, "{\"name\":\"%s\",\"value\":%.1f,\"unit\":\"%s\"}%c",
                protocol.data[i].name, protocol.data[i].value, protocol.data[i].unit, comma);
            (void)strncat(string_list_data, string_data, STRING_DATA_LIST_SIZE_MAX-1);
        }
    }

    /* Serialize metadata field */
    char string_list_metadata[STRING_METADATA_LIST_SIZE_MAX] = "";
    if(protocol.metadata_nb > 0) {
        for(uint8_t i=0; i< protocol.metadata_nb; i++) {
            char comma = ',';
            /* Remove comma if it's the last element*/
            if(i == (protocol.metadata_nb - 1)) {
                comma = '\0';
            }
            char string_metadata[STRING_METADATA_ITEM_SIZE_MAX] = "";
            (void)snprintf(string_metadata, STRING_METADATA_ITEM_SIZE_MAX-1, "{\"name\":\"%s\",\"value\":\"%s\"}%c",
                protocol.metadata[i].name, protocol.metadata[i].value, comma);
            (void)strncat(string_list_metadata, string_metadata, STRING_METADATA_LIST_SIZE_MAX-1);
        }
    }

    /* Fillin final buffer */
    (void)snprintf(string, string_len_max, "{\"request\":%s,\"data\":[%s],\"metadata\":[%s]}\r\n", string_request, string_list_data, string_list_metadata);
}
