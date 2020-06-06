

#ifndef _HICOP_LL_H_
#define _HICOP_LL_H_

#include <stdint.h>

//this library is the low level human interface comunitacion protocol
//it does all frame verfication, reception and transmision task. 

//start hicop_ll protocol
void HicopLLBegin(void);

//check if other MCU has finish or not 
bool HicopLLIsReceptionComplete(void);

//Prepare reception buffer for another tx
void HicopLLCleanBufferRx(void);

//get received header
uint8_t HicopLLGetHeader(void);

//get payload length
uint8_t HicopLLGetLength(void);

//get a pointer where is stored payload
uint8_t *HicopLLGetPayload(void);

// check if data integrity 
bool HicopLLIsValidData(void);

// start a new tx
void HicopLLSetHeader(uint8_t header);

// add data to tx
bool HicopLLAddPayload(uint8_t *payload, uint8_t length);
bool HicopLLAddPayload(uint8_t payload);

// send data to other MCU
bool HicopLLSendPayload(void);

// if the same buffer will be send again, user should use this aferter the first time.
bool HicopLLResendPayload(void);



#endif