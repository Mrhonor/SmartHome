#ifndef __FM25CL64__
#define __FM25CL64__
#include "main.h"
#include "stm32f4xx_spi.h"

void FM25CL64_Configuration(void);
void SPI_SendByte(uint8_t byte);
uint8_t SPI_ReadByte(void);

#endif
