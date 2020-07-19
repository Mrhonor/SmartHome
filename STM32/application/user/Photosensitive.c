#include "Photosensitive.h"

uint32_t LightValue;

void Photosensitive_Configuration(){
    GPIO_InitTypeDef        gpio;
    ADC_InitTypeDef         adc;
    DMA_InitTypeDef         dma;
    ADC_CommonInitTypeDef   adc_com;
    
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,  ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,   ENABLE);
    
    
    gpio.GPIO_Pin                        =   GPIO_Pin_1;
    gpio.GPIO_Mode                       =   GPIO_Mode_AN;
    gpio.GPIO_Speed                      =   GPIO_Speed_2MHz;
    gpio.GPIO_PuPd                       =   GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &gpio);
    
    adc.ADC_ContinuousConvMode           = ENABLE;                          // 连续转换模式
    adc.ADC_DataAlign                    = ADC_DataAlign_Right;             // 数据对齐
    adc.ADC_ExternalTrigConvEdge         = ADC_ExternalTrigConvEdge_None;   // 禁止触发检测，使用软件触发
    adc.ADC_ScanConvMode                 = DISABLE;                         // 单通道AD，关闭扫描模式
    adc.ADC_Resolution                   = ADC_Resolution_12b;              // 12 位模式
    adc.ADC_NbrOfConversion              = 1;                               // 转换通道1个
    ADC_Init(ADC3,&adc); // ADC 初始化
	
    adc_com.ADC_Mode                     = ADC_Mode_Independent;            // 独立ADC模式
    adc_com.ADC_Prescaler                = ADC_Prescaler_Div2;              // 2分频
    adc_com.ADC_DMAAccessMode            = ADC_DMAAccessMode_Disabled;        
    ADC_CommonInit(&adc_com);
    
    ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 1, ADC_SampleTime_56Cycles);
    ADC_Cmd(ADC3,ENABLE); // ADC 使能 	
	

    dma.DMA_Channel                     = DMA_Channel_2;
    dma.DMA_PeripheralBaseAddr          = (uint32_t)&ADC3->DR;               // 外设地址
    dma.DMA_Memory0BaseAddr             = (uint32_t)&LightValue;             // 存储区基地址
    dma.DMA_BufferSize                  = 1;                                 // 整体的数据个数
    dma.DMA_DIR                         = DMA_DIR_PeripheralToMemory;        // 指定外设为发送源
    dma.DMA_MemoryDataSize              = DMA_MemoryDataSize_HalfWord;       // 存储区每次接收两个字节
    dma.DMA_MemoryInc                   = DMA_MemoryInc_Enable;              // 存储区地址自增
    dma.DMA_Mode                        = DMA_Mode_Circular;                 // 开启循环模式
    dma.DMA_PeripheralDataSize          = DMA_PeripheralDataSize_HalfWord;   // 外设每次传输两个字节
    dma.DMA_PeripheralInc               = DMA_PeripheralInc_Disable;         // 外设地址不自增
    dma.DMA_Priority                    = DMA_Priority_High;                 // 设置通道转换优先级
    dma.DMA_FIFOMode                    = DMA_FIFOMode_Disable;              // 使用直连模式
    dma.DMA_MemoryBurst                 = DMA_MemoryBurst_Single;            // 存储器突发单次传输
    dma.DMA_PeripheralBurst             = DMA_PeripheralBurst_Single;        // 外设突发单次传输
    DMA_Init(DMA2_Stream1, &dma); // 初始化
	DMA_Cmd(DMA2_Stream1, ENABLE); //DMA 使能	
    
    ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
    ADC_DMACmd(ADC3, ENABLE); // ADC DMA 使能	
    ADC_SoftwareStartConv(ADC3); //开始ADC转换
    
}
