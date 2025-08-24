// cxx_main.cpp
#include "cxx_main.h"

I2C_HandleTypeDef I2cHandle;
ssd1315_handle_t OledHandle;
uint16_t test_val = 0;

// 主程序入口
void CppMain()
{

    int test_x = 0;
    int test_y = 0;
    float counter = 0;

    while (1)
    {
        // test_i += 5;
        // test_val = key_status;
        ssd1315_clear_by_color(&OledHandle, 0x00, 0);

        char str[32];
        sprintf(str, "%s/%d/%d/%d", float_to_str(SW6208_ReadVBAT()), (SW6208_IsCPortON()), (SW6208_IsCharging()), (SW6208_ReadCapacity()));
        ssd1315_gram_write_string(&OledHandle, 0, 0, str, 18, 0xFF, SSD1315_FONT_16);

        // char tk[32];
        // sprintf(tk, "%d/%d/%d", key_info_arr[2], key_info_arr[3], key_info_arr[4]);
        // ssd1315_gram_write_string(&OledHandle, 0, 16, tk, 16, 0xFF, SSD1315_FONT_16);

        ssd1315_gram_draw_hdotted_line(&OledHandle, 0, 0.0, 20.0);
        ssd1315_gram_draw_vline(&OledHandle, 0.0, 0.0, 50.0);

        // // 绘制填充矩形
        ssd1315_gram_draw_rframe(&OledHandle, (float)(test_x % 128), (float)(test_y % 64), 15.0 + 10 * sinf(counter), 15.0, 5);
        // ssd1315_gram_write_point(&OledHandle, 0, 0, 0x0);

        ssd1315_gram_update(&OledHandle);

        if (key_status != -1)
        {
            switch (key_status)
            {
            case KEY_UP_INDEX:
                test_y--;
                key_status = -1;
                break;
            case KEY_DOWN_INDEX:
                test_y++;
                key_status = -1;
                break;
            case KEY_LEFT_INDEX:
                test_x--;
                key_status = -1;
                break;
            case KEY_RIGHT_INDEX:
                test_x++;
                key_status = -1;
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
                        break;
                    case KEY_LEFT_INDEX:
                        test_x--;
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