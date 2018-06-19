/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : init_flash.h
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/08
 * Description        : 保存在flash中的参数初始化
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/6 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _INIT_FLASH_H_
#define _INIT_FLASH_H_

#include "stm32f10x.h"
#include "stmflash.h"

/*-----结构体---------*/
typedef struct para_save_flash_str
{
	u16 para_writed_flag;
	u16 da_sin_freq;
	u16 sensor_sin_freq;
	u8  ad_sample_cycle;
	u8  filter_deep;
	u8  single_channel_sample_num;
}Para_Save_Flash;
extern Para_Save_Flash saved_in_flash_para;

/*---------flashd读写宏定义-------------*/
#define PARA_FLASH_SAVE_ADDR 0x08003000  //1flash读存开始地址
#define WRITE_FLASH_VALUE 0;//在向flash中写入参数时，第一个半字写0000
//#define HALF_LENGTH_PARA_FLASH_SAVE (sizeof(saved_in_flash_para)/2)//
#define HALF_LENGTH_PARA_FLASH_SAVE 6//写入的半字总长度
#define OPEN_DEBUG 

/*-------全局参数缺省值----------*/
#define DEFAULT_DA_SIN_FREQ (600)//da正弦波周期
#define DEFAULT_SENSOR_SIN_FREQ (540)//传感器正弦波周期
#define DEFAULT_AD_SAMPLE_CYCLE (ADC_SampleTime_1Cycles5)
#define DEFAULT_FILTER_DEEP   (5)//滤波深度
#define DEFAULT_SINGLE_SAMPLE_NUM (90)//单通道采样数

/*--------全局参数最值-------*/
#define MAX_DA_SIN_FREQ (1000)//da正弦波周期最大值
#define MIN_DA_SIN_FREQ (500)//da正弦波周期最小值

#define IS_DA_SIN_FREQ_LEGAL(FREQ) (((FREQ < MAX_DA_SIN_FREQ)\
									||(FREQ == MAX_DA_SIN_FREQ))\
									 &&((FREQ > MIN_DA_SIN_FREQ)\
									 ||(FREQ == MIN_DA_SIN_FREQ)))

#define MAX_SENSOR_SIN_FREQ (980)//传感器正弦波周期最大值
#define MIN_SENSOR_SIN_FREQ (480)//传感器正弦波周期最小值

#define IS_SENSOR_SIN_FREQ_LEGAL(FREQ) (((FREQ < MAX_SENSOR_SIN_FREQ)\
									||(FREQ == MAX_SENSOR_SIN_FREQ))\
									 &&((FREQ > MIN_SENSOR_SIN_FREQ)\
									 ||(FREQ == MIN_SENSOR_SIN_FREQ)))
									 

#define MAX_FILTER_DEEP   (15)//滤波深度最大值
#define MIN_FILTER_DEEP   (5)//滤波深度最小值
#define IS_FILTER_DEEP_LEGAL(DEEP) (((DEEP < MAX_FILTER_DEEP)\
									||(DEEP == MAX_FILTER_DEEP))\
									 &&((DEEP > MIN_FILTER_DEEP)\
									 ||(DEEP == MIN_FILTER_DEEP)))
									 
#define MAX_SINGLE_SAMPLE_NUM  (90)//单通道采样数最大值
#define MIN_SINGLE_SAMPLE_NUM   (60)//单通道采样数最小值
#define IS_SINGLE_SAMPLE_NUM(NUM) (((NUM < MAX_SINGLE_SAMPLE_NUM)\
										||(NUM == MAX_SINGLE_SAMPLE_NUM))\
										 &&((NUM > MIN_SINGLE_SAMPLE_NUM)\
										 ||(NUM == MIN_SINGLE_SAMPLE_NUM)))

/*----------函数声明--------*/
void vlut_para_value(void);
void write_flash_infm(u32 addr, uint16_t* pbuf, uint16_t num_to_write);

static void read_flash_infm(uint32_t addr,uint16_t*init_para, uint16_t read_value_half_num);

#endif
