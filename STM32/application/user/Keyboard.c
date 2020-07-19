#include "Keyboard.h"

enum EKeyboardSetting KeyboardStatus = EShow;
uint8_t SettingKey;
uint8_t RecordKey[10];
uint8_t CurrentIndex = 0;

void KeyBoard_Configuration(){
    GPIO_InitTypeDef  gpio_in;
    GPIO_InitTypeDef  gpio_out;
    GPIO_InitTypeDef  gpio_nvic;
    
    NVIC_InitTypeDef  nvic;
    EXTI_InitTypeDef  exti;
    
    //ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,  ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    //���������
    gpio_in.GPIO_Pin                        =   GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    gpio_in.GPIO_Mode                       =   GPIO_Mode_IN;
    gpio_in.GPIO_Speed                      =   GPIO_Speed_2MHz;
    gpio_in.GPIO_PuPd                       =   GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &gpio_in);
    
    //������ѯ��ѯ��
    gpio_out.GPIO_Pin                       =   GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    gpio_out.GPIO_Mode                      =   GPIO_Mode_OUT;
    gpio_out.GPIO_OType                     =   GPIO_OType_PP;
    gpio_out.GPIO_Speed                     =   GPIO_Speed_2MHz;
    gpio_out.GPIO_PuPd                      =   GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &gpio_out);
    
    //�����жϿ�
    gpio_nvic.GPIO_Pin                        =   GPIO_Pin_15;
    gpio_nvic.GPIO_Mode                       =   GPIO_Mode_IN;
    gpio_nvic.GPIO_Speed                      =   GPIO_Speed_2MHz;
    gpio_nvic.GPIO_PuPd                       =   GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &gpio_nvic);
    
    //�ж�����
    nvic.NVIC_IRQChannel                    = EXTI15_10_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority  = 0;
    nvic.NVIC_IRQChannelSubPriority         = 0;
    nvic.NVIC_IRQChannelCmd                 = ENABLE;
    NVIC_Init(&nvic);    
    
    //�ж�Դ����
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource15);
    
    exti.EXTI_Line    = EXTI_Line15;
    exti.EXTI_Mode    = EXTI_Mode_Interrupt;
    exti.EXTI_Trigger = EXTI_Trigger_Falling; //ѡ���½��ش���
    exti.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti);
    
    //Ĭ������͵�ƽ
    GPIOD->BSRRL = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
}

void EXTI15_10_IRQHandler(void){
    uint8_t Key = FindPressedKey();
    if(Key == 0) return;
    RingTheBuzzer(1);
    switch(KeyboardStatus){
        case EShow:
            if(Key == KEY1 || Key == KEY2 || Key == KEY3){
                SettingKey = Key;
                SwitchKeyboardStatus(ESetting);
            }
            break;
        case ESetting:
            switch(Key){
                case KEY4:
                    SwitchKeyboardStatus(EShow);
                    CurrentIndex = 0;
                    break;
                case ENTER:
                    //ChangeParamrter(SettingKey, RecordKey, CurrentIndex);
                    CurrentIndex = 0;
                    SwitchKeyboardStatus(EShow);
                    break;
                case BACKSPACE:
                    if(CurrentIndex > 0){
                        LCD1602_Write(0x80+CurrentIndex, 0);
                        LCD1602_Write(0xA0, 1);       //��ǰλд��ո񣬸���֮ǰ����
                        CurrentIndex--;
                    }
                    break;
                default:
                    if(Key != KEY1 && Key != KEY2 && Key != KEY3 && CurrentIndex < 10){
                        RecordKey[CurrentIndex] = Key;
                        CurrentIndex++;
                        LCD1602_Write(0x80+CurrentIndex, 0);
                        LCD1602_Write(0x30+RecordKey[CurrentIndex], 1); //��Һ��д������
                    }
                    break;
                    
            }
            break;
        default:
            break;
    }
}

uint8_t FindPressedKey(){
    uint8_t PressNum;
    GPIOD->BSRRH = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;        
    GPIOD->BSRRL = GPIO_Pin_0;
    delay(1000);   //�ȴ���ƽ�ȶ�
    PressNum = IsAnyKeyPress();
    GPIOD->BSRRH = GPIO_Pin_0;
    if(PressNum != 0) return PressNum;
    
    GPIOD->BSRRL = GPIO_Pin_1;
    delay(1000);   //�ȴ���ƽ�ȶ�
    PressNum = IsAnyKeyPress();
    GPIOD->BSRRH = GPIO_Pin_1;
    if(PressNum != 0) return PressNum + 4;
    
    GPIOD->BSRRL = GPIO_Pin_2;
    delay(1000);   //�ȴ���ƽ�ȶ�
    PressNum = IsAnyKeyPress();
    GPIOD->BSRRH = GPIO_Pin_2;
    if(PressNum != 0) return PressNum + 8;
    
    GPIOD->BSRRL = GPIO_Pin_3;
    delay(1000);   //�ȴ���ƽ�ȶ�
    PressNum = IsAnyKeyPress();
    GPIOD->BSRRH = GPIO_Pin_3;
    if(PressNum != 0) return PressNum + 12;
    
    return 0;
}

uint8_t IsAnyKeyPress(void){
    //��������Ƿ�Ϊ�͵�ƽ
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) == Bit_RESET){
        delay(1000); //��ʱ���ٴμ��
        if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) == Bit_RESET) return 1;
    }
    
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) == Bit_RESET){
        delay(1000); //��ʱ���ٴμ��
        if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) == Bit_RESET) return 2;
    }
    
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == Bit_RESET){
        delay(1000); //��ʱ���ٴμ��
        if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == Bit_RESET) return 3;
    }

    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == Bit_RESET){
        delay(1000); //��ʱ���ٴμ��
        if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == Bit_RESET) return 4;
    }
    
    return 0;
}

void SwitchKeyboardStatus(enum EKeyboardSetting NewState){
    if(NewState == KeyboardStatus) return;
    KeyboardStatus = NewState;
    
    if(KeyboardStatus == EShow){
        //��ʾ����
        //LCD1602_Show()
    }
    else{
        LCD1602_Write(0x38, 0);
    }
}
