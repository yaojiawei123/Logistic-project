#ifndef __GPS_H_
#define __GPS_H_

#include "MyApplication.h"

//定义数组长度
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 


typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//是否获取到GPS数据
	char isParseData;	//是否解析完成
	char UTCTime[UTCTime_Length];		//UTC时间
	char latitude[latitude_Length];		//纬度
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//经度
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//定位信息是否有效
} _SaveData;


typedef struct
{
  void     (*CLR_Buf)(void);
  uint8_t  (*Handle)(char *);
	void     (*clrStruct)(void);
}GPS_hanle_t;

typedef struct
{
  double lon;
	double lat;
	
	void (*parseGpsBuffer)(void);
	void (*printGpsBuffer)(void);
  void (*Show_lon_lat)(void);
	void (*errorLog)(int );
}GPS_Decode_t;

/* extern variables-----------------------------------------------------------*/
extern GPS_hanle_t GPS_hanle;
extern _SaveData Save_Data;
extern GPS_Decode_t GPS_Decode;
#endif
/********************************************************
  End Of File
********************************************************/
