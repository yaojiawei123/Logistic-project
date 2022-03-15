/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Start_up(void);
static void MQTT_Connect_Server(void);
static void Transfer_Data(void);
static void Receive_Information(void);
static void DMA_Receive_Set(void);
static void Error(void);
static void Transfer_Warning_Data(void);
uint8_t Get_CSQ(void);
//�ڲ����������ڱ��ļ��е��ã���������MQTT������


static void Moudle_Sync_Baud(void); //ͬ��������
static void Moudle_Check_SIM_Status(void); //���SIM��״̬
static void Moudle_Check_Network_Register(void); //�������ע��
static void Moudle_Check_Attach_GPRS_Service(void); //���ճ��GPRS����
static void Moudle_Check_QIACT(void);//��������
/****---------------����ʵ�ʲ��ԣ����Բ�������APN��ֱ�Ӽ������缴������-----------------------**************/
static void Moudle_Check_APN(void);//���ý���� �й��ƶ�ΪCMNET
/*******************------------------------------------------------------------------------**********/
static void Moudle_Check_MQTT_Server(void);//����MQTT������
static void Moudle_Check_MQTT_Client_ID(void);//����mqtt�ͻ���ID
static void Moudle_Sub_MQTT_Theme(void);//mqtt���ⶩ��

static void Moudle_Check_IMEI(void); //��ȡģ���IMEI��

static uint8_t EC20_Connect_Error_Flag = FALSE; //EC20���������־λ
/* Public variables-----------------------------------------------------------*/
EC20_t EC20 =
{
  FALSE,
	TIMER6_10S,
	0,
	0,
	
	Start_up,
	MQTT_Connect_Server,
	Transfer_Data,
	Receive_Information,
	DMA_Receive_Set,
	Error,
	Transfer_Warning_Data,
	Get_CSQ
};

EC20_Data_t EC20_Data =
{
  0,
	0,
	0
};
/* Private function prototypes------------------------------------------------*/      


/*
	* @name   SendAT
	* @brief  ����ATָ��
	* @param  AT_Command  -> �����͵�ATָ��
	          Respond_Str -> ��Ӧ�����а������ַ���
	* @retval None      
*/
void Send_AT(uint8_t * AT_Command,uint8_t * Respond_Str)
{
	uint8_t* const  Ptr_AT_Command   = AT_Command;
	uint8_t* const  Ptr_Respond_Str  = Respond_Str;

	//DMA���½�������	
	EC20.DMA_Receive_Set();
	
	//����ATָ��
	My_Usart2.SendString(Ptr_AT_Command);
	
	//��ӡ���͵�ATָ��
	printf("-->%s\r\n",Ptr_AT_Command);
	
	//�ȴ�ģ���Ӧ���ݣ���ʱ������
	Timer6.usDelay_Timer = 0;
	while(strstr((const char *)My_Usart2.pucRec_Buffer,"\r\n") == NULL)
	{
	  if(Timer6.usDelay_Timer > TIMER6_100mS)
		{
		  EC20.Error();
			break;
		}
	}
	//��ʱ10ms,������ȫ���ֽ�
	HAL_Delay(10);
	
	//ģ���Ӧ���ݴ���
	if(strstr((const char *)My_Usart2.pucRec_Buffer,(const char *)Ptr_Respond_Str) == NULL)
	{
	  EC20.Error();
	}
	//��ӡ4Gģ���Ӧ������
	printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
}


/*
	* @name   Moudle_Sync_Baud
	* @brief  ͬ��������
	* @param  None 
	* @retval None      
*/
static void Moudle_Sync_Baud(void)
{
  EC20_Connect_Error_Flag = FALSE; 
	Timer6.usDelay_Timer = 0;
	do{
	  //DMA��������
		EC20.DMA_Receive_Set();
		//����ATָ��
		My_Usart2.SendString(AT_CMD_AT);
		//��ӡ��Ϣ
		printf("-->%s\r\n",AT_CMD_AT);
	  //��ʱ100ms���ȴ��������
		HAL_Delay(100);
		//��ӡ��Ϣ
		printf("<--%s\r\n",My_Usart2.pucRec_Buffer);		
		//��ʱ����
		if(Timer6.usDelay_Timer >= TIMER6_10S)
		{
			EC20.Error();
			break;
		}		
	}while(strstr((const char *)My_Usart2.pucRec_Buffer,"OK") == NULL);
}

