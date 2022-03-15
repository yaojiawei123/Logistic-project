/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void CLR_Buf(void);
static uint8_t Hand(char *a);
static void clrStruct(void);

static void parseGpsBuffer(void);
static void printGpsBuffer(void);
static void Show_lon_lat(void);
static void errorLog(int num);
/* Public variables-----------------------------------------------------------*/
_SaveData Save_Data;

GPS_hanle_t GPS_hanle =
{
  CLR_Buf,
	Hand,
  clrStruct	
};

GPS_Decode_t GPS_Decode = 
{
  0.0,
	0.0,
	parseGpsBuffer,
	printGpsBuffer,
  Show_lon_lat,
	errorLog
};
/* Private function prototypes------------------------------------------------*/      
static void CLR_Buf(void)
{
  memset(My_Uart4.Uart4ReadBuff,0,Uart4ReadBuffLen);
	My_Uart4.Usart1ReadCnt = 0;
}

static uint8_t Hand(char *a)
{
  if(strstr((const char *)My_Uart4.Uart4ReadBuff,a)!=NULL)
	    return 1;
	else
		 return 0;
}

static void clrStruct(void)
{
  Save_Data.isGetData = FALSE;
	Save_Data.isParseData = FALSE;
	Save_Data.isUsefull = FALSE;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
}


//解析接收的BUFFER $GNRMC
static void parseGpsBuffer(void)
{
  char *subString;
	char *subStringNext;
	char i = 0;
	char usefullBuffer[2]; 

	if(Save_Data.isGetData)
	{
	  Save_Data.isGetData = FALSE;
		printf("********************************************\r\n");
		printf("Save_Data.GPS_Buffer:%s\r\n",Save_Data.GPS_Buffer);
	  for (i = 0 ; i <= 6 ; i++)
		{
		  if(i == 0)
			{
			  if((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
				{
					errorLog(1);	//解析错误
				}
			}
			else
			{
			  subString++;
			  if((subStringNext = strstr(subString, ",")) != NULL)
				{
				  switch(i)
					{
					  case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W
						default:break;	
					}
				   subString = subStringNext;
					 Save_Data.isParseData = TRUE;
					 if(usefullBuffer[0] == 'A')
						    Save_Data.isUsefull = TRUE;
					 else if(usefullBuffer[0] == 'V')
						   Save_Data.isUsefull = FALSE;
				 }
					else
					{
						errorLog(2);	//解析错误
					}
			   }
		   }
	}
}


//打印解析的数据 $GNRMC
static void printGpsBuffer(void)
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = FALSE;
//		printf("Save_Data.UTCTime =%s\r\n",Save_Data.UTCTime);
		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = FALSE;
			printf("Save_Data.latitude =%s\r\n",Save_Data.latitude);
			printf("Save_Data.N_S =%s\r\n",Save_Data.N_S);
			printf("Save_Data.longitude =%s\r\n",Save_Data.longitude);
			printf("Save_Data.E_W =%s\r\n",Save_Data.E_W);
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
	}
}



static void Show_lon_lat(void)
{
	GPS_Decode.lon = atof(Save_Data.longitude);
	GPS_Decode.lat = atof(Save_Data.latitude);
	
	printf("lon=%.2f\r\n",(GPS_Decode.lon)/100);
	printf("lat=%.2f\r\n",(GPS_Decode.lat)/100);
}


static void errorLog(int num)
{
	while (1)
	{
	  printf("ERROR%d\r\n",num);
	}
}
  

/********************************************************
  End Of File
********************************************************/
