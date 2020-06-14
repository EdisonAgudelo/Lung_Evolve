 
 
 #ifndef _TIME_H_
 #define _TIME_H_
 
 #include <stdint.h>
 #include <stdbool.h>
 
 uint32_t Millis(void);
 
 uint32_t GetDiffTime(uint32_t actual, uint32_t prev);
 
 #endif