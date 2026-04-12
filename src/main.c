/**
 * @file
 */

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "console.h"
#include "fsm.h"
#include "stm32f4xx_hal.h"
#include "task.h"

extern TaskHandle_t fsm_task_handle;

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

void HAL_MspInit(void);  // Use on HAL_Init() function
void SysTick_Handler(void);

int main(void) {
    /* Minimal to setup FSM */
    (void)HAL_Init();
    (void)console_init();
    console_send("[LOG] Start bootloader ...\r\n");
    console_send("[LOG] Console debug is available.\r\n");

    /* Must keep the most priority */
    (void)xTaskCreate(fsm_task, "fsm", 256u, NULL, (configMAX_PRIORITIES - 1),
                      &fsm_task_handle);

    vTaskStartScheduler();

    return 0;
}

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}

volatile uint32_t RTOS_runTimeCounter;

void SysTick_Handler(void) {
    HAL_IncTick();
    RTOS_runTimeCounter++;
#if (INCLUDE_xTaskGetSchedulerState == 1)
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
#endif /* INCLUDE_xTaskGetSchedulerState */
        xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1)
    }
#endif /* INCLUDE_xTaskGetSchedulerState */
}

void RTOS_configureTimerForRuntimeStats(void) { RTOS_runTimeCounter = 0; }

uint32_t RTOS_getRuntimeCounterValue(void) { return RTOS_runTimeCounter; }

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    console_send("[LOG] Stack overflow detected on %s\r\n", pcTaskName);
    configASSERT(0); // Blocking here, useful for debugging
}