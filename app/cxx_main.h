// test.h

#ifndef __TEST_H_
#define __TEST_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // (for) C PART

#include <sys/stat.h>
#include "air001xx_hal.h"

    extern I2C_HandleTypeDef I2cHandle;

    void CppMain();                                                                                             // 主程序函数
    void I2C_Scanner(void);                                                                                     // I2C设备扫描函数
    HAL_StatusTypeDef I2C_TestDevice(uint8_t device_addr);                                                      // 测试特定I2C设备地址
    HAL_StatusTypeDef I2C_ReadRegister(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t length);  // 读取寄存器
    HAL_StatusTypeDef I2C_WriteRegister(uint8_t device_addr, uint8_t reg_addr, uint8_t *data, uint16_t length); // 写入寄存器

    // C PART END

#ifdef __cplusplus
}

// CPP PART

// CPP PART END

#endif

#endif