/*
	* @name   Moudle_Check_SIM_Status
	* @brief  ���SIM��״̬
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_SIM_Status(void)
{
	 if(EC20_Connect_Error_Flag == FALSE)
	 {
	  Timer6.usDelay_Timer = 0;
		do{
		   //DMA���½������� 
			 EC20.DMA_Receive_Set();
			 //����ATָ��
			 My_Usart2.SendString(AT_CMD_CPIN);
			 //��ʱ1000ms���ȴ��������
			 HAL_Delay(1000);			 
			 //��ӡ��Ϣ
			 printf("<--%s\r\n",My_Usart2.pucRec_Buffer);			
			 //��ʱ����
			 if(Timer6.usDelay_Timer >= TIMER6_10S)
			 {
				 EC20.Error();
				 break;
			 }			
		}while(strstr((const char*)My_Usart2.pucRec_Buffer,"OK") == NULL);
	 }


}

/*
	* @name   Moudle_Check_Network_Register
	* @brief  �������ע��
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_Network_Register(void)
{
	if(EC20_Connect_Error_Flag == FALSE)
	{
		Timer6.usDelay_Timer = 0;
		while(1)
		{
			//DMA���½�������
			EC20.DMA_Receive_Set();
			//����ATָ��
			My_Usart2.SendString(AT_CMD_CREG);
			//��ӡ��Ϣ
			printf("%s",AT_CMD_CREG);
			//��ʱ100ms���ȴ��������
			HAL_Delay(100);		
			//��ӡ��Ϣ
			printf("%s",My_Usart2.pucRec_Buffer);
			//�ж�ע��
			if((strstr((const char*)My_Usart2.pucRec_Buffer,",1") != NULL) || (strstr((const char*)My_Usart2.pucRec_Buffer,",5") != NULL))
			{
				break;
			}
			else
			{
				HAL_Delay(1000);
				HAL_Delay(1000);
			}
			
			//��ʱ����
			if(Timer6.usDelay_Timer >= TIMER6_2min)
			{
				EC20.Error();
				break;
			}
		}
	}
}

/*
	* @name   Moudle_Check_Attach_GPRS_Service
	* @brief  ������������
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_Attach_GPRS_Service(void)
{
	if(EC20_Connect_Error_Flag == FALSE)
	{
		Timer6.usDelay_Timer = 0;
		while(1)
		{
			//����ź�ǿ��
			Send_AT(AT_CMD_CSQ,(uint8_t*)"OK");
			//DMA���½�������
			EC20.DMA_Receive_Set();
			//����ATָ��
			My_Usart2.SendString(AT_CMD_CGREG);
			//��ӡ��Ϣ
			printf("-->%s",AT_CMD_CGREG);
			//��ʱ100ms���ȴ��������
			HAL_Delay(100);		
			//��ӡ��Ϣ
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//�ж�ע��
			if((strstr((const char*)My_Usart2.pucRec_Buffer,"+CGREG: 0,5") != NULL) || (strstr((const char*)My_Usart2.pucRec_Buffer,"+CGREG: 0,1") != NULL))
			{
				break;
			}
			else
			{
				HAL_Delay(1000);
				HAL_Delay(1000);
			}
			
			//��ʱ����
			if(Timer6.usDelay_Timer >= TIMER6_2min)
			{
				EC20.Error();
				break;
			}
		}
	}
}


/*
	* @name   Moudle_Check_IMEI
	* @brief  ��ȡ4Gģ��IMEI
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_IMEI(void)
{
  if(EC20_Connect_Error_Flag == FALSE)
	{
	  Timer6.usDelay_Timer = 0;
		while(1)
		{
			//DMA���½�������
			EC20.DMA_Receive_Set();		
			//����ATָ��
			My_Usart2.SendString(AT_CMD_CGSN);
			//��ӡ��Ϣ
			printf("-->%s",AT_CMD_CGSN);
			//��ʱ100ms���ȴ��������
			HAL_Delay(100);		
			//��ӡ��Ϣ��IMEI��Ϣ
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);	
			//
      if(strlen((const char *)My_Usart2.pucRec_Buffer) > 15)
			{
			  //IMEI����
			  strncpy(EC20_Data.IMEI_Buf,(const char *)(My_Usart2.pucRec_Buffer+2),15);
			  //�鿴IMEI�����Ƿ���ȷ
				printf("EC20_Data.IMEI_Buf:%s\r\n",EC20_Data.IMEI_Buf);
        break;  				
			}				
 			//��ʱ����
			if(Timer6.usDelay_Timer >= TIMER6_2min)
			{
				EC20.Error();
				break;
			}    			
		}
	}
}

/*
	* @name   Moudle_Check_QIACT
	* @brief  ��������
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_QIACT(void)
{
	if(EC20_Connect_Error_Flag == FALSE)
	{
		Timer6.usDelay_Timer = 0;
		do{
		  //��������DMA����
			EC20.DMA_Receive_Set();
			//����ATָ��
			My_Usart2.SendString(AT_CMD_QIACT);
			//��ӡ���͵�ָ����Ϣ
			printf("-->%s",AT_CMD_QIACT);
			//��ʱ1000ms�ȴ�ģ�鷵������
			HAL_Delay(1000);
			//��ӡģ�鷵�ص�����
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//�ȴ���ʱ�ж�
			if(Timer6.usDelay_Timer >= TIMER6_2S)
			{
			  EC20.Error();
				break;
			}
		}while(strstr((const char *)My_Usart2.pucRec_Buffer,"OK") == NULL);
	}
}


/*
	* @name   Moudle_Check_APN
	* @brief  ����APN
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_APN(void)
{
  if(EC20_Connect_Error_Flag == FALSE)
	{
	  Timer6.usDelay_Timer = 0;
		while(1)
		{
		  //DMA���½�������
			EC20.DMA_Receive_Set();
			//����ATָ��
			My_Usart2.SendString(AT_CMD_QICSGP);
			//��ʱ100ms���ȴ��������
			HAL_Delay(100);
			//��ӡ������Ϣ
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//APN���óɹ�
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"OK") != NULL)
			{
			  break;
			}
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"ERROR") != NULL)
			{
			  EC20.Error();
				break;
			}
			//��ʱ����
			if(Timer6.usDelay_Timer >= TIMER6_5S)
			{
				EC20.Error();
				break;
			}			
		}
	}
}

/*
	* @name   Moudle_Check_MQTT_Server
	* @brief  ����MQTT������
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_MQTT_Server(void)
{
  if(EC20_Connect_Error_Flag == FALSE)
	{
    printf("Connect MQTT Server!\r\n");	  
    Timer6.usDelay_Timer = 0;
		while(1)
		{
		  EC20.DMA_Receive_Set();
			My_Usart2.SendString(AT_CMD_QMTOPEN);
			printf("-->%s\r\n",AT_CMD_QMTOPEN);
			while(Timer6.usDelay_Timer < TIMER6_5S)
			{
				if(strstr((const char*)My_Usart2.pucRec_Buffer,"+QMTOPEN: 0,0") != NULL|strstr((const char*)My_Usart2.pucRec_Buffer,"+QMTOPEN: 0,-1") != NULL)
				{
				  break;
				}
			}
			HAL_Delay(100);
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//MQTT����������ʧ��
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"+QMTOPEN: 0,-1") != NULL)
			{
				printf("Connect TCP Server Failure!\r\n");

				EC20.MQTT_Reconnect_Timer = 0;
				EC20.Error();
				break;
			}
			
			//MQTT���������ӳɹ�
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"+QMTOPEN: 0,0") != NULL)
			{
				printf("Connect TCP Server Success!\r\n");
				EC20.MQTT_Reconnect_Timer = 0;
				break;
			}		
			
			//��ʱ����
			if(Timer6.usDelay_Timer >= TIMER6_2min)
			{
				EC20.MQTT_Reconnect_Timer = 0;
				EC20.Error();
				break;
			}			
			
		}	
	}
}

/*
	* @name   Moudle_Check_MQTT_Client_ID
	* @brief  ����MQTT�ͻ���ID
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_MQTT_Client_ID(void)
{
	char MQTT_Client_ID[30] = {0};
	
	sprintf(MQTT_Client_ID,"AT+QMTCONN=0,\"%s\"\r\n",EC20_Data.IMEI_Buf);
	
  if(EC20_Connect_Error_Flag == FALSE)
  {
    Timer6.usDelay_Timer = 0;
    while(1)
		{
			//DMA�������½���
		  EC20.DMA_Receive_Set();
			/*�Զ���MQTT�������ͻ���ID��ʹ�����³���*/
