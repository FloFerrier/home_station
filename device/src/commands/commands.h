#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    COMMAND_UNKNOWN, // Must keep on first
    COMMAND_HELP,
    COMMAND_REBOOT,
    COMMAND_SENSOR_SELF_TEST,
    COMMAND_SENSOR_GET_DATA,
    COMMAND_SYSTEM_GET_INFOS,
    COMMAND_INDEX_MAX // Must keep on last
} command_index_e;

command_index_e command_getIndex(const char *cmd);
void command_execute(command_index_e command_index, const uint32_t response_len_max, char *response);

#endif  /* COMMANDS_H */