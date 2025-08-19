// cxx_main.cpp
#include "cxx_main.h"
// 主程序入口
void CppMain()
{
    HAL_Init();
    GPIO_InitTypeDef GPIO_LED;
    GPIO_LED.Pin = GPIO_PIN_0;
    GPIO_LED.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_LED.Pull = GPIO_NOPULL;
    GPIO_LED.Speed = GPIO_SPEED_FREQ_LOW;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_Init(GPIOB, &GPIO_LED);

    for (;;)
    {
        __asm volatile("NOP");
    }
}