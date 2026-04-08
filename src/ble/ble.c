/**
 * @file
 */

#include <string.h>
#include <stdio.h>

#include "ble.h"
#include "console.h" // TMP: only for logging

#ifndef TEST
#include <stm32f4xx_hal.h>
#include <FreeRTOS.h>
#include <task.h>
#define STATIC static
#else
#include "mock_freertos.h"
#include "mock_stm32f4x.h"
#define STATIC
#endif  // TEST

#define RN4871_RX_BUFFER_SIZE (256u)

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

extern TaskHandle_t rn4871_task_handle;

static uint8_t rn4871_rx_buffer[RN4871_RX_BUFFER_SIZE] = "";

static char log_msg[RN4871_RX_BUFFER_SIZE] = "";

static bool ble_init(void) {

    /* GPIO Init */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef gpioInit = {
        .Pin = GPIO_PIN_10 | GPIO_PIN_11,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF7_USART3,
    };
    (void)HAL_GPIO_Init(GPIOC, &gpioInit);

    /* DMA Init */
    __HAL_RCC_DMA1_CLK_ENABLE();
    hdma_usart3_rx.Instance = DMA1_Stream1;
    hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    (void) HAL_DMA_Init(&hdma_usart3_rx);
    __HAL_LINKDMA(&huart3, hdmarx, hdma_usart3_rx);
    hdma_usart3_tx.Instance = DMA1_Stream3;
    hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    (void) HAL_DMA_Init(&hdma_usart3_tx);
    __HAL_LINKDMA(&huart3, hdmatx, hdma_usart3_tx);
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 5);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 5);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 5);
    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

    /* USART Init*/
    __HAL_RCC_USART3_CLK_ENABLE();
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_StatusTypeDef halStatus = HAL_UART_Init(&huart3);
    return ((halStatus == HAL_OK) ? true : false);
}

void ble_task(void *params) {
    (void)params;

    (void)ble_init();

    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rn4871_rx_buffer, RN4871_RX_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT(huart3.hdmarx, DMA_IT_HT);

    uint16_t head = 0;
    uint16_t length = 0;
    uint16_t old_pos = 0;

    while(1) {
        if (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) > 0) {
            int length = strnlen((char*) rn4871_rx_buffer, RN4871_RX_BUFFER_SIZE);
            if( length > 0 && length < RN4871_RX_BUFFER_SIZE) {
                console_send("%s", rn4871_rx_buffer);
            }
            rn4871_rx_buffer[0] = 0;
            (void) HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rn4871_rx_buffer, RN4871_RX_BUFFER_SIZE);
        }
    }
}

void ble_sendCmdMode(void) {
    const uint8_t command[] = "$";
    size_t length = strlen((char*)command);
    HAL_Delay(100); // Wait 100 ms
    for(int i=0; i<3; i++) {
        (void) HAL_UART_Transmit_DMA(&huart3, command, length);
        HAL_Delay(100); // Wait 100 ms
    }
}

void ble_sendVersion(void) {
    const uint8_t command[] = "V\r\n";
    size_t length = strlen((char*)command);
    (void) HAL_UART_Transmit_DMA(&huart3, command, length);
}

void ble_sendFactoryReset(void) {
    const uint8_t command[] = "SF,1\r\n";
    size_t length = strlen((char*)command);
    (void) HAL_UART_Transmit_DMA(&huart3, command, length);
}

void ble_sendReboot(void) {
    const uint8_t command[] = "R,1\r\n";
    size_t length = strlen((char*)command);
    (void) HAL_UART_Transmit_DMA(&huart3, command, length);
}

void ble_sendResetServices(void) {
    const uint8_t command[] = "SS,00\r\n";
    size_t length = strlen((char*)command);
    (void) HAL_UART_Transmit_DMA(&huart3, command, length);
}

void ble_sendAdvertising(void) {
    const uint8_t command[] = "A\r\n";
    size_t length = strlen((char*)command);
    (void) HAL_UART_Transmit_DMA(&huart3, command, length);
}

void ble_sendFakePacket(void) {
    const uint8_t command[] = "IA,01,0201060816D2FC40026A09\r\n";
    size_t length = strlen((char*)command);
    (void) HAL_UART_Transmit_DMA(&huart3, command, length);
}

void DMA1_Stream1_IRQHandler(void) {
    HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

void DMA1_Stream3_IRQHandler(void) {
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

void USART3_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart3);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART3) {
        rn4871_rx_buffer[Size] = 0;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (rn4871_task_handle != NULL) {
            vTaskNotifyGiveFromISR(rn4871_task_handle, &xHigherPriorityTaskWoken);
        }
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