//			My_Usart2.SendString(AT_CMD_QMTCONN);
//			printf("-->%s\r\n",AT_CMD_QMTCONN);
//			HAL_Delay(100);
			/************************************/
			/*MQTT�������ͻ���IDΪ4Gģ���IMEI*/
			//����ATָ��
			My_Usart2.SendString((uint8_t *)MQTT_Client_ID);
			//��ӡ���͵�����
			printf("-->%s\r\n",MQTT_Client_ID);
			//��ʱ�ȴ�ģ�鷵������
			HAL_Delay(100);			
			/***********************************/
			//��ӡģ�鷵�ص�����
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//���óɹ�
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTCONN: 0,0,0") != NULL | strstr((const char *)My_Usart2.pucRec_Buffer,"OK") != NULL)
			{
			  break;
			}
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"ERROR") != NULL)
			{
				EC20.Error();
			  break;
			}
			//��ʱ����
			if(Timer6.usDelay_Timer >= TIMER6_5S)
			{
				EC20.Error();
				break;
			}			
		}
	} 	
}

/*
	* @name   Moudle_Sub_MQTT_Theme
	* @brief  MQTT���ⶩ��
	* @param  None 
	* @retval None      
*/
static void Moudle_Sub_MQTT_Theme(void)
{
  if(EC20_Connect_Error_Flag == FALSE)
  {
    Timer6.usDelay_Timer = 0;
    while(1)
		{
		  EC20.DMA_Receive_Set();
			HAL_Delay(1000);
			My_Usart2.SendString(AT_CMD_QMTSUB);
			HAL_Delay(1000);			
			printf("-->%s\r\n",AT_CMD_QMTSUB);
			HAL_Delay(1000);
			HAL_Delay(1000);			
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//���ĳɹ�
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTSUB: 0,1,0,0") != NULL || strstr((const char *)My_Usart2.pucRec_Buffer,"OK") != NULL)
			{
				EC20.MQTT_Connect_Status = TRUE;
				printf("MQTT Sub Success!\r\n");
			  break;
			}
			//����ʧ��
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"ERROR") != NULL)
			{
				EC20.Error();
			  break;
			}
			//��ʱ����
			if(Timer6.usDelay_Timer >= TIMER6_5S)
			{
				EC20.Error();
				break;
			}			
		}
	} 	
}

