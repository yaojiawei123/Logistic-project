/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
/*����2����ر���*/
static uint8_t  ucSend_Buffer[Usart2_Send_LENGTH] = {0x00};
static uint8_t  ucRec_Buffer [Usart2_Rec_LENGTH]  = {0x00};
static void SendArray(uint8_t*,uint16_t);  //���ڷ�������
static void SendString(uint8_t*);          //���ڷ����ַ���
/*����3����ر���*/
static uint8_t  ucRec_Buffer3 [Usart3_Rec_LENGTH]  = {0x00};
/* Public variables-----------------------------------------------------------*/

//����1�ṹ�������ʼ��
My_Usart1_t My_Usart1 =
{
  0,
	0,
	0
};

//����4�ṹ�������ʼ��
My_Uart4_t My_Uart4 =
{
  0,
	0
};

//����2�ṹ�������ʼ��
My_Usart2_t My_Usart2 =
{
  ucSend_Buffer,
	ucRec_Buffer,
	
	SendArray,
	SendString,	
};

//����3�ṹ�������ʼ��
My_Usart3_t My_Usart3 =
{
	ucRec_Buffer3
};
/* Private function prototypes------------------------------------------------*/      
/*
	* @name   SendArray
	* @brief  ���ڷ�������
	* @param  p_Arr:�����׵�ַ��LEN:���ͳ���
	* @retval None      
*/
static void SendArray(uint8_t* p_Arr,uint16_t LEN) 
{
	HAL_UART_Transmit_DMA(&huart2,p_Arr,LEN);
}

/*
	* @name   SendString
	* @brief  �����ַ���
	* @param  p_Str:�������ַ���
	* @retval None      
*/
static void SendString(uint8_t* p_Str) 
{	
	HAL_UART_Transmit_DMA(&huart2,p_Str,strlen((const char*)p_Str));
}

/********************************************************
  End Of File
********************************************************/
