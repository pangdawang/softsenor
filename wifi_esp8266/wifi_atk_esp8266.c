/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : Wifi_atk_esp8266.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : Wifi_atk_esp8266 ģʽ����
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
*2018.04.25 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _WIFI_ATK_ESP8266_C_
#define _WIFI_ATK_ESP8266_C_

#include <stdlib.h>    
#include "stdio.h"    
#include "sys.h" 
#include "Wifi_atk_esp8266.h"
#include "usart3.h"
#include "delay.h"
#include "string.h"    
#include "common.h"

//#include "math.h"
/*---------static function-------*/
 static void set_wifi_ap_mode(WIFI_MODE_ENUM wifi_mode);
static void set_wifi_sta_mode(WIFI_MODE_ENUM wifi_mode);
static void set_wifi_ap_sta_mode(WIFI_MODE_ENUM wifi_mode);
static void set_tcp_server_mode(WIFI_MODE_ENUM wifi_mode);
static void set_tcp_client_mode(WIFI_MODE_ENUM wifi_mode);
static void set_tcp_udp_mode(WIFI_MODE_ENUM wifi_mode);
//static u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
//static u8 atk_8266_quit_trans(void);
//static u8 atk_8266_ip_set(u8* title,u8* mode,u8* port,u8* ip); 

static void set_far_end_ip(const char * ip,WIFI_MODE_ENUM wifi_mode, SET_IP_TYPE_ENUM ip_type); 
static uint8_t * get_far_end_ip(void) ;
//static u8* atk_8266_check_cmd(char *str);
//static void atk_8266_get_wanip(u8* ipbuf);
//static u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime);

/*--------------WIFIģʽ�����ַ���----------------*/
#if 0
//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8086";		 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="ALIENTEK";			//·����SSID��
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifista_password="15902020353"; 	//��������

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* wifiap_ssid="ATK-ESP8266";			//����SSID��
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifiap_password="12345678"; 		//�������� 

/*----------3������ģʽ----------------*/
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STAģʽ ","APģʽ ","AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ 
/*-------3�ֹ���ģʽ----------*/
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDP ģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
/*---------5�ּ��ܷ�ʽ------------*/
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
#endif
//1 wifiԶ��IP��ַ���ݾ������ӷ�������IP��ַ�����޸�----20180426

u8 wifi_far_end_ip[16]={0};
const u8* default_far_end_ip="192.168.4.2";

/*************************************************
Function: wifi_printf
Description: wifi���ݴ�ӡ������ʵ����printf�Ĺ���
Calls:ͨ�ú���
Called By:  atk_8266_send_cmd---wifi����ͺ���
		atk_8266_send_data----wifi�������ݺ���
Input: 	wifi_mode---wifi_mode
		print_data---����ӡ���ַ���
		data_length---�ַ�������
Output: ����
Return:����
Others:
*************************************************/
void wifi_printf(WIFI_MODE_ENUM  wifi_mode,char * print_data,const uint8_t data_length)
{
	char pdata[14]={0};
	char* sendout_data;

	assert_param(IS_WIFI_MODE(wifi_mode));
	assert_param(print_data!=NULL);

	if(IS_WIFI_TCP_CLIENT_MODE(wifi_mode) )//͸��ģʽ
	{
		sendout_data=malloc(data_length);
		sprintf((char*)sendout_data,print_data);//��������
		u3_printf("%s",sendout_data);
		free(sendout_data);

	}
	else
	{
		if(IS_WIFI_TCP_SERVER_MODE(wifi_mode) )
		{
			sprintf(pdata,"AT+CIPSEND=0,%d",data_length);

		}
		else
		{
			sprintf(pdata,"AT+CIPSEND=%d",data_length);

		}
		if(atk_8266_send_cmd(pdata, ">", 100)==0)
		{
			sendout_data=malloc(data_length);
			sprintf(sendout_data,print_data);
			atk_8266_send_data(sendout_data, "SEND OK", 100);
			free(sendout_data);
		}

	}

}

