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

    char *float_to_str(float value);
    float easeOutExpo(float x);

    void applyPwmConf();
    void startFan();
    void stopFan();

#ifdef __cplusplus
}
#endif

#endif