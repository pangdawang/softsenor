/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : ADC.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/01/22
 * Description        : This file contains the GPIO CONFIG
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/01/22 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#include "adc.h"
#include "delay.h"		 
#include "init_flash.h"

uint16_t adc_sample_num ;
 __IO  uint16_t ADC1ConvertedValue[ADC_SIGNLE_SAMPLE_NUM][ADC_CHANNEL_NUM];

//��ʼ��ADC		
/*
*
*
*/
void  Adc_Init(uint8_t ad_samle_cycle)
{    
	ADC_InitTypeDef       ADC_InitStructure;                                        //ADC��ʼ���ṹ��
	DMA_InitTypeDef       DMA_InitStructure;                                        //DMA��ʼ���ṹ��
	GPIO_InitTypeDef      GPIO_InitStructure;                                       //GPIO��ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;

	/*--------ʱ�� config-------------*/
	
    /* ʹ�� ADC1, DMA1 �� GPIO ʱ�� ****************************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
    	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����Ϊ6 -----72M/6 = 12M,acd���ʱ��Ƶ�ʲ��ܳ���14Mhz

      /*****************����ADC1ͨ������Ϊģ������ģʽ******************************/

	  /*********************** gpio���� **************************************/

	//2wave_monitor----������û�򿪴˹���
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                                      //ģ������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                //�ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	//2 8·������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                                      //ģ������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                //�ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//�ɼ���ѹ������ΪPC4/PC5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                                      //ģ������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                //�ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	

    //**************************ADC1 ��ʼ�� ****************************************************************/
    // ����Ϊ��ʱ��3����ת�� DMAģʽ 8·ͨ���������ɨ��ģʽ��
    
	ADC_DeInit(ADC1);  //��λADC1 
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;                                  //ADC�����ڶ���ģʽ
	// TODO:��ȷ���Ƿ���Ҫ������ת��:
	// TODO:������Ҫ�ر�����ת��ģʽ����Ϊ������ת����ֻҪһ�δ�����ϣ�������ADת���Ͳ��Ƕ�ʱ��������
	//ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;                                  //ADC����ģʽ��
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;                                  //ADC����ģʽ��
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;                              //12λ�����Ҷ���
	//ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;                 //�ⲿ������  ���������
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;                 //ADC ת����TIME3���´���

	ADC_InitStructure.ADC_NbrOfChannel=ADC_CHANNEL_NUM;                              //����ͨ����Ϊ8
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;                                        //ɨ��ģʽ��
	ADC_Init(ADC1, &ADC_InitStructure);	                                              //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ��� 
	

	ADC_Cmd(ADC1, ENABLE);
  
	ADC_ResetCalibration(ADC1);	                //ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	                //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	    //�ȴ�У׼����
	 
	
	/* ���ö�ͨ��ת��˳�� */ 

	ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 1, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11, 2, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12, 3, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13, 4, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 5, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15, 6, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8, 7, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9, 8, ad_samle_cycle);
	
	/* ʹ�� ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/*********************** DMA1 Stream1 channe1 ���� **************************************/
	  
	//DMA �ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	adc_sample_num = ADC_CHANNEL_NUM*ADC_SIGNLE_SAMPLE_NUM;

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr= (uint32_t)&ADC1->DR;                  //����Դ��ַ  ADC���ݼĴ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)ADC1ConvertedValue;;             //�ڴ��д洢DMA������������ݻ���ַ
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;                                //������Ϊ����Դ
	DMA_InitStructure.DMA_BufferSize=adc_sample_num;                                            //1�������ݸ���??
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;                  //�����ַ������
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;                           //�ڴ��ַҪ����
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;       //DMA�������ݿ�� 
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;               //�ڴ�����Ϊ���� 16λ��ADC����Ϊ12λ��
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;                                   //DMA������ѭ��ģʽ
	DMA_InitStructure.DMA_Priority=DMA_Priority_High; 	                             //DMA���ȼ���
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;                                      //��ʹ���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                    //DMA��ʼ��
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);    	                                         //ʹ��DMA
	/* ����ADC����ͨ��ת�� */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}	
#if 0
void tim3_config(uint16_t period)
{
	uint16_t arr;

  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM3 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	arr = (uint16_t)(72000000/((period-ADC_SAMPLE_DELAY_VALU)*ADC_SIGNLE_SAMPLE_NUM)-1);
  
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = arr;          //���嶨ʱ�����Ƶ��Ϊ30000/8
  TIM_TimeBaseStructure.TIM_Prescaler = 0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* TIM3 TRGO selection */
  TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
  

}
#endif
void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1) !=RESET)
	{
		adc_convert_over_flag = 1;
		TIM_Cmd(TIM3, DISABLE);
		//TIM_Cmd(TIM6, DISABLE);
		DMA_ClearITPendingBit( DMA1_IT_TC1);
		//printf("enter the AD_DMA!!\r\n");

	}
}





















