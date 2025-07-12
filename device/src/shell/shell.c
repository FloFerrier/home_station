/**
 * @file
 */

#include "shell.h"

#include <ctype.h>
#include <string.h>

#include "commands.h"
#include "console.h"

#ifndef TEST
#define FOREVER() (1u)
#define STATIC static
#else
int loopCnt;
#define FOREVER() (loopCnt--)
#define STATIC
#endif  // TEST

#define COMMAND_STRING_LEN_MAX (255u)
#define RING_BUFFER_LEN_MAX (255u)

#define SHELL_RESPONSE_SIZE_MAX (2048u)

typedef struct {
    char buffer[RING_BUFFER_LEN_MAX + 1];
    uint32_t index;
} shell_s;
STATIC shell_s shell = {0};

STATIC bool character_isPrintable(char character) {
    return (bool)isprint(character);
}

STATIC bool character_isEndOfLine(char character) {
    return (bool)(character == '\r' || character == '\n');
}

void shell_task(void *params) {
    (void)params;

    (void)memset(shell.buffer, '\0', RING_BUFFER_LEN_MAX);
    shell.index = 0u;
    static char command_string[COMMAND_STRING_LEN_MAX + 1] = "";

    do {
        bool command_is_available = false;
        uint32_t command_len = 0;
        char character = 0;
        bool reception_is_success = console_receive(&character);
        if (reception_is_success == true) {
            if (character_isPrintable(character) == true) {
                shell.buffer[shell.index++] = character;
            } else if (character_isEndOfLine(character) == true) {
                command_is_available = true;
                shell.buffer[shell.index] = '\0';
                command_len =
                    (uint32_t)strnlen(shell.buffer, COMMAND_STRING_LEN_MAX);
                (void)strncpy(command_string, shell.buffer, command_len + 1);
                shell.index = 0u;
            }
        }
        if (command_is_available == true) {
            command_index_e command_index = command_getIndex(command_string);
            static char response[SHELL_RESPONSE_SIZE_MAX] = "";
            command_execute(command_index, SHELL_RESPONSE_SIZE_MAX - 1,
                            response);
            console_send(response);
            command_is_available = false;
        }
    } while (FOREVER());
}