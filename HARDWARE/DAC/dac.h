#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    
								    
 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DAC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
								    
#define open_dac_sin_out //������Ҳ�
#define SIN_PERIOD_NUM   256

#ifdef open_dac_sin_out
#define DAC_DHR12R1_ADDRESS    0x40007408
#endif
//extern Para_Save_Flash saved_in_flash_para;

void Dac1_Init(void);//DA1��ʼ��		 	 
void Dac1_Set_Vol(u16 vol);
#ifdef open_dac_sin_out
void tim6_config(uint16_t period);
void SIN_CUR_Creat(uint16_t cur_num, uint16_t * cur_ptr);
//void set_sin_freq(u16 freq);
#endif

#endif

















