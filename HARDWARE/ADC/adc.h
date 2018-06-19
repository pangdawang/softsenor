/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : ADC.h
 * Author             : pangda_wang
 * Date First Issued  : 2018/01/22
 * Description        : This file contains the GPIO CONFIG
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/01/22 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
 #ifndef __ADC_H
#define __ADC_H	

#include "sys.h" 
#include "init_config.h"


//#define LIMINGYU_BOARD
#define ALIENTEK_BOARD 
//#define DEBUG_2_AD_CHANNEL

#ifndef GLOBAL extern 
#define GLOBAL extern
#else
#define GLOBAL
#endif

#define ADC_CONVERT_MODE_1_CHANNEL    1
#define ADC_CONVERT_MODE_2_CHANNEL    2

#define ADC_CONVERT_MODE_3_CHANNEL	3
#define ADC_CONVERT_MODE_7_CHANNELS 	7


//#define ADC_SAMPLE_NUM    720//一周期内的采样个数为通道数(8)*单通道内采样数(36)

#define ADC_CONVERT_CHANNEL	8
#define ADC_CHANNEL_NUM 8 //ADC通道num
#define ADC_SIGNLE_SAMPLE_NUM 90//每个通道的采样次数
#define ADC_SAMPLE_DELAY_VALU 23
//#define ADC_SAMPLE_NUM     1000//50hz一周期内的采样个数设置为1000
GLOBAL uint8_t adc_convert_over_flag ;
extern __IO  uint16_t ADC1ConvertedValue[ADC_SIGNLE_SAMPLE_NUM][ADC_CHANNEL_NUM];
#undef GLOBAL


void  Adc_Init(uint8_t ad_samle_cycle); 				//ADC通道初始化
void tim3_config(uint16_t period);

#endif 







