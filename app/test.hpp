#include "cxx_main.h"

#define OLED_CMD 0            // 写命令
#define OLED_DATA 1           // 写数据
uint8_t g_ucaOledRam[8][128]; /*显存*/

const uint8_t SSD1306_Init_Array[] = {
    0xAE, // 0xAE, Display OFF (sleep mode)
    0x00, //---set low column address
    0x10, //---set high column address
    0x81,
    0x01, // set contrast control register, 2 bytes, 0x00 - 0xFF
    0xA4, // Output follows RAM content
    0xA6, // Normal display (RESET)
    0x20, // Set Memory Addressing Mode, 2 bytes
    0x00, // Horizontal Addressing Mode (slide horizontally and goto next page)
    0xB0, // Set Page Address, 3 bytes,For Horizontal and Vertical Addressing Mode only
    0x22,
    0x00, // From Page 0
    0x07, // To Page 7
    /**
     * COM Output Scan Direction
     * 0xC0: normal mode (RESET) Scan from COM0 to COM[N –1]
     * 0xC8: remapped mode. Scan from COM[N-1] to COM0 */
    0xC8, // Set COM Output Scan Direction
    /**
     * Set display RAM display start line register from 0-63 */
    0x40,
    /**
     * Segment Re-map
     * 0xA0: column address 0 is mapped to SEG0 (RESET),
     * 0xA1: column address 127 is mapped to SEG0 */
    0xA1,
    /**
     * Set MUX ratio to N+1 MUX
     * N=A[5:0]: from 16MUX to 64MUX, RESET=111111b (i.e. 63d, 64MUX)
     * A[5:0] from 0 to 14 are invalid entry.*/
    0xA8,
    0x3F,
    /**
     * Set Display Offset, Set vertical shift by COM from 0d~63d
     * The value is reset to 00h after RESET */
    0xD3,
    0x00, // offset in vertical
    /**
     * Set COM Pins Hardware Configuration
     * A[4]=0b, Sequential COM pin configuration
     * A[4]=1b(RESET), Alternative COM pin configuration
     * A[5]=0b(RESET), Disable COM Left/Right remap
     * A[5]=1b, Enable COM Left/Right remap */
    0xDA,
    0x12, // A[4]=0, A[5]=1
    0xD5, // Set Display Divide Ratio/Oscillator Frequency
    0xF0, // divide ratio
    0xD9, // Set Pre-charge Period
    0x22,
    0xDB, // Set V COMH Deselect Level
    0x10, // 0.77 * Vcc (RESET)
    0x8D, // charge pump setting
    0x14, // Enable charge pump during display on
    0xAF  // Display ON in normal mode
};

uint8_t OLED_Write(uint8_t *dat, uint8_t send_cmd, uint16_t len)
{
    if (send_cmd == 0x00)
    {
        HAL_I2C_Mem_Write_DMA(&I2cHandle, 0x7A, 0x00, I2C_MEMADD_SIZE_8BIT, dat, 1); //
    }
    else
    {
        HAL_I2C_Mem_Write_DMA(&I2cHandle, 0x7A, 0x40, I2C_MEMADD_SIZE_8BIT, dat, len);
    }

    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
    }

    return 1;
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

void OLED_WriteByte(uint8_t dat, uint8_t cmd)
{
    if (cmd == OLED_CMD)
    {
        // HAL_I2C_Mem_Write_DMA(&I2cHandle, 0x7A, 0x00, I2C_MEMADD_SIZE_8BIT, &dat, 1); //
        OLED_Write(&dat, 0, 1);
    }
    else
    {
        HAL_I2C_Mem_Write_DMA(&I2cHandle, 0x7A, 0x40, I2C_MEMADD_SIZE_8BIT, &dat, 1); //
    }

    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
    {
    }
}

void run_test()
{
    // 示例：尝试读取设备的ID寄存器（如果设备存在）
    uint8_t device_id = 0;
    if (1) // 如果RTC芯片存在
    {
        for (uint8_t i = 0; i < sizeof(SSD1306_Init_Array) / sizeof(uint8_t); i++)
        {
            OLED_WriteByte(SSD1306_Init_Array[i], OLED_CMD);
        }

        OLED_WriteByte(0x8D, OLED_CMD); // 电荷泵使能
        OLED_WriteByte(0x14, OLED_CMD); // 开启电荷泵
        OLED_WriteByte(0xAF, OLED_CMD); // 点亮屏幕

        HAL_Delay(100); // 等待屏幕稳定

        // OLED_WriteByte(0x8D, OLED_CMD); // 电荷泵使能
        // OLED_WriteByte(0x10, OLED_CMD); // 开启电荷泵
        // OLED_WriteByte(0xAE, OLED_CMD); // 点亮屏幕

        uint8_t *puc;
        uint16_t i;
        puc = &g_ucaOledRam[0][0];

        for (i = 0; i < 1024; i++)
        {
            *puc++ = 0xFF;
        }
    }

    // HAL_I2C_Mem_Write_DMA(&I2cHandle, 0x7A, 0x40, I2C_MEMADD_SIZE_8BIT, &g_ucaOledRam[0][0], 1024);
    OLED_Write(&g_ucaOledRam[0][0], 1, 1024);
}