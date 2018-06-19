/******************** (C) COPYRIGHT 2018 SOFT_SENSE PROJECT **********************************
 * File Name          : common_arithmetic.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/03/28
 * Description        : This file is sample data handle
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
  * 2018/03/28 | v1.0  | panda_wang     | �Զ�ά����[m][n]����Ϊ��λ��ȡ���ֵ
  *									�����Է��ֵȡ��ֵ
 *******************************************************************************/
 #ifndef _COMMON_ARITHMETIC_C_
#define _COMMON_ARITHMETIC_C_

/*---------------include------------*/
#include "common_arithmetic.h"


/*************************************************
Function: get_peak_peak
Description: ���ά������ÿ�еķ��ֵ����column_num����֮��ķ��ֵ��
*���Զ�ά�����ÿ������ֵ
Calls:
Called By: ͨ�ú���
Input: data_ptr----�����������ָ��
*				row_num---�����������������
*				column_num---�����������������
*				ppeak_value---������ֵ��ָ��
*				subcript---������ֵ�±��ָ��
Output: ����������ppeak_value_ptrָ��ĵ�ַ
Return:����
Others: 
*************************************************/
void  get_peak_peak(uint16_t (*data_ptr)[8],uint8_t row_num,uint8_t column_num,uint16_t * ppeak_value_ptr,uint8_t (*subcript)[2])
{
	uint8_t i,j;
	uint8_t (*max_min_subscript)[2];
	uint16_t (*data)[8];
	uint16_t max=0;
	uint16_t min=0;
	uint16_t* ppeak_value;

	max_min_subscript=subcript;
	data = data_ptr;
	ppeak_value =ppeak_value_ptr;
	
	for(j =0;j<column_num;j++)
	{

		max_min_subscript[j][0]=0;
		max_min_subscript[j][1] =0;
		max = data[0][j];
		min= data[0][j];
		
		for(i =1;i<row_num;i ++)
		{
			if(data[i][j]>max)
			{
				max = data[i][j];
				max_min_subscript[j][0]=i;
			}
			if(data[i][j]<min)
			{
				min = data[i][j];
				max_min_subscript[j][1] =i;
			}
		}
		ppeak_value[j] = max-min;

	}
	
}

/*************************************************
Function: get_average
Description: ��ƽ��ֵ
Calls:
Called By: ͨ�ú���
Input:data_ptr----�����������ָ��
*				data_num---����Ԫ����
*				aver_value---�����ֵ��ָ��
Output: ����������ppeak_value_ptrָ��ĵ�ַ
Return:����õ���ƽ��ֵ
Others: 
*************************************************/
uint16_t get_average(uint16_t * data_ptr,uint8_t data_num)
{
	uint16_t * data;
	//uint16_t * average;
	uint8_t i;
	uint16_t res=0;

	data =data_ptr;
	for(i =0;i<data_num;i++)
	{
		res += data[i];
	}
	res /=data_num;
	return res;
}

/*************************************************
Function: get_recur_average
Description: ����ƾ�ֵ
Calls:
Called By: ͨ�ú���
Input:data_ptr----�����������ָ��
*				data_num---����Ԫ����
Output: ����������ppeak_value_ptrָ��ĵ�ַ
Return:����õ���ƽ��ֵ
Others: 
*************************************************/
uint16_t get_recur_average(uint16_t * data_ptr,uint8_t data_num)
{
	uint16_t* data;
	//uint16_t * average;
	uint16_t bubble;
	uint8_t i,j;
	uint16_t res=0;
	
	if(data_num<3)
	{
		res =0;
		return res;
	}
	//average = aver_value;
	data = data_ptr;
	// �Ƚ���ð��������ȥ�����ֵ����Сֵ��ʣ�µ���ȡƽ��ֵ
	for(i =0;i<data_num;i++)
	{
		for(j=0;j<data_num-i;j++)
		{
			if(data[j]>data[j+1])
			{
				data[j] = data[j]^data[j+1];
				data[j+1]=data[j]^data[j+1];
				data[j]=data[j]^data[j+1];
			}
		}
	}
	for(i=1;i<data_num-1;i++)
	{
		res += data[i];
	}
	/*
	if(*average ==0)
	{
		*average /=data_num-2;

	}
	else
	*/
	res /=data_num-2;
	return res;
}

#endif

