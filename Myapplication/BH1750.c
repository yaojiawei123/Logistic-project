/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void BH1750_Init(void);
static uint8_t BH1750_Byte_Write(uint8_t);
static uint16_t BH1750_Read_Measure(void);
static float Lux_Continus_Intensity(void);
/* Public variables-----------------------------------------------------------*/
BH1750_t BH1750 =
{
  0.0,
	BH1750_Init,
	BH1750_Byte_Write,
	BH1750_Read_Measure,
	Lux_Continus_Intensity
};
/* Private function prototypes------------------------------------------------*/      
static uint8_t BH1750_Byte_Write(uint8_t data)
{
	I2C_BH1750.i2c_Start();
	//����д��ַ
	I2C_BH1750.i2c_SendByte(BH1750_Addr|0);
	if(I2C_BH1750.i2c_WaitAck()==1)
		return 1;
	//���Ϳ�������
	I2C_BH1750.i2c_SendByte(data);
	if(I2C_BH1750.i2c_WaitAck()==1)
		return 2;
	I2C_BH1750.i2c_Stop();
	return 0;
}

static uint16_t BH1750_Read_Measure(void)
{
	uint16_t receive_data=0; 
	I2C_BH1750.i2c_Start();
	//���Ͷ���ַ
	I2C_BH1750.i2c_SendByte(BH1750_Addr|1);
	if(I2C_BH1750.i2c_WaitAck()==1)
		return 0;
	//��ȡ�߰�λ
	receive_data=I2C_BH1750.i2c_ReadByte();
	I2C_BH1750.i2c_Ack();
	//��ȡ�Ͱ�λ
	receive_data=(receive_data<<8)+I2C_BH1750.i2c_ReadByte();
	I2C_BH1750.i2c_NAck();
	I2C_BH1750.i2c_Stop();
	return receive_data;	//���ض�ȡ��������
}

static float Lux_Continus_Intensity(void)
{
  return (float)(BH1750.BH1750_Read_Measure()/1.1f*Resolurtion);
}


static void BH1750_Init(void)
{
  BH1750.BH1750_Byte_Write(Measure_Mode);
}
/********************************************************
  End Of File
********************************************************/
