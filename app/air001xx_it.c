#include "air001xx_it.h"
#include "air001xx_hal.h"
#include "cxx_main.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(I2cHandle.hdmatx);
}

/**
 * @brief This function handles DMA1 channel2 、channel3 Interrupt .
 */
void DMA1_Channel2_3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(I2cHandle.hdmarx);
}

/**
 * @brief This function handles I2C1 Interrupt .
 */
void I2C1_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&I2cHandle);
    HAL_I2C_ER_IRQHandler(&I2cHandle);
}