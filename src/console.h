#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>

void console_init(void);
void console_send(const char* format, ...);
bool console_receive(char *character);

#endif  /* CONSOLE_H */