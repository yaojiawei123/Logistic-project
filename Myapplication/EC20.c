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
//内部函数，仅在本文件中调用，用于连接MQTT服务器


static void Moudle_Sync_Baud(void); //同步波特率
static void Moudle_Check_SIM_Status(void); //检查SIM卡状态
static void Moudle_Check_Network_Register(void); //检查网络注册
static void Moudle_Check_Attach_GPRS_Service(void); //检查粘附GPRS网络
static void Moudle_Check_QIACT(void);//激活网络
/****---------------经过实际测试，可以不用设置APN，直接激活网络即可入网-----------------------**************/
static void Moudle_Check_APN(void);//设置接入点 中国移动为CMNET
/*******************------------------------------------------------------------------------**********/
static void Moudle_Check_MQTT_Server(void);//连接MQTT服务器
static void Moudle_Check_MQTT_Client_ID(void);//设置mqtt客户端ID
static void Moudle_Sub_MQTT_Theme(void);//mqtt主题订阅

static void Moudle_Check_IMEI(void); //获取模块的IMEI号

static uint8_t EC20_Connect_Error_Flag = FALSE; //EC20联网错误标志位
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
	* @brief  发送AT指令
	* @param  AT_Command  -> 待发送的AT指令
	          Respond_Str -> 回应数据中包含的字符串
	* @retval None      
*/
void Send_AT(uint8_t * AT_Command,uint8_t * Respond_Str)
{
	uint8_t* const  Ptr_AT_Command   = AT_Command;
	uint8_t* const  Ptr_Respond_Str  = Respond_Str;

	//DMA重新接收设置	
	EC20.DMA_Receive_Set();
	
	//发送AT指令
	My_Usart2.SendString(Ptr_AT_Command);
	
	//打印发送的AT指令
	printf("-->%s\r\n",Ptr_AT_Command);
	
	//等待模块回应数据，超时错误处理
	Timer6.usDelay_Timer = 0;
	while(strstr((const char *)My_Usart2.pucRec_Buffer,"\r\n") == NULL)
	{
	  if(Timer6.usDelay_Timer > TIMER6_100mS)
		{
		  EC20.Error();
			break;
		}
	}
	//延时10ms,接收完全部字节
	HAL_Delay(10);
	
	//模块回应数据处理
	if(strstr((const char *)My_Usart2.pucRec_Buffer,(const char *)Ptr_Respond_Str) == NULL)
	{
	  EC20.Error();
	}
	//打印4G模块回应的数据
	printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
}


/*
	* @name   Moudle_Sync_Baud
	* @brief  同步波特率
	* @param  None 
	* @retval None      
*/
static void Moudle_Sync_Baud(void)
{
  EC20_Connect_Error_Flag = FALSE; 
	Timer6.usDelay_Timer = 0;
	do{
	  //DMA重新设置
		EC20.DMA_Receive_Set();
		//发送AT指令
		My_Usart2.SendString(AT_CMD_AT);
		//打印信息
		printf("-->%s\r\n",AT_CMD_AT);
	  //延时100ms，等待接收完成
		HAL_Delay(100);
		//打印信息
		printf("<--%s\r\n",My_Usart2.pucRec_Buffer);		
		//超时处理
		if(Timer6.usDelay_Timer >= TIMER6_10S)
		{
			EC20.Error();
			break;
		}		
	}while(strstr((const char *)My_Usart2.pucRec_Buffer,"OK") == NULL);
}

