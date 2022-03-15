#ifndef __ADC_BAT_H__
#define __ADC_BAT_H__

#include "MyApplication.h"


/*************************************************/
typedef struct
{
  uint16_t  ADC_Value;
	float     BAT_Value;
	
	void (*Get_BAT_Voltage)(void);

}ADC_BAT_t;


/* extern  variables-----------------------------------------------------------*/
extern ADC_BAT_t ADC_BAT;
	
#endif
/********************************************************
  End Of File
********************************************************/
