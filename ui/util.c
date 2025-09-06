#include "util.h"

float sqrtf(float x)
{
    if (x < 0.0f)
    {
        return 0.0f; /* return 0 for negative numbers */
    }
    if (x == 0.0f || x == 1.0f)
    {
        return x;
    }

    /* Newton-Raphson method */
    float guess = x * 0.5f; /* initial guess */
    float prev_guess = 0.0f;

    /* iterate until convergence (about 4-5 iterations for float precision) */
    for (int i = 0; i < 6; i++)
    {
        prev_guess = guess;
        guess = 0.5f * (guess + x / guess);

        /* check for convergence */
        float diff = guess - prev_guess;
        if (diff < 0.0f)
            diff = -diff; /* abs */
        if (diff < 0.001f)
            break; /* good enough precision */
    }

    return guess;
}

float roundf(float x)
{
    if (x >= 0.0f)
    {
        return (float)(int)(x + 0.5f);
    }
    else
    {
        return (float)(int)(x - 0.5f);
    }
}

// 轻量级格式化函数，支持 %d, %02d, %s, %c
int mini_sprintf(char *buffer, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char *buf_ptr = buffer;
    const char *fmt_ptr = format;

    while (*fmt_ptr)
    {
        if (*fmt_ptr == '%')
        {
            fmt_ptr++;

            // 检查是否有前导零标志
            uint8_t pad_zero = 0;
            if (*fmt_ptr == '0')
            {
                pad_zero = 1;
                fmt_ptr++;
            }

            // 获取宽度
            uint8_t width = 0;
            while (*fmt_ptr >= '0' && *fmt_ptr <= '9')
            {
                width = width * 10 + (*fmt_ptr - '0');
                fmt_ptr++;
            }

            switch (*fmt_ptr)
            {
            case 'd':
            {
                int num = va_arg(args, int);
                if (num < 0)
                {
                    *buf_ptr++ = '-';
                    num = -num;
                    if (width > 0)
                        width--; // 减去负号占用的位置
                }

                // 转换数字到字符串
                char temp[12];
                int digit_count = 0;
                if (num == 0)
                {
                    temp[digit_count++] = '0';
                }
                else
                {
                    while (num > 0)
                    {
                        temp[digit_count++] = (num % 10) + '0';
                        num /= 10;
                    }
                }

                // 添加前导零
                if (pad_zero && width > digit_count)
                {
                    for (int i = 0; i < (width - digit_count); i++)
                    {
                        *buf_ptr++ = '0';
                    }
                }

                // 反向复制数字到输出缓冲区
                while (digit_count > 0)
                {
                    *buf_ptr++ = temp[--digit_count];
                }
                break;
            }
            case 's':
            {
                char *str = va_arg(args, char *);
                while (*str)
                {
                    *buf_ptr++ = *str++;
                }
                break;
            }
            case 'c':
            {
                char ch = (char)va_arg(args, int);
                *buf_ptr++ = ch;
                break;
            }
            case '%':
            {
                *buf_ptr++ = '%';
                break;
            }
            default:
                // 不支持的格式符，直接复制
                *buf_ptr++ = '%';
                *buf_ptr++ = *fmt_ptr;
                break;
            }
        }
        else
        {
            *buf_ptr++ = *fmt_ptr;
        }
        fmt_ptr++;
    }

    *buf_ptr = '\0';
    va_end(args);
    return buf_ptr - buffer;
}

char *float_to_str(float value)
{
    static char buffer[32];
    int int_part = (int)value;
    int frac_part = (int)((value - int_part) * 100);

    if (value < 0)
    {
        mini_sprintf(buffer, "-%d.%02d", -int_part, -frac_part);
    }
    else
    {
        mini_sprintf(buffer, "%d.%02d", int_part, frac_part);
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

float easeInExpo(float x)
{
    if (x == 0.0f)
        return 0.0f;

    // 简化的近似实现
    // 使用多项式近似指数函数
    float approx = x * x * x * x * x * x * x * x * x * x; // x^10

    return approx;
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