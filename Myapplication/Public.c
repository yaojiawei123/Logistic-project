/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"
#include "cmsis_armcc.h"
/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Memory_Clr(uint8_t*,uint16_t);   //内存清除函数
static void Reset_MCU(void);//系统复位
/* Public variables-----------------------------------------------------------*/
Public_t Public =
{
  Memory_Clr,
	Reset_MCU
};
/* Private function prototypes------------------------------------------------*/      

/*
	* @name   Memory_Set
	* @brief  内存清除函数
	* @param  pucBuffer -> 内存首地址
						LEN       -> 内存长度   
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
	* @brief  系统软复位
	* @param  None
	* @retval None      
*/
static void Reset_MCU(void)
{
		HAL_NVIC_SystemReset();        
}

/*
	* @name   fputc
	* @brief  fputc映射物理串口
	* @param  ch -> 待发送字符
	* @retval ch -> 已发送字符      
*/
int fputc(int ch,FILE* f)
{
	//通过查询的方式循环发送
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0x000A);
	return ch;
}
/********************************************************
  End Of File
********************************************************/
