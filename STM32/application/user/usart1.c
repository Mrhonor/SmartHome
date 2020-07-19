#include "usart1.h"

/*-----USART1_RX-----PA5----*/ 

#define USART1_RX_BUF_LEN 32



static uint8_t USART1_RX_BUFFER[USART1_RX_BUF_LEN];

static int Rx_Cut = 0;

void USART1_Configuration(){
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    USART_InitTypeDef usart;
    
    GPIO_InitTypeDef  gpio_cfe;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    //ÅäÖÃÍ¨ÐÅ¿Ú
    gpio.GPIO_Pin                           =   GPIO_Pin_9 | GPIO_Pin_10;
    gpio.GPIO_Mode                          =   GPIO_Mode_AF;
    gpio.GPIO_Speed                         =   GPIO_Speed_50MHz;
    gpio.GPIO_PuPd                          =   GPIO_PuPd_UP;
    gpio.GPIO_OType                         =   GPIO_OType_PP;
    GPIO_Init(GPIOA, &gpio);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);
    
    
    usart.USART_BaudRate                    =   921600;
    usart.USART_WordLength                  =   USART_WordLength_8b;
    usart.USART_StopBits                    =   USART_StopBits_1;
    usart.USART_Parity                      =   USART_Parity_No;   
    usart.USART_Mode                        =   USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl         =   USART_HardwareFlowControl_None;
    USART_Init(USART1, &usart);
    
    nvic.NVIC_IRQChannel                    = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  = 0;
    nvic.NVIC_IRQChannelSubPriority         = 0;
    nvic.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&nvic);    
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    
}

void USART1_IRQHandler(void){
    char RxTemp;
    if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET){
        RxTemp = USART_ReceiveData(USART1);
 
        USART1_RX_BUFFER[Rx_Cut] = RxTemp;
        Rx_Cut++;
        if(Rx_Cut >= USART1_RX_BUF_LEN){
            UnpackUSART1(USART1_RX_BUFFER);
            Rx_Cut = 0;
        }
        //clear the RXNE pending flag 
        (void)USART1->SR;
        (void)USART1->DR;
    }
    
}

void USART1_SendByte(uint8_t byte){
    USART_SendData(USART1, byte);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void USART1_SendFloat(float data){
    uint8_t byte[4];
    memcpy(byte, &data, 4);
    USART_SendData(USART1, byte[0]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, byte[1]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, byte[2]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, byte[3]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void USART1_SendUint32(uint32_t data){
    uint8_t byte[4];
    memcpy(byte, &data, 4);
    USART_SendData(USART1, byte[0]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, byte[1]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, byte[2]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, byte[3]);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}


void LoT_SendData(void){
    USART1_SendByte(0xFF);
    USART1_SendFloat(Temperature);
    USART1_SendFloat(Humidity);
    USART1_SendUint32(MQ2Value);
    USART1_SendUint32(MQ6Value);
}

void UnpackUSART1(uint8_t * msg){
}