/*
	* @name   Moudle_Check_SIM_Status
	* @brief  检查SIM卡状态
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_SIM_Status(void)
{
	 if(EC20_Connect_Error_Flag == FALSE)
	 {
	  Timer6.usDelay_Timer = 0;
		do{
		   //DMA重新接收设置 
			 EC20.DMA_Receive_Set();
			 //发送AT指令
			 My_Usart2.SendString(AT_CMD_CPIN);
			 //延时1000ms，等待接收完成
			 HAL_Delay(1000);			 
			 //打印信息
			 printf("<--%s\r\n",My_Usart2.pucRec_Buffer);			
			 //超时处理
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
	* @brief  检查网络注册
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
			//DMA重新接收设置
			EC20.DMA_Receive_Set();
			//发送AT指令
			My_Usart2.SendString(AT_CMD_CREG);
			//打印信息
			printf("%s",AT_CMD_CREG);
			//延时100ms，等待接收完成
			HAL_Delay(100);		
			//打印信息
			printf("%s",My_Usart2.pucRec_Buffer);
			//判断注册
			if((strstr((const char*)My_Usart2.pucRec_Buffer,",1") != NULL) || (strstr((const char*)My_Usart2.pucRec_Buffer,",5") != NULL))
			{
				break;
			}
			else
			{
				HAL_Delay(1000);
				HAL_Delay(1000);
			}
			
			//超时处理
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
	* @brief  建立无线链接
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
			//输出信号强度
			Send_AT(AT_CMD_CSQ,(uint8_t*)"OK");
			//DMA重新接收设置
			EC20.DMA_Receive_Set();
			//发送AT指令
			My_Usart2.SendString(AT_CMD_CGREG);
			//打印信息
			printf("-->%s",AT_CMD_CGREG);
			//延时100ms，等待接收完成
			HAL_Delay(100);		
			//打印信息
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//判断注册
			if((strstr((const char*)My_Usart2.pucRec_Buffer,"+CGREG: 0,5") != NULL) || (strstr((const char*)My_Usart2.pucRec_Buffer,"+CGREG: 0,1") != NULL))
			{
				break;
			}
			else
			{
				HAL_Delay(1000);
				HAL_Delay(1000);
			}
			
			//超时处理
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
	* @brief  获取4G模块IMEI
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
			//DMA重新接收设置
			EC20.DMA_Receive_Set();		
			//发送AT指令
			My_Usart2.SendString(AT_CMD_CGSN);
			//打印信息
			printf("-->%s",AT_CMD_CGSN);
			//延时100ms，等待接收完成
			HAL_Delay(100);		
			//打印信息出IMEI信息
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);	
			//
      if(strlen((const char *)My_Usart2.pucRec_Buffer) > 15)
			{
			  //IMEI复制
			  strncpy(EC20_Data.IMEI_Buf,(const char *)(My_Usart2.pucRec_Buffer+2),15);
			  //查看IMEI复制是否正确
				printf("EC20_Data.IMEI_Buf:%s\r\n",EC20_Data.IMEI_Buf);
        break;  				
			}				
 			//超时处理
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
	* @brief  激活网络
	* @param  None 
	* @retval None      
*/
static void Moudle_Check_QIACT(void)
{
	if(EC20_Connect_Error_Flag == FALSE)
	{
		Timer6.usDelay_Timer = 0;
		do{
		  //重新设置DMA接收
			EC20.DMA_Receive_Set();
			//发送AT指令
			My_Usart2.SendString(AT_CMD_QIACT);
			//打印发送的指令信息
			printf("-->%s",AT_CMD_QIACT);
			//延时1000ms等待模块返回数据
			HAL_Delay(1000);
			//打印模块返回的数据
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//等待超时判断
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
	* @brief  设置APN
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
		  //DMA重新接收设置
			EC20.DMA_Receive_Set();
			//发送AT指令
			My_Usart2.SendString(AT_CMD_QICSGP);
			//延时100ms，等待接收完成
			HAL_Delay(100);
			//打印接收信息
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//APN设置成功
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"OK") != NULL)
			{
			  break;
			}
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"ERROR") != NULL)
			{
			  EC20.Error();
				break;
			}
			//超时处理
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
	* @brief  连接MQTT服务器
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
			//MQTT服务器链接失败
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"+QMTOPEN: 0,-1") != NULL)
			{
				printf("Connect TCP Server Failure!\r\n");

				EC20.MQTT_Reconnect_Timer = 0;
				EC20.Error();
				break;
			}
			
			//MQTT服务器连接成功
			if(strstr((const char*)My_Usart2.pucRec_Buffer,"+QMTOPEN: 0,0") != NULL)
			{
				printf("Connect TCP Server Success!\r\n");
				EC20.MQTT_Reconnect_Timer = 0;
				break;
			}		
			
			//超时处理
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
	* @brief  配置MQTT客户端ID
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
			//DMA设置重新接收
		  EC20.DMA_Receive_Set();
			/*自定义MQTT服务器客户端ID请使用以下程序*/
