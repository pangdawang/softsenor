/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : ad_sample_data_filter.h
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : AD采样数据处理函数头文件
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/6 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _AD_SAMPLE_DATA_FILTER_H_
#define _AD_SAMPLE_DATA_FILTER_H_

#include "ADC.h"
//#include "init_config.h"
#include "init_flash.h"

#ifndef GLOBAL extern 
#define GLOBAL extern
#else
#define GLOBAL
#endif

//GLOBAL __IO  uint16_t ADC1ConvertedValue[ADC_SIGNLE_SAMPLE_NUM][ADC_CHANNEL_NUM];

#undef GLOBAL

#define TEST_CHANNEL 4//打印输出通道数---测试版本中显示的是(此值+1)


/*-------外部变量声明-------*/
extern __IO  uint16_t ADC1ConvertedValue[ADC_SIGNLE_SAMPLE_NUM][ADC_CHANNEL_NUM];

//调试开关
//#define OPEN_AD_SINGLE_SAMPLE_DATA //单通道的采样值输出开关，用于FFP数据上传时用
#define OPEN_SENSOR_VALUE_OUTPUT //输出经过滤波的传感器值
/*----------函数声明--------*/
uint16_t GetVolt(u16 advalue);
void get_sensor_change_value(WIFI_MODE_ENUM wifi_mode);

#endif

