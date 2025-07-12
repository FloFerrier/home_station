#ifndef FSM_H
#define FSM_H

#include <stddef.h>

typedef enum {
    FSM_STATE_INIT = 0,
    FSM_STATE_RUNNING = 1,
    FSM_STATE_REBOOTING = 2,
    FSM_STATE_ERROR = 3,
} fsm_state_e;

void fsm_task(void *params);

void fsm_requestNewState(fsm_state_e new_state);

#endif /* FSM_H */