#ifndef __PCF8563__
#define __PCF8563__
#include "main.h"
#include "stm32f4xx_i2c.h"

#define PCF8563_WRITE_ADD 0xA2
#define PCF8563_READ_ADD  0xA3

void PCF8563_Configuration(void);
void IIC_WriteData(uint8_t SlaveAdd, uint8_t Add, uint8_t Data);
uint8_t IICWriteAndReadData(uint8_t SlaveWriteAdd, uint8_t Add, uint8_t SlaveReadAdd);
uint8_t BCDtoData(uint8_t data);
#endif
