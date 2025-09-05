#ifndef __KEY_HANDLER_H
#define __KEY_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "cxx_main.h"

#define KEY_UP_INDEX 0
#define KEY_DOWN_INDEX 1
#define KEY_LEFT_INDEX 2
#define KEY_RIGHT_INDEX 3
#define KEY_CENTER_INDEX 4

    extern int8_t key_info_arr[5];
    extern int8_t key_status;
    extern uint32_t key_timer;

    void key_handler_update(void);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_HANDLER_H */