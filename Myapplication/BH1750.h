#ifndef __BH1750_H_
#define __BH1750_H_

#include "MyApplication.h"

/* Public define-------------------------------------------------------------*/
//BH1750�ĵ�ַ
#define BH1750_Addr				0x46

#define BH1750_I2C_WR	0		/* д����bit */
#define BH1750_I2C_RD	1		/* ������bit */

//BH1750ָ����
#define POWER_OFF					0x00
#define POWER_ON					0x01
#define MODULE_RESET			0x07
#define	CONTINUE_H_MODE		0x10
#define CONTINUE_H_MODE2	0x11
#define CONTINUE_L_MODE		0x13
#define ONE_TIME_H_MODE		0x20
#define ONE_TIME_H_MODE2	0x21
#define ONE_TIME_L_MODE		0x23

//����ģʽ
#define Measure_Mode			CONTINUE_H_MODE

//�ֱ���	����ǿ�ȣ���λlx��=��High Byte  + Low Byte��/ 1.2 * ��������
#if ((Measure_Mode==CONTINUE_H_MODE2)|(Measure_Mode==ONE_TIME_H_MODE2))
	#define Resolurtion		0.5
#elif ((Measure_Mode==CONTINUE_H_MODE)|(Measure_Mode==ONE_TIME_H_MODE))
	#define Resolurtion		1
#elif ((Measure_Mode==CONTINUE_L_MODE)|(Measure_Mode==ONE_TIME_L_MODE))
	#define Resolurtion		4
#endif


typedef struct
{
  float Lux;
	void (*BH1750_Init)(void);
	uint8_t (*BH1750_Byte_Write)(uint8_t);
	uint16_t (*BH1750_Read_Measure)(void);
	float (*Lux_Continus_Intensity)(void);
}BH1750_t;



/* extern variables-----------------------------------------------------------*/
extern BH1750_t BH1750;


#endif
/********************************************************
  End Of File
********************************************************/
