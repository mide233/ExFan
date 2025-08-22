/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_ssd1315_interface_template.c
 * @brief     driver ssd1315 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-11-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/11/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ssd1315_hal.h"
#include "cxx_main.h"

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t ssd1315_interface_iic_init(void)
{
    return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t ssd1315_interface_iic_deinit(void)
{
    return 0;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr iic device write address
 * @param[in] reg iic register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ssd1315_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    HAL_I2C_Mem_Write_DMA(&I2cHandle, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len);
    while (HAL_I2C_GetState(&I2cHandle) != HAL_I2C_STATE_READY)
        ;

    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms time
 * @note      none
 */
void ssd1315_interface_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt format data
 * @note      none
 */
void ssd1315_interface_debug_print(const char *const fmt, ...)
{
}

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 gpio init failed
 * @note   none
 */
uint8_t ssd1315_interface_reset_gpio_init(void)
{
    return 0;
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 gpio deinit failed
 * @note   none
 */
uint8_t ssd1315_interface_reset_gpio_deinit(void)
{
    return 0;
}

/**
 * @brief     interface reset gpio write
 * @param[in] value written value
 * @return    status code
 *            - 0 success
 *            - 1 gpio write failed
 * @note      none
 */
uint8_t ssd1315_interface_reset_gpio_write(uint8_t value)
{
    return 0;
}

uint8_t ssd1315_iic_init(ssd1315_interface_t interface, ssd1315_address_t addr)
{
    /* link functions */
    DRIVER_SSD1315_LINK_INIT(&OledHandle, ssd1315_handle_t);
    DRIVER_SSD1315_LINK_IIC_INIT(&OledHandle, ssd1315_interface_iic_init);
    DRIVER_SSD1315_LINK_IIC_DEINIT(&OledHandle, ssd1315_interface_iic_deinit);
    DRIVER_SSD1315_LINK_IIC_WRITE(&OledHandle, ssd1315_interface_iic_write);
    DRIVER_SSD1315_LINK_RESET_GPIO_INIT(&OledHandle, ssd1315_interface_reset_gpio_init);
    DRIVER_SSD1315_LINK_RESET_GPIO_DEINIT(&OledHandle, ssd1315_interface_reset_gpio_deinit);
    DRIVER_SSD1315_LINK_RESET_GPIO_WRITE(&OledHandle, ssd1315_interface_reset_gpio_write);
    DRIVER_SSD1315_LINK_DELAY_MS(&OledHandle, ssd1315_interface_delay_ms);
    DRIVER_SSD1315_LINK_DEBUG_PRINT(&OledHandle, ssd1315_interface_debug_print);

    uint8_t res;
    /* set interface */
    res = ssd1315_set_interface(&OledHandle, interface);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set interface failed.\n");

        return 1;
    }

    /* set addr pin */
    res = ssd1315_set_addr_pin(&OledHandle, addr);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set addr failed.\n");

        return 1;
    }

    /* ssd1315 init */
    res = ssd1315_init(&OledHandle);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: init failed.\n");

        return 1;
    }

    const uint8_t SSD1306_Init_Array[] = {
        0xAE, // 0xAE, Display OFF (sleep mode)
        0x00, //---set low column address
        0x10, //---set high column address
        0x81,
        0X3F, // set contrast control register, 2 bytes, 0x00 - 0xFF
        0xA4, // Output follows RAM content
        0xA6, // Normal display (RESET)
        0x20, // Set Memory Addressing Mode, 2 bytes
        0x01, // Horizontal Addressing Mode (slide horizontally and goto next page)
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

    for (uint8_t i = 0; i < sizeof(SSD1306_Init_Array) / sizeof(uint8_t); i++)
    {
        ssd1315_write_cmd(&OledHandle, (uint8_t *)&SSD1306_Init_Array[i], 1);
    }

    return 0;
}
