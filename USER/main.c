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
 ALIENTEK STM32F103开发板 扩展实验5
 ATK-RM04 WIFI模块测试实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

#define WIFI_MODE AP_TCP_SERVER
 uint8_t adc_convert_over_flag = 0;//AD采样8路通道完毕标志
  Para_Save_Flash saved_in_flash_para;

int main(void)
 {	 
	u8 key,fontok=0; 	    
	u8*pdata;
	u8 data_length;
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	//uart_init(115200);	 	//串口初始化为115200
 	//usmart_dev.init(72);		//初始化USMART		
 	LED_Init();		  			//初始化与LED连接的硬件接口
	init_config(WIFI_MODE);	
	//usart3_init(115200);		//初始化串口3 
	delay_ms(1000); 
	delay_ms(1000); 
	delay_ms(1000); 
	printf("waitting config wifi module\r\n");
	//config_wifi_mode(WIFI_MODE,INIT_IP);
		//pdata=malloc(32);
	
	data_length = strlen("wifi_esp8266测试---进入server模式\r\n");
	
	//atk_8266_test();		//进入ATK_ESP8266测试
	while(1)
	{ 
		delay_ms(50); 
		//atk_8266_at_response(1);//检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
		delay_ms(50);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		delay_ms(50);
		//sprintf((char*)pdata,"wifi_esp8266测试---进入透传模式\r\n");//测试数据
		pdata=malloc(data_length);
		while(adc_convert_over_flag == 1)//采集完成后数据处理
		{
			//delay_ms(50);
			GPIO_SetBits(GPIOD, GPIO_Pin_15);
			adc_convert_over_flag = 0;
			get_sensor_change_value(WIFI_MODE);
			//sprintf(pdata,"wifi_esp8266测试---进入server模式\r\n");
			//wifi_printf(WIFI_MODE,pdata,data_length);

			TIM_Cmd(AD_DRIG_TIMER, ENABLE);
			TIM_Cmd(DA_DRIG_TIMER, ENABLE);
	
		}

		/*
		if(IS_WIFI_TCP_CLIENT_MODE(WIFI_MODE))
		{
			DEBUG("the client test!\r\n ");
			sprintf((char*)pdata,"wifi_esp8266测试---进入透传模式\r\n");//测试数据
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
				sprintf(pdata,"wifi_esp8266测试---进入server模式\r\n");
				atk_8266_send_data(pdata, "SEND OK", 100);

			}
			#else
			sprintf(pdata,"wifi_esp8266测试---进入server模式\r\n");
			wifi_printf(WIFI_MODE,pdata,data_length);
			
			#endif
		}
		else if(IS_WIFI_TCP_UDP_MODE(WIFI_MODE))
		{
			DEBUG("the udp test!\r\n ");
			sprintf(pdata,"AT+CIPSEND=%d",data_length);//测试数据
			if(atk_8266_send_cmd(pdata, ">", 100)==0)
			{
				sprintf(pdata,"wifi_esp8266测试---进入UDP模式\r\n");
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


















