/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"
#include "cmsis_armcc.h"
/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Memory_Clr(uint8_t*,uint16_t);   //�ڴ��������
static void Reset_MCU(void);//ϵͳ��λ
/* Public variables-----------------------------------------------------------*/
Public_t Public =
{
  Memory_Clr,
	Reset_MCU
};
/* Private function prototypes------------------------------------------------*/      

/*
	* @name   Memory_Set
	* @brief  �ڴ��������
	* @param  pucBuffer -> �ڴ��׵�ַ
						LEN       -> �ڴ泤��   
	* @retval None      
*/
static void Memory_Clr(uint8_t* pucBuffer,uint16_t LEN)
{
  uint16_t i;
	
	for(i=0;i<LEN;i++)
	{
		*(pucBuffer+i) = (uint8_t)0;
	}
}

/*
	* @name   Reset_MCU
	* @brief  ϵͳ��λ
	* @param  None
	* @retval None      
*/
static void Reset_MCU(void)
{
		HAL_NVIC_SystemReset();        
}

/*
	* @name   fputc
	* @brief  fputcӳ��������
	* @param  ch -> �������ַ�
	* @retval ch -> �ѷ����ַ�      
*/
int fputc(int ch,FILE* f)
{
	//ͨ����ѯ�ķ�ʽѭ������
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0x000A);
	return ch;
}
/********************************************************
  End Of File
********************************************************/
