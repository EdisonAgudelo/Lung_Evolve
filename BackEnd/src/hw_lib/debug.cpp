
#include "debug.h"


char db_buffer[100];


void DebugInit(void)
{
    DebugBegin();
    dbprintf("Debug sension is started\n");
}