


#include "time.h"

uint32_t GetDiffTime(uint32_t actual, uint32_t prev)
{
    if(actual>=prev){
        return actual - prev;
    } else{ //when timer overflow
        return 0xffffffff - (prev - actual);
    }
        
}