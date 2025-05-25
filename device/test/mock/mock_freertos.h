#ifndef TEST_MOCK_FREERTOS_H
#define TEST_MOCK_FREERTOS_H

#include <stdint.h>
#include <stddef.h>

#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short
#define portSTACK_TYPE uint32_t
#define portBASE_TYPE long

typedef portSTACK_TYPE StackType_t;

/* Used for configUSE_16_BIT_TICKS == 0 */
typedef uint32_t TickType_t;
#define portMAX_DELAY (TickType_t) 0xffffffffUL

#define pdFALSE ((BaseType_t) 0)
#define pdTRUE ((BaseType_t) 1)

#define pdPASS (pdTRUE)
#define pdFAIL (pdFALSE)

#define tskIDLE_PRIORITY ((UBaseType_t) 0U)

#define configSTACK_DEPTH_TYPE uint16_t
#define configRUN_TIME_COUNTER_TYPE uint32_t

typedef long BaseType_t;
typedef unsigned long UBaseType_t;

struct tskTaskControlBlock;
typedef struct tskTaskControlBlock *TaskHandle_t;

typedef enum {
    eRunning = 0,
    eReady,
    eBlocked,
    eSuspended,
    eDeleted,
    eInvalid
} eTaskState;

typedef struct xTASK_STATUS {
    TaskHandle_t xHandle;
    const char * pcTaskName;
    UBaseType_t xTaskNumber;
    eTaskState eCurrentState;
    UBaseType_t uxCurrentPriority;
    UBaseType_t uxBasePriority;
    configRUN_TIME_COUNTER_TYPE ulRunTimeCounter;
    StackType_t * pxStackBase;
    configSTACK_DEPTH_TYPE usStackHighWaterMark;
} TaskStatus_t;

typedef void (* TaskFunction_t)(void *);

void mock_assert_call_ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait, uint32_t ret);
uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait);

void mock_assert_call_xTaskNotifyGive(TaskHandle_t xTaskToNotify, BaseType_t ret);
BaseType_t xTaskNotifyGive(TaskHandle_t xTaskToNotify);

void mock_assert_call_xTaskCreate(BaseType_t ret);
BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, const char * const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask);

void mock_assert_call_vTaskGetRunTimeStats(char* mockWriteBuffer);
void vTaskGetRunTimeStats(char* pcWriteBuffer);

#endif  // TEST_MOCK_FREERTOS_H