#ifndef __Myiwdg_H_
#define __Myiwdg_H_

#include "MyApplication.h"



//定义结构体类型
typedef struct
{
	uint8_t FeedDog_Flag;  //喂狗标志位
	
	void (*FeedDog)(void); //喂狗
} MyIWDG_t;

/* extern variables-----------------------------------------------------------*/
extern  MyIWDG_t  MyIWDG;


#endif
/********************************************************
  End Of File
********************************************************/
