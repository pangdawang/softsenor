/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : ad_sample_data_filter.h
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : AD�������ݴ�����ͷ�ļ�
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

#define TEST_CHANNEL 4//��ӡ���ͨ����---���԰汾����ʾ����(��ֵ+1)


/*-------�ⲿ��������-------*/
extern __IO  uint16_t ADC1ConvertedValue[ADC_SIGNLE_SAMPLE_NUM][ADC_CHANNEL_NUM];

//���Կ���
//#define OPEN_AD_SINGLE_SAMPLE_DATA //��ͨ���Ĳ���ֵ������أ�����FFP�����ϴ�ʱ��
#define OPEN_SENSOR_VALUE_OUTPUT //��������˲��Ĵ�����ֵ
/*----------��������--------*/
uint16_t GetVolt(u16 advalue);
void get_sensor_change_value(WIFI_MODE_ENUM wifi_mode);

#endif

