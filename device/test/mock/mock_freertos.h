#ifndef TEST_MOCK_FREERTOS_H
#define TEST_MOCK_FREERTOS_H

#include <stdint.h>

typedef long BaseType_t;
typedef unsigned long UBaseType_t;

struct tskTaskControlBlock;
typedef struct tskTaskControlBlock *TaskHandle_t;

/* Used for configUSE_16_BIT_TICKS == 0 */
typedef uint32_t TickType_t;
#define portMAX_DELAY (TickType_t) 0xffffffffUL

#define pdFALSE ((BaseType_t) 0)
#define pdTRUE ((BaseType_t) 1)

#define pdPASS (pdTRUE)
#define pdFAIL (pdFALSE)

#define tskIDLE_PRIORITY ((UBaseType_t) 0U)

#define configSTACK_DEPTH_TYPE uint16_t

typedef void (* TaskFunction_t)(void *);

void mock_assert_call_ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait, uint32_t ret);
uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait);

void mock_assert_call_xTaskNotifyGive(TaskHandle_t xTaskToNotify, BaseType_t ret);
BaseType_t xTaskNotifyGive(TaskHandle_t xTaskToNotify);

void mock_assert_call_xTaskCreate(BaseType_t ret);
BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, const char * const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask);

#endif  // TEST_MOCK_FREERTOS_H