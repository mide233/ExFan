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
#include "coroOS.h"
#include "driver_ssd1315.h"
#include "driver_ssd1315_hal.h"

    extern I2C_HandleTypeDef I2cHandle;
    extern ssd1315_handle_t OledHandle;

    void CppMain(); // 主程序函数
    void SystemClockConfig(void);
    void Error_Handler(void);

    // C PART END

#ifdef __cplusplus
}

// CPP PART

// CPP PART END

#endif

#endif