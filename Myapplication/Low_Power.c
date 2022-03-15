/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"
/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Sleep_Mode(void);   //睡眠模式
/* Public variables-----------------------------------------------------------*/
LowPower_t LowPower =
{
  FALSE,            //FALSE :不进入睡眠模式 TRUE :进入睡眠模式
	Sleep_Mode
};
/* Private function prototypes------------------------------------------------*/     
/*
	* @name   Sleep_Mode
	* @brief  睡眠模式
	* @param  None
	* @retval None      
*/
static void Sleep_Mode(void)
{
	/***Note:任意中断都可以将系统从睡眠模式中唤醒***/
	//关闭Systick与定时器中断，否则，进入睡眠模式后立马被唤醒
	HAL_SuspendTick();
	HAL_TIM_Base_Stop_IT(&htim6);
	//关闭外部所有中断
	HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel6_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel7_IRQn);	
	HAL_NVIC_DisableIRQ(USART1_IRQn);	
	HAL_NVIC_DisableIRQ(USART2_IRQn);	
	HAL_NVIC_DisableIRQ(USART3_IRQn);	
	HAL_NVIC_DisableIRQ(UART4_IRQn);	
	HAL_NVIC_DisableIRQ(TIM6_IRQn);		
	HAL_NVIC_DisableIRQ(TIM7_IRQn);	
	
	HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);

	//恢复中断
	HAL_ResumeTick();
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_EnableIRQ(USART3_IRQn);	
	HAL_NVIC_EnableIRQ(UART4_IRQn);	
	HAL_NVIC_EnableIRQ(TIM6_IRQn);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);	
}
/********************************************************
  End Of File
********************************************************/
