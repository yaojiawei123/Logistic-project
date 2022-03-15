/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
/*********************************SHT31��غ궨��**********************************************/
//��λ������SCL�ܽ�
#define SET_SCL_SHT31 HAL_GPIO_WritePin(SHT31_SCL_GPIO_Port,SHT31_SCL_Pin,GPIO_PIN_SET) 
#define CLR_SCL_SHT31 HAL_GPIO_WritePin(SHT31_SCL_GPIO_Port,SHT31_SCL_Pin,GPIO_PIN_RESET) 
//��λ������SDA�ܽ�
#define SET_SDA_SHT31 HAL_GPIO_WritePin(SHT31_SDA_GPIO_Port,SHT31_SDA_Pin,GPIO_PIN_SET)
#define CLR_SDA_SHT31 HAL_GPIO_WritePin(SHT31_SDA_GPIO_Port,SHT31_SDA_Pin,GPIO_PIN_RESET)
//��SDA�ܽ�״̬
#define READ_SDA_SHT31	HAL_GPIO_ReadPin(SHT31_SDA_GPIO_Port,SHT31_SDA_Pin)

/*********************************BH1750��غ궨��**********************************************/
#define SET_SCL_BH1750 HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET) 
#define CLR_SCL_BH1750 HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET) 
//��λ������SDA�ܽ�
#define SET_SDA_BH1750 HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET)
#define CLR_SDA_BH1750 HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_RESET)
//��SDA�ܽ�״̬
#define READ_SDA_BH1750	HAL_GPIO_ReadPin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin)
/* Private variables----------------------------------------------------------*/
/*SHT31*/
void Init(void);  //I2C��ʼ��
void Start(void); //I2C��ʼ�ź�
void Stop(void);  //I2Cֹͣ�ź�
ACK_Value_t Write_Byte(uint8_t);      //I2Cд�ֽ�
uint8_t     Read_Byte (ACK_Value_t);  //I2C���ֽ�

/*BH1750*/
static void i2c_Start(void);
static void i2c_Stop(void);
static void i2c_SendByte(uint8_t );
static uint8_t i2c_ReadByte(void);
static uint8_t i2c_WaitAck(void);
static void i2c_Ack(void );
static void i2c_NAck(void);
static uint8_t i2c_CheckDevice(uint8_t);
/* Public variables-----------------------------------------------------------*/
I2C_Soft_t  I2C_Soft =
{
	Init,
	Start,
	Stop,
	Write_Byte,
	Read_Byte
};

I2C_BH1750_t I2C_BH1750 =
{
  i2c_Start,
	i2c_Stop,
	i2c_SendByte,
	i2c_ReadByte,
	i2c_WaitAck,
	i2c_Ack,
	i2c_NAck,
	i2c_CheckDevice,

};
/* Private function prototypes------------------------------------------------*/   
static void I2C_Delay_us(uint8_t);
/*
	* @name   Init
	* @brief  I2C��ʼ��
	* @param  None
	* @retval None      
*/
void Init(void)
{
	SET_SCL_SHT31;
	SET_SDA_SHT31;
}


/*
	* @name   Start
	* @brief  I2C��ʼ�ź�
	* @param  None
	* @retval None      
*/
void Start(void)
{
	//SCLΪ�ߵ�ƽ��SDA���½���ΪI2C��ʼ�ź�
	SET_SDA_SHT31;
	SET_SCL_SHT31;
	I2C_Delay_us(1);
	
	CLR_SDA_SHT31;
	I2C_Delay_us(10);
	
	CLR_SCL_SHT31;
	I2C_Delay_us(1);

}

/*
	* @name   Stop
	* @brief  I2Cֹͣ�ź�
	* @param  None
	* @retval None      
*/
void Stop(void)
{
	//SCLΪ�ߵ�ƽ��SDA��������ΪI2Cֹͣ�ź�
	CLR_SDA_SHT31;
	SET_SCL_SHT31;
	I2C_Delay_us(1);
		
	I2C_Delay_us(10);
	SET_SDA_SHT31;
}

/*
	* @name   Write_Byte
	* @brief  I2Cд�ֽ�
	* @param  WR_Byte -> ��д������
	* @retval ACK_Value_t -> �ӻ�Ӧ��ֵ      
*/
ACK_Value_t Write_Byte(uint8_t WR_Byte)
{
	uint8_t i;
	ACK_Value_t  ACK_Rspond;
	
	//SCLΪ�͵�ƽʱ��SDA׼������,����SCLΪ�ߵ�ƽ����ȡSDA����
	//���ݰ�8λ���䣬��λ��ǰ������forѭ���������
	for(i=0;i<8;i++)
	{
		//SCL���㣬����SDA׼������
		CLR_SCL_SHT31;
		I2C_Delay_us(1);
		if((WR_Byte&BIT7) == BIT7)
		{
			SET_SDA_SHT31;
		}
		else
		{
			CLR_SDA_SHT31;
		}
		I2C_Delay_us(1);
		//SCL�øߣ���������
		SET_SCL_SHT31;
		I2C_Delay_us(10);
		
		//׼��������һ����λ
		WR_Byte <<= 1;
	}
	
	CLR_SCL_SHT31;	
	//�ͷ�SDA���ȴ��ӻ�Ӧ��
	SET_SDA_SHT31;
	I2C_Delay_us(1);
	
	SET_SCL_SHT31;
	I2C_Delay_us(10);
	
	ACK_Rspond = (ACK_Value_t)READ_SDA_SHT31;
	
	CLR_SCL_SHT31;
	I2C_Delay_us(1);
	
	//���شӻ���Ӧ���ź�
	return ACK_Rspond;
}

