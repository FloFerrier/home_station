#include "shell.h"
#include "console.h"
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

typedef struct {
  const char *name;
  void (*handler)(uint32_t argc, char *argv[]);
  const char *desc;
} shell_command_s;

STATIC bool shell_cmd_isFound(char *cmd, uint32_t cmd_size);
STATIC void shell_cmd_dataGet(uint32_t argc, char *argv[]);

STATIC const shell_command_s shell_commands[] = {
  {"data_get", shell_cmd_dataGet, "Display data measurements"},
};

STATIC bool shell_cmd_isFound(char *cmd, uint32_t cmd_size) {
    (void)cmd_size;
    for(uint32_t i=0; i < sizeof(shell_commands)/sizeof(shell_commands[0]); i++) {
        uint32_t cmp = strncmp(cmd, shell_commands[i].name, 255);
        if(cmp == 0) {
            return true;
        }
    }

    return false;
}

STATIC void shell_cmd_dataGet(uint32_t argc, char *argv[]) {
    (void)argc;
    (void)argv;
    console_send("> SUCCESS\r\n");
}

void shell_task(void *params) {
    (void)params;

    console_init();
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
            bool cmdIsFound = shell_cmd_isFound(command_string, command_len);
            if(cmdIsFound == true) {
                shell_commands[0].handler(0, NULL);
            }
            else {
                console_send("> Unknown command\r\n");
            }
        }
    } while(FOREVER());
}