/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : ad_sample_data_filter.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : AD�������ݴ�����
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/6 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _AD_SAMPLE_FILTER_C_
#define _AD_SAMPLE_FILTER_C_

#include "stm32f10x.h"
#include "ad_sample_data_filter.h"
#include "init_config.h"
#include "common_arithmetic.h"
#include "time.h"

/*----extern varable----*/
extern time_t start_time;

/*--------private varable--------*/
static uint8_t max_min_subscript[ADC_CHANNEL_NUM][2];//0-max �±�1-min�±�
static uint16_t ad_ppeak_value[ADC_CHANNEL_NUM] = {0};
static uint16_t sense_channel_mv[ADC_CHANNEL_NUM] = {0};
static uint16_t filter_sense_value[ADC_CHANNEL_NUM][DEFAULT_FILTER_DEEP] = {0};
static uint16_t average_value[ADC_CHANNEL_NUM]={0};
static uint8_t filer_count;

static uint16_t output_num=0;//��ֵ����1000ʱ�����ʱ�䲢��ֹ��������
//#define OPEN_DEBUG_RUN_TIME
/*************************************************
Function: get_sensor_change_value
Description: ÿ��AD����������Ĳ����Ƕ���һ����������������Ҳ��������ǵ���AD��������ɺ�
            �Դ��������ֵ�ļ��㣬��������ֵΪmv
Calls: get_peak_peak--����ֵ
      get_average---��ƽ��ֵ
		get_recur_average---�����ƽ��ֵ

Called By: main.c
Input: ����
Output: ����
Return:����
Others: 
*************************************************/
void get_sensor_change_value(WIFI_MODE_ENUM wifi_mode)
{
   	uint16_t i,j;
	uint16_t m,n;
	uint16_t period,mid_value;
	uint8_t subscri_d_value;
	u16 tim7_count_num=0;
	time_t end_time;
	double run_time;
	char *print_data;
	u8 print_data_length;
#ifdef OPEN_AD_SINGLE_SAMPLE_DATA  //0:�ر���ԭʼ�������1:��
	for(i =0;i<ADC_CHANNEL_NUM;i++)
	{
		for(j=0;j<ADC_SIGNLE_SAMPLE_NUM;j++)
		{
			if(i==TEST_CHANNEL)
			{
				printf("sense_channel[%d]serialnum[%d]\t =%d\r\n",i+1,j,ADC1ConvertedValue[j][i]) ;
			}

		}
	
	}
#endif
	
	//��8·�ķ��ֵ
	get_peak_peak((uint16_t (*)[8]) ADC1ConvertedValue,ADC_SIGNLE_SAMPLE_NUM,ADC_CHANNEL_NUM,ad_ppeak_value,max_min_subscript);
	for(i=0;i<ADC_CHANNEL_NUM;i++)
	{
		subscri_d_value = abs(max_min_subscript[i][0]-max_min_subscript[i][1]);
		if(i==TEST_CHANNEL)
		{
			//printf("max_subscript[%d]\t =%d \!!!\r\n",i+1,max_min_subscript[i][0]) ;							
			//printf("min_subscript[%d]\t =%d \!!!\r\n",i+1,max_min_subscript[i][1]) ;							
		}		
		//�ж���ֵ���±��Ƿ���������Χ��
		if((subscri_d_value>(ADC_SIGNLE_SAMPLE_NUM+20)/2)||(subscri_d_value<(ADC_SIGNLE_SAMPLE_NUM-20)/2))
		{
			//1��ӡ����ͨ��ֵ
			//printf("an err occerd in the %s at the  %d line_value \r\n",__FILE__ ,__LINE__ ); 
			/*
			if(i ==TEST_CHANNEL)
			{
				printf("channel[%d]max subcript is \t %d \r\n",i+1,max_min_subscript[i][0]) ;							
				printf("channel[%d]min subcript is \t %d  \r\n",i+1,max_min_subscript[i][1]) ;							
	
			}
			*/
		}
		else
		{
			filter_sense_value[i][filer_count] = ad_ppeak_value[i];//�����ֵ
		}
	}
	
	if(filer_count < (saved_in_flash_para.filter_deep-1))
	{
		filer_count++;
	}
	else
	{
		//printf("an err occerd in the %s at the  %d line_value \r\n",__FILE__ ,__LINE__ ); 			
		for(i =0;i<ADC_CHANNEL_NUM;i++)
		{
			mid_value =0;
			mid_value=get_average(&filter_sense_value[i][0],filer_count);	//���������ƽ��ֵ
			//mid_value =get_recur_average(&filter_sense_value[i][0],filer_count);//��������ƽ��ֵ
			average_value[i] =(average_value[i] +mid_value)/2;
			//sense_channel_mv[i] = GetVolt(average_value[i]);//ת��Ϊ��ѹֵ
			sense_channel_mv[i] = (uint16_t)(average_value[i] * 3300 / 4096);

			#ifdef OPEN_SENSOR_VALUE_OUTPUT   //1��ӡ����
			if(i == TEST_CHANNEL)			//ƽ��ֵ��ӡ���
			{
				//1�ж��Ƿ�ɹ��������--wifi
				
				print_data_length = strlen("chanl%d is\t%d\tmv  ");
				print_data =(char *) malloc(print_data_length);
				sprintf(print_data,"chanl%d is\t%d\tmv  ",i,sense_channel_mv[i-1]) ;							
				wifi_printf(wifi_mode,print_data,print_data_length);
				sprintf(print_data,"chanl%d is\t%d\tmv  ",i+1,sense_channel_mv[i]) ;							
				wifi_printf(wifi_mode,print_data,print_data_length);
				sprintf(print_data,"chanl%d is\t%d\tmv  ",i+2,sense_channel_mv[i+1]) ;							
				wifi_printf(wifi_mode,print_data,print_data_length);
				print_data_length = strlen("\r\n");
				sprintf(print_data,"\r\n") ;							
				wifi_printf(wifi_mode,print_data,print_data_length);
				free(print_data);
				//print_data_length = strlen("%d|%d\t\t%d|%d\t\t%d|%d\t\t\r\n");
/*					print_data =(char *) malloc(32);
				sprintf(print_data,"chl%d|%d\tchl%d|%d\tchl%d|%d\t\r\n",i,sense_channel_mv[i-1],i+1,sense_channel_mv[i],i+2,sense_channel_mv[i+1]) ;							
				wifi_printf(wifi_mode,print_data,32);
				free(print_data);
				*/
				//printf("%d|%d|%d!\t\t\r\n",sense_channel_mv[i-1],sense_channel_mv[i],sense_channel_mv[i+1]) ;							
					if(output_num++>2000)
					{
						//printf("�Ѿ������2000������\r\n");
						output_num=0;
					}
				
				//printf("ִ��ʱ��Ϊ%fs\r\n",run_time);
			}
			#endif
		}
		
		filer_count = 0;	
	}
}
/*************************************************
Function: GetVolt
Description: ADֵת��Ϊ��ѹֵ(mv)
Calls:no
Called By: get_sensor_change_value---�������仯ֵ���㺯��
Input: advalue
Output: ����
Return:��ѹֵ
Others: 
*************************************************/
uint16_t GetVolt(u16 advalue)

{

	return (uint16_t)(advalue * 3300 / 4096); //��Ľ��������1000���������������С��

}

#endif

