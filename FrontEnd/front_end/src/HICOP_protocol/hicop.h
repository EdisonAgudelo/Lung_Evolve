
#ifndef _HICOP_H_
#define _HICOP_H_

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    kHicopHeaderData = 0x0,
    kHicopHeaderAlarm,
    kHicopHeaderConfig,
    kHicopHeaderFlags
} HicopHeaders;



typedef enum
{
    kHicopNack = 0x0,
    kHicopAck
} HicopFlags;


bool HicopReadData(HicopHeaders *type, uint8_t *data, uint8_t *lengh);
bool HicopSendData(HicopHeaders type, uint8_t *data, uint8_t lengh);

void HicopLoop(void);
void HicopInit(void);

#endif