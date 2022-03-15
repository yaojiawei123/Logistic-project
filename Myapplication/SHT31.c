/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Measure_Period_Mode(void);  //���ڲ���ģʽ
/* Public variables-----------------------------------------------------------*/
SHT31_t SHT31 =
{
  0.0,
	0,
	Measure_Period_Mode
};
/* Private function prototypes------------------------------------------------*/      
static uint8_t CRC_8(uint8_t *,uint8_t);


/*
	* @name   Measure_Period_Mode
	* @brief  ���ڲ���ģʽ
	* @param  None
	* @retval None      
*/
static void Measure_Period_Mode(void)
{
	uint16_t  Measure_Timeout = 0;
  uint8_t   temp_array[6] = {0};
	uint16_t  temp_uint     = 0;
	float     temp_float    = 0;
	
	//���������Բ���
	I2C_Soft.Start();
	I2C_Soft.Write_Byte(SHT31_ADDR & Write_CMD);
	I2C_Soft.Write_Byte(0x27); //High repeat , mps = 10
	I2C_Soft.Write_Byte(0x37);	
	
  Timer6.SHT31_Measure_Timeout = 0;	
	//���ͽ�����������
	do
	{		
		if(Timer6.SHT31_Measure_Timeout >= TIMER6_2S) //2s��û��ȡ�����ݣ��˳��ȴ�
			break;
		
		I2C_Soft.Start();
		I2C_Soft.Write_Byte(SHT31_ADDR & Write_CMD);
		I2C_Soft.Write_Byte(0xE0);
		I2C_Soft.Write_Byte(0x00);
		
		I2C_Soft.Start();
	}
	while(I2C_Soft.Write_Byte(SHT31_ADDR | Read_CMD) ==NACK);	
	
	//��ʼ���ղ������ݣ�������
	if(Measure_Timeout < TIMER6_2S)
	{
		temp_array[0] = I2C_Soft.Read_Byte(ACK);
		temp_array[1] = I2C_Soft.Read_Byte(ACK);
		temp_array[2] = I2C_Soft.Read_Byte(ACK);
		temp_array[3] = I2C_Soft.Read_Byte(ACK);
		temp_array[4] = I2C_Soft.Read_Byte(ACK);
		temp_array[5] = I2C_Soft.Read_Byte(NACK);
		I2C_Soft.Stop();
		
		//////�����¶�,����0.1
		if(CRC_8(temp_array,2) == temp_array[2]) //CRC-8 У��
		{
			temp_uint         = temp_array[0]*256+temp_array[1];
			temp_float        = ((float)temp_uint)*0.267032-4500;
			SHT31.fTemperature = temp_float*0.01;
	  }
		
		//////����ʪ�ȣ�����1%RH
		if(CRC_8(&temp_array[3],2) == temp_array[5]) //CRC-8 У��
		{
			temp_uint      = temp_array[3]*256+temp_array[4];
			temp_float     = ((float)temp_uint)*0.152590;
			temp_float     = temp_float*0.01;
			SHT31.ucHumidity = (unsigned char)temp_float;  
	  }
	}	
}


/*
	* @name   CRC_8
	* @brief  CRC-8У��
	* @param  Crc_ptr -> У�������׵�ַ
						LEN     -> У�����ݳ���
	* @retval CRC_Value -> У��ֵ      
*/
static uint8_t CRC_8(uint8_t *Crc_ptr,uint8_t LEN)
{
	uint8_t CRC_Value = 0xFF;
	uint8_t i = 0,j = 0;

	for(i=0;i<LEN;i++)
	{
		CRC_Value ^= *(Crc_ptr+i);
		for(j=0;j<8;j++)
		{
			if(CRC_Value & 0x80)
				CRC_Value = (CRC_Value << 1) ^ 0x31;
			else
				CRC_Value = (CRC_Value << 1);
		}
	}
	return CRC_Value;
}
/********************************************************
  End Of File
********************************************************/
