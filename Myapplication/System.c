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
	* @brief  ϵͳ����
	* @param  None
	* @retval None      
*/
static void Run()
{
  uint8_t i = 0;
	//ι�������ʱ��26.208s,���������г���26.082sû��ι������ϵͳ�Զ���λ
	if(MyIWDG.FeedDog_Flag == TRUE)
	{
	  MyIWDG.FeedDog();
	}
	
	//�ж�4G�ź��Ƿ���Ч
	if(EC20.CSQ_Flag == 0)
	{
	  if(EC20.Get_CSQ() == 1)
		{
			EC20.CSQ_Flag = 1;
		}			
	}
	
	//����4G�ź���δ���ӷ������������ ����MQTT�����������ӳɹ�֮����ִ�иó���
	if((EC20.MQTT_Connect_Status == FALSE) && (EC20.CSQ_Flag == 1))
	{
			if(EC20.MQTT_Reconnect_Timer >= TIMER6_5S )
			{
				EC20.MQTT_Connect_Server();
			} 			
	}

	//��ѹ��ʵʱ��� ��ʱ�趨Ϊ10s���һ�ε�ص�ѹ  ���ڿ��Ե���
	if((Timer6.Battery_Bat_Timer > TIMER6_10S) && (EC20.MQTT_Connect_Status == TRUE))
	{
	  Timer6.Battery_Bat_Timer = 0;
		//��ȡ��ص�ѹֵ
		ADC_BAT.Get_BAT_Voltage();
		//��ص�ѹ����2.8v �Զ��������� ��ͨ������LED3������ʾ ϵͳ��������״̬
		if((ADC_BAT.BAT_Value<=2.8)) 
		{
			printf("battery voltage lower!!!\r\n");
			HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_SET); //��������״̬ ����LED3��
			LowPower.Enter_Sleep_Mode_Flag = TRUE;
		}		
		MyIWDG.FeedDog();
	}
	
	//������������
	//�¶����ݵ������ �ݶ�7s���һ�� �¶ȳ�����ֵ���� ��������
	if((Timer6.SHT31_Measure_Timeout > TIMER6_7S) && (EC20.MQTT_Connect_Status == TRUE))
	{
    Timer6.SHT31_Measure_Timeout = 0;
		SHT31.Measure_Period_Mode();   
    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
		if(SHT31.fTemperature >Temperature_Limit.TH)  //�¶ȳ�����������
		{ 
			printf("Temperature higher\r\n");
      EC20.Transfer_Warning_Data();		 
		}
		if(SHT31.fTemperature <Temperature_Limit.TL)  //�¶ȹ���
		{
			printf("Temperature lower\r\n");			
      EC20.Transfer_Warning_Data();		 
		}
	}

	//��ص�ѹ����4.2v~4.0v �����ϱ�Ƶ��Ϊ1min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=4.0)&&(ADC_BAT.BAT_Value <=4.2)) && (Timer7.Sensor_Pub_Timer > TIMER7_1min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750�ɼ�*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS�ɼ�*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*�ϱ�����*/		
    EC20.Transfer_Data();				
	}

	//��ص�ѹ����3.9v~3.7v �����ϱ�Ƶ��Ϊ2min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value >= 3.7)&&(ADC_BAT.BAT_Value<=3.9)) && (Timer7.Sensor_Pub_Timer > TIMER7_2min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750�ɼ�*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS�ɼ�*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*�ϱ�����*/		
    EC20.Transfer_Data();				
	}	

	//��ص�ѹ����3.6v~3.4v�����ϱ�Ƶ��Ϊ3min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=3.4)&&(ADC_BAT.BAT_Value<=3.6)) && (Timer7.Sensor_Pub_Timer > TIMER7_3min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750�ɼ�*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS�ɼ�*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*�ϱ�����*/		
    EC20.Transfer_Data();				
	}		

	//��ص�ѹ����3.3v~3.1v �����ϱ�Ƶ��Ϊ4min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=3.1)&&(ADC_BAT.BAT_Value<=3.3)) && (Timer7.Sensor_Pub_Timer > TIMER7_4min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750�ɼ�*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS�ɼ�*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*�ϱ�����*/		
    EC20.Transfer_Data();				
	}		

	//��ص�ѹ����3.0v~2.9v �����ϱ�Ƶ��Ϊ5min
	if((EC20.MQTT_Connect_Status == TRUE) && ((ADC_BAT.BAT_Value>=2.9)&&(ADC_BAT.BAT_Value<=3.0)) && (Timer7.Sensor_Pub_Timer > TIMER7_5min))
	{
	  Timer7.Sensor_Pub_Timer = 0;
	  MyIWDG.FeedDog();		
//		SHT31.Measure_Period_Mode();   
//    printf("Temperature=%.1f Humidity=%dRH\r\n",SHT31.fTemperature,SHT31.ucHumidity);    
    /*BH1750�ɼ�*/		
    if(!I2C_BH1750.i2c_CheckDevice(BH1750_Addr))
	  {
	    BH1750.Lux = BH1750.Lux_Continus_Intensity();
			printf("BH1750.Lux:%.1f\r\n",BH1750.Lux);
	  }
		/*GPS�ɼ�*/
		GPS_Decode.parseGpsBuffer();
		GPS_Decode.printGpsBuffer();
		GPS_Decode.Show_lon_lat();		
    /*�ϱ�����*/		
    EC20.Transfer_Data();				
	}	

  //��������
	if(LowPower.Enter_Sleep_Mode_Flag == TRUE)
	{
		printf("sys enter sleep !!!\r\n");
		LowPower.Enter_Sleep_Mode_Flag = FALSE;
	  LowPower.Sleep_Mode();
	}
	
	//��ֱ��ͨ���������� start up ϵͳ��λ
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
	* @brief  ϵͳ������
	* @param  None
	* @retval None      
*/
static void Error_Handler()
{
	/* User can add his own implementation to report the HAL error return state */
}

/*
	* @name   Assert_Failed
	* @brief  ��������������
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
