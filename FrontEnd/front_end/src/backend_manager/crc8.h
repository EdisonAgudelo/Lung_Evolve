#ifndef _CRC_8_H_
#define _CRC_8_H_

uint8_t CRC8Calculate(uint8_t *dat, uint8_t length);

uint8_t CRC8Configure(uint8_t poly, uint8_t initial);


#endif