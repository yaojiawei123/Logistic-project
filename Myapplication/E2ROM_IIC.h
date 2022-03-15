#ifndef __E2ROM_IIC_H__
#define __E2ROM_IIC_H__

#include "MyApplication.h"




/*********************IIC_E2PROM结构体初始化****************************/

typedef struct
{
  void (*Init)(void); 
	void (*Start)(void);                        //发送IIC开始信号
	void (*Stop)(void);                         //发送IIC停止信号
	ACK_Value_t (*Write_Byte)(uint8_t);      //I2C写字节
	uint8_t     (*Read_Byte) (ACK_Value_t);  //I2C读字节
}IIC_E2PROM_t;


/* extern  variables-----------------------------------------------------------*/
extern  IIC_E2PROM_t  IIC_E2PROM;
#endif
/********************************************************
  End Of File
********************************************************/
