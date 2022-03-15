#ifndef __Low_Power_H_
#define __Low_Power_H_

#include "MyApplication.h"


//定义结构体类型
typedef struct 
{
  uint8_t Enter_Sleep_Mode_Flag;
	void (*Sleep_Mode)(void);//睡眠模式
}LowPower_t;

/* extern variables-----------------------------------------------------------*/
extern LowPower_t LowPower;
#endif
/********************************************************
  End Of File
********************************************************/
