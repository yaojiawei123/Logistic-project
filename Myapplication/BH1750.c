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
	//发送写地址
	I2C_BH1750.i2c_SendByte(BH1750_Addr|0);
	if(I2C_BH1750.i2c_WaitAck()==1)
		return 1;
	//发送控制命令
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
	//发送读地址
	I2C_BH1750.i2c_SendByte(BH1750_Addr|1);
	if(I2C_BH1750.i2c_WaitAck()==1)
		return 0;
	//读取高八位
	receive_data=I2C_BH1750.i2c_ReadByte();
	I2C_BH1750.i2c_Ack();
	//读取低八位
	receive_data=(receive_data<<8)+I2C_BH1750.i2c_ReadByte();
	I2C_BH1750.i2c_NAck();
	I2C_BH1750.i2c_Stop();
	return receive_data;	//返回读取到的数据
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
