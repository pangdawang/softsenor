/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : Wifi_atk_esp8266.c
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : Wifi_atk_esp8266 模式配置
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

/*--------------WIFI模式配置字符串----------------*/
#if 0
//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="ALIENTEK";			//路由器SSID号
const u8* wifista_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifista_password="15902020353"; 	//连接密码

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* wifiap_ssid="ATK-ESP8266";			//对外SSID号
const u8* wifiap_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* wifiap_password="12345678"; 		//连接密码 

/*----------3种网络模式----------------*/
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STA模式 ","AP模式 ","AP&STA模式 "};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式 
/*-------3种工作模式----------*/
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP 模式"};	//ATK-ESP8266,4种工作模式
/*---------5种加密方式------------*/
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
#endif
//1 wifi远端IP地址根据具体连接服务器的IP地址进行修改----20180426

u8 wifi_far_end_ip[16]={0};
const u8* default_far_end_ip="192.168.4.2";

/*************************************************
Function: wifi_printf
Description: wifi数据打印，类似实现了printf的功能
Calls:通用函数
Called By:  atk_8266_send_cmd---wifi命令发送函数
		atk_8266_send_data----wifi命令数据函数
Input: 	wifi_mode---wifi_mode
		print_data---待打印的字符串
		data_length---字符串长度
Output: 暂无
Return:暂无
Others:
*************************************************/
void wifi_printf(WIFI_MODE_ENUM  wifi_mode,char * print_data,const uint8_t data_length)
{
	char pdata[14]={0};
	char* sendout_data;

	assert_param(IS_WIFI_MODE(wifi_mode));
	assert_param(print_data!=NULL);

	if(IS_WIFI_TCP_CLIENT_MODE(wifi_mode) )//透传模式
	{
		sendout_data=malloc(data_length);
		sprintf((char*)sendout_data,print_data);//测试数据
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
Description: 配置无线模块的工作模式
Calls:set_wifi_ap_mode---APMODE
	set_wifi_sta_mode----STAMODE
	set_wifi_ap_sta_mode----AP+STAMODE
Called By:  init_config---系统初始化函数
		ip_type----0，初始化IP，1，赋了新值IP
Input: wifi_mode---无线模块模式
		ip_type---远程IP 模式---初始化IP或重新设置IP
Output: 暂无
Return:暂无
Others:
*************************************************/
void config_wifi_mode(WIFI_MODE_ENUM wifi_mode,SET_IP_TYPE_ENUM ip_type)
{
	u8 *p;
	char *ip;
	u8 i;
	assert_param(IS_WIFI_MODE(wifi_mode));
	assert_param(IS_SET_IP_TYPE_FLAG(ip_type));

	//检查模块是否连接
	while(atk_8266_send_cmd("AT","OK",20))//轮询检查WIFI模块是否在线
	{
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		DEBUG("wifi 模块连接轮询进行中\r\n");
		delay_ms(100);
	}

	DEBUG("wifi_esp8266 模块----连接通道正常\r\n");
	// TODO:给远端IP地址赋初值
	set_far_end_ip(default_far_end_ip,wifi_mode,INIT_IP);
	//wifi_far_end_ip = ip;
	DEBUG("远程IP地址:%s\r\n",wifi_far_end_ip);
	/*
	//写入默认的IP地址，此处处理是避免在调用IP地址时，地址不存在的情况
	i = strlen(default_far_end_ip);
	ip=malloc(i);							//申请内存
	sprintf(ip,default_far_end_ip);
	set_far_end_ip(ip,INIT_IP,wifi_mode,ip_type);
	*/
	switch(wifi_mode)
	{
		case AP_TCP_SERVER:  //1此模式测试成功
		case AP_TCP_CLIENT: //1此模式测试成功
		case AP_UDP:        //1此模式测试成功
			//调用AP模式函数
			set_wifi_ap_mode(wifi_mode);
			break;
		case STA_TCP_SERVER:
		case STA_TCP_CLIENT:
		case STA_UDP:
	//调用STA模式函数
			set_wifi_sta_mode(wifi_mode);
			break;
		default:
			//调用AP+STA模式函数
			set_wifi_ap_sta_mode(wifi_mode);//ap+sta
			break;
			
	}
	
	DEBUG("wifi_esp8266 模块----配置成功，可进行数据发送\r\n");
	//free((void*)wifi_far_end_ip);
}

/*************************************************
Function: set_wifi_ap_mode
Description: 配置无线模块的工作模式为AP模式
Calls:
Called By:  config_wifi_mode---系统初始化函数
Input: WIFI_MODE_ENUM---无线模块模式
Output: 暂无
Return:暂无
Others:
*************************************************/
 static void set_wifi_ap_mode(WIFI_MODE_ENUM wifi_mode)
{
	u8 *p ,*ipbuf;

	assert_param(IS_WIFI_AP_MODE(wifi_mode));

	//2 AP模式配置---第一步设置模式，重启模块
	atk_8266_send_cmd("AT+CWMODE=2","OK",20);//ap
	atk_8266_send_cmd("AT+RST","OK",20);		//重启模块 
	delay_ms(1000);         //延时2S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	
	p=malloc(32);							//申请32字节内存
	DEBUG("进入wifi 模块AP模式设置函数\r\n");
	
	//2 AP模式配置---第二步设置AP模式的参数:ssid和password

	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //配置模块AP模式无线参数

	DEBUG("wifi 模块ap模式设置:\r\n");
	atk_8266_send_cmd(p,"OK",1000);
	free(p);		//释放内存 

	//2网络模式配置---第三步，配置TCP/UDP模式
	switch(wifi_mode)
	{
		case AP_TCP_SERVER:
			//配置网络模式为SERVER模式
			set_tcp_server_mode(wifi_mode);			
			break;
		case AP_TCP_CLIENT:
			//配置网络模式为CLIENT模式
			set_tcp_client_mode(wifi_mode);			
			break;
		case AP_UDP:
			//配置网络模式为UDP模式
			set_tcp_udp_mode(wifi_mode);
			break;
		default:
			break;
	}

}

 /*************************************************
Function: set_wifi_sta_mode
Description: 配置无线模块的工作模式为sta模式
Calls:
Called By:  config_wifi_mode---系统初始化函数
Input: WIFI_MODE_ENUM---无线模块模式
Output: 暂无
Return:暂无
Others:
*************************************************/
 static void set_wifi_sta_mode(WIFI_MODE_ENUM wifi_mode)
{
	u8 *ipbuf;
	u8 p[32]={0};
	assert_param(IS_WIFI_STA_MODE(wifi_mode));

	atk_8266_send_cmd("AT+CWMODE=1","OK",20);//sta
	atk_8266_send_cmd("AT+RST","OK",20);		//重启模块 
	delay_ms(1000);         //延时2S等待重启成功
	delay_ms(1000);
	delay_ms(1000);
	
	DEBUG("wifi 模块STA模式设置重启成功\r\n");
/*
	p=malloc(32);							//申请32字节内存
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	if(p==NULL)
	{
		DEBUG("内存分配失败\r\n");
	}
*/
	//sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	//while(atk_8266_send_cmd(p,"WIFI GOT IP",300));
	//DEBUG("wifi 模块STA模式GOTIP轮询中\r\n");//连接目标路由器,并且获得IP

	DEBUG("wifi 模块STA模式无线参数设置成功\r\n");

	switch(wifi_mode)
	{
		case AP_TCP_SERVER:
			//配置网络模式为SERVER模式
			set_tcp_server_mode(wifi_mode);			
			break;
		case AP_TCP_CLIENT:
			//配置网络模式为CLIENT模式
			set_tcp_client_mode(wifi_mode);			
			break;
		case AP_UDP:
			//配置网络模式为UDP模式
			set_tcp_udp_mode(wifi_mode);
			break;
		default:
			break;
	}
	// TODO: 获取WANIP
	atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
	sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);

	//free(p);		//释放内存 

}
  /*************************************************
Function: set_wifi_AP_sta_mode
Description: 配置无线模块的工作模式为sta模式
Calls:
Called By:  config_wifi_mode---系统初始化函数
Input: WIFI_MODE_ENUM---无线模块模式
Output: 暂无
Return:暂无
Others:
*************************************************/
 static void set_wifi_ap_sta_mode(WIFI_MODE_ENUM wifi_mode)
{
	u8 *p;

	assert_param(IS_WIFI_AP_STA_MODE(wifi_mode));
	// TODO:9种模式待添加
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",wifiap_ssid,wifiap_password);    //配置模块AP模式无线参数
	atk_8266_send_cmd(p,"OK",1000);

	switch(wifi_mode)
	{
		case AP_TCP_SERVER:
			//配置网络模式为SERVER模式
			break;
		case AP_TCP_CLIENT:
			//配置网络模式为CLIENT模式
			break;
		case AP_UDP:
			//配置网络模式为UDP模式
			break;
		default:
			break;
	}
	// TODO: 获取WANIP
	
}
 /*************************************************
Function: set_tcp_server_mode
Description: 配置无线模块的网络模式为服务器模式
Calls:
Called By:  set_wifi_ap_mode----配置无线模块为AP模式
			set_wifi_sta_mode----配置无线模块为STA模式
			set_wifi_ap_sta_mode----配置无线模块为AP+STA模式
Input: WIFI_MODE_ENUM---无线模块模式
Output: 暂无
Return:暂无
Others:
*************************************************/
static void set_tcp_server_mode(WIFI_MODE_ENUM wifi_mode)
{
	uint8_t *p=0;
	uint8_t i=0;
	assert_param(IS_WIFI_TCP_SERVER_MODE(wifi_mode));

	//open 多连接
	DEBUG("开启多链接设置\r\n");
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
	
	p=malloc(32);							//申请32字节内存

	// TODO:此处的portnum改为参数输入
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);    //开启Server模式(0，关闭；1，打开)，端口号为portnum
	DEBUG("开启服务器模式设置\r\n");

	atk_8266_send_cmd(p,"OK",50);  
	sprintf((char*)p,"AT+CIFSR");//获取本机IP地址
	if(atk_8266_send_cmd(p,"+",50)==0)
	{
		atk_8266_at_response(0);		
	}

	free(p);							//申请32字节内存
	
	//printf("STA模式服务器设置成功\r\n");
}
 
 /*************************************************
Function: set_tcp_client_mode
Description: 配置无线模块的网络模式为客户端模式
Calls:
Called By:  set_wifi_ap_mode----配置无线模块为AP模式
		set_wifi_sta_mode----配置无线模块为STA模式
		set_wifi_ap_sta_mode----配置无线模块为AP+STA模式
Input: WIFI_MODE_ENUM---无线模块模式
Output: 暂无
Return:暂无
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
	DEBUG("wifi_esp8266测试---进入TCP_CLIENT设置\r\n");
	
	ipbuf =get_far_end_ip();//获取远端IP地址
	DEBUG("wifi_esp8266测试---获取的远端IP地址: %s\r\n",ipbuf);
	//open单连接
	if(ipbuf!= NULL)
	{
		if(atk_8266_send_cmd("AT+CIPMUX=0","OK",20)==0)   //0：单连接，1：多连接
		{
			DEBUG("wifi_esp8266测试---CLIENT模式下单连接建立成功\r\n");
			
			//delay_ms(200);
			// 连接到IP地址
			DEBUG("wifi_esp8266测试---开始配置目标TCP服务器参数\r\n");
			if(tcp_ptr!=NULL)
			{
				sprintf((char*)tcp_ptr,"AT+CIPSTART=\"TCP\",\"%s\",%s",(char *)ipbuf,(char *)portnum);    //配置目标TCP服务器
				DEBUG("wifi_esp8266测试---要写入的目标TCP参数 :%s\r\n",tcp_ptr);
				while(atk_8266_send_cmd(tcp_ptr,"OK",50)); //一直连接直到返回OK为止
				DEBUG("wifi_esp8266测试---CLIENT模式成功建立TCP连接\r\n");
				
				atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传	
				
				atk_8266_quit_trans();
				atk_8266_send_cmd("AT+CIPSEND","OK",20);       //开始透传
				sprintf((char*)tcp_ptr,"wifi_esp8266测试---进入透传模式\r\n");//测试数据
				u3_printf("%s",tcp_ptr);

			}
			else
			{
				DEBUG("wifi_esp8266测试---获取内存失败\r\n");

			}

		}
		else
		{
			DEBUG("wifi_esp8266测试---CLIENT模式下单连接建立失败\r\n");

		}

	}
	else
	{
		DEBUG("wifi_esp8266测试---获取远端IP地址失败r\n");

	}
	//free(p);
	free(tcp_ptr);
	free(ipbuf);
}
 
 /*************************************************
Function: set_tcp_udp_mode
Description: 配置无线模块的网络模式为udp模式:
		1.开启单连接模式
		2.配置目标UDP服务器参数，包括IP地址和端口号
Calls:     get_far_end_ip----获取远端IP地址
		atk_8266_send_cmd----向8266发送命令
Called By:  set_wifi_ap_mode----配置无线模块为AP模式
		set_wifi_sta_mode----配置无线模块为STA模式
		set_wifi_ap_sta_mode----配置无线模块为AP+STA模式
Input: WIFI_MODE_ENUM---无线模块模式
Output: 暂无
Return:暂无
Others:
*************************************************/
static void set_tcp_udp_mode(WIFI_MODE_ENUM wifi_mode)
{
	uint8_t *tcp_ptr=0;
	uint8_t *ipbuf;
	assert_param(IS_WIFI_TCP_UDP_MODE(wifi_mode));
	
	DEBUG("wifi_esp8266测试---进入TCP_UDP设置\r\n");

	ipbuf=malloc(32);
	ipbuf=get_far_end_ip();//获取远端IP地址
	DEBUG("wifi_esp8266测试---获取的远端IP地址: %s\r\n",ipbuf);
	tcp_ptr=malloc(32);
	if(ipbuf !=NULL)
	{
		sprintf((char*)tcp_ptr,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器

		if(atk_8266_send_cmd("AT+CIPMUX=0","OK",20)==0)   //0：单连接，1：多连接
		{
			DEBUG("wifi_esp8266测试---UDP模式下单连接建立成功\r\n");
			
			//delay_ms(200);
			// 连接到IP地址
			DEBUG("wifi_esp8266测试---开始配置目标TCP服务器参数\r\n");
			if(tcp_ptr!=NULL)
			{
				sprintf((char*)tcp_ptr,"AT+CIPSTART=\"UDP\",\"%s\",%s",(char *)ipbuf,(char *)portnum);    //配置目标TCP服务器
				DEBUG("wifi_esp8266测试---要写入的目标TCP参数 :%s\r\n",tcp_ptr);
				while(atk_8266_send_cmd(tcp_ptr,"OK",50)); //一直连接直到返回OK为止
				DEBUG("wifi_esp8266测试---UDP模式成功建立TCP连接\r\n");

			}
			else
			{
				DEBUG("wifi_esp8266测试---获取内存失败\r\n");

			}

		}
		else
		{
			DEBUG("wifi_esp8266测试---UDP模式下单连接建立失败\r\n");

		}

	}
	free(tcp_ptr);
	free(ipbuf);
}
  /*************************************************
Function: set_far_end_ip
Description: ATK-ESP8266 IP设置
Calls:
Called By:  
Input: 	ip:要写入的远端IP地址
		ip_type:被调用的类型0----初始化，1----设置新的ID 
		wifi_mode----无线模式
Output: 暂无
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
	if(ip_type!=INIT_IP)//不是初始化IP ，则要进行重连
	{
		// TODO:当重新配置IP地址后，要重新连接IP
	}
	
}
 
 /*************************************************
Function: get_far_end_ip
Description: 获取ATK-ESP8266 far_end IP地址
Calls:
Called By:  
Input: 	
Output: 暂无
Return: wifi_far_end_ip--IP地址指针
Others:
*************************************************/
static uint8_t * get_far_end_ip(void) 
{
	return (uint8_t *)wifi_far_end_ip;
}

#if 0
 /*************************************************
Function: atk_8266_send_cmd
Description: 向ATK-ESP8266发送命令
Calls:
Called By:  set_wifi_ap_mode----配置无线模块为AP模式
			set_wifi_sta_mode----配置无线模块为STA模式
			set_wifi_ap_sta_mode----配置无线模块为AP+STA模式
Input: 	cmd:发送的命令字符串
		ack:期待的应答结果,如果为空,则表示不需要等待应答
		waittime:等待时间(单位:10ms)
Output: 暂无
Return:0,发送成功(得到了期待的应答结果)  1,发送失败
Others:
*************************************************/
static u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd((char *)ack))
				{
					//printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
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
Description: ATK-ESP8266退出透传模式
Calls:
Called By:  
Input: 	
Output: 暂无
Return:0----退出成功 1---退出失败
Others:
*************************************************/

 //
//返回值:0,退出成功;
//       1,退出失败
static u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断
}

 
 /*************************************************
Function: atk_8266_check_cmd
Description:ATK-ESP8266发送命令后,检测接收到的应答
Calls:
Called By:  
Input: 	str:期待的应答结果
		mode:工作模式
		port:端口号
		*ip:ip缓存区(返回IP给上层函数)
Output: 暂无
Return:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
Others:
*************************************************/
static u8* atk_8266_check_cmd(char *str)
{
	
	char *strx=0;
	
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
	
}
 /*************************************************
Function: atk_8266_get_wanip
Description:获取Client ip地址
Calls:
Called By:  
Input: 	ipbuf:ip地址输出缓存区
		*ip:ip缓存区(返回IP给上层函数)
Output: 暂无
Return:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
Others:
*************************************************/

static void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
		if(atk_8266_send_cmd("AT+CIFSR","OK",50))//获取WAN IP地址失败
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
Description:向ATK-ESP8266发送指定数据
Calls:
Called By:  
Input: 	data:发送的数据(不需要添加回车了)
		ack:期待的应答结果,如果为空,则表示不需要等待应答
		waittime:等待时间(单位:10ms)
Output: 暂无
Return:0,发送成功(得到了期待的应答结果)luojian
Others:
*************************************************/
#if 1
static u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	
	u8 res=0; 
	USART3_RX_STA=0;
	//u3_printf("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd((char *)ack))break;//得到有效数据 
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

