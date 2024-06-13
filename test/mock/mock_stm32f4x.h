#ifndef TEST_MOCK_STM32F4X_H
#define TEST_MOCK_STM32F4X_H

#include <stdint.h>

typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

#define GPIO_PIN_8 (0u)
#define GPIO_PIN_9 (0u)
#define GPIO_MODE_AF_OD (0u)
#define GPIO_NOPULL (0u)
#define GPIO_SPEED_FREQ_VERY_HIGH (0u)

typedef struct {
    uint32_t dummy;
} GPIO_TypeDef;

extern GPIO_TypeDef mock_GPIOB;
#define GPIOB (&mock_GPIOB)

typedef struct {
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t Alternate;
} GPIO_InitTypeDef;

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

void mock_assert_call_HAL_RCC_GPIOB_CLK_ENABLE(void);
void __HAL_RCC_GPIOB_CLK_ENABLE(void);

void mock_assert_call_HAL_RCC_I2C1_CLK_ENABLE(void);
void __HAL_RCC_I2C1_CLK_ENABLE(void);

void mock_assert_call_HAL_GPIO_Init(void);
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);

void mock_assert_call_HAL_I2C_Init(HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);

void mock_assert_call_HAL_I2C_Mem_Read(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void mock_assert_call_HAL_I2C_Mem_Write(uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, const uint8_t *pData, uint16_t Size, uint32_t Timeout, HAL_StatusTypeDef ret);
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);

void mock_assert_call_HAL_Delay(uint32_t Delay);
void HAL_Delay(uint32_t Delay);

#endif  // TEST_MOCK_STM32F4X_H