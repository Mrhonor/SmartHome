#include "PCF8563.h"

void PCF8563_Configuration(){
    GPIO_InitTypeDef  gpio;
    I2C_InitTypeDef   iic;
    
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,  ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,   ENABLE);
    
    //配置SCL引脚
    gpio.GPIO_Pin              =   GPIO_Pin_6;
    gpio.GPIO_Mode             =   GPIO_Mode_AF;
    gpio.GPIO_Speed            =   GPIO_Speed_50MHz;
    gpio.GPIO_PuPd             =   GPIO_PuPd_NOPULL;
    gpio.GPIO_OType            =   GPIO_OType_OD;  //设置为开漏模式
    GPIO_Init(GPIOB, &gpio);
    
    
    //连接引脚到IIC1_SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);

    //配置SDA引脚
    gpio.GPIO_Pin              =   GPIO_Pin_7;
    GPIO_Init(GPIOB, &gpio);
    
    //连接引脚到IIC1_SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

    iic.I2C_Mode                = I2C_Mode_I2C;
    iic.I2C_Ack                 = I2C_Ack_Enable;
    iic.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //PCF8563为7Bit寻找模式
    iic.I2C_DutyCycle           = I2C_DutyCycle_2;        // PCF8563低电平1.3μs，高电平时间0.6μs，属于2：1模式
    iic.I2C_ClockSpeed          = 400000;                 // 400kHz
    iic.I2C_OwnAddress1         = 0x0A;                   // 自身地址
    I2C_Init(I2C1, &iic);
    I2C_Cmd(I2C1, ENABLE);
    
    //启动PCF8563
    IIC_WriteData(PCF8563_WRITE_ADD, 0x00, 0x00);
    
}

void IIC_WriteData(uint8_t SlaveAdd, uint8_t Add, uint8_t Data){
    uint32_t WaitTime = 10000;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)){
        if(WaitTime-- == 0) return;
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return;
        }
    }
    
    I2C_Send7bitAddress(I2C1, SlaveAdd, I2C_Direction_Transmitter);
    
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return;
        }
    }
    
    I2C_SendData(I2C1, Add);
    
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return;
        }
    }
    
    I2C_SendData(I2C1, Data);
    
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return;
        }
    }
    
    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t IICWriteAndReadData(uint8_t SlaveWriteAdd, uint8_t Add, uint8_t SlaveReadAdd){
    uint32_t WaitTime = 10000;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)){
        if(WaitTime-- == 0) return 0;
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return 0;
        }
    }
    
    I2C_Send7bitAddress(I2C1, SlaveWriteAdd, I2C_Direction_Transmitter);
    
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return 0;
        }
    }
    
    I2C_SendData(I2C1, Add);
    
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return 0;
        }
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return 0;
        }
    }
    
    I2C_Send7bitAddress(I2C1, SlaveReadAdd, I2C_Direction_Receiver);
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return 0;
        }
    }
    

    
    WaitTime = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)){
        if(WaitTime-- == 0) {
            I2C_GenerateSTOP(I2C1, ENABLE);
            return 0;
        }
    }
    uint8_t data = I2C_ReceiveData(I2C1);
    
    I2C_GenerateSTOP(I2C1, ENABLE);
    return BCDtoData(data);
}

uint8_t BCDtoData(uint8_t Data){
    // BCD码转二进制
    return 0;
}
