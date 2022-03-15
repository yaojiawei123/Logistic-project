#ifndef __SHT31_H_
#define __SHT31_H_

#include "MyApplication.h"

/* Public define-------------------------------------------------------------*/
#define SHT31_ADDR  (uint8_t)(0x44 << 1) //传感器地址

#define	Write_CMD   0xFE
#define	Read_CMD    0x01

//定义结构体类型
typedef struct
{
	float   fTemperature;  //温度 -40至125℃    精度0.1℃ 
	uint8_t ucHumidity;    //湿度 0%RH至100%RH  精度1%RH           
	
	void (*Measure_Period_Mode)(void);  //周期测量模式
}SHT31_t;
/* extern variables-----------------------------------------------------------*/
extern SHT31_t SHT31;


#endif
/********************************************************
  End Of File
********************************************************/
