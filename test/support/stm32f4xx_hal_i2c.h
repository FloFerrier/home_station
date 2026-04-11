#ifndef STM32F4XX_HAL_I2C_H
#define STM32F4XX_HAL_I2C_H

#include "stm32f4xx_hal_def.h"

#define I2C_ADDRESSINGMODE_7BIT 0x00000001U
#define I2C_DUTYCYCLE_2 0x00000000U
#define I2C_DUTYCYCLE_16_9 0x00040000U
#define I2C_GENERALCALL_DISABLE 0x00000000U
#define I2C_GENERALCALL_ENABLE 0x00000001U
#define I2C_NOSTRETCH_DISABLE 0x00000000U
#define I2C_NOSTRETCH_ENABLE 0x00000080U
#define I2C_ANALOGFILTER_ENABLE 0x00000000U
#define I2C_ANALOGFILTER_DISABLE 0x00000001U

typedef struct {
    uint32_t ClockSpeed;
    uint32_t DutyCycle;
    uint32_t AddressingMode;
    uint32_t DualAddressMode;
    uint32_t GeneralCallMode;
    uint32_t NoStretchMode;
} I2C_InitTypeDef;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t TIMINGR;
    volatile uint32_t TIMEOUTR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t PECR;
    volatile uint32_t RXDR;
    volatile uint32_t TXDR;
} I2C_TypeDef;

#define I2C1 ((I2C_TypeDef *)0x40005400UL)

typedef struct {
    I2C_TypeDef *Instance;
    I2C_InitTypeDef Init;
} I2C_HandleTypeDef;

HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c,
                                    uint16_t DevAddress, uint16_t MemAddress,
                                    uint16_t MemAddSize, uint8_t *pData,
                                    uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                   uint16_t MemAddress, uint16_t MemAddSize,
                                   uint8_t *pData, uint16_t Size,
                                   uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_IsDeviceReady(I2C_HandleTypeDef *hi2c,
                                        uint16_t DevAddress, uint32_t Trials,
                                        uint32_t Timeout);

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);

#endif /* STM32F4XX_HAL_I2C_H */
