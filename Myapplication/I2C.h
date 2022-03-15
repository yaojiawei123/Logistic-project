#ifndef __I2C_H_
#define __I2C_H_

#include "MyApplication.h"


//定义枚举类型
typedef enum
{
	ACK	 = GPIO_PIN_RESET,
	NACK = GPIO_PIN_SET,
}ACK_Value_t;

//定义结构体类型
typedef struct
{
	void (*Init)(void);  //I2C初始化
	void (*Start)(void); //I2C起始信号
	void (*Stop)(void);  //I2C停止信号
	ACK_Value_t (*Write_Byte)(uint8_t);      //I2C写字节
	uint8_t     (*Read_Byte) (ACK_Value_t);  //I2C读字节
}I2C_Soft_t;


typedef struct 
{
	void (*i2c_Start)(void);
  void (*i2c_Stop)(void);
  void (*i2c_SendByte)(uint8_t _ucByte);
  uint8_t (*i2c_ReadByte)(void);
  uint8_t (*i2c_WaitAck)(void);
  void (*i2c_Ack)(void);
  void (*i2c_NAck)(void);
  uint8_t (*i2c_CheckDevice)(uint8_t _Address);
	
}I2C_BH1750_t;

/* extern variables-----------------------------------------------------------*/
extern I2C_Soft_t  I2C_Soft;
extern I2C_BH1750_t I2C_BH1750;

#endif
/********************************************************
  End Of File
********************************************************/
