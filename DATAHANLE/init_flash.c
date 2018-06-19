/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : init_flash.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/08
 * Description        : ������flash�еĲ�����ʼ��
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
Description: ��ȡ������FLASH�еĲ���������ȡʧ�ܣ���д��ȱʡֵ
Calls: read_flash_infm---��ȡflashָ����ַָ��������Ϣ
	write_flash_infm---��flashָ����ַд����Ϣ
	STMFLASH_ReadHalfWord---��ȡflashָ����ַ������Ϣ
Called By: init_para_value---��ʼ����������
Input: ����
Output: ����
Return:����
Others: 
*************************************************/
void vlut_para_value(void)
{
	uint16_t write_flash_success_flg;
	uint16_t para_value[HALF_LENGTH_PARA_FLASH_SAVE]={0};
	
		printf("the code is running \r\nin the %s at the	line %d \r\n",__FILE__ ,__LINE__ );

	//��ȡָ����ַ����Ϣ,�����ַ�������Ƿ�д������ı�־	
	if(STMFLASH_ReadHalfWord(PARA_FLASH_SAVE_ADDR) == 0)	//para��д�룬���ȫ�ֱ���
	{
		read_flash_infm(PARA_FLASH_SAVE_ADDR,para_value,HALF_LENGTH_PARA_FLASH_SAVE);
	}
	else//δд�룬д��ȱʡֵ�������ȫ�ֱ���
	{
		para_value[0]=WRITE_FLASH_VALUE;
		para_value[1]=DEFAULT_DA_SIN_FREQ;
		para_value[2]=DEFAULT_SENSOR_SIN_FREQ;
		para_value[3]=DEFAULT_AD_SAMPLE_CYCLE;
		para_value[4]=DEFAULT_FILTER_DEEP;
		para_value[5]=DEFAULT_SINGLE_SAMPLE_NUM;
		
		write_flash_infm(PARA_FLASH_SAVE_ADDR,para_value,HALF_LENGTH_PARA_FLASH_SAVE);
	}
	//���ȫ�ֱ����ṹ��
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
Calls: read_flash_infm---��ȡflashָ����ַ��Ϣ
	
Called By: init_config---��ʼ�����ú���
Input: addr ---д���ַ
		pbuf ---��д�������ָ��
		num_to_write---��д������ݸ���(����)
Output: ����
Return:����
Others: 
*************************************************/
void write_flash_infm(u32 addr, uint16_t* pbuf, uint16_t num_to_write)
{
	//int16_t init_para[];
	STMFLASH_Write(addr,pbuf,num_to_write);
}

/*************************************************
Function: read_flash_infm
Description: ��ȡ������FLASH�еĲ���������ȡʧ�ܣ���д��ȱʡֵ
Calls: 
Called By: vlut_para_value---��ʼ�����ú���
Input: addr----��ȡ���ݵĵ�ַ
Output: ����
Return: ָ��flash�������ݵ�����ָ��
Others: 
*************************************************/

static void read_flash_infm(uint32_t addr,uint16_t*init_para, uint16_t read_value_half_num)
{
	//uint16_t init_para[HALF_LENGTH_PARA_FLASH_SAVE];
	
	STMFLASH_Read(addr,init_para,HALF_LENGTH_PARA_FLASH_SAVE);
	//return init_para;
	
}
#endif

