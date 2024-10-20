#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_MESSAGE_LEN_MAX (255u)
#define PROTOCOL_RESPONSE_LEN_MAX (255u)

typedef enum {
    PROTOCOL_CODE_FAILURE = 0,
    PROTOCOL_CODE_SUCCESS = 1,
} protocol_code_e;

typedef struct {
    protocol_code_e code;
    char message[PROTOCOL_MESSAGE_LEN_MAX+1];
    char response[PROTOCOL_RESPONSE_LEN_MAX+1];
} protocol_s;

void protocol_serialize(protocol_s data, const uint32_t string_len_max, char *string);

#endif  /* PROTOCOL_H */