/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Peripheral_Set(void); 

/* Public variables-----------------------------------------------------------*/
MyInit_t MyInit = 
{
	Peripheral_Set
};

/* Private function prototypes------------------------------------------------*/      


/*
	* @name   Peripheral_Set
	* @brief  ��������
	* @param  None
	* @retval None      
*/
static void Peripheral_Set()
{

  Timer6.Timer6_Start_IT();
	Timer7.Timer7_Start_IT();
	I2C_Soft.Init();
  BH1750.BH1750_Init();
	GPS_hanle.clrStruct();

	IIC_E2PROM.Init();
	while(AT24CXX_Check())
	{ 
	  printf("δ��⵽AT24C02\r\n");
	}
	printf("��⵽AT24C02\r\n");
  
	HAL_GPIO_WritePin(BAT_CTRL_GPIO_Port,BAT_CTRL_Pin,GPIO_PIN_RESET); //������ѹ�������
	printf("OK\r\n");
}

/********************************************************
  End Of File
********************************************************/
