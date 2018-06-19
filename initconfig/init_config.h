/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : init_config.h
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : 上电后参数初始化函数
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/6 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _INIT_CONFIG_H_
#define _INIT_CONFIG_H_

#include "Wifi_atk_esp8266.h"

#ifndef GLOBAL extern 
#define GLOBAL extern
#else
#define GLOBAL
#endif


/*----------结构体-------------*/



typedef enum TIMER_NUM_ENUM
{
	TIM1TRI,
	TIM2TRI,
	TIM3TRI,
	TIM4TRI,
	TIM5TRI,
	TIM6TRI,
	TIM7TRI,
	TIM8TRI,
	TIM9TRI,
	TIM10TRI,
	TIM11TRI,
	TIM12TRI
	
}TIMER_ENUM;

#define IS_TIMER_TRIG(TIMER) (((TIMER)==TIM1TRI)\
							 ||((TIMER)==TIM2TRI)\
							 ||((TIMER)==TIM3TRI)\
							 ||((TIMER)==TIM4TRI)\
							 ||((TIMER)==TIM5TRI)\
							 ||((TIMER)==TIM6TRI)\
							 ||((TIMER)==TIM7TRI)\
							 ||((TIMER)==TIM8TRI)\
							 ||((TIMER)==TIM9TRI)\
							 ||((TIMER)==TIM10TRI)\
							 ||((TIMER)==TIM12TRI)\
							 ||((TIMER)==TIM11TRI))

#define IS_SOFT_SENSOR_TIMER_TRIG(TIMER) (((TIMER)==TIM3TRI)\
							 ||((TIMER)==TIM6TRI))
#undef GLOBAL
//extern Para_Save_Flash saved_in_flash_para;

/*---------debug 开关宏定义------*/
//#define OPEN_USMART_DEBUG_CMD //串口调试命令开关
#define OPEN_WIFI_DEBUG_CMD //串口调试命令开关

#define __DEBUG // 调试信息打印

#ifdef __DEBUG

#define DEBUG(format,...) printf("FILE: "__FILE__", LINE: %d: "format, __LINE__, ##__VA_ARGS__)
#else
 
#define DEBUG(format,...)
#endif
//#debug DEBUG_PRINT printf

#define WIFI_MODE AP_TCP_SERVER

/*-------外设参数宏定义----------*/
#define DEBUG_USART USART3
#define DEBUG_USART_BAUD_RATE 115200 

#define DA_DRIG_TIMER TIM6
#define AD_DRIG_TIMER TIM3
#define USMART_DEV_CLK SystemCoreClock/1000000

/*----------函数声明--------*/
void init_config(WIFI_MODE_ENUM wifi_mode);



#endif

