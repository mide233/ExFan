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
    float counter = 0;

    while (1)
    {
        // SW6208_ReadVBUS, SW6208_IsCharging, SW6208_ReadCapacity
        // SW6208_ReadVBUS() * SW6208_ReadIDischarge() / 1000, PwmTimConfig.Pulse, Spd
        // UI START
        ssd1315_clear_by_color(&OledHandle, 0x00, 0);
        counter = counter < 26 ? counter : 26;
        // ssd1315_gram_draw_rbox(&OledHandle, counter, 0, 32, 48, 5);
        // ssd1315_gram_write_string(&OledHandle, 0, 0, (char *)STR_TEST, 14, 100, SSD1315_FONT_16);
        // ssd1315_gram_draw_rframe(&OledHandle, (float)((int)(easeOutExpo(counter / 30) * 10) % 128), (float)(test_y % 64), 15.0, 15.0, 5);
        switch (page_index)
        {
        case -1:
            ui_page_sleep();
            break;
        case 0:
            ui_page_main();
            break;
        case 1:
            ui_page_fan_status();
            break;

        default:
            break;
        }

        ssd1315_gram_update(&OledHandle);

        // UI END

        if (!SW6208_IsA1PortON())
        {
            SW6208_PortA1Insert();
        }

        if (key_status != -1)
        {
            switch (key_status)
            {
            case KEY_UP_INDEX:
                key_status = -1;

                if (page_index == 0)
                {
                    page_index++;
                    ease_counter = 20;
                }

                // startFan();
                break;
            case KEY_DOWN_INDEX:
                key_status = -1;

                if (page_index == 1)
                {
                    page_index--;
                    ease_counter = 20;
                }
                else if (page_index == 0)
                {
                    page_index = -1;
                    ease_counter = 20;
                }

                // stopFan();
                break;
            case KEY_RIGHT_INDEX:
                key_status = -1;

                if (page_index == 0)
                {
                    if (PwmTimConfig.Pulse < 1800)
                        PwmTimConfig.Pulse += 50;
                    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
                        applyPwmConf();
                }

                break;
            case KEY_LEFT_INDEX:
                key_status = -1;

                if (page_index == 0)
                {
                    if (PwmTimConfig.Pulse > 0)
                        PwmTimConfig.Pulse -= 50;
                    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
                        applyPwmConf();
                }

                break;

            case KEY_CENTER_INDEX:
                key_status = -1;
                if (page_index == 0 || page_index == 1)
                {
                    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
                        stopFan();
                }

                break;
            case KEY_CENTER_LONG_PRESS_INDEX:
                key_status = -1;
                if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_RESET)
                {
                    applyPwmConf();
                    startFan();
                }
                break;
            case KEY_LEFT_LONG_PRESS_INDEX:
                key_status = -1;
                if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
                    applyPwmConf();
                break;
            case KEY_RIGHT_LONG_PRESS_INDEX:
                key_status = -1;
                if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
                    applyPwmConf();
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
                        break;
                    case KEY_DOWN_INDEX:
                        break;
                    case KEY_LEFT_INDEX:
                        if (PwmTimConfig.Pulse > 0 && page_index == 0)
                            PwmTimConfig.Pulse -= 50;
                        break;
                    case KEY_RIGHT_INDEX:
                        if (PwmTimConfig.Pulse < 1800 && page_index == 0)
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