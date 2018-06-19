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

//初始化ADC		
/*
*
*
*/
void  Adc_Init(uint8_t ad_samle_cycle)
{    
	ADC_InitTypeDef       ADC_InitStructure;                                        //ADC初始化结构体
	DMA_InitTypeDef       DMA_InitStructure;                                        //DMA初始化结构体
	GPIO_InitTypeDef      GPIO_InitStructure;                                       //GPIO初始化结构体
	NVIC_InitTypeDef NVIC_InitStructure;

	/*--------时钟 config-------------*/
	
    /* 使能 ADC1, DMA1 和 GPIO 时钟 ****************************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
    	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子为6 -----72M/6 = 12M,acd最大时钟频率不能超过14Mhz

      /*****************配置ADC1通道引脚为模拟输入模式******************************/

	  /*********************** gpio配置 **************************************/

	//2wave_monitor----程序中没打开此功能
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                                      //模拟输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                //速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	//2 8路传感器
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                                      //模拟输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                //速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//采集电压的引脚为PC4/PC5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;                                      //模拟输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                //速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	

    //**************************ADC1 初始化 ****************************************************************/
    // 配置为定时器3触发转换 DMA模式 8路通道连续输出扫描模式打开
    
	ADC_DeInit(ADC1);  //复位ADC1 
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;                                  //ADC工作在独立模式
	// TODO:待确定是否需要打开连续转换:
	// TODO:这里需要关闭连续转换模式，因为打开连续转换后只要一次触发完毕，后续的AD转换就不是定时器触发了
	//ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;                                  //ADC连续模式开
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;                                  //ADC连续模式关
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;                              //12位数据右对齐
	//ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;                 //外部开启关  由软件开启
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T3_TRGO;                 //ADC 转换由TIME3更新触发

	ADC_InitStructure.ADC_NbrOfChannel=ADC_CHANNEL_NUM;                              //开启通道数为8
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;                                        //扫描模式开
	ADC_Init(ADC1, &ADC_InitStructure);	                                              //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器 
	

	ADC_Cmd(ADC1, ENABLE);
  
	ADC_ResetCalibration(ADC1);	                //使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	                //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	    //等待校准结束
	 
	
	/* 配置多通道转换顺序 */ 

	ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 1, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11, 2, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12, 3, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13, 4, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 5, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15, 6, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8, 7, ad_samle_cycle);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9, 8, ad_samle_cycle);
	
	/* 使能 ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/*********************** DMA1 Stream1 channe1 配置 **************************************/
	  
	//DMA 中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	adc_sample_num = ADC_CHANNEL_NUM*ADC_SIGNLE_SAMPLE_NUM;

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr= (uint32_t)&ADC1->DR;                  //数据源地址  ADC数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)ADC1ConvertedValue;;             //内存中存储DMA传输过来的数据基地址
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;                                //以外设为数据源
	DMA_InitStructure.DMA_BufferSize=adc_sample_num;                                            //1缓存数据个数??
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;                  //外设地址不增加
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;                           //内存地址要增加
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;       //DMA外设数据宽度 
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;               //内存数据为半字 16位（ADC精度为12位）
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;                                   //DMA工作在循环模式
	DMA_InitStructure.DMA_Priority=DMA_Priority_High; 	                             //DMA优先级高
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;                                      //不使用内存到内存
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                    //DMA初始化
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);    	                                         //使能DMA
	/* 开启ADC规则通道转换 */
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
  TIM_TimeBaseStructure.TIM_Period = arr;          //定义定时器溢出频率为30000/8
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





















