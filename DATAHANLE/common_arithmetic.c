/******************** (C) COPYRIGHT 2018 SOFT_SENSE PROJECT **********************************
 * File Name          : common_arithmetic.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/03/28
 * Description        : This file is sample data handle
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
  * 2018/03/28 | v1.0  | panda_wang     | 对二维数组[m][n]以列为单位求取峰峰值
  *									，并对峰峰值取均值
 *******************************************************************************/
 #ifndef _COMMON_ARITHMETIC_C_
#define _COMMON_ARITHMETIC_C_

/*---------------include------------*/
#include "common_arithmetic.h"


/*************************************************
Function: get_peak_peak
Description: 求二维数组中每列的峰峰值求间隔column_num个数之间的峰峰值，
*即对二维数组的每列求峰峰值
Calls:
Called By: 通用函数
Input: data_ptr----待处理数组的指针
*				row_num---待处理数组的总行数
*				column_num---待处理数组的总列数
*				ppeak_value---保存峰峰值的指针
*				subcript---保存最值下标的指针
Output: 计算结果填入ppeak_value_ptr指向的地址
Return:暂无
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
Description: 求平均值
Calls:
Called By: 通用函数
Input:data_ptr----待处理数组的指针
*				data_num---数组元素数
*				aver_value---保存均值的指针
Output: 计算结果填入ppeak_value_ptr指向的地址
Return:计算得到的平均值
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
Description: 求递推均值
Calls:
Called By: 通用函数
Input:data_ptr----待处理数组的指针
*				data_num---数组元素数
Output: 计算结果填入ppeak_value_ptr指向的地址
Return:计算得到的平均值
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
	// 先进行冒泡排序，再去掉最大值和最小值，剩下的求取平均值
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

