#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_REQUEST_MESSAGE_LEN_MAX (256u)

typedef enum {
    PROTOCOL_REQUEST_CODE_FAILURE = 0,
    PROTOCOL_REQUEST_CODE_SUCCESS = 1,
} protocol_request_code_e;

typedef struct {
    protocol_request_code_e code;
    char message[PROTOCOL_REQUEST_MESSAGE_LEN_MAX];
} protocol_request_s;

typedef enum {
    PROTOCOL_DATA_FIELD_TEMPERATURE = 0,
    PROTOCOL_DATA_FIELD_HUMIDITY = 1,
    PROTOCOL_DATA_FIELD_PRESSURE = 2,
} protocol_data_field_e;

typedef enum {
    PROTOCOL_DATA_UNIT_DEGREE_CELSIUS = 0,
    PROTOCOL_DATA_UNIT_PERCENTAGE = 1,
    PROTOCOL_DATA_UNIT_PASCAL = 2,
} protocol_data_unit_e;

typedef struct {
    protocol_data_field_e field;
    float value;
    protocol_data_unit_e unit;
} protocol_data_s;

typedef enum {
    PROTOCOL_METADATA_FIELD_0 = 0,
    PROTOCOL_METADATA_FIELD_1 = 1,
} protocol_metadata_field_e;

typedef enum {
    PROTOCOL_METADATA_VALUE_0 = 0,
    PROTOCOL_METADATA_VALUE_1 = 1,
} protocol_metadata_value_e;

typedef struct {
    protocol_metadata_field_e field;
    protocol_metadata_value_e value;
} protocol_metadata_s;

typedef enum {
    PROTOCOL_DATA_TEMPERATURE = 0,
    PROTOCOL_DATA_HUMIDITY = 1,
    PROTOCOL_DATA_PRESSURE = 2,
} protocol_data_e;

#define PROTOCOL_DATA_NB_MAX (3u)
#define PROTOCOL_METADATA_NB_MAX (2u)

typedef struct {
    protocol_request_s request;
    uint8_t data_nb;
    protocol_data_s data[PROTOCOL_DATA_NB_MAX];
    uint8_t metadata_nb;
    protocol_metadata_s metadata[PROTOCOL_METADATA_NB_MAX];
} protocol_s;

void protocol_serialize(protocol_s data, const uint32_t string_len_max, char *string);

#endif  /* PROTOCOL_H */