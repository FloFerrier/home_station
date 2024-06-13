#include "mock_stm32f4x.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

GPIO_TypeDef mock_GPIOB = {0};
I2C_TypeDef mock_I2C1 = {0};

void mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE(void) {
    expect_function_call(__HAL_RCC_GPIOB_CLK_ENABLE);
}
void __HAL_RCC_GPIOB_CLK_ENABLE(void) {
    function_called();
}

void mock_assert_call_HAL_RCC_I2C1_CLK_ENABLE(void) {
    expect_function_call(__HAL_RCC_I2C1_CLK_ENABLE);
}
void __HAL_RCC_I2C1_CLK_ENABLE(void) {
    function_called();
}

void mock_assert_call_HAL_GPIO_Init(void) {
    expect_function_call(HAL_GPIO_Init);
}
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
    function_called();
    (void) GPIOx;
    (void) GPIO_Init;
}

void mock_assert_call_HAL_I2C_Init(HAL_StatusTypeDef ret) {
    expect_function_call(HAL_I2C_Init);
    will_return(HAL_I2C_Init, ret);
}
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c) {
    function_called();
    (void) hi2c;
    return mock();
}

void mock_assert_call_HAL_I2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret) {
    expect_function_call(HAL_I2C_Mem_Read);
    expect_value(HAL_I2C_Mem_Read, DevAddress, DevAddress);
    expect_value(HAL_I2C_Mem_Read, MemAddress, MemAddress);
    expect_value(HAL_I2C_Mem_Read, MemAddSize, MemAddSize);
    expect_value(HAL_I2C_Mem_Read, Size, Size);
    expect_value(HAL_I2C_Mem_Read, Timeout, Timeout);
    assert_non_null(pData);
    will_return(HAL_I2C_Mem_Read, pData);
    will_return(HAL_I2C_Mem_Read, ret);
}
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    function_called();
    assert_non_null(hi2c);
    check_expected(DevAddress);
    check_expected(MemAddress);
    check_expected(MemAddSize);
    check_expected(Size);
    check_expected(Timeout);
    assert_non_null(pData);
    uint8_t *expected_data = mock_ptr_type(uint8_t *);
    memcpy(pData, expected_data, Size * sizeof(uint8_t));
    return mock();
}

void mock_assert_call_HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret) {
    expect_function_call(HAL_I2C_Mem_Write);
    expect_value(HAL_I2C_Mem_Write, DevAddress, DevAddress);
    expect_value(HAL_I2C_Mem_Write, MemAddress, MemAddress);
    expect_value(HAL_I2C_Mem_Write, MemAddSize, MemAddSize);
    expect_value(HAL_I2C_Mem_Write, Size, Size);
    expect_value(HAL_I2C_Mem_Write, Timeout, Timeout);
    assert_non_null(pData);
    expect_memory(HAL_I2C_Mem_Write, pData, pData, Size);
    will_return(HAL_I2C_Mem_Write, ret);
}
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    function_called();
    assert_non_null(hi2c);
    check_expected(DevAddress);
    check_expected(MemAddress);
    check_expected(MemAddSize);
    check_expected(pData);
    check_expected(Size);
    check_expected(Timeout);
    return mock();
}

void mock_assert_call_HAL_Delay(uint32_t Delay) {
    expect_function_call(HAL_Delay);
    expect_value(HAL_Delay, Delay, Delay);
}
void HAL_Delay(uint32_t Delay) {
    function_called();
    check_expected(Delay);
}