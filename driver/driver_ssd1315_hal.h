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
 * @file      driver_ssd1315_interface.h
 * @brief     driver ssd1315 interface header file
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

#ifndef DRIVER_SSD1315_INTERFACE_H
#define DRIVER_SSD1315_INTERFACE_H

#include "driver_ssd1315.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SSD1315_BASIC_DEFAULT_DESELECT_LEVEL SSD1315_DESELECT_LEVEL_0P77            /**< set deselect level 0.77 */
#define SSD1315_BASIC_DEFAULT_LEFT_RIGHT_REMAP SSD1315_LEFT_RIGHT_REMAP_DISABLE     /**< disable remap */
#define SSD1315_BASIC_DEFAULT_PIN_CONF SSD1315_PIN_CONF_ALTERNATIVE                 /**< set alternative */
#define SSD1315_BASIC_DEFAULT_PHASE1_PERIOD 0x01                                    /**< set phase 1 */
#define SSD1315_BASIC_DEFAULT_PHASE2_PERIOD 0x0F                                    /**< set phase F */
#define SSD1315_BASIC_DEFAULT_IREF SSD1315_IREF_ENABLE                              /**< enable */
#define SSD1315_BASIC_DEFAULT_IREF_VALUE SSD1315_IREF_VALUE_19UA_150UA              /**< internal iref setting: 19uA, output maximum iseg 150uA */
#define SSD1315_BASIC_DEFAULT_OSCILLATOR_FREQUENCY 0x08                             /**< set 8 */
#define SSD1315_BASIC_DEFAULT_CLOCK_DIVIDE 0x00                                     /**< set clock div 0 */
#define SSD1315_BASIC_DEFAULT_DISPLAY_OFFSET 0x00                                   /**< set display offset */
#define SSD1315_BASIC_DEFAULT_MULTIPLEX_RATIO 0x3F                                  /**< set ratio */
#define SSD1315_BASIC_DEFAULT_DISPLAY_MODE SSD1315_DISPLAY_MODE_NORMAL              /**< set normal mode */
#define SSD1315_BASIC_DEFAULT_SCAN_DIRECTION SSD1315_SCAN_DIRECTION_COMN_1_START    /**< set scan 1 */
#define SSD1315_BASIC_DEFAULT_SEGMENT SSD1315_SEGMENT_COLUMN_ADDRESS_127            /**< set column 127 */
#define SSD1315_BASIC_DEFAULT_CONTRAST 0xCF                                         /**< set contrast CF */
#define SSD1315_BASIC_DEFAULT_ZOOM_IN SSD1315_ZOOM_IN_DISABLE                       /**< disable zoom in */
#define SSD1315_BASIC_DEFAULT_FADE_BLINKING_MODE SSD1315_FADE_BLINKING_MODE_DISABLE /**< disable fade */
#define SSD1315_BASIC_DEFAULT_FADE_FRAMES 0x00                                      /**< set frame 0 */
#define SSD1315_BASIC_DEFAULT_DISPLAY_START_LINE 0x00                               /**< set start line 0 */
#define SSD1315_BASIC_DEFAULT_HIGH_COLUMN_START_ADDRESS 0x00                        /**< set high start 0 */
#define SSD1315_BASIC_DEFAULT_LOW_COLUMN_START_ADDRESS 0x00                         /**< set low start 0 */
#define SSD1315_BASIC_DEFAULT_PAGE_ADDRESS_RANGE_START 0x00                         /**< set page range start */
#define SSD1315_BASIC_DEFAULT_PAGE_ADDRESS_RANGE_END 0x07                           /**< set page range end */
#define SSD1315_BASIC_DEFAULT_COLUMN_ADDRESS_RANGE_START 0x00                       /**< set range start */
#define SSD1315_BASIC_DEFAULT_COLUMN_ADDRESS_RANGE_END 0x7F                         /**< set range end */

    uint8_t ssd1315_iic_init(ssd1315_interface_t interface, ssd1315_address_t addr);

    /**
     * @defgroup ssd1315_interface_driver ssd1315 interface driver function
     * @brief    ssd1315 interface driver modules
     * @ingroup  ssd1315_driver
     * @{
     */

    /**
     * @brief  interface iic bus init
     * @return status code
     *         - 0 success
     *         - 1 iic init failed
     * @note   none
     */
    uint8_t ssd1315_interface_iic_init(void);

    /**
     * @brief  interface iic bus deinit
     * @return status code
     *         - 0 success
     *         - 1 iic deinit failed
     * @note   none
     */
    uint8_t ssd1315_interface_iic_deinit(void);

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
    uint8_t ssd1315_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len);

    /**
     * @brief  interface spi bus init
     * @return status code
     *         - 0 success
     *         - 1 spi init failed
     * @note   none
     */
    uint8_t ssd1315_interface_spi_init(void);

    /**
     * @brief  interface spi bus deinit
     * @return status code
     *         - 0 success
     *         - 1 spi deinit failed
     * @note   none
     */
    uint8_t ssd1315_interface_spi_deinit(void);

    /**
     * @brief     interface spi bus write
     * @param[in] *buf pointer to a data buffer
     * @param[in] len length of data buffer
     * @return    status code
     *            - 0 success
     *            - 1 write failed
     * @note      none
     */
    uint8_t ssd1315_interface_spi_write_cmd(uint8_t *buf, uint16_t len);

    /**
     * @brief     interface delay ms
     * @param[in] ms time
     * @note      none
     */
    void ssd1315_interface_delay_ms(uint32_t ms);

    /**
     * @brief     interface print format data
     * @param[in] fmt format data
     * @note      none
     */
    void ssd1315_interface_debug_print(const char *const fmt, ...);

    /**
     * @brief  interface command && data gpio init
     * @return status code
     *         - 0 success
     *         - 1 gpio init failed
     * @note   none
     */
    uint8_t ssd1315_interface_spi_cmd_data_gpio_init(void);

    /**
     * @brief  interface command && data gpio deinit
     * @return status code
     *         - 0 success
     *         - 1 gpio deinit failed
     * @note   none
     */
    uint8_t ssd1315_interface_spi_cmd_data_gpio_deinit(void);

    /**
     * @brief     interface command && data gpio write
     * @param[in] value written value
     * @return    status code
     *            - 0 success
     *            - 1 gpio write failed
     * @note      none
     */
    uint8_t ssd1315_interface_spi_cmd_data_gpio_write(uint8_t value);

    /**
     * @brief  interface reset gpio init
     * @return status code
     *         - 0 success
     *         - 1 gpio init failed
     * @note   none
     */
    uint8_t ssd1315_interface_reset_gpio_init(void);

    /**
     * @brief  interface reset gpio deinit
     * @return status code
     *         - 0 success
     *         - 1 gpio deinit failed
     * @note   none
     */
    uint8_t ssd1315_interface_reset_gpio_deinit(void);

    /**
     * @brief     interface reset gpio write
     * @param[in] value written value
     * @return    status code
     *            - 0 success
     *            - 1 gpio write failed
     * @note      none
     */
    uint8_t ssd1315_interface_reset_gpio_write(uint8_t value);

    /**
     * @}
     */

#ifdef __cplusplus
}
#endif

#endif
