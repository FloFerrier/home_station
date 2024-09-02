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

void HAL_MspInit(void); // Use on HAL_Init() function
void SysTick_Handler(void);
void Error_Handler(void);
void Warning_Handler(void);

int main(void) {
    (void)HAL_Init();
    led_init();
    (void)led_setState(LED_ID_GREEN, LED_STATE_ON);

    if(console_init() != true) {
        Error_Handler();
    }
    if(sensor_init() != SENSOR_OK) {
        Warning_Handler();
    }

    if(xTaskCreate(shell_task, "shell", 1024u, NULL, tskIDLE_PRIORITY, NULL) != pdPASS) {
        Error_Handler();
    }

    (void)console_send("Hello world\r\n");

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

void Error_Handler(void) {
    (void)led_setState(LED_ID_RED, LED_STATE_ON);
    (void)led_setState(LED_ID_GREEN, LED_STATE_OFF);
}

void Warning_Handler(void) {
    (void)led_setState(LED_ID_RED, LED_STATE_ON);
    (void)led_setState(LED_ID_GREEN, LED_STATE_ON);
}