//			My_Usart2.SendString(AT_CMD_QMTCONN);
//			printf("-->%s\r\n",AT_CMD_QMTCONN);
//			HAL_Delay(100);
			/************************************/
			/*MQTT服务器客户端ID为4G模块的IMEI*/
			//发送AT指令
			My_Usart2.SendString((uint8_t *)MQTT_Client_ID);
			//打印发送的内容
			printf("-->%s\r\n",MQTT_Client_ID);
			//延时等待模块返回数据
			HAL_Delay(100);			
			/***********************************/
			//打印模块返回的数据
			printf("<--%s\r\n",My_Usart2.pucRec_Buffer);
			//配置成功
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTCONN: 0,0,0") != NULL | strstr((const char *)My_Usart2.pucRec_Buffer,"OK") != NULL)
			{
			  break;
			}
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"ERROR") != NULL)
			{
				EC20.Error();
			  break;
			}
			//超时处理
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
	* @brief  MQTT主题订阅
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
			//订阅成功
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTSUB: 0,1,0,0") != NULL || strstr((const char *)My_Usart2.pucRec_Buffer,"OK") != NULL)
			{
				EC20.MQTT_Connect_Status = TRUE;
				printf("MQTT Sub Success!\r\n");
			  break;
			}
			//订阅失败
			if(strstr((const char *)My_Usart2.pucRec_Buffer,"ERROR") != NULL)
			{
				EC20.Error();
			  break;
			}
			//超时处理
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
	* @brief  4G模块获取当地信号
  * @param  +CSQ: ##,**   ##:信号质量 范围10~31  **：误码率 范围0~99 
  * @example  +CSQ: 22,99
  * @retval  Ret  0：4G无信号 1：有信号      
*/
uint8_t Get_CSQ(void)
{
  uint8_t Ret = 0;     // 返回值
  char *CSQ_String = NULL;
	uint8_t CSQ = 0;	

  __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);   //禁止串口2的空闲中断
	
	EC20.Start_up();     //模块开机
	Moudle_Sync_Baud();  //波特率适应
	Send_AT(AT_CMD_ECHO_OFF,(uint8_t *)"OK");  //关闭回显
	
	EC20.DMA_Receive_Set();
	HAL_Delay(1000);
	My_Usart2.SendString(AT_CMD_CSQ);  //发送AT指令获取信号质量
	HAL_Delay(1000);			             //延时
	printf("-->%s\r\n",AT_CMD_CSQ);	   //打印发送的AT指令
	HAL_Delay(1000);	                
  printf("<--%s\r\n",My_Usart2.pucRec_Buffer);	//打印串口返回的数据信息
	if(strstr((const char *)My_Usart2.pucRec_Buffer,"+CSQ")) //串口返回+CSQ，开始解析数据
	{
	  CSQ_String =  StrBetwString((char *)My_Usart2.pucRec_Buffer,"+CSQ: ",","); //获取信号质量
		CSQ = atoi(CSQ_String);
		printf("CSQ_String:%s  CSQ:%d\r\n",CSQ_String,CSQ);
	}
	/*判断信号值是否在有效范围内*/
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
	* @brief  模块开机
	* @param  None
	* @retval None      
*/
static void Start_up(void)
{

}

/*
	* @name   MQTT_Connect_Server
	* @brief  连接MQTT服务器
	* @param  None
	* @retval None      
*/
static void MQTT_Connect_Server(void)
{
  __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);   //禁止串口2的空闲中断
	
