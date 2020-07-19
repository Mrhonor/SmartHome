#include "Buzzer.h"

uint32_t RingTime = 0;

void Buzzer_Configuration(){
    GPIO_InitTypeDef         gpio;
    NVIC_InitTypeDef         nvic;
    TIM_TimeBaseInitTypeDef  tim;
    
    
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,  ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,  ENABLE); 
    
    
    gpio.GPIO_Pin                        =   GPIO_Pin_13;
    gpio.GPIO_Mode                       =   GPIO_Mode_OUT;
    gpio.GPIO_Speed                      =   GPIO_Speed_2MHz;
    gpio.GPIO_PuPd                       =   GPIO_PuPd_UP;
    gpio.GPIO_OType                      =   GPIO_OType_PP;
    GPIO_Init(GPIOB, &gpio);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    nvic.NVIC_IRQChannel                   = TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority        = 0;
    nvic.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&nvic);
    
    tim.TIM_Period                         = 84000;
    tim.TIM_Prescaler                      = 999;

    //清除更新中断标志位
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
    
    //开启定时器更新中断标志位
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    
}

void TIM6_DAC_IRQnHandle(void){
    if(TIM_GetFlagStatus(TIM6, TIM_IT_Update) != RESET){
        if(--RingTime == 0){
            TIM_Cmd(TIM6, DISABLE);
            GPIOB->BSRRL = GPIO_Pin_13;
        }
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}

void RingTheBuzzer(uint32_t s){
    //使能定时器
    TIM_Cmd(TIM6, ENABLE);
    RingTime = s;
    GPIOB->BSRRH = GPIO_Pin_13;
}
