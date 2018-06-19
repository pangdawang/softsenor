/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : init_flash.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/08
 * Description        : 保存在flash中的参数初始化
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/6 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _INIT_FLASH_C_
#define _INIT_FLASH_C_
#include "init_flash.h"


/*************************************************
Function: vlut_para_value
Description: 获取保存在FLASH中的参数，若获取失败，则写入缺省值
Calls: read_flash_infm---读取flash指定地址指定长度信息
	write_flash_infm---向flash指定地址写入信息
	STMFLASH_ReadHalfWord---读取flash指定地址半字信息
Called By: init_para_value---初始化参数函数
Input: 暂无
Output: 暂无
Return:暂无
Others: 
*************************************************/
void vlut_para_value(void)
{
	uint16_t write_flash_success_flg;
	uint16_t para_value[HALF_LENGTH_PARA_FLASH_SAVE]={0};
	
		printf("the code is running \r\nin the %s at the	line %d \r\n",__FILE__ ,__LINE__ );

	//读取指定地址的信息,这个地址保存了是否写入参数的标志	
	if(STMFLASH_ReadHalfWord(PARA_FLASH_SAVE_ADDR) == 0)	//para已写入，填充全局变量
	{
		read_flash_infm(PARA_FLASH_SAVE_ADDR,para_value,HALF_LENGTH_PARA_FLASH_SAVE);
	}
	else//未写入，写入缺省值，再填充全局变量
	{
		para_value[0]=WRITE_FLASH_VALUE;
		para_value[1]=DEFAULT_DA_SIN_FREQ;
		para_value[2]=DEFAULT_SENSOR_SIN_FREQ;
		para_value[3]=DEFAULT_AD_SAMPLE_CYCLE;
		para_value[4]=DEFAULT_FILTER_DEEP;
		para_value[5]=DEFAULT_SINGLE_SAMPLE_NUM;
		
		write_flash_infm(PARA_FLASH_SAVE_ADDR,para_value,HALF_LENGTH_PARA_FLASH_SAVE);
	}
	//填充全局变量结构体
#ifdef OPEN_DEBUG
	saved_in_flash_para.da_sin_freq = para_value[1];
	printf("da_sin_freq is  %d \r\n",saved_in_flash_para.da_sin_freq);
	assert_param(IS_DA_SIN_FREQ_LEGAL(saved_in_flash_para.da_sin_freq));
	saved_in_flash_para.sensor_sin_freq = para_value[2];
	printf("sensor_sin_freq is  %d \r\n",saved_in_flash_para.sensor_sin_freq);
	assert_param(IS_SENSOR_SIN_FREQ_LEGAL(saved_in_flash_para.sensor_sin_freq));
	saved_in_flash_para.ad_sample_cycle = (uint8_t)para_value[3];
	saved_in_flash_para.filter_deep = (uint8_t)para_value[4];
	assert_param(IS_FILTER_DEEP_LEGAL(saved_in_flash_para.ad_sample_cycle));
	saved_in_flash_para.single_channel_sample_num = (uint8_t)para_value[5];
	assert_param(IS_SINGLE_SAMPLE_NUM(saved_in_flash_para.single_channel_sample_num));
#else
	saved_in_flash_para.da_sin_freq = para_value[1];
	if(IS_DA_SIN_FREQ_LEGAL(saved_in_flash_para.da_sin_freq)==0)
	{
		saved_in_flash_para.da_sin_freq =DEFAULT_DA_SIN_FREQ;
		printf("da_sin_freq set err!! \r\nin the %s at the	line %d \r\n",__FILE__ ,__LINE__ );
	}
	saved_in_flash_para.sensor_sin_freq = para_value[2];
	if(IS_SENSOR_SIN_FREQ_LEGAL(saved_in_flash_para.sensor_sin_freq)==0)
	{
		saved_in_flash_para.sensor_sin_freq =DEFAULT_SENSOR_SIN_FREQ;
		printf("sensor_sin_freq set err!! \r\nin the %s at the	line %d \r\n",__FILE__ ,__LINE__ );
	}
	saved_in_flash_para.ad_sample_cycle = (uint8_t)para_value[3];
	saved_in_flash_para.filter_deep = (uint8_t)para_value[4];
	if(IS_FILTER_DEEP_LEGAL(saved_in_flash_para.filter_deep)==0)
	{
		saved_in_flash_para.filter_deep = DEFAULT_FILTER_DEEP;
		printf("filter_deep set err!! \r\nin the %s at the	line %d  \r\n",__FILE__ ,__LINE__ );
	}
	saved_in_flash_para.single_channel_sample_num = (uint8_t)para_value[5];
	if(IS_FILTER_DEEP_LEGAL(saved_in_flash_para.single_channel_sample_num)==0)
	{
		saved_in_flash_para.single_channel_sample_num = DEFAULT_FILTER_DEEP;
		printf("single_channel_sample_num set err!! \r\nin the %s at the	line %d  \r\n",__FILE__ ,__LINE__ );
	}

#endif
	
}
/*************************************************
Function: write_flash_infm
Description: 
Calls: read_flash_infm---读取flash指定地址信息
	
Called By: init_config---初始化配置函数
Input: addr ---写入地址
		pbuf ---待写入的数据指针
		num_to_write---待写入的数据个数(半字)
Output: 暂无
Return:暂无
Others: 
*************************************************/
void write_flash_infm(u32 addr, uint16_t* pbuf, uint16_t num_to_write)
{
	//int16_t init_para[];
	STMFLASH_Write(addr,pbuf,num_to_write);
}

/*************************************************
Function: read_flash_infm
Description: 获取保存在FLASH中的参数，若获取失败，则写入缺省值
Calls: 
Called By: vlut_para_value---初始化配置函数
Input: addr----读取数据的地址
Output: 暂无
Return: 指向flash读出数据的数组指针
Others: 
*************************************************/

static void read_flash_infm(uint32_t addr,uint16_t*init_para, uint16_t read_value_half_num)
{
	//uint16_t init_para[HALF_LENGTH_PARA_FLASH_SAVE];
	
	STMFLASH_Read(addr,init_para,HALF_LENGTH_PARA_FLASH_SAVE);
	//return init_para;
	
}
#endif

