#ifndef SENSOR_H
#define SENSOR_H

#include <stdbool.h>

bool sensor_init(void);
bool sensor_selfTest(void);
bool sensor_getData(void);

#endif  /* SENSOR_H */