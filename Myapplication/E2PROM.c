/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
static uint8_t AT24CXX_ReadOneByte(uint16_t);
static void AT24CXX_WriteOneByte(uint16_t, uint8_t);
static void AT24CXX_WriteLenByte(uint16_t,uint32_t,uint8_t);
static uint32_t AT24CXX_ReadLenByte(uint16_t,uint8_t);
static void AT24CXX_Write(uint16_t,uint8_t*,uint16_t);
static void AT24CXX_Read(uint16_t,uint8_t*,uint16_t);
/* Public variables-----------------------------------------------------------*/
E2PROM_t E2PROM =
{
  AT24CXX_ReadOneByte,
	AT24CXX_WriteOneByte,
	AT24CXX_WriteLenByte,
	AT24CXX_ReadLenByte,
  AT24CXX_Write,
	AT24CXX_Read
};

/*
	* @name   AT24CXX_ReadOneByte
	* @brief  E2PROM读一个字节
	* @param  ReadAddr -> 读数据的地址
	* @retval temp -> 读出的数据      
*/
static uint8_t AT24CXX_ReadOneByte(uint16_t ReadAddr)
{
  uint8_t temp = 0;
  IIC_E2PROM.Start();
	if(EE_TYPE>AT24C16)
	{
	  IIC_E2PROM.Write_Byte(0xA0);
		HAL_Delay(10);
	  IIC_E2PROM.Write_Byte(ReadAddr>>8);
		HAL_Delay(10);
	}
	else
	{
    IIC_E2PROM.Write_Byte(0xA0+((ReadAddr/256)<<1));	
	}
  HAL_Delay(10);
	IIC_E2PROM.Write_Byte(ReadAddr%256);
	HAL_Delay(10);
	IIC_E2PROM.Start();
	IIC_E2PROM.Write_Byte(0xA1);
	HAL_Delay(10);	
	temp = IIC_E2PROM.Read_Byte(NACK);
	IIC_E2PROM.Stop();
	return temp;
}

/*
	* @name   AT24CXX_WriteOneByte
	* @brief  E2PROM写一个字节
	* @param  WriteAddr -> 写数据的地址 DataToWrite -> 要写入的数据
	* @retval None
*/
static void AT24CXX_WriteOneByte(uint16_t WriteAddr, uint8_t DataToWrite)
{
  IIC_E2PROM.Start();
  if(EE_TYPE>AT24C16)
	{
		IIC_E2PROM.Write_Byte(0xA0);
		HAL_Delay(10);
		IIC_E2PROM.Write_Byte(WriteAddr>>8);
	}
  else
	{
	  IIC_E2PROM.Write_Byte(0XA0+((WriteAddr/256)<<1));
	}
  HAL_Delay(10);
	IIC_E2PROM.Write_Byte(WriteAddr%256);
	HAL_Delay(10);
	IIC_E2PROM.Write_Byte(DataToWrite);
	HAL_Delay(10);
	IIC_E2PROM.Stop();
	HAL_Delay(10);
}

/*
	* @name   AT24CXX_WriteLenByte
	* @brief  E2PROM在指定地址写入指定长度数据
	* @param  WriteAddr -> 写数据的地址  DataToWrite -> 数据  Len ->数据长度
	* @retval None
*/
static void AT24CXX_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len)
{
  uint8_t t;
	for(t=0;t<Len;t++)
	{
	  E2PROM.AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}

/*
	* @name   AT24CXX_ReadLenByte
	* @brief  E2PROM在指定地址读出长度为Len的数据
	* @param  ReadAddr -> 读数据的地址  Len ->数据长度
	* @retval None
*/
static uint32_t AT24CXX_ReadLenByte(uint16_t ReadAddr,uint8_t Len)
{
  uint8_t t;
	uint32_t temp;
	for(t=0;t<Len;t++)
	{
	  temp<<=8;
		E2PROM.AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}
  return temp;
}

/*
	* @name   AT24CXX_Write
	* @brief  E2PROM指定地址写入指定个数的数据
	* @param  WriteAddr -> 写数据的地址  pBuffer ->数据数组首地址  NumToWrite ->要写入数据的个数
	* @retval None
*/
static void AT24CXX_Write(uint16_t WriteAddr,uint8_t* pBuffer,uint16_t NumToWrite)
{
	while(NumToWrite--)
	{
	  E2PROM.AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

/*
	* @name   AT24CXX_Read
	* @brief  E2PROM指定地址读出指定个数的数据
	* @param  ReadAddr -> 读数据的地址  pBuffer ->数据数组首地址  NumToWrite ->要读数据的个数
	* @retval None
*/
static void AT24CXX_Read(uint16_t ReadAddr,uint8_t* pBuffer,uint16_t NumToRead)
{
	while(NumToRead)
	{
	  *pBuffer++=E2PROM.AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}

/*
	* @name   AT24CXX_Check
	* @brief  检测E2PROM是否正常
	* @param  使用用了24XX的最后一个地址(255)来存储标志字.
	* @retval 返回1:检测失败
            返回0:检测成功
*/
uint8_t  AT24CXX_Check(void)
{
  uint8_t temp;
  temp = E2PROM.AT24CXX_ReadOneByte(255);
	if(temp==0X55)return 0;	
	else
	{
	  E2PROM.AT24CXX_WriteOneByte(255,0x55);
		temp = E2PROM.AT24CXX_ReadOneByte(255);
		if(temp==0X55)return 0;
	}
	return 1;
}

/********************************************************
  End Of File
********************************************************/
