/**
 * @file
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

typedef enum {
    PROTOCOL_REQUEST_CODE_UNKNOWN = 0,  // Keep per default
    PROTOCOL_REQUEST_CODE_FAILURE = 1,
    PROTOCOL_REQUEST_CODE_SUCCESS = 2,
} protocol_request_code_e;

#define PROTOCOL_REQUEST_MESSAGE_SIZE_MAX (32u)
typedef struct {
    protocol_request_code_e code;
    char message[PROTOCOL_REQUEST_MESSAGE_SIZE_MAX];
} protocol_request_s;

#define PROTOCOL_DATA_NAME_SIZE_MAX (16u)
#define PROTOCOL_DATA_UNIT_SIZE_MAX (16u)
typedef struct {
    char name[PROTOCOL_DATA_NAME_SIZE_MAX];
    float value;
    char unit[PROTOCOL_DATA_UNIT_SIZE_MAX];
} protocol_data_s;

#define PROTOCOL_METADATA_NAME_SIZE_MAX (32u)
#define PROTOCOL_METADATA_VALUE_SIZE_MAX (32u)
typedef struct {
    char name[PROTOCOL_METADATA_NAME_SIZE_MAX];
    char value[PROTOCOL_METADATA_VALUE_SIZE_MAX];
} protocol_metadata_s;

#define PROTOCOL_DATA_NB_MAX (3u)
#define PROTOCOL_METADATA_NB_MAX (2u)
typedef struct {
    protocol_request_s request;
    uint8_t data_nb;
    protocol_data_s data[PROTOCOL_DATA_NB_MAX];
    uint8_t metadata_nb;
    protocol_metadata_s metadata[PROTOCOL_METADATA_NB_MAX];
} protocol_s;

void protocol_serialize(protocol_s data, const uint32_t string_len_max,
                        char *string);

#endif /* PROTOCOL_H */