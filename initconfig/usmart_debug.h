/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : usmart_debug.h
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/08
 * Description        : 串口调试命令头文件
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/08 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
 #ifndef _USMART_DEBUG_H_
 #define _USMART_DEBUG_H_

#include "dac.h"
#include "stm32f10x.h"
#include "init_flash.h"

typedef enum PARA_SERIAL_NUM
{
	WRITE_FILL,
	DA_SIN_FREQ,
	SENSOR_SIN_FREQ,
	AD_SAM_CYCLE,
	FILTER_DEEP,
	SINGL_CHANNEL_SAMPLE_NUM

} PARA_SERIAL_ENUM;

#define IS_THE_PARA_SERIAL(NUM) (((NUM)==WRITE_FLAG)\
								||((NUM)==DA_SIN_FREQ)\
								||((NUM)==SENSOR_SIN_FREQ)\
								||((NUM)==AD_SAM_CYCLE)\
								||((NUM)==FILTER_DEEP)\
								||((NUM)==SINGL_CHANNEL_SAMPLE_NUM))

typedef enum WRITE_FLAG
{
	WRITE,//写入到flash
	FILL  //填充数据数组

}WRITE_FLAG_ENUM;

#define IS_THE_WRITE_FLAG(FLAG) (((FLAG)==WRITE)||((FLAG)==FILL))

#define PARA_NUM 6
#define ALL_PARA_FILLED 0x3F
/*--------私人变量-------*/
static uint16_t para_serial[PARA_NUM];
static uint16_t para_filled_flag=0;

void set_sin_freq(uint8_t TIMER_NUM,uint16_t sin_freq);
void write_para_to_flash_debug( PARA_SERIAL_ENUM serial_num, uint16_t para_value);

 #endif
 
