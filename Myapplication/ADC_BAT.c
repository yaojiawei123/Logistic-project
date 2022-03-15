/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/
static void Get_BAT_Voltage(void);
/* Private variables----------------------------------------------------------*/
ADC_BAT_t ADC_BAT =
{
  0,
	0.0,
	Get_BAT_Voltage,

};


static void Get_BAT_Voltage(void)
{
  float bat =0.0;
	
	HAL_ADC_Start(&hadc1);
  if(HAL_ADC_PollForConversion(&hadc1,1000) == HAL_OK)
	{
	  ADC_BAT.ADC_Value= HAL_ADC_GetValue(&hadc1);
		bat = (ADC_BAT.ADC_Value * 3.3)/4096;
		ADC_BAT.BAT_Value = (bat* 2);
		
		printf("电池电压 = %.1f V\r\n",ADC_BAT.BAT_Value);
	}
  else
	{
	  printf("AD转换错误或超时!!!\r\n");
	}

}



	



/********************************************************
  End Of File
********************************************************/
