#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    COMMAND_UNKNOWN = 0,
    COMMAND_HELP = 1,
    COMMAND_SENSOR_SELF_TEST = 2,
    COMMAND_SENSOR_GET_DATA = 3,
} command_index_e;

command_index_e command_getIndex(const char *cmd);
bool command_execute(command_index_e command_index);

#endif  /* COMMANDS_H */