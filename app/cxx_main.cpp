// cxx_main.cpp
#include "cxx_main.h"

I2C_HandleTypeDef I2cHandle;
ssd1315_handle_t OledHandle;

TIM_HandleTypeDef PwmTimHandle;
TIM_HandleTypeDef SpdMesTimHandle;
TIM_OC_InitTypeDef PwmTimConfig;
TIM_IC_InitTypeDef SpdMesTimConfig;

uint16_t SpdA, SpdB;

uint8_t page_index = 0;
uint8_t ease_counter = 20;
float fan_lock_ease = 0;

void ui_draw_status_bar(uint8_t y_position, float watt, uint8_t batt)
{
    uint8_t progress_l = watt / 22.5 * 64;
    uint8_t progress_r = batt / 100.0 * 64;

    ssd1315_gram_draw_box(&OledHandle, 0, y_position, 16, progress_l, SSD1315_COLOR_WHITE, 0);
    ssd1315_gram_draw_box(&OledHandle, 128 - progress_r, y_position, 16, progress_r, SSD1315_COLOR_WHITE, 0);
    ssd1315_gram_draw_frame(&OledHandle, 0, y_position, 128, 16);
    ssd1315_gram_draw_box(&OledHandle, 60, y_position, 16, 12, SSD1315_COLOR_BLACK, -0.5);
    ssd1315_gram_draw_box(&OledHandle, 60, y_position, 16, 1, SSD1315_COLOR_WHITE, -0.5);
    ssd1315_gram_draw_box(&OledHandle, 60 + 12, y_position, 16, 1, SSD1315_COLOR_WHITE, -0.5);

    char msg_buf[8];
    mini_sprintf(msg_buf, "%sW", float_to_str(watt));
    ssd1315_gram_write_string(&OledHandle, 2, y_position, msg_buf, 8, SSD1315_COLOR_INVERSE, SSD1315_FONT_16);
    // clear buf
    memset(msg_buf, 0, 8);
    mini_sprintf(msg_buf, "%d%%", batt);
    ssd1315_gram_write_string(&OledHandle, 126 - (batt > 99 ? 8 * 4 : (batt > 9 ? 8 * 3 : 8 * 2)), y_position, msg_buf, 8, SSD1315_COLOR_INVERSE, SSD1315_FONT_16);
}

void ui_draw_control_bar(uint8_t y_position)
{
    if (y_position - 40 > 0 && y_position - 40 < 32)
    {
        uint8_t fan_duty_percent = PwmTimConfig.Pulse / 1800.0f * 100;
        float fan_lock = (SW6208_ReadVBUS() > 7 ? 1 : (key_info_arr[4] < 0 ? (-(key_info_arr[4] < -12 ? -12 : key_info_arr[4]) / 12.0f) : 0));

        ssd1315_gram_draw_rframe(&OledHandle, 0, y_position - 40, 128, 32, 5);
        ssd1315_gram_draw_rframe(&OledHandle, 0, y_position - 40, 128 * fan_duty_percent / 100, 32, 5);
        if (fan_lock > 0)
        {
            ssd1315_gram_draw_rbox(&OledHandle, 0, y_position - 40, (128 * fan_duty_percent / 100) * fan_lock, 32, 5);
            fan_lock_ease = fan_lock;
        }
        else if (fan_lock_ease > 0 && (fan_lock_ease < 0.9f))
        {
            ssd1315_gram_draw_rbox(&OledHandle, 0, y_position - 40, (128 * fan_duty_percent / 100) * fan_lock_ease, 32, 5);
            fan_lock_ease = fan_lock_ease > 0.1f ? fan_lock_ease - 0.1f : 0;
        }

        char msg_buf[16];
        mini_sprintf(msg_buf, "%d%%", fan_duty_percent);
        ssd1315_gram_write_string(&OledHandle, 24, y_position - 32, msg_buf, 16, SSD1315_COLOR_INVERSE, SSD1315_FONT_16);
    }
}

void ui_page_main()
{
    uint8_t ease_offset = easeOutExpo((float)(20 - (page_index == 0 ? ease_counter : 0)) / 40) * 40 + 5;
    ui_draw_control_bar(ease_offset);
    ui_draw_status_bar(ease_offset, SW6208_ReadVBUS() * SW6208_ReadIDischarge() / 1000.0f, SW6208_ReadCapacity());
    if (ease_counter > 0 && page_index == 0)
        ease_counter--;
}

void ui_page_fan_status()
{
    ui_draw_status_bar(46 - (easeOutExpo((float)(20 - (page_index == 1 ? ease_counter : 0)) / 40) * 40 + 5), SW6208_ReadVBUS() * SW6208_ReadIDischarge() / 1000.0f, SW6208_ReadCapacity());
    if (ease_counter > 0 && page_index == 1)
        ease_counter--;
}

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

                // stopFan();
                break;
            case KEY_RIGHT_INDEX:
                key_status = -1;

                if (page_index == 0)
                {
                    if (PwmTimConfig.Pulse < 1800)
                        PwmTimConfig.Pulse += 50;
                    if (SW6208_ReadVBUS() > 7)
                        applyPwmConf();
                }

                break;
            case KEY_LEFT_INDEX:
                key_status = -1;

                if (page_index == 0)
                {
                    if (PwmTimConfig.Pulse > 0)
                        PwmTimConfig.Pulse -= 50;
                    if (SW6208_ReadVBUS() > 7)
                        applyPwmConf();
                }

                break;

            case KEY_CENTER_INDEX:
                key_status = -1;
                if (page_index == 0)
                {
                    if (SW6208_ReadVBUS() > 7)
                    {
                        stopFan();
                    }
                }
                break;
            case KEY_CENTER_LONG_PRESS_INDEX:
                key_status = -1;
                if (SW6208_ReadVBUS() < 7)
                    startFan();
                break;
            case KEY_LEFT_LONG_PRESS_INDEX:
                key_status = -1;
                if (SW6208_ReadVBUS() > 7)
                    applyPwmConf();
                break;
            case KEY_RIGHT_LONG_PRESS_INDEX:
                key_status = -1;
                if (SW6208_ReadVBUS() > 7)
                    applyPwmConf();
                break;

            default:
                // key_status = -1;
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
                        if (PwmTimConfig.Pulse > 0)
                            PwmTimConfig.Pulse -= 50;
                        break;
                    case KEY_RIGHT_INDEX:
                        if (PwmTimConfig.Pulse < 1800)
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