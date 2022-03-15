#ifndef __I2C_H_
#define __I2C_H_

#include "MyApplication.h"


//����ö������
typedef enum
{
	ACK	 = GPIO_PIN_RESET,
	NACK = GPIO_PIN_SET,
}ACK_Value_t;

//����ṹ������
typedef struct
{
	void (*Init)(void);  //I2C��ʼ��
	void (*Start)(void); //I2C��ʼ�ź�
	void (*Stop)(void);  //I2Cֹͣ�ź�
	ACK_Value_t (*Write_Byte)(uint8_t);      //I2Cд�ֽ�
	uint8_t     (*Read_Byte) (ACK_Value_t);  //I2C���ֽ�
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
