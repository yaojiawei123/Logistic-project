#ifndef __EC20_H_
#define __EC20_H_

#include "MyApplication.h"


#define		  AT_CMD_AT 		         (uint8_t*)"AT\r\n"			               //AT指令,同步波特率
#define     AT_CMD_ECHO_OFF        (uint8_t*)"ATE0\r\n"		               //关闭回显
#define     AT_CMD_CPIN            (uint8_t*)"AT+CPIN?\r\n"	             //查看SIM卡是否准备好
#define     AT_CMD_CSQ             (uint8_t*)"AT+CSQ\r\n"	               //检查信号强度
#define     AT_CMD_CREG            (uint8_t*)"AT+CREG?\r\n"              //检查网络注册状态
#define  	  AT_CMD_CGREG   		     (uint8_t*)"AT+CGREG?\r\n"		         //查询网络附着状态
#define     AT_CMD_QICSGP          (uint8_t*)"AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n"	
#define     AT_CMD_QIACT           (uint8_t*)"AT+QIACT=1\r\n"
#define     AT_CMD_QMTOPEN         (uint8_t*)"AT+QMTOPEN=0,\"45.76.188.38\",1883\r\n"
#define     AT_CMD_QMTCONN         (uint8_t*)"AT+QMTCONN=0,\"YJW\"\r\n"
#define     AT_CMD_QMTPUB          (uint8_t*)"AT+QMTPUB=0,0,0,0,\"gps/msg\"\r\n"
#define     AT_CMD_QMTSUB          (uint8_t*)"AT+QMTSUB=0,1,\"gps/command\",0\r\n"
#define     AT_CMD_CGSN            (uint8_t*)"AT+CGSN\r\n"


typedef struct
{
  uint8_t   MQTT_Connect_Status;  //MQTT连接状态
	uint16_t  MQTT_Reconnect_Timer;  //MQTT重连定时器
	uint8_t   CSQ_Flag;              //信号值状态      0：表示无信号 1：表示有信号
	uint8_t   Instore_Data;          //数据是否存储到AT240C02    0:表示不需要存储  1:需要存储  默认为0
	
	void      (*Start_up)(void);    //开机
	void      (*MQTT_Connect_Server)(void);//连接MQTT服务器
	void      (*Transfer_Data)(void); //向MQTT服务器发送数据
	void      (*Receive_Information)(void); //接收服务器数据信息
	void      (*DMA_Receive_Set)(void); //DMA接收设置
	void      (*Error)(void); //错误处理
	void      (*Transfer_Warning_Data)(void); //警告触发上报命令
	uint8_t   (*Get_CSQ)(void);  //4G模块获取信号
}EC20_t;


#define Tx_MQTT_buf_Len 255
#define Tx_MQTT_Pub_Len 255
typedef struct
{
  char Tx_MQTT_buf[Tx_MQTT_buf_Len];
	char Tx_MQTT_Pub[Tx_MQTT_Pub_Len];
	char IMEI_Buf[15];
}EC20_Data_t;

void Send_AT(uint8_t *,uint8_t *); //发送AT指令函数
/* extern variables-----------------------------------------------------------*/
extern EC20_t EC20;
extern EC20_Data_t EC20_Data;
#endif
/********************************************************
  End Of File
********************************************************/
