#ifndef STM32F4XX_HAL_DMA_H
#define STM32F4XX_HAL_DMA_H

#include "stm32f4xx_hal_def.h"

#define DMA_CHANNEL_0 0x00000000U
#define DMA_CHANNEL_1 0x02020202U
#define DMA_CHANNEL_2 0x04040404U
#define DMA_CHANNEL_3 0x06060606U
#define DMA_CHANNEL_4 0x08080808U
#define DMA_CHANNEL_5 0x0A0A0A0AU
#define DMA_CHANNEL_6 0x0C0C0C0CU
#define DMA_CHANNEL_7 0x0E0E0E0EU

#define DMA_PERIPH_TO_MEMORY 0x00000000U
#define DMA_MEMORY_TO_PERIPH 0x00000010U
#define DMA_MEMORY_TO_MEMORY 0x00000020U

#define DMA_PINC_DISABLE 0x00000000U
#define DMA_PINC_ENABLE 0x00000004U

#define DMA_MINC_DISABLE 0x00000000U
#define DMA_MINC_ENABLE 0x00000040U

#define DMA_PDATAALIGN_BYTE 0x00000000U
#define DMA_PDATAALIGN_HALFWORD 0x00000001U
#define DMA_PDATAALIGN_WORD 0x00000002U

#define DMA_MDATAALIGN_BYTE 0x00000000U
#define DMA_MDATAALIGN_HALFWORD 0x00000200U
#define DMA_MDATAALIGN_WORD 0x00000400U

#define DMA_NORMAL 0x00000000U
#define DMA_CIRCULAR 0x00000001U

#define DMA_PRIORITY_LOW 0x00000000U
#define DMA_PRIORITY_MEDIUM 0x00000001U
#define DMA_PRIORITY_HIGH 0x00000002U
#define DMA_PRIORITY_VERY_HIGH 0x00000003U

#define DMA_FIFOMODE_DISABLE 0x00000000U
#define DMA_FIFOMODE_ENABLE 0x00000004U

#define DMA_IT_TC 0x00000002U
#define DMA_IT_HT 0x00000004U
#define DMA_IT_TE 0x00000008U

#define __HAL_DMA_DISABLE_IT(HANDLER, FLAG) \
    do {                                    \
    } while (0)

typedef struct {
    uint32_t Channel;
    uint32_t Direction;
    uint32_t PeriphInc;
    uint32_t MemInc;
    uint32_t PeriphDataAlignment;
    uint32_t MemDataAlignment;
    uint32_t Mode;
    uint32_t Priority;
    uint32_t FIFOMode;
} DMA_InitTypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CNDTR;
    volatile uint32_t CPAR;
    volatile uint32_t CMAR;
} DMA_Stream_TypeDef;

typedef struct {
    DMA_Stream_TypeDef *Instance;
    DMA_InitTypeDef Init;
} DMA_HandleTypeDef;

#define DMA1_Stream0 ((DMA_Stream_TypeDef *)0x40026000UL)
#define DMA1_Stream1 ((DMA_Stream_TypeDef *)0x40026400UL)
#define DMA1_Stream2 ((DMA_Stream_TypeDef *)0x40026800UL)
#define DMA1_Stream3 ((DMA_Stream_TypeDef *)0x40026C00UL)
#define DMA1_Stream4 ((DMA_Stream_TypeDef *)0x40027000UL)
#define DMA1_Stream5 ((DMA_Stream_TypeDef *)0x40027400UL)
#define DMA1_Stream6 ((DMA_Stream_TypeDef *)0x40027800UL)
#define DMA1_Stream7 ((DMA_Stream_TypeDef *)0x40027C00UL)

typedef void (*DMA_Stream_IRQHandlerType)(DMA_HandleTypeDef *);

HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                                uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                                   uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma);
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);

#define __HAL_LINKDMA(HANDLER, POINTER, DMA_FIELD) \
    do {                                           \
    } while (0)

#endif /* STM32F4XX_HAL_DMA_H */
