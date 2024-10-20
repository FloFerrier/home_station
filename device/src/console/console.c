#include "console.h"
#include <stdarg.h>
#include <stdio.h>

#ifndef TEST
#include <stm32f4xx_hal.h>
#define STATIC static
#else
#include "mock_stm32f4x.h"
#define STATIC
#endif // TEST

#define UART_BUFFER_LEN_MAX (255u)
#define UART_TIMEOUT_DURATION (1000u)

STATIC char uartBufferTx[UART_BUFFER_LEN_MAX+1] = "";
STATIC UART_HandleTypeDef uartHandle = {0};

/* UART2
 * Tx -> PA2
 * Rx -> PA3
 */
bool console_init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef gpioInit = {
        .Pin = GPIO_PIN_2 | GPIO_PIN_3,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF7_USART2,
    };
    (void)HAL_GPIO_Init(GPIOA, &gpioInit);

    uartHandle.Instance = USART2;
    uartHandle.Init.BaudRate = 115200;
    uartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    uartHandle.Init.StopBits = UART_STOPBITS_1;
    uartHandle.Init.Parity = UART_PARITY_NONE;
    uartHandle.Init.Mode = UART_MODE_TX_RX;
    uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_StatusTypeDef halStatus = HAL_UART_Init(&uartHandle);
    return ((halStatus == HAL_OK) ? true : false);
}

bool console_send(const char* format, ...) {
    va_list va;
    va_start(va, format);
    uint16_t uartBufferTxLen = (uint16_t)vsnprintf(uartBufferTx, UART_BUFFER_LEN_MAX, format, va);
    va_end(va);
    HAL_StatusTypeDef halStatus = HAL_UART_Transmit(&uartHandle, (uint8_t*)uartBufferTx, uartBufferTxLen, UART_TIMEOUT_DURATION);
    return ((halStatus == HAL_OK) ? true : false);
}

bool console_receive(char *character) {
    HAL_StatusTypeDef halStatus = HAL_UART_Receive(&uartHandle, (uint8_t*)character,(uint16_t)1u, UART_TIMEOUT_DURATION);
    return ((halStatus == HAL_OK) ? true : false);
}
