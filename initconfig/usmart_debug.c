/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : usmart_debug.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/08
 * Description        : ���ڵ��������ļ�
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
Description: �޸ĵ����Ҳ�Ƶ��:
Calls: TIM_SetAutoreload-��ʱ���Զ���װ�غ���(�⺯��)
Called By: stmflash.c---���ڵ����ļ�
Input: TIMx-----��Ӧ�Ķ�ʱ��
sin_freq----���Ҳ�����Ƶ��ֵ
Output: ����
Return:����
Others: �ú�������������������Ŀ
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
Description: ������д��flash
Calls: TIM_SetAutoreload-��ʱ���Զ���װ�غ���(�⺯��)
Called By: stmflash.c---���ڵ����ļ�
Input: para_serial-----������Ӧ���к�
para_value----����ֵ
write_flag----д���־λ:0--��д��1--д��
Output: ����
Return:����
Others: �ú�������������������Ŀ
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
				write_flag = 1;//���
			}
			else
			{
				write_flag = 0;//������д��
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
		// TODO: д�����
		para_serial[0] = 0;
		write_flash_infm(PARA_FLASH_SAVE_ADDR, para_serial,PARA_NUM);
	}
	else //2������
	{
		para_serial[serial_num] = para_value;
		para_filled_flag |= serial_num<<1; // ��Ӧλ��1������д����ȫ����ֵ�����д��

	}
}

 
 #endif
 
