/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : usmart_debug.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/08
 * Description        : 串口调试命令文件
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/08 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
 #ifndef _USMART_DEBUG_C_
 #define _USMART_DEBUG_C_
#include "usmart_debug.h"

/*************************************************
Function: set_da_sin_freq
Description: 修改的正弦波频率:
Calls: TIM_SetAutoreload-定时器自动重装载函数(库函数)
Called By: stmflash.c---串口调试文件
Input: TIMx-----对应的定时器
sin_freq----正弦波波形频率值
Output: 暂无
Return:暂无
Others: 该函数仅适用于软传感器项目
*************************************************/
void set_sin_freq( uint8_t TIMER_NUM, uint16_t sin_freq)
{
	TIM_TypeDef* TIMx;
	uint32_t arr;
	
	switch(TIMER_NUM)
	{
		case 3:
			TIMx = TIM3;
			break;
		case 6:
			TIMx = TIM6;
			break;
		default:
			TIMx = 0;
			break;
	}
	if(TIMx !=0)
	{
		arr = (uint16_t)((72000000/(sin_freq*SIN_PERIOD_NUM))-1);

		TIM_SetAutoreload(TIMx,arr);	

	}
}

/*************************************************
Function: write_para_to_flash
Description: 将参数写入flash
Calls: TIM_SetAutoreload-定时器自动重装载函数(库函数)
Called By: stmflash.c---串口调试文件
Input: para_serial-----参数对应序列号
para_value----参数值
write_flag----写入标志位:0--不写入1--写入
Output: 暂无
Return:暂无
Others: 该函数仅适用于软传感器项目
*************************************************/
void write_para_to_flash_debug( PARA_SERIAL_ENUM serial_num, uint16_t para_value)
{
	uint8_t write_flag = 0xff;
	
	assert_param(IS_THE_PARA_SERIAL(serial_num));
	
	switch(serial_num)
	{
		case WRITE_FILL:
			IS_THE_WRITE_FLAG(para_value);
			if(para_value == FILL)
			{
				write_flag = 1;//填充
			}
			else
			{
				write_flag = 0;//命令是写入
			}
			break;
		case DA_SIN_FREQ:
			IS_DA_SIN_FREQ_LEGAL(para_value);
			break;
		case SENSOR_SIN_FREQ:
			IS_SENSOR_SIN_FREQ_LEGAL(para_value);
			
			break;
		case FILTER_DEEP:
			IS_FILTER_DEEP_LEGAL(para_value);
			
			break;
		case SINGL_CHANNEL_SAMPLE_NUM:
			IS_SINGLE_SAMPLE_NUM(para_value);
			break;
		default:
			break;
	}
	if((write_flag == 0)&&(para_filled_flag = ALL_PARA_FILLED ) )
	{
		// TODO: 写入参数
		para_serial[0] = 0;
		write_flash_infm(PARA_FLASH_SAVE_ADDR, para_serial,PARA_NUM);
	}
	else //2填充参数
	{
		para_serial[serial_num] = para_value;
		para_filled_flag |= serial_num<<1; // 对应位置1，当所写参数全部赋值后才能写入

	}
}

 
 #endif
 
