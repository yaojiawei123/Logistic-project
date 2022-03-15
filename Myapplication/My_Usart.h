#ifndef __My_Usart_H_
#define __My_Usart_H_

#include "MyApplication.h"


#define Usart1ReadBuffLen 512 //串口1接收长度
#define Uart4ReadBuffLen  200 //串口2接收数据长度
#define Usart2_Send_LENGTH  150
#define Usart2_Rec_LENGTH 	 150
#define Usart3_Rec_LENGTH 	 150

typedef struct
{
  unsigned char USART1_IT_IDLE;         //空闲中断标志
	uint32_t      Usart1ReadCnt;           //接收数据个数
	unsigned char Usart1ReadBuff[Usart1ReadBuffLen]; //串口1接收buffer
}My_Usart1_t;



typedef struct
{
  unsigned char Uart4ReadBuff[Uart4ReadBuffLen];
	uint32_t      Usart1ReadCnt;     
}My_Uart4_t;


typedef struct
{
	uint8_t* pucSend_Buffer;           //发送缓存指针 
	uint8_t* pucRec_Buffer;            //接收缓存指针 
	
	void (*SendArray)(uint8_t*,uint16_t);  //串口发送数组
	void (*SendString)(uint8_t*);          //串口发送字符串
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
