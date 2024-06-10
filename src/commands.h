#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

void command_sensorSelfTest(uint32_t argc, char *argv[]);
void command_sensorGetData(uint32_t argc, char *argv[]);

#endif  /* COMMANDS_H */