#ifndef STM32F4XX_HAL_UART_H
#define STM32F4XX_HAL_UART_H

#include "stm32f4xx_hal_def.h"

#define USART_CR1_M 0x00001000U
#define USART_CR2_STOP_1 0x00002000U
#define USART_CR1_TE 0x00000008U
#define USART_CR1_RE 0x00000004U
#define USART_CR1_PCE 0x00000400U
#define USART_CR1_PS 0x00000200U
#define USART_CR1_OVER8 0x00008000U

#define UART_WORDLENGTH_8B 0x00000000U
#define UART_WORDLENGTH_9B ((uint32_t)USART_CR1_M)

#define UART_STOPBITS_1 0x00000000U
#define UART_STOPBITS_2 ((uint32_t)USART_CR2_STOP_1)

#define UART_PARITY_NONE 0x00000000U
#define UART_PARITY_EVEN ((uint32_t)USART_CR1_PCE)
#define UART_PARITY_ODD ((uint32_t)(USART_CR1_PCE | USART_CR1_PS))

#define UART_MODE_TX_RX (USART_CR1_TE | USART_CR1_RE)
#define UART_HWCONTROL_NONE 0x00000000U

#define UART_OVERSAMPLING_16 0x00000000U
#define UART_OVERSAMPLING_8 ((uint32_t)USART_CR1_OVER8)

typedef struct {
    uint32_t BaudRate;
    uint32_t WordLength;
    uint32_t StopBits;
    uint32_t Parity;
    uint32_t Mode;
    uint32_t HwFlowCtl;
    uint32_t OverSampling;
    uint32_t CLKLastClk;
} UART_InitTypeDef;

typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

#define USART2 ((USART_TypeDef *)0x40004400UL)
#define USART3 ((USART_TypeDef *)0x40004800UL)

typedef struct {
    USART_TypeDef *Instance;
    UART_InitTypeDef Init;
} UART_HandleTypeDef;

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_DeInit(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData,
                                    uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData,
                                   uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Transmit_IT(UART_HandleTypeDef *huart,
                                       uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData,
                                      uint16_t Size);
HAL_StatusTypeDef HAL_UART_Transmit_DMA(UART_HandleTypeDef *huart,
                                        uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UARTEx_ReceiveToIdle_DMA(UART_HandleTypeDef *huart,
                                               uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_DMAStop(UART_HandleTypeDef *huart);

void HAL_UART_IRQHandler(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);

void HAL_UART_MspInit(UART_HandleTypeDef *huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);

#endif /* STM32F4XX_HAL_UART_H */
