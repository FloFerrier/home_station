#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LED_ID_BLUE = 0,
    LED_ID_GREEN = 1,
    LED_ID_RED = 2,
} led_id_e;

typedef enum {
    LED_STATE_OFF = 0,
    LED_STATE_ON = 1,
} led_state_e;

void led_init(void);
bool led_setState(led_id_e id, led_state_e state);

#endif  /* LED_H */