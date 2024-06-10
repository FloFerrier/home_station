#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

void shellCommand_sensorSelfTest(uint32_t argc, char *argv[]);
void shellCommand_sensorGetData(uint32_t argc, char *argv[]);

#endif  /* COMMANDS_H */