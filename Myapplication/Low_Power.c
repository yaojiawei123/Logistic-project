/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"
/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Sleep_Mode(void);   //˯��ģʽ
/* Public variables-----------------------------------------------------------*/
LowPower_t LowPower =
{
  FALSE,            //FALSE :������˯��ģʽ TRUE :����˯��ģʽ
	Sleep_Mode
};
/* Private function prototypes------------------------------------------------*/     
/*
	* @name   Sleep_Mode
	* @brief  ˯��ģʽ
	* @param  None
	* @retval None      
*/
static void Sleep_Mode(void)
{
	/***Note:�����ж϶����Խ�ϵͳ��˯��ģʽ�л���***/
	//�ر�Systick�붨ʱ���жϣ����򣬽���˯��ģʽ����������
	HAL_SuspendTick();
	HAL_TIM_Base_Stop_IT(&htim6);
	//�ر��ⲿ�����ж�
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

	//�ָ��ж�
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
