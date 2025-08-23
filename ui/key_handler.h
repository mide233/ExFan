#ifndef __KEY_HANDLER_H
#define __KEY_HANDLER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "cxx_main.h"

    typedef struct
    {
        int8_t key_pressing;
        int8_t key_last_press;
    } key_info_t;

    extern key_info_t key_info_arr[5];
    extern int8_t key_status;

    void key_handler_update(void);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_HANDLER_H */