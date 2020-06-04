

#ifndef _HICOP_H_
#define _HICOP_H_

#include <stdint.h>


//check if other MCU has finish or not 
bool HicopIsReceptionComplete(void);

//Prepare reception buffer for another tx
void HicopCleanBufferRx(void);

//get received header
uint8_t HicopGetHeader(void);

//get payload length
uint8_t HicopGetLength(void);

//get a pointer where is stored payload
uint8_t *HicopGetPayload(void);

// check if data integrity 
bool HicopIsValidData(void);

// start a new tx
void HicopSetHeader(uint8_t header);

// add data to tx
bool HicopAddPayload(uint8_t *payload, uint8_t length);

// send data to other MCU
bool HicopSendPayload(void);



#endif