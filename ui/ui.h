#ifndef __UI_H_
#define __UI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "cxx_main.h"

    extern int8_t page_index;
    extern uint8_t ease_counter;
    extern float fan_lock_ease;

    void ui_draw_status_bar(uint8_t y_position, float watt, uint8_t batt);
    void ui_draw_control_bar(uint8_t y_position);
    void ui_page_main();
    void ui_page_fan_status();
    void ui_page_sleep();

#ifdef __cplusplus
}
#endif

#endif