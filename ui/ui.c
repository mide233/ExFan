#include "ui.h"

int8_t page_index = 0;
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
    mini_sprintf(msg_buf, "%s%d%%", SW6208_IsCharging() ? "+" : "", batt);
    ssd1315_gram_write_string(&OledHandle, 126 - (batt > 99 ? 8 * 4 : (batt > 9 ? 8 * 3 : 8 * 2)) - (SW6208_IsCharging() ? 8 : 0), y_position, msg_buf, 8, SSD1315_COLOR_INVERSE, SSD1315_FONT_16);
}

void ui_draw_control_bar(uint8_t y_position)
{
    if (y_position - 40 > 0 && y_position - 40 < 32)
    {
        uint8_t fan_duty_percent = PwmTimConfig.Pulse / 1800.0f * 100;
        float fan_lock = ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET) ? 1 : (key_info_arr[4] < 0 ? (-(key_info_arr[4] < -12 ? -12 : key_info_arr[4]) / 12.0f) : 0));

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
        mini_sprintf(msg_buf, "DUTY %d%%", fan_duty_percent);
        ssd1315_gram_write_string(&OledHandle, 16, y_position - 32, msg_buf, 16, SSD1315_COLOR_INVERSE, SSD1315_FONT_16);
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
    uint8_t ease_offset = 46 - (easeOutExpo((float)(20 - (page_index == 1 ? ease_counter : 0)) / 40) * 40 + 5);
    ui_draw_status_bar(ease_offset, SW6208_ReadVBUS() * SW6208_ReadIDischarge() / 1000.0f, SW6208_ReadCapacity());
    char msg_buf[16];
    mini_sprintf(msg_buf, "FAN-A RPM %d", HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET ? SpdA : 0);
    ssd1315_gram_write_string(&OledHandle, 2, ease_offset + 18, msg_buf, 16, SSD1315_COLOR_INVERSE, SSD1315_FONT_16);
    mini_sprintf(msg_buf, "FAN-B RPM %d", HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET ? SpdB : 0);
    ssd1315_gram_write_string(&OledHandle, 2, ease_offset + 28, msg_buf, 16, SSD1315_COLOR_INVERSE, SSD1315_FONT_16);

    if (SW6208_IsCharging())
    {
        //    ssd1315_gram_draw_box(&OledHandle, 80, ease_offset + 18, 48, 16, SSD1315_COLOR_WHITE, 0);
    }

    if (ease_counter > 0 && page_index == 1)
        ease_counter--;
}

void ui_page_sleep()
{
    if (page_index == -1)
    {
        if (ease_counter > 0)
        {
            ui_page_main();
            ssd1315_gram_draw_box(&OledHandle, 0, 0, 64 * easeOutExpo((float)(20 - ease_counter) / 20), 128, SSD1315_COLOR_BLACK, 0);
            ease_counter -= 2;
        }
        else
        {
            enterSleep();
            page_index = 0;
            ease_counter = 20;
        }
    }
}