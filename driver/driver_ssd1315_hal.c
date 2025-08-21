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
    uint8_t res;

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

    /* close display */
    res = ssd1315_set_display(&OledHandle, SSD1315_DISPLAY_OFF);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set display failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set column address range */
    res = ssd1315_set_column_address_range(&OledHandle, SSD1315_BASIC_DEFAULT_COLUMN_ADDRESS_RANGE_START, SSD1315_BASIC_DEFAULT_COLUMN_ADDRESS_RANGE_END);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set column address range failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set page address range */
    res = ssd1315_set_page_address_range(&OledHandle, SSD1315_BASIC_DEFAULT_PAGE_ADDRESS_RANGE_START, SSD1315_BASIC_DEFAULT_PAGE_ADDRESS_RANGE_END);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set page address range failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set low column start address */
    res = ssd1315_set_low_column_start_address(&OledHandle, SSD1315_BASIC_DEFAULT_LOW_COLUMN_START_ADDRESS);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set low column start address failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set high column start address */
    res = ssd1315_set_high_column_start_address(&OledHandle, SSD1315_BASIC_DEFAULT_HIGH_COLUMN_START_ADDRESS);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set high column start address failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set display start line */
    res = ssd1315_set_display_start_line(&OledHandle, SSD1315_BASIC_DEFAULT_DISPLAY_START_LINE);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set display start line failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set fade blinking mode */
    res = ssd1315_set_fade_blinking_mode(&OledHandle, SSD1315_BASIC_DEFAULT_FADE_BLINKING_MODE, SSD1315_BASIC_DEFAULT_FADE_FRAMES);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set fade blinking failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* deactivate scroll */
    res = ssd1315_deactivate_scroll(&OledHandle);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set deactivate scroll failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set zoom in */
    res = ssd1315_set_zoom_in(&OledHandle, SSD1315_BASIC_DEFAULT_ZOOM_IN);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set set zoom in failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set contrast */
    res = ssd1315_set_contrast(&OledHandle, SSD1315_BASIC_DEFAULT_CONTRAST);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set contrast failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set segment remap */
    res = ssd1315_set_segment_remap(&OledHandle, SSD1315_BASIC_DEFAULT_SEGMENT);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set segment remap failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set scan direction */
    res = ssd1315_set_scan_direction(&OledHandle, SSD1315_BASIC_DEFAULT_SCAN_DIRECTION);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set scan direction failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set display mode */
    res = ssd1315_set_display_mode(&OledHandle, SSD1315_BASIC_DEFAULT_DISPLAY_MODE);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set display mode failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set multiplex ratio */
    res = ssd1315_set_multiplex_ratio(&OledHandle, SSD1315_BASIC_DEFAULT_MULTIPLEX_RATIO);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set multiplex ratio failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set display offset */
    res = ssd1315_set_display_offset(&OledHandle, SSD1315_BASIC_DEFAULT_DISPLAY_OFFSET);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set display offset failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set display clock */
    res = ssd1315_set_display_clock(&OledHandle, SSD1315_BASIC_DEFAULT_OSCILLATOR_FREQUENCY, SSD1315_BASIC_DEFAULT_CLOCK_DIVIDE);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set display clock failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set pre charge period */
    res = ssd1315_set_precharge_period(&OledHandle, SSD1315_BASIC_DEFAULT_PHASE1_PERIOD, SSD1315_BASIC_DEFAULT_PHASE2_PERIOD);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set pre charge period failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set iref */
    res = ssd1315_set_iref(&OledHandle, SSD1315_BASIC_DEFAULT_IREF, SSD1315_BASIC_DEFAULT_IREF_VALUE);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set iref failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set hardware pins conf */
    res = ssd1315_set_com_pins_hardware_conf(&OledHandle, SSD1315_BASIC_DEFAULT_PIN_CONF, SSD1315_BASIC_DEFAULT_LEFT_RIGHT_REMAP);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set com pins hardware conf failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set deselect level 0.77 */
    res = ssd1315_set_deselect_level(&OledHandle, SSD1315_BASIC_DEFAULT_DESELECT_LEVEL);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set deselect level failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* set page memory addressing mode */
    res = ssd1315_set_memory_addressing_mode(&OledHandle, SSD1315_MEMORY_ADDRESSING_MODE_PAGE);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set memory addressing level failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* enable charge pump */
    res = ssd1315_set_charge_pump(&OledHandle, SSD1315_CHARGE_PUMP_ENABLE, SSD1315_CHARGE_PUMP_MODE_7P5V);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set charge pump failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* entire display off */
    res = ssd1315_set_entire_display(&OledHandle, SSD1315_ENTIRE_DISPLAY_OFF);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set entire display failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* enable display */
    res = ssd1315_set_display(&OledHandle, SSD1315_DISPLAY_ON);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: set display failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    /* clear screen */
    res = ssd1315_clear(&OledHandle);
    if (res != 0)
    {
        ssd1315_interface_debug_print("ssd1315: clear failed.\n");
        (void)ssd1315_deinit(&OledHandle);

        return 1;
    }

    return 0;
}