/*************************************************
Function: config_wifi_mode
Description: ��������ģ��Ĺ���ģʽ
Calls:set_wifi_ap_mode---APMODE
	set_wifi_sta_mode----STAMODE
	set_wifi_ap_sta_mode----AP+STAMODE
Called By:  init_config---ϵͳ��ʼ������
		ip_type----0����ʼ��IP��1��������ֵIP
Input: wifi_mode---����ģ��ģʽ
		ip_type---Զ��IP ģʽ---��ʼ��IP����������IP
Output: ����
Return:����
Others:
*************************************************/
void config_wifi_mode(WIFI_MODE_ENUM wifi_mode,SET_IP_TYPE_ENUM ip_type)
{
	u8 *p;
	char *ip;
	u8 i;
	assert_param(IS_WIFI_MODE(wifi_mode));
	assert_param(IS_SET_IP_TYPE_FLAG(ip_type));

	//���ģ���Ƿ�����
	while(atk_8266_send_cmd("AT","OK",20))//��ѯ���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		DEBUG("wifi ģ��������ѯ������\r\n");
		delay_ms(100);
	}

	DEBUG("wifi_esp8266 ģ��----����ͨ������\r\n");
	// TODO:��Զ��IP��ַ����ֵ
	set_far_end_ip(default_far_end_ip,wifi_mode,INIT_IP);
	//wifi_far_end_ip = ip;
	DEBUG("Զ��IP��ַ:%s\r\n",wifi_far_end_ip);
	/*
	//д��Ĭ�ϵ�IP��ַ���˴������Ǳ����ڵ���IP��ַʱ����ַ�����ڵ����
	i = strlen(default_far_end_ip);
	ip=malloc(i);							//�����ڴ�
	sprintf(ip,default_far_end_ip);
	set_far_end_ip(ip,INIT_IP,wifi_mode,ip_type);
	*/
	switch(wifi_mode)
	{
		case AP_TCP_SERVER:  //1��ģʽ���Գɹ�
		case AP_TCP_CLIENT: //1��ģʽ���Գɹ�
		case AP_UDP:        //1��ģʽ���Գɹ�
			//����APģʽ����
			set_wifi_ap_mode(wifi_mode);
			break;
		case STA_TCP_SERVER:
		case STA_TCP_CLIENT:
		case STA_UDP:
	//����STAģʽ����
			set_wifi_sta_mode(wifi_mode);
			break;
		default:
			//����AP+STAģʽ����
			set_wifi_ap_sta_mode(wifi_mode);//ap+sta
			break;
			
	}
	
	DEBUG("wifi_esp8266 ģ��----���óɹ����ɽ������ݷ���\r\n");
	//free((void*)wifi_far_end_ip);
}

