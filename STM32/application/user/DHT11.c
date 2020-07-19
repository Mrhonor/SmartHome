#include "DHT11.h"
float Temperature = 0.0f;
float Humidity = 0.0f;
uint32_t TimeDelay = 0;
uint8_t Data[5];

void DHT11_Configuration(){
    GPIO_InitTypeDef  gpio;
    
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,  ENABLE); 
    
    
    gpio.GPIO_Pin                        =   GPIO_Pin_12;
    gpio.GPIO_Mode                       =   GPIO_Mode_OUT;
    gpio.GPIO_Speed                      =   GPIO_Speed_2MHz;
    gpio.GPIO_PuPd                       =   GPIO_PuPd_UP;
    gpio.GPIO_OType                      =   GPIO_OType_OD;
    GPIO_Init(GPIOB, &gpio);
    
    SysTick_Config(SystemCoreClock / 1000000); // 1us中断一次
}

void SysTick_Handle(){
    TimeDelay--;
}

void Delay_us(uint32_t us){
    TimeDelay = us;
    while(TimeDelay == 0);
}

void GetDHTData(void){
    uint32_t WaitTime;
    char DataCount;
    
    GPIOB->BSRRH = GPIO_Pin_12;
    WaitTime = 1000;
    while(WaitTime--);
    
    GPIOB->BSRRL = GPIO_Pin_12;
    Delay_us(20000);
    
    GPIOB->BSRRH = GPIO_Pin_12;
    
    WaitTime = 1000;
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET){
        if(--WaitTime <= 0) return;
    }
    
    WaitTime = 1500;
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET){
        if(--WaitTime <= 0) return;
    }
        
    DataCount = 40;
    while(DataCount--){
        //等待低电平信号
        WaitTime = 1000;
        while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET){
            if(--WaitTime <= 0) return;
        }
        
        Delay_us(100);
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET) Data[4-DataCount/8] |=   1 << (DataCount % 8); 
        else                                                     Data[4-DataCount/8] &= ~(1 << (DataCount % 8));           
    }
    
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET);
    Delay_us(50);
    GPIOB->BSRRH = GPIO_Pin_12;
    
    //根据校验位检查数据
    if(((Data[0]+Data[1]+Data[2]+Data[3]) & 0xFF) == Data[4]){
        Unpack();
    }
}

void Unpack(void){
}

