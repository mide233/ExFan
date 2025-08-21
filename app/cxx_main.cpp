// cxx_main.cpp
#include "cxx_main.h"

// #define RUN_TEST
#define I2C_ADDRESS 0xAA                 /* 本机地址0xA0 */
#define I2C_SPEEDCLOCK 400000            /* 通讯速度400K */
#define I2C_DUTYCYCLE I2C_DUTYCYCLE_16_9 /* 占空比 */

#ifdef RUN_TEST
#include "test.hpp"
#define RUN_TEST_FUNC() run_test();
#else
#define RUN_TEST_FUNC() __asm("nop");
#endif

I2C_HandleTypeDef I2cHandle;
ssd1315_handle_t OledHandle;

// 主程序入口
void CppMain()
{
    HAL_Init();
    SystemClockConfig();

    I2cHandle.Instance = I2C;                                 /* I2C */
    I2cHandle.Init.ClockSpeed = I2C_SPEEDCLOCK;               /* I2C通讯速度 */
    I2cHandle.Init.DutyCycle = I2C_DUTYCYCLE;                 /* I2C占空比 */
    I2cHandle.Init.OwnAddress1 = I2C_ADDRESS;                 /* I2C地址 */
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE; /* 禁止广播呼叫 */
    I2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;     /* 允许时钟延长 */
    if (HAL_I2C_Init(&I2cHandle) != HAL_OK)
        Error_Handler();

    int test_i = 0;

    ssd1315_iic_init(SSD1315_INTERFACE_IIC, SSD1315_ADDR_SA0_1);

    ssd1315_set_display(&OledHandle, SSD1315_DISPLAY_ON);
    ssd1315_set_contrast(&OledHandle, 0x0F);

    // ssd1315

    RUN_TEST_FUNC();

    while (1)
    {
        test_i += 5;
        ssd1315_clear_by_color(&OledHandle, 0x00, 0);
        // ssd1315_gram_fill_rect(&OledHandle, 10, 10, 50, 30, 0xFF);
        // ssd1315_gram_write_string(&OledHandle, 0, 0, (char *)"Hello, World!", 12, 0xFF, SSD1315_FONT_16);
        // 绘制垂直虚线
        // ssd1315_gram_draw_hdotted_line(&OledHandle, 10.5, 5.0, 20.0);

        // // // 绘制水平实线
        // ssd1315_gram_draw_vline(&OledHandle, 0.0, 10.0, 50.0);

        // // 绘制填充矩形
        ssd1315_gram_draw_rframe(&OledHandle, (float)(test_i % 128), 30.0, 30.0, 15.0, 5);

        // // 绘制圆角边框
        // ssd1315_gram_draw_frame(&OledHandle, 60.0, 30.0, 40.0, 25.0);
        ssd1315_gram_update(&OledHandle);
    }
}



void SystemClockConfig(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* 配置振荡器 */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE |
                                       RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* 启用HSI */
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI不分频 */
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; /* HSI校准频率24MHz */
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* 关闭HSE */
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* 关闭LSI */
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* 关闭LSE */
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                     /* 关闭PLL (使用HSI直接作为系统时钟) */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        Error_Handler();

    /* 配置系统时钟 */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; /* 配置HSI作为系统时钟源 */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* AHB时钟不分频 */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* APB1时钟不分频 */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
        Error_Handler();

    /* 配置SysTick为1ms中断 */
    if (HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000) != HAL_OK)
        Error_Handler();

    /* 配置SysTick中断优先级 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void Error_Handler(void)
{
    while (1)
        __asm("nop");
}