/*
	* @name   Get_CSQ
	* @brief  4Gģ���ȡ�����ź�
  * @param  +CSQ: ##,**   ##:�ź����� ��Χ10~31  **�������� ��Χ0~99 
  * @example  +CSQ: 22,99
  * @retval  Ret  0��4G���ź� 1�����ź�      
*/
uint8_t Get_CSQ(void)
{
  uint8_t Ret = 0;     // ����ֵ
  char *CSQ_String = NULL;
	uint8_t CSQ = 0;	

  __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);   //��ֹ����2�Ŀ����ж�
	
	EC20.Start_up();     //ģ�鿪��
	Moudle_Sync_Baud();  //��������Ӧ
	Send_AT(AT_CMD_ECHO_OFF,(uint8_t *)"OK");  //�رջ���
	
	EC20.DMA_Receive_Set();
	HAL_Delay(1000);
	My_Usart2.SendString(AT_CMD_CSQ);  //����ATָ���ȡ�ź�����
	HAL_Delay(1000);			             //��ʱ
	printf("-->%s\r\n",AT_CMD_CSQ);	   //��ӡ���͵�ATָ��
	HAL_Delay(1000);	                
  printf("<--%s\r\n",My_Usart2.pucRec_Buffer);	//��ӡ���ڷ��ص�������Ϣ
	if(strstr((const char *)My_Usart2.pucRec_Buffer,"+CSQ")) //���ڷ���+CSQ����ʼ��������
	{
	  CSQ_String =  StrBetwString((char *)My_Usart2.pucRec_Buffer,"+CSQ: ",","); //��ȡ�ź�����
		CSQ = atoi(CSQ_String);
		printf("CSQ_String:%s  CSQ:%d\r\n",CSQ_String,CSQ);
	}
	/*�ж��ź�ֵ�Ƿ�����Ч��Χ��*/
	if((CSQ >= 10) && (CSQ <= 31))
	{
	  Ret = 1;
		EC20.Instore_Data = 0;
	}
	else
	{
		Ret = 0;
		EC20.Instore_Data = 1;		
	}
	return Ret;
}

