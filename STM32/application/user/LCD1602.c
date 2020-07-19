#include "LCD1602.h"

void LCD1602_Configuration(){
    GPIO_InitTypeDef  gpio;
    
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,  ENABLE); 
    
    
    gpio.GPIO_Pin                        =   GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    gpio.GPIO_Mode                       =   GPIO_Mode_OUT;
    gpio.GPIO_Speed                      =   GPIO_Speed_2MHz;
    gpio.GPIO_PuPd                       =   GPIO_PuPd_UP;
    gpio.GPIO_OType                      =   GPIO_OType_OD;  //设置为开漏模式可以双向IO
    GPIO_Init(GPIOC, &gpio);
    
    Delay_us(15000);
    LCD1602_Write(0x38, 0);
    Delay_us(5000);
    LCD1602_Write(0x38, 0);
    LCD1602_Write(0x08, 0);
    LCD1602_Write(0x01, 0);
    Delay_us(1640);           //清屏需要1.64ms
    LCD1602_Write(0x06, 0);
    LCD1602_Write(0x0c, 0);
    
}

void SetLevel(uint16_t GPIO_Pin_x, int8_t Level){
    if((Level & 0x01) == 0)  GPIOE->BSRRL = GPIO_Pin_x;
    else            GPIOE->BSRRH = GPIO_Pin_x;
        
}

void LCD1602_Write(uint8_t cmd, int8_t IsData){
    SetLevel(GPIO_Pin_0 , IsData);
    SetLevel(GPIO_Pin_1 , 0);
    SetLevel(GPIO_Pin_2 , 0);
    
    SetLevel(GPIO_Pin_3 , cmd);
    SetLevel(GPIO_Pin_4 , cmd >> 1);
    SetLevel(GPIO_Pin_5 , cmd >> 2);
    SetLevel(GPIO_Pin_6 , cmd >> 3);
    SetLevel(GPIO_Pin_7 , cmd >> 4);
    SetLevel(GPIO_Pin_8 , cmd >> 5);
    SetLevel(GPIO_Pin_9 , cmd >> 6);
    SetLevel(GPIO_Pin_10, cmd >> 7);
     
    Delay_us(1);
   
    SetLevel(GPIO_Pin_2 , 1);
    Delay_us(1);
    
    SetLevel(GPIO_Pin_2 , 0);
    Delay_us(1);
}
