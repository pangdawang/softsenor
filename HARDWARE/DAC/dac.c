#include "dac.h"
#include "init_config.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DAC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//DAC通道1输出初始化
const u16 Sine12bit[256] = { //正弦波描点
2048, 2098, 2148, 2198, 2248, 2298, 2348, 2398, 2447, 2496,
2545, 2594, 2642, 2690, 2737, 2785, 2831, 2877, 2923, 2968,
3013, 3057, 3100, 3143, 3185, 3227, 3267, 3307, 3347, 3385,
3423, 3460, 3496, 3531, 3565, 3598, 3631, 3662, 3692, 3722,
3750, 3778, 3804, 3829, 3854, 3877, 3899, 3920, 3940, 3958,
3976, 3992, 4007, 4021, 4034, 4046, 4056, 4065, 4073, 4080,
4086, 4090, 4093, 4095, 4095, 4095, 4093, 4090, 4086, 4080,
4073, 4065, 4056, 4046, 4034, 4021, 4007, 3992, 3976, 3958,
3940, 3920, 3899, 3877, 3854, 3829, 3804, 3778, 3750, 3722,
3692, 3662, 3631, 3598, 3565, 3531, 3496, 3460, 3423, 3385,
3347, 3307, 3267, 3227, 3185, 3143, 3100, 3057, 3013, 2968,
2923, 2877, 2831, 2785, 2737, 2690, 2642, 2594, 2545, 2496,
2447, 2398, 2348, 2298, 2248, 2198, 2148, 2098, 2047, 1997,
1947, 1897, 1847, 1797, 1747, 1697, 1648, 1599, 1550, 1501,
1453, 1405, 1358, 1310, 1264, 1218, 1172, 1127, 1082, 1038,
995, 952, 910, 868, 828, 788, 748, 710, 672, 635,
599, 564, 530, 497, 464, 433, 403, 373, 345, 317,
291, 266, 241, 218, 196, 175, 155, 137, 119, 103,
88, 74, 61, 49, 39, 30, 22, 15, 9, 5,
2, 0, 0, 0, 2, 5, 9, 15, 22, 30,
39, 49, 61, 74, 88, 103, 119, 137, 155, 175,
196, 218, 241, 266, 291, 317, 345, 373, 403, 433,
464, 497, 530, 564, 599, 635, 672, 710, 748, 788,
828, 868, 910, 952, 995, 1038, 1082, 1127, 1172, 1218,
1264, 1310, 1358, 1405, 1453, 1501, 1550, 1599, 1648, 1697,
1747, 1797, 1847, 1897, 1947, 1997 };

#define PI 3.1415926
//#define USE_SIN_CUR_CREAT_FUN

void Dac1_Init(void)
{
  
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
  	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*--------时钟 config-------------*/

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 
   	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

#ifdef open_dac_sin_out
	//GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 输出高
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_T6_TRGO;	//定时器6更新触发DA转换
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
	
	DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	//DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
	//DAC_DMACmd(DAC_Channel_1, ENABLE);//使能DMA模式
	//DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值

	//-------------------------DMA CONFIG-------------------------------//
	//DMA 中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr= (uint32_t)(DAC_DHR12R1_ADDRESS);                  //数据源地址  ADC数据寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)Sine12bit;;             //内存中存储DMA传输过来的数据基地址
	DMA_InitStructure.DMA_BufferSize= sizeof(Sine12bit)/2;                                            //缓存数据个数
	//DMA_InitStructure.DMA_BufferSize= 256;                                            //缓存数据个数
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;                                //从内存到外设
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;                  //外设地址不增
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;                           //内存地址要增加
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;       //DMA外设数据宽度 
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;               //内存数据为半字 16位（ADC精度为12位）
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;                                   //DMA工作在循环模式
	DMA_InitStructure.DMA_Priority=DMA_Priority_High; 	                             //DMA优先级高
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;                                      //不使用内存到内存
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);                                    //DMA初始化
	DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);

  /* DMA1_Stream6 channel7 configuration **************************************/
	DMA_Cmd(DMA2_Channel3, ENABLE);    	                                         //使能DMA

  /* Enable DMA1_Stream6 */
	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
	DAC_DMACmd(DAC_Channel_1, ENABLE);//使能DMA模式

	//DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
	
#else
	//GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 输出高
					
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
	DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
	DAC_DMACmd(DAC_Channel_1, ENABLE);//使能DMA模式
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
	#endif

}
void SIN_CUR_Creat(uint16_t cycle_num, uint16_t * cur_ptr)
{
	uint16_t i;
	float sin_umidd;
	double arc_value = 0;
	sin_umidd=3.3/2;
	for(i =0;i<cycle_num;i++)
	{
		arc_value = (double)(2*PI*i/(cycle_num-1))+1;
		cur_ptr[i]=(uint16_t)(2048*sin(arc_value)+2048);
	}
}

#if 0 
void tim6_config(uint16_t period)
{
	uint16_t arr;
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  /* --------------------------------------------------------
  TIM3 input clock (TIM6CLK) is set to 2 * APB1 clock (PCLK1), 
  since APB1 prescaler is different from 1.   
    TIM6CLK = 2 * PCLK1  
    TIM6CLK = HCLK / 2 = SystemCoreClock /2 
          
  TIM6 Update event occurs each TIM6CLK/256 

  Note: 
   SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f2xx.c file.
   Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
   function to update SystemCoreClock variable value. Otherwise, any configuration
   based on this variable will be incorrect.    

  ----------------------------------------------------------- */
  /* Time base configuration */
	arr = (uint16_t)((72000000/(period*SIN_PERIOD_NUM))-1);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = arr;          //定义定时器溢出周期为period*precaler/36000000
  TIM_TimeBaseStructure.TIM_Prescaler = 0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  /* TIM6 TRGO selection */
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
  

}

#endif
/*
void set_sin_freq(u16 freq)
{
	uint16_t arr;
	arr = (uint16_t)((72000000/(SIN_PERIOD_NUM*freq))-1);
	// 
	TIM_SetAutoreload( TIM6, arr);
}
*/
//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12位右对齐数据格式设置DAC值
}

void DMA2_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_IT_TC3) !=RESET)
	{
		//DMA_Cmd(DMA2_Channel3, DISABLE);    	                                         //使能DMA
		TIM_Cmd(TIM6, DISABLE);
		DMA_ClearITPendingBit( DMA2_IT_TC3);
		//printf("enter the AD_DMA!!\r\n");

	}
}





















































