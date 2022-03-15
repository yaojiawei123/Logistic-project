/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Zigbee_Receive_Information(void);
static void DMA_Receive_Set3(void);
static void Zigbee_MQTT_Pub(void);
/* Public variables-----------------------------------------------------------*/
Zigbee_t Zigbee = 
{
  Zigbee_Receive_Information,
	DMA_Receive_Set3,
	Zigbee_MQTT_Pub
};
/* Private function prototypes------------------------------------------------*/   
/*
	* @name   Zigbee_Receive_Information
	* @brief  ����Zigbee�ڵ���Ϣ
	* @param  None
	* @retval None      
*/
static void Zigbee_Receive_Information(void)
{
	if(EC20.MQTT_Connect_Status == TRUE)
	{
		/* ��ص�ѹ 4.0 ~ 4.2 1min�ϱ�һ��*/
		if(((ADC_BAT.BAT_Value>=4.0)&&(ADC_BAT.BAT_Value <=4.2))&& (Timer7.Sensor_Pub_Timer1 > TIMER7_1min))
		{
		  Timer7.Sensor_Pub_Timer1 = 0;
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xe735")) //�ж��Ƿ�ΪZigbee�ڵ��е��¶�ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);
				Zigbee.Zigbee_MQTT_Pub();
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x7d9e")) //�ж��Ƿ�ΪZigbee�ڵ��е���ʪ��ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x00ae")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xd20d")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}						
		}
		/* ��ص�ѹ 3.7 ~ 3.9 2min�ϱ�һ��*/
		if(((ADC_BAT.BAT_Value >= 3.7)&&(ADC_BAT.BAT_Value<=3.9)) && (Timer7.Sensor_Pub_Timer1 > TIMER7_2min))
		{
		  Timer7.Sensor_Pub_Timer1 = 0;
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xe735")) //�ж��Ƿ�ΪZigbee�ڵ��е��¶�ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);
				Zigbee.Zigbee_MQTT_Pub();
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x7d9e")) //�ж��Ƿ�ΪZigbee�ڵ��е���ʪ��ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x00ae")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xd20d")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}						
		}
		/* ��ص�ѹ 3.4 ~ 3.6 3min�ϱ�һ��*/
		if(((ADC_BAT.BAT_Value>=3.4)&&(ADC_BAT.BAT_Value<=3.6))&& (Timer7.Sensor_Pub_Timer1 > TIMER7_3min))
		{
		  Timer7.Sensor_Pub_Timer1 = 0;
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xe735")) //�ж��Ƿ�ΪZigbee�ڵ��е��¶�ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);
				Zigbee.Zigbee_MQTT_Pub();
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x7d9e")) //�ж��Ƿ�ΪZigbee�ڵ��е���ʪ��ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x00ae")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xd20d")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}						
		}	
		/* ��ص�ѹ 3.1 ~ 3.3 4min�ϱ�һ��*/
		if(((ADC_BAT.BAT_Value>=3.1)&&(ADC_BAT.BAT_Value<=3.3)) && (Timer7.Sensor_Pub_Timer1 > TIMER7_4min))
		{
		  Timer7.Sensor_Pub_Timer1 = 0;
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xe735")) //�ж��Ƿ�ΪZigbee�ڵ��е��¶�ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);
				Zigbee.Zigbee_MQTT_Pub();
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x7d9e")) //�ж��Ƿ�ΪZigbee�ڵ��е���ʪ��ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x00ae")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xd20d")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}						
		}	
		/* ��ص�ѹ 2.9 ~ 3.0 5min�ϱ�һ��*/
		if(((ADC_BAT.BAT_Value>=2.9)&&(ADC_BAT.BAT_Value<=3.0))&& (Timer7.Sensor_Pub_Timer1 > TIMER7_5min))
		{
		  Timer7.Sensor_Pub_Timer1 = 0;
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xe735")) //�ж��Ƿ�ΪZigbee�ڵ��е��¶�ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);
				Zigbee.Zigbee_MQTT_Pub();
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x7d9e")) //�ж��Ƿ�ΪZigbee�ڵ��е���ʪ��ģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0x00ae")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}
			if(strstr((const char*)My_Usart3.pucRec_Buffer3,"Node=0xd20d")) //�ж��Ƿ�ΪZigbee�ڵ��еĹ�ǿģ��
			{
				printf("%s",My_Usart3.pucRec_Buffer3);	
				Zigbee.Zigbee_MQTT_Pub();			
			}						
		}					
	}
	Zigbee.DMA_Receive_Set3();
}


/*
	* @name 	DMA_Receive_Set
	* @brief	DMA��������
	* @param	None 
	* @retval None			
*/
static void DMA_Receive_Set3(void)
{
	//����3��ֹDMA����
	HAL_UART_DMAStop(&huart3);
	//�建��
	Public.Memory_Clr(My_Usart3.pucRec_Buffer3,strlen((const char*)My_Usart3.pucRec_Buffer3));		
	//����2����DMA����
	HAL_UART_Receive_DMA(&huart3,My_Usart3.pucRec_Buffer3,Usart3_Rec_LENGTH);
}

/*
	* @name   Zigbee_MQTT_Pub
	* @brief  �����յ�Zigbee�ڵ���Ϣ����EMQƽ̨
	* @param  None
	* @retval None      
*/
static void Zigbee_MQTT_Pub(void)
{
  sprintf(EC20_Data.Tx_MQTT_Pub,"AT+QMTPUBEX=0,0,0,0,\"gps/msg\",%d\r\n",strlen((const char *)My_Usart3.pucRec_Buffer3));
	Send_AT((uint8_t*)EC20_Data.Tx_MQTT_Pub,(uint8_t *)">");
	HAL_Delay(3);
	My_Usart2.SendString((uint8_t *)My_Usart3.pucRec_Buffer3);
	HAL_Delay(3);
	//������ݷ��ͳɹ�֮���Ƿ�ظ�����Ϣ
	if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTPUBEX: 0,0,0"))
	{
	  printf("publish ok\r\n");
	}	
	
}

/********************************************************
  End Of File
********************************************************/
