#ifndef __Low_Power_H_
#define __Low_Power_H_

#include "MyApplication.h"


//����ṹ������
typedef struct 
{
  uint8_t Enter_Sleep_Mode_Flag;
	void (*Sleep_Mode)(void);//˯��ģʽ
}LowPower_t;

/* extern variables-----------------------------------------------------------*/
extern LowPower_t LowPower;
#endif
/********************************************************
  End Of File
********************************************************/
