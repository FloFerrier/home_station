#include "shell.h"
#include "console.h"
#include "commands.h"
#include <ctype.h>
#include <string.h>

#ifndef TEST
#define FOREVER() (1u)
#define STATIC static
#else
int loopCnt;
#define FOREVER() (loopCnt--)
#define STATIC
#endif // TEST

#define COMMAND_STRING_LEN_MAX (255u)

STATIC shell_s shell = {0};

void shell_task(void *params) {
    (void)params;

    (void)memset(shell.buffer, '\0', RING_BUFFER_LEN_MAX);
    shell.index = 0u;

    do {
        static char character = 0;
        static bool isSuccess = true;
        static bool cmdIsAvailable = false;
        static char command_string[COMMAND_STRING_LEN_MAX+1] = "";
        static uint32_t command_len = 0;
        isSuccess = console_receive(&character);
        if(isSuccess == true) {
            if(isprint(character) != 0) {
                shell.buffer[shell.index++] = character;
            }
            else if(character == '\r' || character == '\n'){
                cmdIsAvailable = true;
                shell.buffer[shell.index] = '\0';
                command_len = (uint32_t)strnlen(shell.buffer, COMMAND_STRING_LEN_MAX);
                (void)strncpy(command_string, shell.buffer, command_len + 1);
                shell.index = 0u;
            }
        }
        if(cmdIsAvailable == true) {
            cmdIsAvailable = false;
            int8_t cmdIndex = shell_commandIndex(command_string, command_len);
            if(cmdIndex >= 0) {
                shell_commands[cmdIndex].handler(0, NULL);
            }
            else {
                console_send("> Unknown command\r\n");
            }
        }
    } while(FOREVER());
}