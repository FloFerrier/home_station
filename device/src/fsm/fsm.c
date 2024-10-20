#include "fsm.h"

#include <stdbool.h>

#include "console.h"
#include "led.h"
#include "sensor.h"
#include "shell.h"

#ifndef TEST
#include <FreeRTOS.h>
#include <task.h>
#include <stm32f4xx_hal.h>
#define LOOP (1u)
#define STATIC static
#else
#include "mock_stm32f4x.h"
#include "mock_freertos.h"
#define LOOP (0u)
#define STATIC
#endif // TEST

STATIC fsm_state_e fsm_state = FSM_STATE_INIT;
TaskHandle_t fsm_task_handle = NULL;

STATIC fsm_state_e fsm_state_init(void);

STATIC fsm_state_e fsm_state_init(void) {
    /* BSP Initialization*/
    led_init();
    (void)led_setState(LED_ID_GREEN, LED_STATE_ON);

    if(console_init() != true) {
        return FSM_STATE_ERROR;
    }

    (void)sensor_init();

    /* Middleware Initialization*/
    if(xTaskCreate(shell_task, "shell", 1024u, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {
        return FSM_STATE_ERROR;
    }

    return FSM_STATE_RUNNING;
}

void fsm_task(void *params) {
    (void)params;

    do {
        switch (fsm_state)
        {
            case FSM_STATE_INIT: {
                fsm_state_e next_state = fsm_state_init();
                fsm_requestNewState(next_state);
            } break;
            case FSM_STATE_RUNNING: {
                (void)led_setState(LED_ID_GREEN, LED_STATE_ON);
                /* The system is running */
            } break;
            case FSM_STATE_REBOOTING: {
                NVIC_SystemReset();
                /* Next step is bootloader managed by hardware */
            } break;
            case FSM_STATE_ERROR:
            default: {
                (void)led_setState(LED_ID_RED, LED_STATE_ON);
                (void)led_setState(LED_ID_GREEN, LED_STATE_OFF);
                /* The system must be blocking here */
            } break;
        }
        (void)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    } while(LOOP);
}

STATIC bool checkTransitionIsAllowed(fsm_state_e next_state) {
    fsm_state_e current_state = fsm_state;
    bool result = false;
    if(current_state == FSM_STATE_INIT) {
        if((next_state == FSM_STATE_RUNNING) || (next_state == FSM_STATE_ERROR)) {
            result = true;
        }
    }
    else if(current_state == FSM_STATE_RUNNING) {
        if((next_state == FSM_STATE_REBOOTING) || (next_state == FSM_STATE_ERROR)) {
            result = true;
        }
    }

    return result;
}

void fsm_requestNewState(fsm_state_e new_state) {
    /* Check if transition is allowed */
    if(checkTransitionIsAllowed(new_state) == true) {
        fsm_state = new_state;
        (void)xTaskNotifyGive(fsm_task_handle);
    }
}
