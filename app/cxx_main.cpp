// cxx_main.cpp
#include "cxx_main.h"

#define DARA_LENGTH 15                   /* 数据长度 */
#define I2C_ADDRESS 0xA0                 /* 本机地址0xA0 */
#define I2C_SPEEDCLOCK 400000            /* 通讯速度400K */
#define I2C_DUTYCYCLE I2C_DUTYCYCLE_16_9 /* 占空比 */

I2C_HandleTypeDef I2cHandle;

uint8_t aTxBuffer[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
uint8_t aRxBuffer[15] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// I2C设备地址扫描函数
void I2C_Scanner(void)
{
    printf("正在扫描I2C设备...\r\n");
    printf("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n");
    uint8_t devices_found = 0;

    for (uint8_t row = 0; row < 8; row++)
    {
        printf("%02X: ", row * 16);

        for (uint8_t col = 0; col < 16; col++)
        {
            uint8_t addr = row * 16 + col;

            // 跳过保留地址
            if (addr < 0x08 || addr > 0x77)
            {
                printf("   ");
            }
            else
            {
                // 尝试检测设备
                if (HAL_I2C_IsDeviceReady(&I2cHandle, addr << 1, 1, 10) == HAL_OK)
                {
                    printf("%02X ", addr);
                    devices_found++;
                }
                else
                {
                    printf("-- ");
                }
            }
        }
        printf("\r\n");
    }

    printf("扫描完成，发现 %d 个I2C设备\r\n\r\n", devices_found);
}

// 读取I2C设备的寄存器数据
HAL_StatusTypeDef I2C_ReadRegister(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t length)
{
    HAL_StatusTypeDef status;

    // 先写入寄存器地址
    status = HAL_I2C_Master_Transmit(&I2cHandle, device_addr << 1, &reg_addr, 1, 1000);
    if (status != HAL_OK)
    {
        printf("写入寄存器地址失败: 0x%02X\r\n", status);
        return status;
    }

    // 读取寄存器数据
    status = HAL_I2C_Master_Receive(&I2cHandle, device_addr << 1, data, length, 1000);
    if (status != HAL_OK)
    {
        printf("读取寄存器数据失败: 0x%02X\r\n", status);
        return status;
    }

    return HAL_OK;
}

// 写入I2C设备的寄存器数据
HAL_StatusTypeDef I2C_WriteRegister(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t length)
{
    HAL_StatusTypeDef status;
    uint8_t tx_buffer[256]; // 临时缓冲区

    if (length > 255)
    {
        printf("数据长度超出限制\r\n");
        return HAL_ERROR;
    }

    // 组合寄存器地址和数据
    tx_buffer[0] = reg_addr;
    for (uint16_t i = 0; i < length; i++)
    {
        tx_buffer[i + 1] = data[i];
    }

    // 发送数据
    status = HAL_I2C_Master_Transmit(&I2cHandle, device_addr << 1, tx_buffer, length + 1, 1000);
    if (status != HAL_OK)
    {
        printf("写入寄存器数据失败: 0x%02X\r\n", status);
        return status;
    }

    return HAL_OK;
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
        printf("设备地址 0x%02X 响应正常\r\n", device_addr);
        return HAL_OK;
    }
    else
    {
        printf("设备地址 0x%02X 无响应\r\n", device_addr);
        return status;
    }
}

// 主程序入口
void CppMain()
{
    HAL_Init();
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

    // 执行I2C设备扫描
    I2C_Scanner();
    I2C_TestDevice(0x3D);

    // 示例：尝试读取设备的ID寄存器（如果设备存在）
    uint8_t device_id = 0;
    if (I2C_TestDevice(0x3D) == HAL_OK) // 如果RTC芯片存在
    {
        // 尝试读取设备ID寄存器（地址0x00）
        if (I2C_ReadRegister(0x3D, 0x00, &device_id, 1) == HAL_OK)
        {
            printf("设备0x3D的ID寄存器值: 0x%02X\r\n", device_id);
        }
    }

    while (1)
    {
        // 主循环
        HAL_Delay(1000);
    }
}