#include "util.h"

char *float_to_str(float value)
{
    static char buffer[32];
    int int_part = (int)value;
    int frac_part = (int)((value - int_part) * 100);

    if (value < 0)
    {
        sprintf(buffer, "-%d.%02d", -int_part, -frac_part);
    }
    else
    {
        sprintf(buffer, "%d.%02d", int_part, frac_part);
    }

    return buffer;
}
