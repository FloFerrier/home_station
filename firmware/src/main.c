#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#include <FreeRTOS.h>
#include <task.h>
#include <stm32f4xx_hal.h>

#include "shell.h"
#include "sensor.h"
#include "console.h"
#include "led.h"
#include "fsm.h"

extern TaskHandle_t fsm_task_handle;

void HAL_MspInit(void); // Use on HAL_Init() function
void SysTick_Handler(void);

int main(void) {
    /* Minimal to setup FSM */
    (void)HAL_Init();

    /* Must keep the most priority */
    (void)xTaskCreate(fsm_task, "fsm", 1024u, NULL, (configMAX_PRIORITIES - 1), &fsm_task_handle);

    vTaskStartScheduler();

    return 0;
}

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}

void SysTick_Handler(void) {
    HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
#endif /* INCLUDE_xTaskGetSchedulerState */
        xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1)
    }
#endif /* INCLUDE_xTaskGetSchedulerState */
}
