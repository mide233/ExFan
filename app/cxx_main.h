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

    void CppMain();                                        // 主程序函数
    HAL_StatusTypeDef I2C_TestDevice(uint8_t device_addr); // 测试特定I2C设备地址

    // C PART END

#ifdef __cplusplus
}

// CPP PART

// CPP PART END

#endif

#endif