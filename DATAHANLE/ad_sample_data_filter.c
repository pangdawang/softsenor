/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : ad_sample_data_filter.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : AD采样数据处理函数
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
static uint8_t max_min_subscript[ADC_CHANNEL_NUM][2];//0-max 下标1-min下标
static uint16_t ad_ppeak_value[ADC_CHANNEL_NUM] = {0};
static uint16_t sense_channel_mv[ADC_CHANNEL_NUM] = {0};
static uint16_t filter_sense_value[ADC_CHANNEL_NUM][DEFAULT_FILTER_DEEP] = {0};
static uint16_t average_value[ADC_CHANNEL_NUM]={0};
static uint8_t filer_count;

static uint16_t output_num=0;//当值大于1000时，输出时间并终止程序运行
//#define OPEN_DEBUG_RUN_TIME
/*************************************************
Function: get_sensor_change_value
Description: 每次AD采样（这里的采样是对于一个传感器输出的正弦波采样不是单次AD采样）完成后，
            对传感器输出值的计算，这里的输出值为mv
Calls: get_peak_peak--求峰峰值
      get_average---求平均值
		get_recur_average---求递推平均值

Called By: main.c
Input: 暂无
Output: 暂无
Return:暂无
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
#ifdef OPEN_AD_SINGLE_SAMPLE_DATA  //0:关闭了原始数据输出1:打开
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
	
	//求8路的峰峰值
	get_peak_peak((uint16_t (*)[8]) ADC1ConvertedValue,ADC_SIGNLE_SAMPLE_NUM,ADC_CHANNEL_NUM,ad_ppeak_value,max_min_subscript);
	for(i=0;i<ADC_CHANNEL_NUM;i++)
	{
		subscri_d_value = abs(max_min_subscript[i][0]-max_min_subscript[i][1]);
		if(i==TEST_CHANNEL)
		{
			//printf("max_subscript[%d]\t =%d \!!!\r\n",i+1,max_min_subscript[i][0]) ;							
			//printf("min_subscript[%d]\t =%d \!!!\r\n",i+1,max_min_subscript[i][1]) ;							
		}		
		//判断最值点下标是否在允许误差范围内
		if((subscri_d_value>(ADC_SIGNLE_SAMPLE_NUM+20)/2)||(subscri_d_value<(ADC_SIGNLE_SAMPLE_NUM-20)/2))
		{
			//1打印出错通道值
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
			filter_sense_value[i][filer_count] = ad_ppeak_value[i];//填充峰峰值
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
			mid_value=get_average(&filter_sense_value[i][0],filer_count);	//填充满后求平均值
			//mid_value =get_recur_average(&filter_sense_value[i][0],filer_count);//递推算术平均值
			average_value[i] =(average_value[i] +mid_value)/2;
			//sense_channel_mv[i] = GetVolt(average_value[i]);//转换为电压值
			sense_channel_mv[i] = (uint16_t)(average_value[i] * 3300 / 4096);

			#ifdef OPEN_SENSOR_VALUE_OUTPUT   //1打印开关
			if(i == TEST_CHANNEL)			//平均值打印输出
			{
				//1判断是否成功输出数据--wifi
				
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
						//printf("已经输出了2000次数据\r\n");
						output_num=0;
					}
				
				//printf("执行时间为%fs\r\n",run_time);
			}
			#endif
		}
		
		filer_count = 0;	
	}
}
/*************************************************
Function: GetVolt
Description: AD值转换为电压值(mv)
Calls:no
Called By: get_sensor_change_value---传感器变化值计算函数
Input: advalue
Output: 暂无
Return:电压值
Others: 
*************************************************/
uint16_t GetVolt(u16 advalue)

{

	return (uint16_t)(advalue * 3300 / 4096); //求的结果扩大了1000倍，方便下面求出小数

}

#endif

