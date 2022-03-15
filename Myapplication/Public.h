#ifndef __PUBLIC_H_
#define __PUBLIC_H_

#include "MyApplication.h"

//extern UART_HandleTypeDef huart1;
/* Public define-------------------------------------------------------------*/
#define SoftWare_Version 	(float)1.0
#define	huart_debug		huart1

//定义枚举类型 -> BIT位
typedef enum
{
	BIT0 = (uint8_t)(0x01 << 0),  
	BIT1 = (uint8_t)(0x01 << 1),  
	BIT2 = (uint8_t)(0x01 << 2),  
	BIT3 = (uint8_t)(0x01 << 3),  
	BIT4 = (uint8_t)(0x01 << 4),
	BIT5 = (uint8_t)(0x01 << 5),
	BIT6 = (uint8_t)(0x01 << 6),
	BIT7 = (uint8_t)(0x01 << 7),
}BIT_t;

//定义枚举类型 -> TRUE/FALSE位
typedef enum 
{
  FALSE = 0U, 
  TRUE = !FALSE
} FlagStatus_t;

typedef enum 
{
  FAILED = 0U, 
  PASSED = !FAILED
} TestStatus_t;


typedef struct
{
	void (*Memory_Clr)(uint8_t*,uint16_t); //内存清除函数
	void (*Reset_MCU)(void);
} Public_t;

/* extern variables-----------------------------------------------------------*/
extern Public_t Public;
#endif
/********************************************************
  End Of File
********************************************************/