/*
	* @name   Start_up
	* @brief  ģ�鿪��
	* @param  None
	* @retval None      
*/
static void Start_up(void)
{

}

/*
	* @name   MQTT_Connect_Server
	* @brief  ����MQTT������
	* @param  None
	* @retval None      
*/
static void MQTT_Connect_Server(void)
{
  __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);   //��ֹ����2�Ŀ����ж�
	
//	EC20.Start_up();     //ģ�鿪��
//	Moudle_Sync_Baud();  //��������Ӧ
//	Send_AT(AT_CMD_ECHO_OFF,(uint8_t *)"OK");  //�رջ���
	Moudle_Check_SIM_Status();                //���SIM��״̬
	Moudle_Check_IMEI(); //��ȡ4Gģ���IMEI
	Moudle_Check_Network_Register(); //�ж��Ƿ�פ��
	Moudle_Check_Attach_GPRS_Service(); //�ж��Ƿ���������
	Moudle_Check_QIACT(); //��������
	Moudle_Check_MQTT_Server(); //����MQTT������
	Moudle_Check_MQTT_Client_ID();//����MQTT�ͻ���
	Moudle_Sub_MQTT_Theme(); //MQTT��������
//	Send_AT(AT_CMD_QMTSUB,(uint8_t *)"ERROR");
	EC20.DMA_Receive_Set();  //DMA�������½���

	if(EC20.MQTT_Connect_Status == TRUE)
	{
		__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);   //ʹ�ܴ���2�Ŀ����ж�
	}
}


/*
	* @name   Transfer_Data
  * @brief  ��MQTT��������������
	* @param  None
	* @retval None      
*/
static void Transfer_Data(void)
{
  sprintf(EC20_Data.Tx_MQTT_buf,"Lux:%.1f,Temperature:%.1f,Hum:%d,lon:%.2f,lat:%.1f",BH1750.Lux,SHT31.fTemperature,SHT31.ucHumidity,GPS_Decode.lon/100,GPS_Decode.lat/100);
	sprintf(EC20_Data.Tx_MQTT_Pub,"AT+QMTPUBEX=0,0,0,0,\"gps/msg\",%d\r\n",strlen(EC20_Data.Tx_MQTT_buf));
	
	Send_AT((uint8_t*)EC20_Data.Tx_MQTT_Pub,(uint8_t *)">");
	HAL_Delay(3);
	My_Usart2.SendString((uint8_t *)EC20_Data.Tx_MQTT_buf);
	HAL_Delay(3);
	//������ݷ��ͳɹ�֮���Ƿ�ظ�����Ϣ
	if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTPUBEX: 0,0,0"))
	{
	  printf("publish ok\r\n");
	}
}


/*
	* @name   Transfer_Warning_Data
  * @brief  ��MQTT��������������
	* @param  ���µ��¶������ϱ�
	* @retval None      
*/
static void Transfer_Warning_Data(void)
{
	memset(EC20_Data.Tx_MQTT_buf,0,sizeof(EC20_Data.Tx_MQTT_buf));
	memset(EC20_Data.Tx_MQTT_Pub,0,sizeof(EC20_Data.Tx_MQTT_Pub));

  sprintf(EC20_Data.Tx_MQTT_buf,"Temperature:%.1f",SHT31.fTemperature);
	sprintf(EC20_Data.Tx_MQTT_Pub,"AT+QMTPUBEX=0,0,0,0,\"gps/msg\",%d\r\n",strlen(EC20_Data.Tx_MQTT_buf));	
	
	Send_AT((uint8_t*)EC20_Data.Tx_MQTT_Pub,(uint8_t *)">");
	HAL_Delay(3);
	My_Usart2.SendString((uint8_t *)EC20_Data.Tx_MQTT_buf);
	HAL_Delay(3);
	//������ݷ��ͳɹ�֮���Ƿ�ظ�����Ϣ
	if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTPUBEX: 0,0,0"))
	{
	  printf("publish ok\r\n");
	}
}

