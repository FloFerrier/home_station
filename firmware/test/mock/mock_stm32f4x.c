#include "mock_stm32f4x.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "cmocka.h"

GPIO_TypeDef mock_GPIOA = {0};
GPIO_TypeDef mock_GPIOB = {0};
GPIO_TypeDef mock_GPIOC = {0};
I2C_TypeDef mock_I2C1 = {0};
USART_TypeDef mock_USART2 = {0};

void mock_assert_call_HAL_RCC_GPIOA_CLK_ENABLE(void) {
    expect_function_call(__HAL_RCC_GPIOA_CLK_ENABLE);
}
void __HAL_RCC_GPIOA_CLK_ENABLE(void) {
    function_called();
}

void mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE(void) {
    expect_function_call(__HAL_RCC_GPIOB_CLK_ENABLE);
}
void __HAL_RCC_GPIOB_CLK_ENABLE(void) {
    function_called();
}

void mock_assert_call_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    expect_function_call(__HAL_RCC_GPIOC_CLK_ENABLE);
}
void __HAL_RCC_GPIOC_CLK_ENABLE(void) {
    function_called();
}

void mock_assert_call_HAL_RCC_I2C1_CLK_ENABLE(void) {
    expect_function_call(__HAL_RCC_I2C1_CLK_ENABLE);
}
void __HAL_RCC_I2C1_CLK_ENABLE(void) {
    function_called();
}

void mock_assert_call_HAL_RCC_USART2_CLK_ENABLE(void) {
    expect_function_call(__HAL_RCC_USART2_CLK_ENABLE);
}
void __HAL_RCC_USART2_CLK_ENABLE(void) {
    function_called();
}

void mock_assert_call_HAL_GPIO_Init(void) {
    expect_function_call(HAL_GPIO_Init);
}
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
    function_called();
    assert_non_null(GPIOx);
    assert_non_null(GPIO_Init);
}

void mock_assert_call_HAL_GPIO_WritePin(GPIO_PinState PinState) {
    expect_function_call(HAL_GPIO_WritePin);
    expect_value(HAL_GPIO_WritePin, PinState, PinState);
}
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) {
    function_called();
    assert_non_null(GPIOx);
    (void)GPIO_Pin;
    check_expected(PinState);
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

void mock_assert_call_HAL_UART_Init(HAL_StatusTypeDef ret) {
    expect_function_call(HAL_UART_Init);
    will_return(HAL_UART_Init, ret);
}
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart) {
    function_called();
    assert_non_null(huart);
    return mock();
}

void mock_assert_call_HAL_UART_Transmit(const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret) {
    expect_function_call(HAL_UART_Transmit);
    assert_non_null(pData);
    expect_value(HAL_UART_Transmit, Size, Size);
    expect_value(HAL_UART_Transmit, Timeout, Timeout);
    will_return(HAL_UART_Transmit, pData);
    will_return(HAL_UART_Transmit, ret);
}
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    function_called();
    assert_non_null(huart);
    assert_non_null(pData);
    check_expected(Size);
    check_expected(Timeout);
    uint8_t *expected_data = mock_ptr_type(uint8_t *);
    int expected_len = strlen((char*)expected_data);
    assert_int_equal(Size, expected_len);
    assert_string_equal(pData, expected_data);
    return mock();
}

void mock_assert_call_HAL_UART_Receive(const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret) {
    expect_function_call(HAL_UART_Receive);
    assert_non_null(pData);
    expect_value(HAL_UART_Receive, Size, Size);
    expect_value(HAL_UART_Receive, Timeout, Timeout);
    will_return(HAL_UART_Receive, pData);
    will_return(HAL_UART_Receive, ret);
}
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    function_called();
    assert_non_null(huart);
    assert_non_null(pData);
    check_expected(Size);
    check_expected(Timeout);
    uint8_t *expected_data = mock_ptr_type(uint8_t *);
    memcpy(pData, expected_data, Size * sizeof(uint8_t));
    return mock();
}