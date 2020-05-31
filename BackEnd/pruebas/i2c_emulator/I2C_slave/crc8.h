
#ifndef _CRC_8_H_
#define _CRC_8_H_

uint8_t CRC8Calculate(uint8_t *data, uint8_t length);

//for single uint16_nummbers
uint8_t CRC8Calculate(uint16_t data);

uint8_t CRC8Configure(uint8_t poly, uint8_t initial);


#endif