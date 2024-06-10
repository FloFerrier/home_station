#include "commands.h"

void shellCommand_sensorSelfTest(uint32_t argc, char *argv[]) {
    (void)argc;
    (void)argv;
    console_send("> sensor_selfTest command done !\r\n");
}

void shellCommand_sensorGetData(uint32_t argc, char *argv[]) {
    (void)argc;
    (void)argv;
    console_send("> sensor_getData command done !\r\n");
}