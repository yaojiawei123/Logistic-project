#ifndef __E2PROM_H__
#define __E2PROM_H__

#include "MyApplication.h"

/***************************define*********************/
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
#define EE_TYPE   AT24C02

/*************************************************/
typedef struct
{
  uint8_t (*AT24CXX_ReadOneByte)(uint16_t);
	void (*AT24CXX_WriteOneByte)(uint16_t,uint8_t);
  void (*AT24CXX_WriteLenByte)(uint16_t,uint32_t,uint8_t);
  uint32_t (*AT24CXX_ReadLenByte)(uint16_t,uint8_t);
	void (*AT24CXX_Write)(uint16_t,uint8_t *,uint16_t);
	void (*AT24CXX_Read)(uint16_t,uint8_t *,uint16_t);
	
}E2PROM_t;


/* extern  variables-----------------------------------------------------------*/
extern E2PROM_t E2PROM;
uint8_t AT24CXX_Check(void);  //¼ì²éÆ÷¼þ
#endif
/********************************************************
  End Of File
********************************************************/
