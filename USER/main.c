#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"	
#include "touch.h"		
#include "usart3.h"
#include "common.h" 
 #include "Wifi_atk_esp8266.h"
#include <stdlib.h>    
#include "init_config.h"
#include "init_flash.h"

/************************************************
 ALIENTEK STM32F103������ ��չʵ��5
 ATK-RM04 WIFIģ�����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#define WIFI_MODE AP_TCP_SERVER
 uint8_t adc_convert_over_flag = 0;//AD����8·ͨ����ϱ�־
  Para_Save_Flash saved_in_flash_para;

int main(void)
 {	 
	u8 key,fontok=0; 	    
	u8*pdata;
	u8 data_length;
	delay_init();	    	 //��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	//usmart_dev.init(72);		//��ʼ��USMART		
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	init_config(WIFI_MODE);	
	//usart3_init(115200);		//��ʼ������3 
	delay_ms(1000); 
	delay_ms(1000); 
	delay_ms(1000); 
	printf("waitting config wifi module\r\n");
	//config_wifi_mode(WIFI_MODE,INIT_IP);
		//pdata=malloc(32);
	
	data_length = strlen("wifi_esp8266����---����serverģʽ\r\n");
	
	//atk_8266_test();		//����ATK_ESP8266����
	while(1)
	{ 
		delay_ms(50); 
		//atk_8266_at_response(1);//���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������
		delay_ms(50);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay_ms(50);
		//sprintf((char*)pdata,"wifi_esp8266����---����͸��ģʽ\r\n");//��������
		pdata=malloc(data_length);
		while(adc_convert_over_flag == 1)//�ɼ���ɺ����ݴ���
		{
			//delay_ms(50);
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
			adc_convert_over_flag = 0;
			get_sensor_change_value(WIFI_MODE);
			//sprintf(pdata,"wifi_esp8266����---����serverģʽ\r\n");
			//wifi_printf(WIFI_MODE,pdata,data_length);

			TIM_Cmd(AD_DRIG_TIMER, ENABLE);
			TIM_Cmd(DA_DRIG_TIMER, ENABLE);
	
		}

		/*
		if(IS_WIFI_TCP_CLIENT_MODE(WIFI_MODE))
		{
			DEBUG("the client test!\r\n ");
			sprintf((char*)pdata,"wifi_esp8266����---����͸��ģʽ\r\n");//��������
			//sprintf(pdata,"AT+CIPSEND=25");
			u3_printf("%s",pdata);

		}
		else if(IS_WIFI_TCP_SERVER_MODE(WIFI_MODE))
		{
			DEBUG("the server test!\r\n ");
			#if 0
			sprintf(pdata,"AT+CIPSEND=0,%d",data_length);
			if(atk_8266_send_cmd(pdata, ">", 100)==0)
			{
				sprintf(pdata,"wifi_esp8266����---����serverģʽ\r\n");
				atk_8266_send_data(pdata, "SEND OK", 100);

			}
			#else
			sprintf(pdata,"wifi_esp8266����---����serverģʽ\r\n");
			wifi_printf(WIFI_MODE,pdata,data_length);
			
			#endif
		}
		else if(IS_WIFI_TCP_UDP_MODE(WIFI_MODE))
		{
			DEBUG("the udp test!\r\n ");
			sprintf(pdata,"AT+CIPSEND=%d",data_length);//��������
			if(atk_8266_send_cmd(pdata, ">", 100)==0)
			{
				sprintf(pdata,"wifi_esp8266����---����UDPģʽ\r\n");
				atk_8266_send_data(pdata, "SEND OK", 100);
				DEBUG("the udp test!\r\n ");

			}
		}
		//delay_ms(100);
		//printf("waitting config wifi module\r\n");
		*/
		GPIO_ResetBits(GPIOD, GPIO_Pin_15);
		free(pdata);
	}

}


















