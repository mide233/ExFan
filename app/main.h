#ifndef AIR001_CMAKE_MAIN_H
#define AIR001_CMAKE_MAIN_H

#include "cxx_main.h"
int _close(int file) { return -1; }
int _lseek(int file, int ptr, int dir) { return 0; }
int _read(int file, char *ptr, int len) { return 0; }
int _write(int file, char *ptr, int len) { return len; }
void *_sbrk(ptrdiff_t incr) { return (void *)-1; }

#define I2C_ADDRESS 0xAA                 /* 本机地址0xA0 */
#define I2C_SPEEDCLOCK 400000            /* 通讯速度400K */
#define I2C_DUTYCYCLE I2C_DUTYCYCLE_16_9 /* 占空比 */

void GPIOInit(void);
void SystemClockConfig(void);
void Error_Handler(void);
void TIMInit(void);

#endif // AIR001_CMAKE_MAIN_H
