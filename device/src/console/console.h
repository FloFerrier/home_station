#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>

bool console_init(void);
bool console_send(const char* format, ...);
bool console_receive(char* character);

#endif /* CONSOLE_H */