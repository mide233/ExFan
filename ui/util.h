#ifndef __UTIL_H_
#define __UTIL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "cxx_main.h"

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
    float roundf(float x);
    float sqrtf(float x);
    int mini_sprintf(char *buffer, const char *format, ...);

    char *float_to_str(float value);
    float easeOutExpo(float x);
    float easeInExpo(float x);

    void applyPwmConf();
    void startFan();
    void stopFan();
    void enterSleep();

#ifdef __cplusplus
}
#endif

#endif