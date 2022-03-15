/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Timer6_Start_IT(void);
static void Timer7_Start_IT(void);
/* Public variables-----------------------------------------------------------*/
Timer6_t Timer6 =
{
	0,
	0,
	0,
	0,
	0,
	0,
	Timer6_Start_IT   
};

Timer7_t Timer7 =
{
  0,
	0,
	0,
	0,
	0,
	Timer7_Start_IT
};
/* Private function prototypes------------------------------------------------*/  
/*
	* @name   Timer6_Start_IT
	* @brief  ��ʱ��6���ж�ģʽ����
	* @param  None
	* @retval None      
*/
static void Timer6_Start_IT(void)
{
	HAL_TIM_Base_Start_IT(&htim6); //������ʱ��6
}

/*
	* @name   Timer7_Start_IT
	* @brief  ��ʱ��7���ж�ģʽ����
	* @param  None
	* @retval None      
*/
static void Timer7_Start_IT(void)
{
	HAL_TIM_Base_Start_IT(&htim7); //������ʱ��6
}
/********************************************************
  End Of File
********************************************************/
