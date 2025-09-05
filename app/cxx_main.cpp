// cxx_main.cpp
#include "cxx_main.h"

I2C_HandleTypeDef I2cHandle;
ssd1315_handle_t OledHandle;

TIM_HandleTypeDef PwmTimHandle;
TIM_HandleTypeDef SpdMesTimHandle;
TIM_OC_InitTypeDef PwmTimConfig;
TIM_IC_InitTypeDef SpdMesTimConfig;

uint16_t SpdA, SpdB;

// 主程序入口
void CppMain()
{

    int test_x = 0;
    int test_y = 36;
    float counter = 0;
    uint16_t p;

    while (1)
    {
        // test_i += 5;
        ssd1315_clear_by_color(&OledHandle, 0x00, 0);

        char str[32];
        sprintf(str, "%s/%d/%d", float_to_str(SW6208_ReadVBUS()), (SW6208_IsCharging()), (SW6208_ReadCapacity()));
        ssd1315_gram_write_string(&OledHandle, 0, 0, str, 18, 0xFF, SSD1315_FONT_16);

        char tk[32];
        sprintf(tk, "%s/%d/%d", float_to_str(SW6208_ReadVBUS() * SW6208_ReadIDischarge() / 1000), PwmTimConfig.Pulse, (SpdB));
        ssd1315_gram_write_string(&OledHandle, 0, 16, tk, 16, 0xFF, SSD1315_FONT_16);

        // ssd1315_gram_draw_hdotted_line(&OledHandle, 0, 0.0, 20.0);
        // ssd1315_gram_draw_vline(&OledHandle, 0.0, 0.0, 50.0);

        // // 绘制填充矩形
        ssd1315_gram_draw_rframe(&OledHandle, (float)(test_x % 128), (float)(test_y % 64), 15.0, 15.0, 5);
        // ssd1315_gram_write_point(&OledHandle, 0, 0, 0x0);

        ssd1315_gram_update(&OledHandle);

        if (!SW6208_IsA1PortON())
        {
            SW6208_PortA1Insert();
        }

        if (key_status != -1)
        {
            switch (key_status)
            {
            case KEY_UP_INDEX:
                test_y--;
                key_status = -1;
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
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
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
                break;
            case KEY_DOWN_INDEX:
                test_y++;
                key_status = -1;
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
                p = PwmTimConfig.Pulse;
                PwmTimConfig.Pulse = 0;
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
                PwmTimConfig.Pulse = p;
                break;
            case KEY_LEFT_INDEX:
                test_x--;
                key_status = -1;
                PwmTimConfig.Pulse += 50;
                if (SW6208_ReadVBUS() > 7)
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

                break;
            case KEY_RIGHT_INDEX:
                test_x++;
                key_status = -1;
                PwmTimConfig.Pulse -= 50;
                if (SW6208_ReadVBUS() > 7)
                {
                    if (HAL_TIM_PWM_ConfigChannel(&PwmTimHandle, &PwmTimConfig, TIM_CHANNEL_1) != HAL_OK)
                    {
                        // Error_Handler();
                    }
                    if (HAL_TIM_PWM_Start(&PwmTimHandle, TIM_CHANNEL_1) != HAL_OK)
                    {
                        while (1)
                        {
                        }
                    }
                }
                break;
            default:
                key_status = -1;
                break;
            }
        }
        else
        {
            for (uint8_t i = 0; i < 5; i++)
            {
                if (key_info_arr[i] <= -10)
                {
                    switch (i)
                    {
                    case KEY_UP_INDEX:
                        test_y--;
                        break;
                    case KEY_DOWN_INDEX:
                        test_y++;
                        break;
                    case KEY_RIGHT_INDEX:
                        test_x++;
                        PwmTimConfig.Pulse -= 50;
                        break;
                    case KEY_LEFT_INDEX:
                        test_x--;
                        PwmTimConfig.Pulse += 50;
                        break;

                    default:
                        break;
                    }
                }
            }
        }

        key_handler_update();
        SW6208_WorkerUpdate();
        counter++;
    }
}