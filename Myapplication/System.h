#ifndef __System_H__
#define __System_H__

//����ṹ������
typedef struct
{
	void (*Run)(void);
	void (*Error_Handler)(void);
	void (*Assert_Failed)(void);
} System_t;

/* extern variables-----------------------------------------------------------*/
extern System_t  System;
/* extern function prototypes-------------------------------------------------*/

#endif
/********************************************************
  End Of File
********************************************************/
