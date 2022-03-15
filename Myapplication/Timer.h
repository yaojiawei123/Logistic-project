#ifndef __Timer_H_
#define __Timer_H_

#include "MyApplication.h"

/* Public define-------------------------------------------------------------*/

/**********************************定时器6****************************************************/
//定义枚举类型
typedef enum
{
	TIMER6_10mS  	= (uint16_t)2,
	TIMER6_50mS  	= (uint16_t)10,
	TIMER6_100mS	= (uint16_t)20,
	TIMER6_200mS	= (uint16_t)40,
	TIMER6_500mS	= (uint16_t)100,
	TIMER6_1S     = (uint16_t)200,
	TIMER6_2S     = (uint16_t)400,
	TIMER6_3S     = (uint16_t)600,
	TIMER6_5S     = (uint16_t)1000,
	TIMER6_7S     = (uint16_t)1400,
	TIMER6_10S    = (uint16_t)2000,
	TIMER6_2min   = (uint16_t)24000,
	TIMER6_3min   = (uint16_t)36000,
}TIMER6_Value_t;


//定义结构体类型
typedef struct
{
  uint16_t volatile usMCU_Run_Timer;  //系统运行定时器
  uint16_t volatile usDelay_Timer;    //延时定时器
	uint16_t volatile SHT31_Measure_Timeout;
	uint16_t volatile EG25_Transmit_Data_Timer;  //EG25上报数据定时器
	uint16_t volatile Sensors_No_Zigbee_Timer;
	uint16_t volatile	Battery_Bat_Timer;
	void (*Timer6_Start_IT)(void);      //定时器6以中断模式启动
}Timer6_t;

/**********************************定时器7****************************************************/
//定义枚举类型 基准定时50ms
typedef enum
{
  TIMER7_1s  = (uint32_t)20,
  TIMER7_1min = (uint32_t)1200,
  TIMER7_2min = (uint32_t)2400,	
	TIMER7_3min = (uint32_t)3600,
	TIMER7_4min = (uint32_t)4800,	
	TIMER7_5min = (uint32_t)6000,
	TIMER7_6min = (uint32_t)7200,
	TIMER7_7min = (uint32_t)8400,
	TIMER7_8min = (uint32_t)9600,		
	TIMER7_30min = (uint32_t)36000,
	TIMER7_1h   = (uint32_t)72000,
	TIMER7_24h  = (uint32_t)172800,
}TIMER7_Value_t;

//定义结构体类型
typedef struct
{
	uint32_t volatile Sensor_Pub_Timer;
	uint32_t volatile Sensor_Pub_Timer1;
	uint32_t volatile Sensor_Pub_Timer2;
	uint32_t volatile Sensor_Pub_Timer3;
	uint32_t volatile Sensor_Pub_Timer4;	
	void (*Timer7_Start_IT)(void);      //定时器6以中断模式启动
}Timer7_t;

/* extern variables-----------------------------------------------------------*/
extern Timer6_t Timer6;
extern Timer7_t Timer7;

#endif
/********************************************************
  End Of File
********************************************************/
