#ifndef __CallBack_H__
#define __CallBack_H__

/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* extern variables-----------------------------------------------------------*/

/* extern function prototypes-------------------------------------------------*/
extern void HAL_UART_IdleCallback(UART_HandleTypeDef *huart); //串口空闲中断回调函数

#endif
/********************************************************
  End Of File
********************************************************/
