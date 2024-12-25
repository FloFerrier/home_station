#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_REQUEST_MESSAGE_LEN_MAX (255u)

typedef enum {
    PROTOCOL_REQUEST_CODE_FAILURE = 0,
    PROTOCOL_REQUEST_CODE_SUCCESS = 1,
} protocol_request_code_e;

typedef struct {
    protocol_request_code_e code;
    char message[PROTOCOL_REQUEST_MESSAGE_LEN_MAX+1];
} protocol_request_s;

typedef enum {
    PROTOCOL_DATA_FIELD_0 = 0,
} protocol_data_field_e;

typedef enum {
    PROTOCOL_DATA_UNIT_0 = 0,
} protocol_data_unit_e;

typedef struct {
    protocol_data_field_e field;
    float value;
    protocol_data_unit_e unit;
} protocol_data_s;

typedef enum {
    PROTOCOL_METADATA_FIELD_0 = 0,
} protocol_metadata_field_e;

typedef enum {
    PROTOCOL_METADATA_VALUE_0 = 0,
} protocol_metadata_value_e;

typedef struct {
    protocol_metadata_field_e field;
    protocol_metadata_value_e value;
} protocol_metadata_s;

typedef struct {
    protocol_request_s *request;
    protocol_data_s *data;
    protocol_metadata_s *metadata;
} protocol_s;

void protocol_serialize(protocol_s data, const uint32_t string_len_max, char *string);

#endif  /* PROTOCOL_H */