#include "mock_freertos.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

void mock_assert_call_ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait, uint32_t ret) {
    expect_function_call(ulTaskNotifyTake);
    expect_value(ulTaskNotifyTake, xClearCountOnExit, xClearCountOnExit);
    expect_value(ulTaskNotifyTake, xTicksToWait, xTicksToWait);
    will_return(ulTaskNotifyTake, ret);
}
uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait) {
    function_called();
    check_expected(xClearCountOnExit);
    check_expected(xTicksToWait);
    return mock();
}

void mock_assert_call_xTaskNotifyGive(TaskHandle_t xTaskToNotify, BaseType_t ret) {
    expect_function_call(xTaskNotifyGive);
    expect_value(xTaskNotifyGive, xTaskToNotify, xTaskToNotify);
    will_return(xTaskNotifyGive, ret);
}
BaseType_t xTaskNotifyGive(TaskHandle_t xTaskToNotify) {
    function_called();
    check_expected(xTaskToNotify);
    return mock();
}

void mock_assert_call_xTaskCreate(BaseType_t ret) {
    expect_function_call(xTaskCreate);
    will_return(xTaskCreate, ret);
}
BaseType_t xTaskCreate(TaskFunction_t pxTaskCode, const char * const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask) {
    function_called();
    (void) pxTaskCode;
    (void) pcName;
    (void) usStackDepth;
    (void) pvParameters;
    (void) uxPriority;
    (void) pxCreatedTask;
    return mock();
}

void mock_assert_call_vTaskGetRunTimeStats(char* mockWriteBuffer) {
    expect_function_call(vTaskGetRunTimeStats);
    will_return(vTaskGetRunTimeStats, mockWriteBuffer);
}
void vTaskGetRunTimeStats(char* pcWriteBuffer) {
    function_called();
    assert_non_null(pcWriteBuffer);
    char* mockWriteBuffer = mock_ptr_type(char*);
    (void)strncpy(pcWriteBuffer, mockWriteBuffer, strlen(mockWriteBuffer));
}
