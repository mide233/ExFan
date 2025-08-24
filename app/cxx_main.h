// cxx_main.h

#ifndef __CXX_MAIN_H_
#define __CXX_MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // (for) C PART

#include <sys/stat.h>
#include <stdlib.h>
#include <math.h>
#include "air001xx_hal.h"
#include "util.h"
#include "driver_ssd1315.h"
#include "driver_ssd1315_hal.h"
#include "sw6208.h"

#include "key_handler.h"

    extern I2C_HandleTypeDef I2cHandle;
    extern ssd1315_handle_t OledHandle;
    extern uint16_t test_val;

    void CppMain(); // 主程序函数

    // C PART END

#ifdef __cplusplus
}

// CPP PART

// CPP PART END

#endif

#endif