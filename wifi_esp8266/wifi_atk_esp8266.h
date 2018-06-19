/******************** (C) COPYRIGHT 2017 SOFT_SENSE PROJECT **********************************
 * File Name          : Wifi_atk_esp8266.H
 * Author             : pangda_wang
 * Date First Issued  : 2018/04/06
 * Description        : Wifi_atk_esp8266 模式配置头文件
 * version			  : v1.0
 *******************************************************************************
 * History:
 * DATE       | VER   | AUTOR      | Description
*2018.04.25 | v1.0  | panda_wang     | initial released
 *******************************************************************************/
#ifndef _WIFI_ATK_ESP8266_H_
#define _WIFI_ATK_ESP8266_H_


#ifndef _WIFI_ATK_ESP8266_C_ 
#define GLOBAL  extern
#else
#define GLOBAL
#endif

//#define __DEBUG

#ifdef __DEBUG
#define DEBUG(format,...) printf("FILE: "__FILE__", LINE: %d: "format, __LINE__, ##__VA_ARGS__)
#else
 
#define DEBUG(format,...)
#endif
//#debug DEBUG_PRINT printf
/*----------结构体-------------*/
typedef enum SET_IP_TYPE
{
	INIT_IP=0,
	NEW_IP
}SET_IP_TYPE_ENUM;
#define IS_SET_IP_TYPE_FLAG(FLAG)   (((FLAG)==INIT_IP)\
								 ||((FLAG)==NEW_IP))


typedef enum WIFI_MODE
{
	AP_TCP_SERVER,
	AP_TCP_CLIENT,
	AP_UDP,
	STA_TCP_SERVER,
	STA_TCP_CLIENT,
	STA_UDP,
	AP_TCP_SERVER_STA_TCP_SERVER,
	AP_TCP_SERVER_STA_TCP_CLIENT,
	AP_TCP_SERVER_STA_UDP,
	AP_TCP_CLIENT_STA_TCP_SERVER,
	AP_TCP_CLIENT_STA_TCP_CLIENT,
	AP_TCP_CLIENT_STA_UDP,
	AP_UDP_STA_TCP_SERVER,
	AP_UDP_STA_TCP_CLIENT,
	AP_UDP_STA_UDP,
	
}WIFI_MODE_ENUM;//WIFI 模式

#define IS_WIFI_MODE(MODE) (((MODE)==AP_TCP_SERVER)\
							 ||((MODE)==AP_TCP_CLIENT)\
							 ||((MODE)==AP_UDP)\
							 ||((MODE)==STA_TCP_SERVER)\
							 ||((MODE)==STA_TCP_CLIENT)\
							 ||((MODE)==STA_UDP)\
							 ||((MODE)==AP_TCP_SERVER_STA_TCP_SERVER)\
							 ||((MODE)==AP_TCP_SERVER_STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_SERVER_STA_UDP)\
							 ||((MODE)==AP_TCP_CLIENT_STA_TCP_SERVER)\
							 ||((MODE)==AP_TCP_CLIENT_STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_CLIENT_STA_UDP)\
							 ||((MODE)==AP_UDP_STA_TCP_SERVER)\
							 ||((MODE)==AP_UDP_STA_TCP_CLIENT)\
							 ||((MODE)==AP_UDP_STA_UDP))
							 
#define IS_WIFI_AP_MODE(MODE)   (((MODE)==AP_TCP_SERVER)\
							 	||((MODE)==AP_TCP_CLIENT)\
								 ||((MODE)==AP_UDP))
#define IS_WIFI_STA_MODE(MODE)  (((MODE)==STA_TCP_SERVER)\
							 	||((MODE)==STA_TCP_CLIENT)\
							 	||((MODE)==STA_UDP))
#define IS_WIFI_AP_STA_MODE(MODE) (((MODE)==AP_TCP_SERVER_STA_TCP_SERVER)\
							 ||((MODE)==AP_TCP_SERVER_STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_SERVER_STA_UDP)\
							 ||((MODE)==AP_TCP_CLIENT_STA_TCP_SERVER)\
							 ||((MODE)==AP_TCP_CLIENT_STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_CLIENT_STA_UDP)\
							 ||((MODE)==AP_UDP_STA_TCP_SERVER)\
							 ||((MODE)==AP_UDP_STA_TCP_CLIENT)\
							 ||((MODE)==AP_UDP_STA_UDP))
#define IS_WIFI_TCP_SERVER_MODE(MODE) (((MODE)==AP_TCP_SERVER)\
							 ||((MODE)==STA_TCP_SERVER)\
							 ||((MODE)==AP_TCP_SERVER_STA_TCP_SERVER)\
							 ||((MODE)==AP_TCP_SERVER_STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_SERVER_STA_UDP)\
							 ||((MODE)==AP_TCP_CLIENT_STA_TCP_SERVER)\
							 ||((MODE)==AP_UDP_STA_TCP_SERVER))
#define IS_WIFI_TCP_CLIENT_MODE(MODE) (((MODE)==AP_TCP_CLIENT)\
							 ||((MODE)==STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_SERVER_STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_CLIENT_STA_TCP_CLIENT)\
							 ||((MODE)==AP_TCP_CLIENT_STA_UDP)\
							 ||((MODE)==AP_UDP_STA_TCP_CLIENT))

#define IS_WIFI_TCP_UDP_MODE(MODE) (((MODE)==AP_UDP)\
							 ||((MODE)==STA_UDP)\
							 ||((MODE)==AP_TCP_SERVER_STA_UDP)\
							 ||((MODE)==AP_TCP_CLIENT_STA_UDP)\
							 ||((MODE)==AP_UDP_STA_TCP_SERVER)\
							 ||((MODE)==AP_UDP_STA_TCP_CLIENT)\
							 ||((MODE)==AP_UDP_STA_UDP))


#undef GLOBAL

/*-------external function-------*/
void config_wifi_mode(WIFI_MODE_ENUM wifi_mode,SET_IP_TYPE_ENUM ip_type);
void wifi_printf(WIFI_MODE_ENUM  wifi_mode,char * print_data,const uint8_t data_length);


#endif

