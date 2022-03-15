#ifndef __SHT31_H_
#define __SHT31_H_

#include "MyApplication.h"

/* Public define-------------------------------------------------------------*/
#define SHT31_ADDR  (uint8_t)(0x44 << 1) //��������ַ

#define	Write_CMD   0xFE
#define	Read_CMD    0x01

//����ṹ������
typedef struct
{
	float   fTemperature;  //�¶� -40��125��    ����0.1�� 
	uint8_t ucHumidity;    //ʪ�� 0%RH��100%RH  ����1%RH           
	
	void (*Measure_Period_Mode)(void);  //���ڲ���ģʽ
}SHT31_t;
/* extern variables-----------------------------------------------------------*/
extern SHT31_t SHT31;


#endif
/********************************************************
  End Of File
********************************************************/