/*
	* @name   Read_Byte
	* @brief  I2C���ֽ�
	* @param  ACK_Value -> ������Ӧֵ
	* @retval �ӻ�����ֵ      
*/
uint8_t Read_Byte (ACK_Value_t ACK_Value)
{
	uint8_t RD_Byte = 0,i;
		
	////��������
	//SCLΪ�͵�ƽʱ��SDA׼������,����SCLΪ�ߵ�ƽ����ȡSDA����
	//���ݰ�8λ���䣬��λ��ǰ������forѭ���������
	for(i=0;i<8;i++)
	{
		//׼��������һ����λ
		RD_Byte <<= 1;
		
		//SCL���㣬�ӻ�SDA׼������
		CLR_SCL_SHT31;
		I2C_Delay_us(10);
		
		//SCL�øߣ���ȡ����
		SET_SCL_SHT31;
		I2C_Delay_us(10);	

		RD_Byte |= READ_SDA_SHT31;		
	}
	
	
	//SCL���㣬����׼��Ӧ���ź�
	CLR_SCL_SHT31;
	I2C_Delay_us(1);
	
	//��������Ӧ���ź�	
	if(ACK_Value == ACK)
	{
		CLR_SDA_SHT31;
	}
	else
	{
		SET_SDA_SHT31;	
  }	
	I2C_Delay_us(1);
	
	
	SET_SCL_SHT31; 	
	I2C_Delay_us(10);
	
	//Note:
  //�ͷ�SDA������
	//SCL�����㣬���ͷ�SDA����ֹ������������ʱ���ӻ���SDA�ͷ��źŵ���NACk�ź�
	CLR_SCL_SHT31;
  SET_SDA_SHT31; 	
	I2C_Delay_us(1);

	//��������
	return RD_Byte;
}

/*
	* @name   I2C_Delay
	* @brief  I2C��ʱ
	* @param  None
	* @retval None      
*/
static void I2C_Delay_us(uint8_t us)
{
	uint8_t i = 0;
	
	//ͨ��ʾ������������У׼
	while(us--)
	{
		for(i=0;i<7;i++);
	}
}

/******************************************************************************************************************/
static void i2c_Delay(void);

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Start(void)
{
	SET_SDA_BH1750;
  SET_SCL_BH1750;
	i2c_Delay();
	CLR_SDA_BH1750;
	i2c_Delay();
  CLR_SCL_BH1750;  
  i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */	
	CLR_SDA_BH1750;
  SET_SCL_BH1750;
  i2c_Delay();
	SET_SDA_BH1750;	
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			SET_SDA_BH1750;
		}
		else
		{
			CLR_SDA_BH1750;
		}
		i2c_Delay();
		SET_SCL_BH1750;
		i2c_Delay();	
		CLR_SCL_BH1750;
		if (i == 7)
		{
		  CLR_SDA_BH1750;
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
static uint8_t i2c_ReadByte(void)
{
 	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
	  value <<= 1;
		SET_SCL_BH1750;
		i2c_Delay();
		if(READ_SDA_BH1750)
		{
		  value++;
		}
		CLR_SCL_BH1750;
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
static uint8_t i2c_WaitAck(void)
{
	uint8_t re;
	
	SET_SDA_BH1750;
	i2c_Delay();	
	SET_SCL_BH1750;
	i2c_Delay();	
  if(READ_SDA_BH1750)
    re = 1;
	else
		re = 0;
	CLR_SCL_BH1750;
	i2c_Delay();
  return re;	
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Ack(void )
{
  CLR_SDA_BH1750;
  i2c_Delay();
	SET_SCL_BH1750;
  i2c_Delay();
	CLR_SCL_BH1750;
  i2c_Delay();	
	SET_SDA_BH1750;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_NAck(void)
{
	SET_SDA_BH1750;	/* CPU����SDA = 1 */
	i2c_Delay();
	SET_SCL_BH1750;	/* CPU����1��ʱ�� */
	i2c_Delay();
	CLR_SCL_BH1750;
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
static uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	I2C_BH1750.i2c_Start();		/* ���������ź� */
	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | BH1750_I2C_WR);
	ucAck = I2C_BH1750.i2c_WaitAck();	/* ����豸��ACKӦ�� */

	I2C_BH1750.i2c_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}


/*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/********************************************************
  End Of File
********************************************************/
