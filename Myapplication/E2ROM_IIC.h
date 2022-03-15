#ifndef __E2ROM_IIC_H__
#define __E2ROM_IIC_H__

#include "MyApplication.h"




/*********************IIC_E2PROM�ṹ���ʼ��****************************/

typedef struct
{
  void (*Init)(void); 
	void (*Start)(void);                        //����IIC��ʼ�ź�
	void (*Stop)(void);                         //����IICֹͣ�ź�
	ACK_Value_t (*Write_Byte)(uint8_t);      //I2Cд�ֽ�
	uint8_t     (*Read_Byte) (ACK_Value_t);  //I2C���ֽ�
}IIC_E2PROM_t;


/* extern  variables-----------------------------------------------------------*/
extern  IIC_E2PROM_t  IIC_E2PROM;
#endif
/********************************************************
  End Of File
********************************************************/
