#ifndef __My_Usart_H_
#define __My_Usart_H_

#include "MyApplication.h"


#define Usart1ReadBuffLen 512 //����1���ճ���
#define Uart4ReadBuffLen  200 //����2�������ݳ���
#define Usart2_Send_LENGTH  150
#define Usart2_Rec_LENGTH 	 150
#define Usart3_Rec_LENGTH 	 150

typedef struct
{
  unsigned char USART1_IT_IDLE;         //�����жϱ�־
	uint32_t      Usart1ReadCnt;           //�������ݸ���
	unsigned char Usart1ReadBuff[Usart1ReadBuffLen]; //����1����buffer
}My_Usart1_t;



typedef struct
{
  unsigned char Uart4ReadBuff[Uart4ReadBuffLen];
	uint32_t      Usart1ReadCnt;     
}My_Uart4_t;


typedef struct
{
	uint8_t* pucSend_Buffer;           //���ͻ���ָ�� 
	uint8_t* pucRec_Buffer;            //���ջ���ָ�� 
	
	void (*SendArray)(uint8_t*,uint16_t);  //���ڷ�������
	void (*SendString)(uint8_t*);          //���ڷ����ַ���
}My_Usart2_t;

typedef struct
{
  uint8_t* pucRec_Buffer3; 
	
}My_Usart3_t;

/* extern variables-----------------------------------------------------------*/
extern My_Usart1_t My_Usart1;
extern My_Uart4_t My_Uart4;
extern My_Usart2_t My_Usart2;
extern My_Usart3_t My_Usart3;
#endif
/********************************************************
  End Of File
********************************************************/
