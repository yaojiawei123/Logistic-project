/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
/*串口2的相关变量*/
static uint8_t  ucSend_Buffer[Usart2_Send_LENGTH] = {0x00};
static uint8_t  ucRec_Buffer [Usart2_Rec_LENGTH]  = {0x00};
static void SendArray(uint8_t*,uint16_t);  //串口发送数组
static void SendString(uint8_t*);          //串口发送字符串
/*串口3的相关变量*/
static uint8_t  ucRec_Buffer3 [Usart3_Rec_LENGTH]  = {0x00};
/* Public variables-----------------------------------------------------------*/

//串口1结构体变量初始化
My_Usart1_t My_Usart1 =
{
  0,
	0,
	0
};

//串口4结构体变量初始化
My_Uart4_t My_Uart4 =
{
  0,
	0
};

//串口2结构体变量初始化
My_Usart2_t My_Usart2 =
{
  ucSend_Buffer,
	ucRec_Buffer,
	
	SendArray,
	SendString,	
};

//串口3结构体变量初始化
My_Usart3_t My_Usart3 =
{
	ucRec_Buffer3
};
/* Private function prototypes------------------------------------------------*/      
/*
	* @name   SendArray
	* @brief  串口发送数组
	* @param  p_Arr:数组首地址，LEN:发送长度
	* @retval None      
*/
static void SendArray(uint8_t* p_Arr,uint16_t LEN) 
{
	HAL_UART_Transmit_DMA(&huart2,p_Arr,LEN);
}

/*
	* @name   SendString
	* @brief  发送字符串
	* @param  p_Str:待发送字符串
	* @retval None      
*/
static void SendString(uint8_t* p_Str) 
{	
	HAL_UART_Transmit_DMA(&huart2,p_Str,strlen((const char*)p_Str));
}

/********************************************************
  End Of File
********************************************************/
