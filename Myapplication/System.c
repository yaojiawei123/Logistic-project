/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Run(void); 
static void Error_Handler(void);
static void Assert_Failed(void);
	
/* Public variables-----------------------------------------------------------*/
System_t System = 
{
	Run,
	Error_Handler,
	Assert_Failed
};

/* Private function prototypes------------------------------------------------*/      

/*
	* @name   Run
	* @brief  系统运行
	* @param  None
	* @retval None      
*/
static void Run()
{
  uint8_t i = 0;
	//喂狗，溢出时间26.208s,若程序运行超过26.082s没有喂狗，则系统自动复位
	if(MyIWDG.FeedDog_Flag == TRUE)
	{
	  MyIWDG.FeedDog();
	}
	
	//判断4G信号是否有效
	if(EC20.CSQ_Flag == 0)
	{
	  if(EC20.Get_CSQ() == 1)
		{
			EC20.CSQ_Flag = 1;
		}			
	}
	
	//在有4G信号且未连接服务器的情况下 连接MQTT服务器，连接成功之后不再执行该程序
	if((EC20.MQTT_Connect_Status == FALSE) && (EC20.CSQ_Flag == 1))
	{
			if(EC20.MQTT_Reconnect_Timer >= TIMER6_5S )
			{
				EC20.MQTT_Connect_Server();
			} 			
	}

	//电压的实时监测 暂时设定为10s检测一次电池电压  后期可以调整
	if((Timer6.Battery_Bat_Timer > TIMER6_10S) && (EC20.MQTT_Connect_Status == TRUE))
	{
	  Timer6.Battery_Bat_Timer = 0;
		//读取电池电压值
		ADC_BAT.Get_BAT_Voltage();
		//电池电压低于2.8v 自动进入休眠 并通过板载LED3常亮提示 系统处于休眠状态
		if((ADC_BAT.BAT_Value<=2.8)) 
		{
			printf("battery voltage lower!!!\r\n");
			HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_SET); //进入休眠状态 板载LED3亮
			LowPower.Enter_Sleep_Mode_Flag = TRUE;
		}		
		MyIWDG.FeedDog();
	}
	
	//触发警报设置
	//温度数据单独检测 暂定7s检测一次 温度超过阈值设置 触发警报
	if((Timer6.SHT31_Measure_Timeout > TIMER6_7S) && (EC20.MQTT_Connect_Status == TRUE))
	{
    Timer6.SHT31_Measure_Timeout = 0;
		SHT31.Measure_Period_Mode();   
    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
		if(SHT31.fTemperature >Temperature_Limit.TH)  //温度超过高温上限
		{ 
			printf("Temperature higher\r\n");
      EC20.Transfer_Warning_Data();		 
		}
		if(SHT31.fTemperature <Temperature_Limit.TL)  //温度过低
		{
			printf("Temperature lower\r\n");			
      EC20.Transfer_Warning_Data();		 
		}
	}

	//电池电压区间4.2v~4.0v 控制上报频率为1min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=4.0)&&(ADC_BAT.BAT_Value <=4.2)) && (Timer7.Sensor_Pub_Timer > TIMER7_1min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750采集*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS采集*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*上报数据*/		
    EC20.Transfer_Data();				
	}

	//电池电压区间3.9v~3.7v 控制上报频率为2min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value >= 3.7)&&(ADC_BAT.BAT_Value<=3.9)) && (Timer7.Sensor_Pub_Timer > TIMER7_2min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750采集*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS采集*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*上报数据*/		
    EC20.Transfer_Data();				
	}	

	//电池电压区间3.6v~3.4v控制上报频率为3min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=3.4)&&(ADC_BAT.BAT_Value<=3.6)) && (Timer7.Sensor_Pub_Timer > TIMER7_3min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750采集*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS采集*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*上报数据*/		
    EC20.Transfer_Data();				
	}		

	//电池电压区间3.3v~3.1v 控制上报频率为4min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=3.1)&&(ADC_BAT.BAT_Value<=3.3)) && (Timer7.Sensor_Pub_Timer > TIMER7_4min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750采集*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS采集*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*上报数据*/		
    EC20.Transfer_Data();				
	}		

	//电池电压区间3.0v~2.9v 控制上报频率为5min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=2.9)&&(ADC_BAT.BAT_Value<=3.0)) && (Timer7.Sensor_Pub_Timer > TIMER7_5min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750采集*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS采集*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*上报数据*/		
    EC20.Transfer_Data();				
	}	

  //进入休眠
	if(LowPower.Enter_Sleep_Mode_Flag == TRUE)
	{
		printf("sys enter sleep !!!\r\n");
		LowPower.Enter_Sleep_Mode_Flag = FALSE;
	  LowPower.Sleep_Mode();
	}
	
	//可直接通过串口输入 start up 系统软复位
	if(My_Usart1.USART1_IT_IDLE)
	{
		My_Usart1.USART1_IT_IDLE = 0;
    if(strstr((const char*)My_Usart1.Usart1ReadBuff,"start up"))
		{
			Public.Reset_MCU();
		}
		memset(My_Usart1.Usart1ReadBuff,0,sizeof(My_Usart1.Usart1ReadBuff));
	}
	
}

/*
	* @name   Error_Handler
	* @brief  系统错误处理
	* @param  None
	* @retval None      
*/
static void Error_Handler()
{
	/* User can add his own implementation to report the HAL error return state */
}

/*
	* @name   Assert_Failed
	* @brief  函数参数错误处理
	* @param  None
	* @retval None      
*/
static void Assert_Failed()
{
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	

}
/********************************************************
  End Of File
********************************************************/
