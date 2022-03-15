/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/


/* Public variables-----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/      
/*
	* @name   HAL_TIM_PeriodElapsedCallback
	* @brief  定时器中断回调函数
	* @param  *htim -> 处理定时器的结构体指针
	* @retval None      
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim ->Instance == htim6.Instance)
	{
		Timer6.usDelay_Timer++;
		Timer6.SHT31_Measure_Timeout++;
    Timer6.usMCU_Run_Timer++;
		EC20.MQTT_Reconnect_Timer++;
		Timer6.Battery_Bat_Timer++;
	}
	if(htim ->Instance == htim7.Instance)
	{
	  Timer7.Sensor_Pub_Timer++;
		Timer7.Sensor_Pub_Timer1++;
	}
}


/*
	* @name   HAL_UART_IdleCallback
	* @brief  串口空闲中断回调函数
	* @param  *htim -> 处理串口的结构体指针
	* @retval None      
*/
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{
  if(huart ->Instance == huart2.Instance)
	{
	  EC20.Receive_Information();
	}
	if(huart ->Instance == huart3.Instance)
	{
	  Zigbee.Zigbee_Receive_Information();
	}
	
}


/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == KEY1_Pin)
	{
		/*KEY1按下 唤醒单片机*/
	  printf("KEY1\r\n");
		HAL_GPIO_WritePin(GPIOA, LED1_Pin, GPIO_PIN_RESET);
		LowPower.Enter_Sleep_Mode_Flag = FALSE;
	}
}
/********************************************************
  End Of File
********************************************************/
