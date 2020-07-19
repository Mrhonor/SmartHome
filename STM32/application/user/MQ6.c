#include "MQ6.h"

//PC0 -- DOut
uint32_t MQ6Value = 0;

void MQ6_Configuration(){
    GPIO_InitTypeDef        gpio;
    ADC_InitTypeDef         adc;
    DMA_InitTypeDef         dma;
    ADC_CommonInitTypeDef   adc_com;
    
    //ʱ��ʹ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,  ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,   ENABLE);
    
    
    gpio.GPIO_Pin                        =   GPIO_Pin_2;
    gpio.GPIO_Mode                       =   GPIO_Mode_AN;
    gpio.GPIO_Speed                      =   GPIO_Speed_2MHz;
    gpio.GPIO_PuPd                       =   GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &gpio);
    
    adc.ADC_ContinuousConvMode           = ENABLE;                          // ����ת��ģʽ
    adc.ADC_DataAlign                    = ADC_DataAlign_Right;             // ���ݶ���
    adc.ADC_ExternalTrigConvEdge         = ADC_ExternalTrigConvEdge_None;   // ��ֹ������⣬ʹ���������
    adc.ADC_ScanConvMode                 = DISABLE;                         // ��ͨ��AD���ر�ɨ��ģʽ
    adc.ADC_Resolution                   = ADC_Resolution_12b;              // 12 λģʽ
    adc.ADC_NbrOfConversion              = 1;                               // ת��ͨ��1��
    ADC_Init(ADC2,&adc); // ADC ��ʼ��
	
    adc_com.ADC_Mode                     = ADC_Mode_Independent;            // ����ADCģʽ
    adc_com.ADC_Prescaler                = ADC_Prescaler_Div2;              // 2��Ƶ
    adc_com.ADC_DMAAccessMode            = ADC_DMAAccessMode_Disabled;        
    ADC_CommonInit(&adc_com);
    
    ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 1, ADC_SampleTime_56Cycles);
    ADC_Cmd(ADC2,ENABLE); // ADC ʹ�� 	
	

    dma.DMA_Channel                     = DMA_Channel_1;
    dma.DMA_PeripheralBaseAddr          = (uint32_t)&ADC2->DR;               // �����ַ
    dma.DMA_Memory0BaseAddr             = (uint32_t)&MQ6Value;               // �洢������ַ
    dma.DMA_BufferSize                  = 1;                                 // ��������ݸ���
    dma.DMA_DIR                         = DMA_DIR_PeripheralToMemory;        // ָ������Ϊ����Դ
    dma.DMA_MemoryDataSize              = DMA_MemoryDataSize_HalfWord;       // �洢��ÿ�ν��������ֽ�
    dma.DMA_MemoryInc                   = DMA_MemoryInc_Enable;              // �洢����ַ����
    dma.DMA_Mode                        = DMA_Mode_Circular;                 // ����ѭ��ģʽ
    dma.DMA_PeripheralDataSize          = DMA_PeripheralDataSize_HalfWord;   // ����ÿ�δ��������ֽ�
    dma.DMA_PeripheralInc               = DMA_PeripheralInc_Disable;         // �����ַ������
    dma.DMA_Priority                    = DMA_Priority_High;                 // ����ͨ��ת�����ȼ�
    dma.DMA_FIFOMode                    = DMA_FIFOMode_Disable;              // ʹ��ֱ��ģʽ
    dma.DMA_MemoryBurst                 = DMA_MemoryBurst_Single;            // �洢��ͻ�����δ���
    dma.DMA_PeripheralBurst             = DMA_PeripheralBurst_Single;        // ����ͻ�����δ���
    DMA_Init(DMA2_Stream2, &dma); // ��ʼ��
	DMA_Cmd(DMA2_Stream2, ENABLE); //DMA ʹ��	
    
    ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
    ADC_DMACmd(ADC2, ENABLE); // ADC DMA ʹ��	
    ADC_SoftwareStartConv(ADC2); //��ʼADCת��
}
