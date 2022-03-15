/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
//��λ������SCL�ܽ� 
#define  SET_SCL  HAL_GPIO_WritePin(E2ROM_SCL_GPIO_Port,E2ROM_SCL_Pin,GPIO_PIN_SET)
#define  CLR_SCL  HAL_GPIO_WritePin(E2ROM_SCL_GPIO_Port,E2ROM_SCL_Pin,GPIO_PIN_RESET)

//��λ������SDA�ܽ�
#define	 SET_SDA	HAL_GPIO_WritePin(E2ROM_SDA_GPIO_Port,E2ROM_SDA_Pin,GPIO_PIN_SET)
#define  CLR_SDA  HAL_GPIO_WritePin(E2ROM_SDA_GPIO_Port,E2ROM_SDA_Pin,GPIO_PIN_RESET)

//��SDA�ܽ�״̬
#define  READ_SDA	HAL_GPIO_ReadPin(E2ROM_SDA_GPIO_Port,E2ROM_SDA_Pin)

static void Init(void);
static void Start(void);
static void Stop(void);
ACK_Value_t Write_Byte(uint8_t);      //I2Cд�ֽ�
uint8_t     Read_Byte (ACK_Value_t);  //I2C���ֽ�

/* Private function prototypes------------------------------------------------*/      
static void I2C_Delay_us(uint8_t);

/* Public variables-----------------------------------------------------------*/
IIC_E2PROM_t  IIC_E2PROM = 
{
  Init,
	Start,
	Stop,
	Write_Byte,
	Read_Byte
};

/*
	* @name   Init
	* @brief  I2C��ʼ��
	* @param  None
	* @retval None      
*/
static void Init(void)
{
	SET_SCL;
	SET_SDA;
}


/*
	* @name   Start
	* @brief  I2C��ʼ�ź�
	* @param  None
	* @retval None      
*/
static void Start(void)
{
	//SCLΪ�ߵ�ƽ��SDA���½���ΪI2C��ʼ�ź�
	SET_SDA;
	SET_SCL;
	I2C_Delay_us(1);
	
	CLR_SDA;
	I2C_Delay_us(10);
	
	CLR_SCL;
	I2C_Delay_us(1);
}
	
/*
	* @name   Stop
	* @brief  I2Cֹͣ�ź�
	* @param  None
	* @retval None      
*/
static void Stop(void)
{
  //SCLΪ�ߵ�ƽ��SDA��������ΪI2Cֹͣ�ź�
	CLR_SDA;
	SET_SCL;
	I2C_Delay_us(1);
		
	I2C_Delay_us(10);
	SET_SDA;
}

/*
	* @name   Write_Byte
	* @brief  I2Cд�ֽ�
	* @param  WR_Byte -> ��д������
	* @retval ACK_Value_t -> �ӻ�Ӧ��ֵ      
*/
static ACK_Value_t Write_Byte(uint8_t WR_Byte)
{
	uint8_t i;
	ACK_Value_t  ACK_Rspond;
	
	//SCLΪ�͵�ƽʱ��SDA׼������,����SCLΪ�ߵ�ƽ����ȡSDA����
	//���ݰ�8λ���䣬��λ��ǰ������forѭ���������
	for(i=0;i<8;i++)
	{
		//SCL���㣬����SDA׼������
		CLR_SCL;
		I2C_Delay_us(1);
		if((WR_Byte&BIT7) == BIT7)
		{
			SET_SDA;
		}
		else
		{
			CLR_SDA;
		}
		I2C_Delay_us(1);
		//SCL�øߣ���������
		SET_SCL;
		I2C_Delay_us(10);
		
		//׼��������һ����λ
		WR_Byte <<= 1;
	}
	
	CLR_SCL;	
	//�ͷ�SDA���ȴ��ӻ�Ӧ��
	SET_SDA;
	I2C_Delay_us(1);
	
	SET_SCL;
	I2C_Delay_us(10);
	
	ACK_Rspond = (ACK_Value_t)READ_SDA;
	
	CLR_SCL;
	I2C_Delay_us(1);
	
	//���شӻ���Ӧ���ź�
	return ACK_Rspond;
}


/*
	* @name   Write_Byte
	* @brief  I2Cд�ֽ�
	* @param  ACK_Value -> ������Ӧֵ
	* @retval �ӻ�����ֵ      
*/
static uint8_t Read_Byte(ACK_Value_t ACK_Value)
{
	uint8_t RD_Byte = 0,i;
		
	//��������
	//SCLΪ�͵�ƽʱ��SDA׼������,����SCLΪ�ߵ�ƽ����ȡSDA����
	//���ݰ�8λ���䣬��λ��ǰ������forѭ���������
	for(i=0;i<8;i++)
	{
		//׼��������һ����λ
		RD_Byte <<= 1;
		
		//SCL���㣬�ӻ�SDA׼������
		CLR_SCL;
		I2C_Delay_us(10);
		
		//SCL�øߣ���ȡ����
		SET_SCL;
		I2C_Delay_us(10);	

		RD_Byte |= READ_SDA;		
	}
	
	
	//SCL���㣬����׼��Ӧ���ź�
	CLR_SCL;
	I2C_Delay_us(1);
	
	//��������Ӧ���ź�	
	if(ACK_Value == ACK)
	{
		CLR_SDA;
	}
	else
	{
		SET_SDA;	
  }	
	I2C_Delay_us(1);
	
	
	SET_SCL; 	
	I2C_Delay_us(10);
	
	//Note:
  //�ͷ�SDA������
	//SCL�����㣬���ͷ�SDA����ֹ������������ʱ���ӻ���SDA�ͷ��źŵ���NACk�ź�
	CLR_SCL;
  SET_SDA; 	
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
/********************************************************
  End Of File
********************************************************/
