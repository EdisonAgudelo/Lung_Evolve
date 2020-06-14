 
 #include "time.h"
 #include <Arduino.h>
 
 uint32_t Millis(void)
 {
		return millis();
 }
 
 uint32_t GetDiffTime(uint32_t actual, uint32_t prev)
{
    if (actual >= prev)
    {
        return (actual - prev);
    }
    else
    { //when resgiter overflow
        return (0xffffffff - (prev - actual));
    }
}