/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void FeedDog(void);//2sι��
/* Public variables-----------------------------------------------------------*/
MyIWDG_t  MyIWDG =
{
	TRUE,
	
	FeedDog
};
/* Private function prototypes------------------------------------------------*/     
/*
	* @name   FeedDog
	* @brief  ι��
	* @param  None
	* @retval None      
*/
static void FeedDog(void)
{
	HAL_IWDG_Refresh(&hiwdg);
}
/********************************************************
  End Of File
********************************************************/
