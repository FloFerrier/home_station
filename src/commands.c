#include "commands.h"

void command_sensorSelfTest(uint32_t argc, char *argv[]) {
    (void)argc;
    (void)argv;
    console_send("> sensor_selfTest command done !\r\n");
}

void command_sensorGetData(uint32_t argc, char *argv[]) {
    (void)argc;
    (void)argv;
    console_send("> sensor_getData command done !\r\n");
}