//	EC20.Start_up();     //模块开机
//	Moudle_Sync_Baud();  //波特率适应
//	Send_AT(AT_CMD_ECHO_OFF,(uint8_t *)"OK");  //关闭回显
	Moudle_Check_SIM_Status();                //检查SIM卡状态
	Moudle_Check_IMEI(); //获取4G模块的IMEI
	Moudle_Check_Network_Register(); //判断是否驻网
	Moudle_Check_Attach_GPRS_Service(); //判断是否连上网络
	Moudle_Check_QIACT(); //激活网络
	Moudle_Check_MQTT_Server(); //连接MQTT服务器
	Moudle_Check_MQTT_Client_ID();//创建MQTT客户端
	Moudle_Sub_MQTT_Theme(); //MQTT订阅主题
//	Send_AT(AT_CMD_QMTSUB,(uint8_t *)"ERROR");
	EC20.DMA_Receive_Set();  //DMA设置重新接收

	if(EC20.MQTT_Connect_Status == TRUE)
	{
		__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);   //使能串口2的空闲中断
	}
}


/*
	* @name   Transfer_Data
  * @brief  向MQTT服务器发送数据
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
	//检测数据发送成功之后是否回复该信息
	if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTPUBEX: 0,0,0"))
	{
	  printf("publish ok\r\n");
	}
}


/*
	* @name   Transfer_Warning_Data
  * @brief  向MQTT服务器发送数据
	* @param  高温低温度数据上报
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
	//检测数据发送成功之后是否回复该信息
	if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTPUBEX: 0,0,0"))
	{
	  printf("publish ok\r\n");
	}
}

/*
	* @name   Receive_Information
	* @brief  接收MQTT服务器的信息
	* @param  None
	* @retval None      
*/
static void Receive_Information(void)
{
	char *str1 = NULL,*str2 = NULL;
	
/*Ps
**服务器下发配高温低温阈值设置置指令格式
	 {"id":123123,"TH":50,"TL":10}
	"id"名称不能改变  其后面的数据用4G的IMEI表示  "TH"表示高温阈值 "TL"表示低温阈值
	*/	
  if(EC20.MQTT_Connect_Status == TRUE) //EG25连接上MQTT服务器
	{
		//服务器下发高温/低温配置指令
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
//			printf("%s\r\n %s\r\n",Instuction_Buf1[4],Instuction_Buf1[5]);  //Instuction_Buf1[4] "TH":50  Instuction_Buf1[5] "TL":0}" 打印输出调试信息
//      str2 = StrBetwString((char *)Instuction_Buf1[5],"\"TL\":","}\"");
//		  if(str2 != NULL)
//			{
//				Temperature_Limit.TL = atoi(str2);	
//        printf("Temperature_Limit.TL=%d\r\n",Temperature_Limit.TL);				
//			}

		}
		
		//如果客户端被服务器T踢，返回+QMTSTAT: 0,1,软件复位模块
    if(strstr((const char *)My_Usart2.pucRec_Buffer,"+QMTSTAT: 0,1"))
		{
		  printf("Restart Module!!!\r\n");
		  Public.Reset_MCU();
		}
		//服务器下发指令，设备进入睡眠模式
		if(strstr((const char *)My_Usart2.pucRec_Buffer,"mode:sleep"))
		{
			printf("enter sleep!!!\r\n");
			LowPower.Enter_Sleep_Mode_Flag = TRUE;  //设备进入睡眠模式的FLAG
		}
		
		
		
	 EC20.DMA_Receive_Set();
	}
}

/*
	* @name 	DMA_Receive_Set
	* @brief	DMA接收设置
	* @param	None 
	* @retval None			
*/
static void DMA_Receive_Set(void)
{
	//串口2禁止DMA接收
	HAL_UART_DMAStop(&huart2);
	//清缓存
	Public.Memory_Clr(My_Usart2.pucRec_Buffer,strlen((const char*)My_Usart2.pucRec_Buffer));		
	//串口2开启DMA接收
	HAL_UART_Receive_DMA(&huart2,My_Usart2.pucRec_Buffer,Usart2_Rec_LENGTH);
}

/*
	* @name 	Error
	* @brief	错误信息
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
