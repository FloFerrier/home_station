#ifndef TEST_MOCK_STM32F4X_H
#define TEST_MOCK_STM32F4X_H

#include <stdint.h>

typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

#define GPIO_PIN_0 (0u)
#define GPIO_PIN_1 (0u)
#define GPIO_PIN_2 (0u)
#define GPIO_PIN_3 (0u)
#define GPIO_PIN_8 (0u)
#define GPIO_PIN_9 (0u)
#define GPIO_MODE_OUTPUT_OD (0u)
#define GPIO_MODE_AF_PP (0u)
#define GPIO_MODE_AF_OD (0u)
#define GPIO_NOPULL (0u)
#define GPIO_SPEED_FREQ_LOW (0u)
#define GPIO_SPEED_FREQ_VERY_HIGH (0u)

typedef struct {
    uint32_t dummy;
} GPIO_TypeDef;

extern GPIO_TypeDef mock_GPIOA;
#define GPIOA (&mock_GPIOA)

extern GPIO_TypeDef mock_GPIOB;
#define GPIOB (&mock_GPIOB)

extern GPIO_TypeDef mock_GPIOC;
#define GPIOC (&mock_GPIOC)

typedef struct {
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t Alternate;
} GPIO_InitTypeDef;

typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET
} GPIO_PinState;

typedef struct {
    uint32_t dummy;
} I2C_TypeDef;

extern I2C_TypeDef mock_I2C1;
#define I2C1 (&mock_I2C1)

#define GPIO_AF4_I2C1 (0u)

#define I2C_DUTYCYCLE_2 (0u)
#define I2C_ADDRESSINGMODE_7BIT (0u)
#define I2C_DUALADDRESS_DISABLE (0u)
#define I2C_GENERALCALL_DISABLE (0u)
#define I2C_NOSTRETCH_DISABLE (0u)

typedef struct {
  uint32_t ClockSpeed;
  uint32_t DutyCycle;
  uint32_t OwnAddress1;
  uint32_t AddressingMode;
  uint32_t DualAddressMode;
  uint32_t OwnAddress2;
  uint32_t GeneralCallMode;
  uint32_t NoStretchMode;
} I2C_InitTypeDef;

typedef struct {
    I2C_TypeDef *Instance;
    I2C_InitTypeDef Init;  
} I2C_HandleTypeDef;

void mock_assert_call_HAL_RCC_GPIOA_CLK_ENABLE(void);
void __HAL_RCC_GPIOA_CLK_ENABLE(void);

void mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE(void);
void __HAL_RCC_GPIOB_CLK_ENABLE(void);

void mock_assert_call_HAL_RCC_I2C1_CLK_ENABLE(void);
void __HAL_RCC_I2C1_CLK_ENABLE(void);

void mock_assert_call_HAL_RCC_USART2_CLK_ENABLE(void);
void __HAL_RCC_USART2_CLK_ENABLE(void);

void mock_assert_call_HAL_RCC_GPIOC_CLK_ENABLE(void);
void __HAL_RCC_GPIOC_CLK_ENABLE(void);

void mock_assert_call_HAL_GPIO_Init(void);
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);

void mock_assert_call_HAL_GPIO_WritePin(GPIO_PinState PinState);
void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

void mock_assert_call_HAL_I2C_Init(HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);

void mock_assert_call_HAL_I2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void mock_assert_call_HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void mock_assert_call_HAL_Delay(uint32_t Delay);
void HAL_Delay(uint32_t Delay);

#define GPIO_AF7_USART2 (0u)
#define UART_WORDLENGTH_8B (0u)
#define UART_STOPBITS_1 (0u)
#define UART_PARITY_NONE (0u)
#define UART_MODE_TX_RX (0u)
#define UART_HWCONTROL_NONE (0u)
#define UART_OVERSAMPLING_16 (0u)

typedef struct {
    uint32_t dummy;
} USART_TypeDef;

extern USART_TypeDef mock_USART2;
#define USART2 (&mock_USART2)

typedef uint32_t HAL_UART_RxTypeTypeDef;

typedef struct {
  uint32_t BaudRate;
  uint32_t WordLength;
  uint32_t StopBits;
  uint32_t Parity;
  uint32_t Mode;
  uint32_t HwFlowCtl;
  uint32_t OverSampling;
} UART_InitTypeDef;

typedef struct __UART_HandleTypeDef {
    USART_TypeDef *Instance;
    UART_InitTypeDef Init;
} UART_HandleTypeDef;

void mock_assert_call_HAL_UART_Init(HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);

void mock_assert_call_HAL_UART_Transmit(const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout);

void mock_assert_call_HAL_UART_Receive(const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void mock_assert_call_NVIC_SystemReset(void);
void NVIC_SystemReset(void);

#endif  // TEST_MOCK_STM32F4X_H