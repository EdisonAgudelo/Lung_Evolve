
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "low_level/hardware_interface.h"

extern char db_buffer[];

#define dbprintf(...)                                           \
    do                                                          \
    {                                                           \
        DebugWrite(db_buffer, sprintf(db_buffer, __VA_ARGS__)); \
    } while (0)



#endif