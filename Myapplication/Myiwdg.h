#ifndef __Myiwdg_H_
#define __Myiwdg_H_

#include "MyApplication.h"



//����ṹ������
typedef struct
{
	uint8_t FeedDog_Flag;  //ι����־λ
	
	void (*FeedDog)(void); //ι��
} MyIWDG_t;

/* extern variables-----------------------------------------------------------*/
extern  MyIWDG_t  MyIWDG;


#endif
/********************************************************
  End Of File
********************************************************/
