/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : init_config.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : �ϵ�������ʼ������
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
 * 2018/04/6 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _INIT_CONFIG_C_
#define _INIT_CONFIG_C_

#include "stm32f10x.h"
#include "init_config.h"
#include "ADC.h"
#include "DAC.h"
#include "usmart.h"
#include "init_flash.h"
#include "usart3.h"	  
/*-------------��̬�������� -----------*/
static void init_para_value(void);
static void timer_config(TIMER_ENUM timer, uint16_t period);
static void config_peripheral(void);

/*************************************************
Function: init_config
Description: ��ȡ������FLASH�еĲ���������ȡʧ�ܣ���д��ȱʡֵ
Calls: vlut_para_value---��ȡ������FLASH�еĲ���
	config_peripheral---�õ��������ʼ����
Called By: main.c
Input: ����
Output: ����
Return:����
Others: ����������������Ŀ
*************************************************/

void init_config(WIFI_MODE_ENUM wifi_mode)
{
	assert_param(IS_WIFI_MODE(wifi_mode));
/*-----------��ȫ�ֱ�����ֵ-------*/
	uart_init(DEBUG_USART_BAUD_RATE);	
	#ifdef OPEN_USMART_DEBUG_CMD  //ʹ�ô���3���е��� �ر���WIFIģ�鹦��
		DEBUG("the soft version is usart testcmd!\r\n");
	#else if OPEN_WIFI_DEBUG_CMD
		usart3_init(DEBUG_USART_BAUD_RATE);
		delay_ms(1000); 
		delay_ms(1000); 
		delay_ms(1000); 
		config_wifi_mode(wifi_mode,INIT_IP);
		// TODO: ����WIFI
	#endif
	
	init_para_value();

/*----------��������-------------*/
	config_peripheral();


}

/*************************************************
Function: init_para_value
Description: ȫ�ֱ�������ֵ
Calls: vlut_para_value----
Called By: init_config---��ʼ�����ú���
Input: ����
Output: ����
Return:����
Others: ����������������Ŀ
*************************************************/
static void init_para_value(void)
{
	vlut_para_value();
	
}
/*************************************************
Function: config_peripheral
Description: �õ��������ʼ������������+���蹦������
Calls: Dac1_Init----DAconfig
		TIM_Cmd ----��ʱ��config
		Adc_Init----ADconfig
		uart_init ----����config
		usmart_dev.init----���ڵ�������config

Called By: init_config--�����ʼ��
Input: ��
Output: ����
Return: ��
Others: ����������������Ŀ
*************************************************/
static void config_peripheral(void)
{
	//DA+timer+dma
	//���Ҳ�����
	timer_config(TIM6TRI,saved_in_flash_para.da_sin_freq);
	Dac1_Init();
	
	TIM_Cmd(DA_DRIG_TIMER, ENABLE);
	//AD+timer+dma
	timer_config(TIM3TRI,saved_in_flash_para.sensor_sin_freq);
	Adc_Init(saved_in_flash_para.ad_sample_cycle);		  
	TIM_Cmd(AD_DRIG_TIMER, ENABLE);

	//usart
#ifdef OPEN_USMART_TEST_CMD
	usmart_dev.init(USMART_DEV_CLK);	//��ʼ��USMART	

#endif

}

/*************************************************
Function: timer_config
Description: ��ʱ�������������
Calls: �⺯��
Called By: config_peripheral--�����ʼ��
Input: ��
Output: ����
Return: ��
Others: ����������������Ŀ
*************************************************/
static void timer_config(TIMER_ENUM timer, uint16_t period)
{
	uint16_t arr;
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

	//assert_param(IS_SOFT_SENSOR_TIMER_TRIG(TIMER_ENUM));

  /* TIM3 Periph clock enable */
  if(timer == TIM3TRI)
  {
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

 	 arr = (uint16_t)(72000000/(period*ADC_SIGNLE_SAMPLE_NUM)-1);
  }
  else if(timer == TIM6TRI)
  {
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	arr = (uint16_t)((72000000/(period*SIN_PERIOD_NUM))-1);
  }
   else if(timer == TIM7TRI)
  {
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	arr = (uint16_t)((72000000/(period*100000))-1);
  }
 
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = arr;          //���嶨ʱ�����Ƶ��Ϊ30000/8
  TIM_TimeBaseStructure.TIM_Prescaler = 0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  if(timer == TIM3TRI)
  {
	  TIM_TimeBaseInit(AD_DRIG_TIMER, &TIM_TimeBaseStructure);

	  /* TIM3 TRGO selection */
	  TIM_SelectOutputTrigger(AD_DRIG_TIMER, TIM_TRGOSource_Update);
  }
  else if(timer == TIM6TRI)
  {
	  TIM_TimeBaseInit(DA_DRIG_TIMER, &TIM_TimeBaseStructure);

	  /* TIM6 TRGO selection */
	  TIM_SelectOutputTrigger(DA_DRIG_TIMER, TIM_TRGOSource_Update);
  }
   else if(timer == TIM7TRI)
  {
	  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
  }

}

#endif

 

