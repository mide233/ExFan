#include "util.h"

char *float_to_str(float value)
{
    static char buffer[32];
    int int_part = (int)value;
    int frac_part = (int)((value - int_part) * 100);

    if (value < 0)
    {
        sprintf(buffer, "-%d.%02d", -int_part, -frac_part);
    }
    else
    {
        sprintf(buffer, "%d.%02d", int_part, frac_part);
    }

    return buffer;
}

float easeOutExpo(float x)
{
    if (x == 1.0f)
        return 1.0f;

    // 简化的近似实现
    // 使用多项式近似指数函数
    float t = 1.0f - x;
    float approx = t * t * t * t * t * t * t * t * t * t; // t^10

    return 1.0f - approx;
}

void applyPwmConf()
{
    if (HAL_TIM_PWM_ConfigChannel(&PwmTimHandle, &PwmTimConfig, TIM_CHANNEL_1) != HAL_OK)
    {
        while (1)
        {
        }
    }
    if (HAL_TIM_PWM_Start(&PwmTimHandle, TIM_CHANNEL_1) != HAL_OK)
    {
        while (1)
        {
        }
    }
}

void startFan()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    applyPwmConf();
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
}

void stopFan()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    uint32_t p = PwmTimConfig.Pulse;
    PwmTimConfig.Pulse = 0;
    applyPwmConf();
    PwmTimConfig.Pulse = p;
}