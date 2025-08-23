// cxx_main.cpp
#include "cxx_main.h"

// #define RUN_TEST
#define I2C_ADDRESS 0xAA                 /* 本机地址0xA0 */
#define I2C_SPEEDCLOCK 400000            /* 通讯速度400K */
#define I2C_DUTYCYCLE I2C_DUTYCYCLE_16_9 /* 占空比 */

#ifdef RUN_TEST
#include "test.hpp"
#define RUN_TEST_FUNC() \
    run_test();         \
    while (1)           \
    {                   \
    }
#else
#define RUN_TEST_FUNC() __asm("nop");
#endif

I2C_HandleTypeDef I2cHandle;
ssd1315_handle_t OledHandle;
uint16_t test_val = 0;

// 主程序入口
void CppMain()
{

    int test_i = 0;

    RUN_TEST_FUNC();

    ssd1315_iic_init(SSD1315_INTERFACE_IIC, SSD1315_ADDR_SA0_1);

    ssd1315_set_display(&OledHandle, SSD1315_DISPLAY_ON);
    ssd1315_set_contrast(&OledHandle, 0x1F);

    // ssd1315

    while (1)
    {
        test_i += 5;
        // test_val = key_status;
        ssd1315_clear_by_color(&OledHandle, 0x00, 0);
        char str[32];
        sprintf(str, "V: %d/%d/%d", key_status, key_info_arr[0].key_pressing, key_info_arr[0].key_last_press);

        ssd1315_gram_write_string(&OledHandle, 0, 0, str, 12, 0xFF, SSD1315_FONT_16);

        ssd1315_gram_draw_hdotted_line(&OledHandle, 0, 0.0, 20.0);
        ssd1315_gram_draw_vline(&OledHandle, 0.0, 0.0, 50.0);

        // // 绘制填充矩形
        ssd1315_gram_draw_rframe(&OledHandle, (float)(test_i % 128), 30.0, 30.0, 15.0, 5);
        // ssd1315_gram_write_point(&OledHandle, 0, 0, 0x0);

        ssd1315_gram_update(&OledHandle);
        key_handler_update();
    }
}
