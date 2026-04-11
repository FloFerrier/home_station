#ifndef FREERTOS_H
#define FREERTOS_H

#include <stdint.h>

#define pdPASS (1)
#define pdFAIL (0)
#define pdTRUE (1)
#define pdFALSE (0)
#define portMAX_DELAY (0xFFFFFFFFUL)
#define tskIDLE_PRIORITY (0U)

typedef void *TaskHandle_t;
typedef void *QueueHandle_t;
typedef void *SemaphoreHandle_t;
typedef uint32_t TickType_t;
typedef int32_t BaseType_t;
typedef uint32_t UBaseType_t;
typedef void (*TaskFunction_t)(void *);

#define vTaskNotifyGive(...)
#define configMINIMAL_STACK_SIZE (128U)

void vTaskDelay(const TickType_t xTicksToDelay);
void vTaskDelayUntil(TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement);
void vTaskSuspend(TaskHandle_t xTaskToSuspend);
void vTaskResume(TaskHandle_t xTaskToResume);
TaskHandle_t xTaskGetCurrentTaskHandle(void);
void vTaskNotifyGiveFromISR(TaskHandle_t xTaskToNotify,
                            int32_t *pxHigherPriorityTaskWoken);
uint32_t ulTaskNotifyTake(uint32_t xClearCountOnExit, uint32_t xTicksToWait);
void portYIELD_FROM_ISR(int32_t xYield);
BaseType_t xTaskCreate(TaskFunction_t pvTaskCode, const char *pcName,
                       uint16_t usStackDepth, void *pvParameters,
                       UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask);
void vTaskDelete(TaskHandle_t xTaskToDelete);

#endif /* FREERTOS_H */
