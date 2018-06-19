#ifndef __DAC_H
#define __DAC_H	 
#include "sys.h"	    
								    
 //////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DAC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/8
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
								    
#define open_dac_sin_out //输出正弦波
#define SIN_PERIOD_NUM   256

#ifdef open_dac_sin_out
#define DAC_DHR12R1_ADDRESS    0x40007408
#endif
//extern Para_Save_Flash saved_in_flash_para;

void Dac1_Init(void);//DA1初始化		 	 
void Dac1_Set_Vol(u16 vol);
#ifdef open_dac_sin_out
void tim6_config(uint16_t period);
void SIN_CUR_Creat(uint16_t cur_num, uint16_t * cur_ptr);
//void set_sin_freq(u16 freq);
#endif

#endif

















