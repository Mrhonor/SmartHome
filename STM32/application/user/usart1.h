#ifndef __USART1__
#define __USART1__

#include "main.h"

void USART1_Configuration(void);
void UnpackUSART1(uint8_t* msg);
void USART1_SendByte(uint8_t byte);
void USART1_SendFloat(float  data);
void USART1_SendUint32(uint32_t data);
void LoT_SendData(void);
#endif
