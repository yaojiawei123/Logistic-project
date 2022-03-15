#ifndef __EC20_H_
#define __EC20_H_

#include "MyApplication.h"


#define		  AT_CMD_AT 		         (uint8_t*)"AT\r\n"			               //ATָ��,ͬ��������
#define     AT_CMD_ECHO_OFF        (uint8_t*)"ATE0\r\n"		               //�رջ���
#define     AT_CMD_CPIN            (uint8_t*)"AT+CPIN?\r\n"	             //�鿴SIM���Ƿ�׼����
#define     AT_CMD_CSQ             (uint8_t*)"AT+CSQ\r\n"	               //����ź�ǿ��
#define     AT_CMD_CREG            (uint8_t*)"AT+CREG?\r\n"              //�������ע��״̬
#define  	  AT_CMD_CGREG   		     (uint8_t*)"AT+CGREG?\r\n"		         //��ѯ���總��״̬
#define     AT_CMD_QICSGP          (uint8_t*)"AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n"	
#define     AT_CMD_QIACT           (uint8_t*)"AT+QIACT=1\r\n"
#define     AT_CMD_QMTOPEN         (uint8_t*)"AT+QMTOPEN=0,\"45.76.188.38\",1883\r\n"
#define     AT_CMD_QMTCONN         (uint8_t*)"AT+QMTCONN=0,\"YJW\"\r\n"
#define     AT_CMD_QMTPUB          (uint8_t*)"AT+QMTPUB=0,0,0,0,\"gps/msg\"\r\n"
#define     AT_CMD_QMTSUB          (uint8_t*)"AT+QMTSUB=0,1,\"gps/command\",0\r\n"
#define     AT_CMD_CGSN            (uint8_t*)"AT+CGSN\r\n"


typedef struct
{
  uint8_t   MQTT_Connect_Status;  //MQTT����״̬
	uint16_t  MQTT_Reconnect_Timer;  //MQTT������ʱ��
	uint8_t   CSQ_Flag;              //�ź�ֵ״̬      0����ʾ���ź� 1����ʾ���ź�
	uint8_t   Instore_Data;          //�����Ƿ�洢��AT240C02    0:��ʾ����Ҫ�洢  1:��Ҫ�洢  Ĭ��Ϊ0
	
	void      (*Start_up)(void);    //����
	void      (*MQTT_Connect_Server)(void);//����MQTT������
	void      (*Transfer_Data)(void); //��MQTT��������������
	void      (*Receive_Information)(void); //���շ�����������Ϣ
	void      (*DMA_Receive_Set)(void); //DMA��������
	void      (*Error)(void); //������
	void      (*Transfer_Warning_Data)(void); //���津���ϱ�����
	uint8_t   (*Get_CSQ)(void);  //4Gģ���ȡ�ź�
}EC20_t;


#define Tx_MQTT_buf_Len 255
#define Tx_MQTT_Pub_Len 255
typedef struct
{
  char Tx_MQTT_buf[Tx_MQTT_buf_Len];
	char Tx_MQTT_Pub[Tx_MQTT_Pub_Len];
	char IMEI_Buf[15];
}EC20_Data_t;

void Send_AT(uint8_t *,uint8_t *); //����ATָ���
/* extern variables-----------------------------------------------------------*/
extern EC20_t EC20;
extern EC20_Data_t EC20_Data;
#endif
/********************************************************
  End Of File
********************************************************/
