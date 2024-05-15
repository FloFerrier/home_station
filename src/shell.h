#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

#define RING_BUFFER_LEN_MAX (255u)

typedef struct {
    char buffer[RING_BUFFER_LEN_MAX+1];
    uint32_t index;
} shell_s;

void shell_task(void *params);

#endif  /* SHELL_H */