#ifndef __Sensor_Process_H_
#define __Sensor_Process_H_

#include "MyApplication.h"

//extern UART_HandleTypeDef huart1;
/* Public define-------------------------------------------------------------*/



typedef struct
{
  uint8_t gps;
	uint8_t zigbee;
	uint8_t sht;
	uint8_t lux;
	uint8_t alt;
	uint16_t sbs;
}Sensor_Value_t;

typedef struct
{
  uint8_t EC20_Close_Start_Power; 
}EC20_Auto_Close_t;

typedef struct
{
  uint8_t TH;  //高温度阈值
	uint8_t TL;  //低温度阈值
}Temperature_Limit_t;

/* extern variables-----------------------------------------------------------*/
extern Sensor_Value_t Sensor_Value;
extern EC20_Auto_Close_t EC20_Auto_Close;
extern Temperature_Limit_t Temperature_Limit;
#endif
/********************************************************
  End Of File
********************************************************/