/*************************************************
Function: set_wifi_ap_mode
Description: ��������ģ��Ĺ���ģʽΪAPģʽ
Calls:
Called By:  config_wifi_mode---ϵͳ��ʼ������
Input: WIFI_MODE_ENUM---����ģ��ģʽ
Output: ����
Return:����
Others:
*************************************************/
 static void set_wifi_ap_mode(WIFI_MODE_ENUM wifi_mode)
{
	u8 *p ,*ipbuf;

	assert_param(IS_WIFI_AP_MODE(wifi_mode));

	//2 APģʽ����---��һ������ģʽ������ģ��
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);//ap
	atk_8266_send_cmd("AT+RST","OK",20);		//����ģ�� 
	delay_ms(1000);         //��ʱ2S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	
	p=malloc(32);							//����32�ֽ��ڴ�
	DEBUG("����wifi ģ��APģʽ���ú���\r\n");
	
	//2 APģʽ����---�ڶ�������APģʽ�Ĳ���:ssid��password

	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���

	DEBUG("wifi ģ��apģʽ����:\r\n");
	atk_8266_send_cmd(p,"OK",1000);
	free(p);		//�ͷ��ڴ� 

	//2����ģʽ����---������������TCP/UDPģʽ
	switch(wifi_mode)
	{
		case AP_TCP_SERVER:
			//��������ģʽΪSERVERģʽ
			set_tcp_server_mode(wifi_mode);			
			break;
		case AP_TCP_CLIENT:
			//��������ģʽΪCLIENTģʽ
			set_tcp_client_mode(wifi_mode);			
			break;
		case AP_UDP:
			//��������ģʽΪUDPģʽ
			set_tcp_udp_mode(wifi_mode);
			break;
		default:
			break;
	}

}

 /*************************************************
Function: set_wifi_sta_mode
Description: ��������ģ��Ĺ���ģʽΪstaģʽ
Calls:
Called By:  config_wifi_mode---ϵͳ��ʼ������
Input: WIFI_MODE_ENUM---����ģ��ģʽ
Output: ����
Return:����
Others:
*************************************************/
 static void set_wifi_sta_mode(WIFI_MODE_ENUM wifi_mode)
{
	u8 *ipbuf;
	u8 p[32]={0};
	assert_param(IS_WIFI_STA_MODE(wifi_mode));

	atk_8266_send_cmd("AT+CWMODE=1","OK",20);//sta
	atk_8266_send_cmd("AT+RST","OK",20);		//����ģ�� 
	delay_ms(1000);         //��ʱ2S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	
	DEBUG("wifi ģ��STAģʽ���������ɹ�\r\n");
/*
	p=malloc(32);							//����32�ֽ��ڴ�
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	if(p==NULL)
	{
		DEBUG("�ڴ����ʧ��\r\n");
	}
*/
	//sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	//while(atk_8266_send_cmd(p,"WIFI GOT IP",300));
	//DEBUG("wifi ģ��STAģʽGOTIP��ѯ��\r\n");//����Ŀ��·����,���һ��IP

	DEBUG("wifi ģ��STAģʽ���߲������óɹ�\r\n");

	switch(wifi_mode)
	{
		case AP_TCP_SERVER:
			//��������ģʽΪSERVERģʽ
			set_tcp_server_mode(wifi_mode);			
			break;
		case AP_TCP_CLIENT:
			//��������ģʽΪCLIENTģʽ
			set_tcp_client_mode(wifi_mode);			
			break;
		case AP_UDP:
			//��������ģʽΪUDPģʽ
			set_tcp_udp_mode(wifi_mode);
			break;
		default:
			break;
	}
	// TODO: ��ȡWANIP
	atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
	sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);

	//free(p);		//�ͷ��ڴ� 

}
  /*************************************************
Function: set_wifi_AP_sta_mode
Description: ��������ģ��Ĺ���ģʽΪstaģʽ
Calls:
Called By:  config_wifi_mode---ϵͳ��ʼ������
Input: WIFI_MODE_ENUM---����ģ��ģʽ
Output: ����
Return:����
Others:
*************************************************/
 static void set_wifi_ap_sta_mode(WIFI_MODE_ENUM wifi_mode)
{
	u8 *p;

	assert_param(IS_WIFI_AP_STA_MODE(wifi_mode));
	// TODO:9��ģʽ�����
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);

	switch(wifi_mode)
	{
		case AP_TCP_SERVER:
			//��������ģʽΪSERVERģʽ
			break;
		case AP_TCP_CLIENT:
			//��������ģʽΪCLIENTģʽ
			break;
		case AP_UDP:
			//��������ģʽΪUDPģʽ
			break;
		default:
			break;
	}
	// TODO: ��ȡWANIP
	
}
 /*************************************************
Function: set_tcp_server_mode
Description: ��������ģ�������ģʽΪ������ģʽ
Calls:
Called By:  set_wifi_ap_mode----��������ģ��ΪAPģʽ
			set_wifi_sta_mode----��������ģ��ΪSTAģʽ
			set_wifi_ap_sta_mode----��������ģ��ΪAP+STAģʽ
Input: WIFI_MODE_ENUM---����ģ��ģʽ
Output: ����
Return:����
Others:
*************************************************/
static void set_tcp_server_mode(WIFI_MODE_ENUM wifi_mode)
{
	uint8_t *p=0;
	uint8_t i=0;
	assert_param(IS_WIFI_TCP_SERVER_MODE(wifi_mode));

	//open ������
	DEBUG("��������������\r\n");
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
	
	p=malloc(32);							//����32�ֽ��ڴ�

	// TODO:�˴���portnum��Ϊ��������
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
	DEBUG("����������ģʽ����\r\n");

	atk_8266_send_cmd(p,"OK",50);  
	sprintf((char*)p,"AT+CIFSR");//��ȡ����IP��ַ
	if(atk_8266_send_cmd(p,"+",50)==0)
	{
		atk_8266_at_response(0);		
	}

	free(p);							//����32�ֽ��ڴ�
	
	//printf("STAģʽ���������óɹ�\r\n");
}
 
 /*************************************************
Function: set_tcp_client_mode
Description: ��������ģ�������ģʽΪ�ͻ���ģʽ
Calls:
Called By:  set_wifi_ap_mode----��������ģ��ΪAPģʽ
		set_wifi_sta_mode----��������ģ��ΪSTAģʽ
		set_wifi_ap_sta_mode----��������ģ��ΪAP+STAģʽ
Input: WIFI_MODE_ENUM---����ģ��ģʽ
Output: ����
Return:����
Others:
*************************************************/
static void set_tcp_client_mode(WIFI_MODE_ENUM wifi_mode)
{
	uint8_t *ipbuf ,*tcp_ptr;
	char *p;
	
	assert_param(IS_WIFI_TCP_CLIENT_MODE(wifi_mode));
	tcp_ptr=malloc(40);
	ipbuf=malloc(16);
	//p=malloc(50);
	DEBUG("wifi_esp8266����---����TCP_CLIENT����\r\n");
	
	ipbuf =get_far_end_ip();//��ȡԶ��IP��ַ
	DEBUG("wifi_esp8266����---��ȡ��Զ��IP��ַ: %s\r\n",ipbuf);
	//open������
	if(ipbuf!= NULL)
	{
		if(atk_8266_send_cmd("AT+CIPMUX=0","OK",20)==0)   //0�������ӣ�1��������
		{
			DEBUG("wifi_esp8266����---CLIENTģʽ�µ����ӽ����ɹ�\r\n");
			
			//delay_ms(200);
			// ���ӵ�IP��ַ
			DEBUG("wifi_esp8266����---��ʼ����Ŀ��TCP����������\r\n");
			if(tcp_ptr!=NULL)
			{
				sprintf((char*)tcp_ptr,"AT+CIPSTART=\"TCP\",\"%s\",%s",(char *)ipbuf,(char *)portnum);    //����Ŀ��TCP������
				DEBUG("wifi_esp8266����---Ҫд���Ŀ��TCP���� :%s\r\n",tcp_ptr);
				while(atk_8266_send_cmd(tcp_ptr,"OK",50)); //һֱ����ֱ������OKΪֹ
				DEBUG("wifi_esp8266����---CLIENTģʽ�ɹ�����TCP����\r\n");
				
				atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��	
				
				atk_8266_quit_trans();
				atk_8266_send_cmd("AT+CIPSEND","OK",20);       //��ʼ͸��
				sprintf((char*)tcp_ptr,"wifi_esp8266����---����͸��ģʽ\r\n");//��������
				u3_printf("%s",tcp_ptr);

			}
			else
			{
				DEBUG("wifi_esp8266����---��ȡ�ڴ�ʧ��\r\n");

			}

		}
		else
		{
			DEBUG("wifi_esp8266����---CLIENTģʽ�µ����ӽ���ʧ��\r\n");

		}

	}
	else
	{
		DEBUG("wifi_esp8266����---��ȡԶ��IP��ַʧ��r\n");

	}
	//free(p);
	free(tcp_ptr);
	free(ipbuf);
}
 
 /*************************************************
Function: set_tcp_udp_mode
Description: ��������ģ�������ģʽΪudpģʽ:
		1.����������ģʽ
		2.����Ŀ��UDP����������������IP��ַ�Ͷ˿ں�
Calls:     get_far_end_ip----��ȡԶ��IP��ַ
		atk_8266_send_cmd----��8266��������
Called By:  set_wifi_ap_mode----��������ģ��ΪAPģʽ
		set_wifi_sta_mode----��������ģ��ΪSTAģʽ
		set_wifi_ap_sta_mode----��������ģ��ΪAP+STAģʽ
Input: WIFI_MODE_ENUM---����ģ��ģʽ
Output: ����
Return:����
Others:
*************************************************/
static void set_tcp_udp_mode(WIFI_MODE_ENUM wifi_mode)
{
	uint8_t *tcp_ptr=0;
	uint8_t *ipbuf;
	assert_param(IS_WIFI_TCP_UDP_MODE(wifi_mode));
	
	DEBUG("wifi_esp8266����---����TCP_UDP����\r\n");

	ipbuf=malloc(32);
	ipbuf=get_far_end_ip();//��ȡԶ��IP��ַ
	DEBUG("wifi_esp8266����---��ȡ��Զ��IP��ַ: %s\r\n",ipbuf);
	tcp_ptr=malloc(32);
	if(ipbuf !=NULL)
	{
		sprintf((char*)tcp_ptr,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������

		if(atk_8266_send_cmd("AT+CIPMUX=0","OK",20)==0)   //0�������ӣ�1��������
		{
			DEBUG("wifi_esp8266����---UDPģʽ�µ����ӽ����ɹ�\r\n");
			
			//delay_ms(200);
			// ���ӵ�IP��ַ
			DEBUG("wifi_esp8266����---��ʼ����Ŀ��TCP����������\r\n");
			if(tcp_ptr!=NULL)
			{
				sprintf((char*)tcp_ptr,"AT+CIPSTART=\"UDP\",\"%s\",%s",(char *)ipbuf,(char *)portnum);    //����Ŀ��TCP������
				DEBUG("wifi_esp8266����---Ҫд���Ŀ��TCP���� :%s\r\n",tcp_ptr);
				while(atk_8266_send_cmd(tcp_ptr,"OK",50)); //һֱ����ֱ������OKΪֹ
				DEBUG("wifi_esp8266����---UDPģʽ�ɹ�����TCP����\r\n");

			}
			else
			{
				DEBUG("wifi_esp8266����---��ȡ�ڴ�ʧ��\r\n");

			}

		}
		else
		{
			DEBUG("wifi_esp8266����---UDPģʽ�µ����ӽ���ʧ��\r\n");

		}

	}
	free(tcp_ptr);
	free(ipbuf);
}
  /*************************************************
Function: set_far_end_ip
Description: ATK-ESP8266 IP����
Calls:
Called By:  
Input: 	ip:Ҫд���Զ��IP��ַ
		ip_type:�����õ�����0----��ʼ����1----�����µ�ID 
		wifi_mode----����ģʽ
Output: ����
Return:void
Others:
*************************************************/
static void set_far_end_ip(const char * ip,WIFI_MODE_ENUM wifi_mode, SET_IP_TYPE_ENUM ip_type) 
{
	//u8* p;
	u8 res =0;
	assert_param(IS_WIFI_MODE(wifi_mode));
	assert_param(IS_SET_IP_TYPE_FLAG(ip_type));
	
	strcpy((char*)wifi_far_end_ip,ip);
	if(ip_type!=INIT_IP)//���ǳ�ʼ��IP ����Ҫ��������
	{
		// TODO:����������IP��ַ��Ҫ��������IP
	}
	
}
 
 /*************************************************
Function: get_far_end_ip
Description: ��ȡATK-ESP8266 far_end IP��ַ
Calls:
Called By:  
Input: 	
Output: ����
Return: wifi_far_end_ip--IP��ַָ��
Others:
*************************************************/
static uint8_t * get_far_end_ip(void) 
{
	return (uint8_t *)wifi_far_end_ip;
}

#if 0
 /*************************************************
Function: atk_8266_send_cmd
Description: ��ATK-ESP8266��������
Calls:
Called By:  set_wifi_ap_mode----��������ģ��ΪAPģʽ
			set_wifi_sta_mode----��������ģ��ΪSTAģʽ
			set_wifi_ap_sta_mode----��������ģ��ΪAP+STAģʽ
Input: 	cmd:���͵������ַ���
		ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
		waittime:�ȴ�ʱ��(��λ:10ms)
Output: ����
Return:0,���ͳɹ�(�õ����ڴ���Ӧ����)  1,����ʧ��
Others:
*************************************************/
static u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd((char *)ack))
				{
					//printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 	
 /*************************************************
Function: atk_8266_quit_trans
Description: ATK-ESP8266�˳�͸��ģʽ
Calls:
Called By:  
Input: 	
Output: ����
Return:0----�˳��ɹ� 1---�˳�ʧ��
Others:
*************************************************/

 //
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
static u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�
}

 
 /*************************************************
Function: atk_8266_check_cmd
Description:ATK-ESP8266���������,�����յ���Ӧ��
Calls:
Called By:  
Input: 	str:�ڴ���Ӧ����
		mode:����ģʽ
		port:�˿ں�
		*ip:ip������(����IP���ϲ㺯��)
Output: ����
Return:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
Others:
*************************************************/
static u8* atk_8266_check_cmd(char *str)
{
	
	char *strx=0;
	
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
	
}
 /*************************************************
Function: atk_8266_get_wanip
Description:��ȡClient ip��ַ
Calls:
Called By:  
Input: 	ipbuf:ip��ַ���������
		*ip:ip������(����IP���ϲ㺯��)
Output: ����
Return:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
Others:
*************************************************/

static void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//��ȡWAN IP��ַʧ��
		{
			ipbuf[0]=0;
			return;
		}		
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		*p1=0;
		sprintf((char*)ipbuf,"%s",p+1);	
}
 /*************************************************
Function: atk_8266_send_data
Description:��ATK-ESP8266����ָ������
Calls:
Called By:  
Input: 	data:���͵�����(����Ҫ��ӻس���)
		ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
		waittime:�ȴ�ʱ��(��λ:10ms)
Output: ����
Return:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
Others:
*************************************************/
#if 1
static u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	
	u8 res=0; 
	USART3_RX_STA=0;
	//u3_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd((char *)ack))break;//�õ���Ч���� 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
#endif
#endif

#endif