/*
	* @name   Receive_Information
	* @brief  ����MQTT����������Ϣ
	* @param  None
	* @retval None      
*/
static void Receive_Information(void)
{
	char *str1 = NULL,*str2 = NULL;
	
/*Ps
**�������·�����µ�����ֵ������ָ���ʽ
	 {"id":123123,"TH":50,"TL":10}
	"id"���Ʋ��ܸı�  ������������4G��IMEI��ʾ  "TH"��ʾ������ֵ "TL"��ʾ������ֵ
	*/	
  if(EC20.MQTT_Connect_Status == TRUE) //EG25������MQTT������
	{
		//�������·�����/��������ָ��
		if(strstr((const char *)My_Usart2.pucRec_Buffer,"id")) 
		{
			printf("%s\r\n",My_Usart2.pucRec_Buffer);
			str2 = StrBetwString((char *)My_Usart2.pucRec_Buffer,"\"TL\":","}\"");
			if(str2!=NULL)
			{
				Temperature_Limit.TL = atoi(str2);	
        printf("Temperature_Limit.TL=%d\r\n",Temperature_Limit.TL);					
			}
			cStringRestore();
			str1 = StrBetwString((char *)My_Usart2.pucRec_Buffer,"\"TH\":",",\"TL\"");
			if(str1 != NULL)
			{
        Temperature_Limit.TH = atoi(str1);	
        printf("Temperature_Limit.TH=%d\r\n",Temperature_Limit.TH);						
			}

//		  split((char *)My_Usart2.pucRec_Buffer,",",Instuction_Buf1,&Instuction_Buf1_Num);
//			printf("%s\r\n %s\r\n",Instuction_Buf1[4],Instuction_Buf1[5]);  //Instuction_Buf1[4] "TH":50  Instuction_Buf1[5] "TL":0}" ��ӡ���������Ϣ
//      str2 = StrBetwString((char *)Instuction_Buf1[5],"\"TL\":","}\"");
//		  if(str2 != NULL)
//			{
//				Temperature_Limit.TL = atoi(str2);	
//        printf("Temperature_Limit.TL=%d\r\n",Temperature_Limit.TL);				
//			}

		}
		
		//����ͻ��˱�������T�ߣ�����+QMTSTAT: 0,1,�����λģ��
    if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTSTAT: 0,1"))
		{
		  printf("Restart Module!!!\r\n");
		  Public.Reset_MCU();
		}
		//�������·�ָ��豸����˯��ģʽ
		if(strstr((const char *)My_Usart2.pucRec_Buffer,"mode:sleep"))
		{
			printf("enter sleep!!!\r\n");
			LowPower.Enter_Sleep_Mode_Flag = TRUE;  //�豸����˯��ģʽ��FLAG
		}
		
		
		
	 EC20.DMA_Receive_Set();
	}
}

/*
	* @name 	DMA_Receive_Set
	* @brief	DMA��������
	* @param	None 
	* @retval None			
*/
static void DMA_Receive_Set(void)
{
	//����2��ֹDMA����
	HAL_UART_DMAStop(&huart2);
	//�建��
	Public.Memory_Clr(My_Usart2.pucRec_Buffer,strlen((const char*)My_Usart2.pucRec_Buffer));		
	//����2����DMA����
	HAL_UART_Receive_DMA(&huart2,My_Usart2.pucRec_Buffer,Usart2_Rec_LENGTH);
}

/*
	* @name 	Error
	* @brief	������Ϣ
	* @param	None 
	* @retval None			
*/
static void Error(void)
{
  EC20_Connect_Error_Flag = TRUE;
	EC20.MQTT_Reconnect_Timer = 0;
}
/********************************************************
  End Of File
********************************************************/
