// cxx_main.cpp
#include "cxx_main.h"

#define I2C_ADDRESS 0xAA                 /* 本机地址0xA0 */
#define I2C_SPEEDCLOCK 400000            /* 通讯速度400K */
#define I2C_DUTYCYCLE I2C_DUTYCYCLE_16_9 /* 占空比 */

#define OLED_CMD 0            // 写命令
#define OLED_DATA 1           // 写数据
uint8_t g_ucaOledRam[8][128]; /*显存*/

I2C_HandleTypeDef I2cHandle;

void OLED_WriteByte(uint8_t dat, uint8_t cmd)
{
    if (cmd == OLED_CMD)
    {
        HAL_I2C_Mem_Write(&I2cHandle, 0x7A, 0x00, I2C_MEMADD_SIZE_8BIT, &dat, 1, 100); //
    }
    else
    {
        HAL_I2C_Mem_Write(&I2cHandle, 0x7A, 0x40, I2C_MEMADD_SIZE_8BIT, &dat, 1, 100); //
    }
}

// 测试特定地址的I2C设备
HAL_StatusTypeDef I2C_TestDevice(uint8_t device_addr)
{
    uint8_t test_data = 0x00;
    HAL_StatusTypeDef status;

    // 方法1: 使用HAL_I2C_IsDeviceReady检测设备
    status = HAL_I2C_IsDeviceReady(&I2cHandle, device_addr << 1, 3, 1000);
    if (status == HAL_OK)
    {
        return HAL_OK;
    }
    else
    {
        return status;
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
    {
        while (1)
            ;
    }

    /* 配置系统时钟 */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; /* 配置HSI作为系统时钟源 */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* AHB时钟不分频 */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* APB1时钟不分频 */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while (1)
            ;
    }

    /* 配置SysTick为1ms中断 */
    if (HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000) != HAL_OK)
    {
        while (1)
            ;
    }

    /* 配置SysTick中断优先级 */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

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
    {
        while (1)
            ;
    }

    // 示例：尝试读取设备的ID寄存器（如果设备存在）
    uint8_t device_id = 0;
    if (1) // 如果RTC芯片存在
    {
        OLED_WriteByte(0xAE, OLED_CMD); //--turn off oled panel
        OLED_WriteByte(0x00, OLED_CMD); //---set low column address
        OLED_WriteByte(0x10, OLED_CMD); //---set high column address
        OLED_WriteByte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
        OLED_WriteByte(0x81, OLED_CMD); //--set contrast control register
        OLED_WriteByte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
        OLED_WriteByte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
        OLED_WriteByte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
        OLED_WriteByte(0xA6, OLED_CMD); //--set normal display
        OLED_WriteByte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
        OLED_WriteByte(0x3f, OLED_CMD); //--1/64 duty
        OLED_WriteByte(0xD3, OLED_CMD); //-set display offset        Shift Mapping RAM Counter (0x00~0x3F)
        OLED_WriteByte(0x00, OLED_CMD); //-not offset
        OLED_WriteByte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
        OLED_WriteByte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
        OLED_WriteByte(0xD9, OLED_CMD); //--set pre-charge period
        OLED_WriteByte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
        OLED_WriteByte(0xDA, OLED_CMD); //--set com pins hardware configuration
        OLED_WriteByte(0x12, OLED_CMD);
        OLED_WriteByte(0xDB, OLED_CMD); //--set vcomh
        OLED_WriteByte(0x40, OLED_CMD); // Set VCOM Deselect Level
        OLED_WriteByte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
        OLED_WriteByte(0x00, OLED_CMD); //
        OLED_WriteByte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
        OLED_WriteByte(0x14, OLED_CMD); //--set(0x10) disable
        OLED_WriteByte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
        OLED_WriteByte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
        OLED_WriteByte(0xAF, OLED_CMD);

        // 寻址方式
        OLED_WriteByte(0X20, OLED_CMD); // 设置GDDRAM模式
        OLED_WriteByte(0X00, OLED_CMD); // 水平寻址模式

        OLED_WriteByte(0X21, OLED_CMD); // 设置列起始和结束地址
        OLED_WriteByte(0X00, OLED_CMD); // 列起始地址 0
        OLED_WriteByte(0X7F, OLED_CMD); // 列终止地址 127

        OLED_WriteByte(0X22, OLED_CMD); // 设置页起始和结束地址
        OLED_WriteByte(0X00, OLED_CMD); // 页起始地址 0
        OLED_WriteByte(0X07, OLED_CMD); // 页终止地址 7

        // OLED_WriteByte(0x8D, OLED_CMD); // 电荷泵使能
        // OLED_WriteByte(0x14, OLED_CMD); // 开启电荷泵
        // OLED_WriteByte(0xAF, OLED_CMD); // 点亮屏幕

        uint8_t *puc;
        uint16_t i;
        puc = &g_ucaOledRam[0][0];

        for (i = 0; i < 1024; i++)
        {
            *puc++ = 0xFF;
        }
    }

    while (1)
    {
        HAL_I2C_Mem_Write(&I2cHandle, 0x7A, 0x40, I2C_MEMADD_SIZE_8BIT, &g_ucaOledRam[0][0], 1024, 200);
    }
}