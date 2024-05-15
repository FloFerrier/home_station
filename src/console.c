#include "console.h"
#include <stdarg.h>
#include <stdio.h>
#include <stm32f4xx_hal.h>

#define UART_BUFFER_LEN_MAX (255u)

static char uartBufferTx[UART_BUFFER_LEN_MAX+1] = "";
static UART_HandleTypeDef uartHandle = {0};

/* UART2
 * Tx -> PA2
 * Rx -> PA3
 */
void console_init(void) {
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
    (void)HAL_UART_Init(&uartHandle);
}

void console_send(const char* format, ...) {
    va_list va;
    va_start(va, format);
    uint16_t uartBufferTxLen = 0;
    uartBufferTxLen = (uint16_t)vsnprintf(uartBufferTx, UART_BUFFER_LEN_MAX, format, va);
    va_end(va);
    (void)HAL_UART_Transmit(&uartHandle, (uint8_t*)uartBufferTx, uartBufferTxLen, (uint32_t)1000u);
}

bool console_receive(char *character) {
    HAL_StatusTypeDef halStatus = HAL_OK;
    halStatus = HAL_UART_Receive(&uartHandle, (uint8_t*)character,(uint16_t)1u, (uint16_t)10u);
    return ((halStatus == HAL_OK) ? true : false);
}
