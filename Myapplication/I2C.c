/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
/*********************************SHT31相关宏定义**********************************************/
//置位与清零SCL管脚
#define SET_SCL_SHT31 HAL_GPIO_WritePin(SHT31_SCL_GPIO_Port,SHT31_SCL_Pin,GPIO_PIN_SET) 
#define CLR_SCL_SHT31 HAL_GPIO_WritePin(SHT31_SCL_GPIO_Port,SHT31_SCL_Pin,GPIO_PIN_RESET) 
//置位与清零SDA管脚
#define SET_SDA_SHT31 HAL_GPIO_WritePin(SHT31_SDA_GPIO_Port,SHT31_SDA_Pin,GPIO_PIN_SET)
#define CLR_SDA_SHT31 HAL_GPIO_WritePin(SHT31_SDA_GPIO_Port,SHT31_SDA_Pin,GPIO_PIN_RESET)
//读SDA管脚状态
#define READ_SDA_SHT31	HAL_GPIO_ReadPin(SHT31_SDA_GPIO_Port,SHT31_SDA_Pin)

/*********************************BH1750相关宏定义**********************************************/
#define SET_SCL_BH1750 HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_SET) 
#define CLR_SCL_BH1750 HAL_GPIO_WritePin(BH1750_SCL_GPIO_Port,BH1750_SCL_Pin,GPIO_PIN_RESET) 
//置位与清零SDA管脚
#define SET_SDA_BH1750 HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_SET)
#define CLR_SDA_BH1750 HAL_GPIO_WritePin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin,GPIO_PIN_RESET)
//读SDA管脚状态
#define READ_SDA_BH1750	HAL_GPIO_ReadPin(BH1750_SDA_GPIO_Port,BH1750_SDA_Pin)
/* Private variables----------------------------------------------------------*/
/*SHT31*/
void Init(void);  //I2C初始化
void Start(void); //I2C起始信号
void Stop(void);  //I2C停止信号
ACK_Value_t Write_Byte(uint8_t);      //I2C写字节
uint8_t     Read_Byte (ACK_Value_t);  //I2C读字节

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
	* @brief  I2C初始化
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
	* @brief  I2C起始信号
	* @param  None
	* @retval None      
*/
void Start(void)
{
	//SCL为高电平，SDA的下降沿为I2C起始信号
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
	* @brief  I2C停止信号
	* @param  None
	* @retval None      
*/
void Stop(void)
{
	//SCL为高电平，SDA的上升沿为I2C停止信号
	CLR_SDA_SHT31;
	SET_SCL_SHT31;
	I2C_Delay_us(1);
		
	I2C_Delay_us(10);
	SET_SDA_SHT31;
}

/*
	* @name   Write_Byte
	* @brief  I2C写字节
	* @param  WR_Byte -> 待写入数据
	* @retval ACK_Value_t -> 从机应答值      
*/
ACK_Value_t Write_Byte(uint8_t WR_Byte)
{
	uint8_t i;
	ACK_Value_t  ACK_Rspond;
	
	//SCL为低电平时，SDA准备数据,接着SCL为高电平，读取SDA数据
	//数据按8位传输，高位在前，利用for循环逐个接收
	for(i=0;i<8;i++)
	{
		//SCL清零，主机SDA准备数据
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
		//SCL置高，传输数据
		SET_SCL_SHT31;
		I2C_Delay_us(10);
		
		//准备发送下一比特位
		WR_Byte <<= 1;
	}
	
	CLR_SCL_SHT31;	
	//释放SDA，等待从机应答
	SET_SDA_SHT31;
	I2C_Delay_us(1);
	
	SET_SCL_SHT31;
	I2C_Delay_us(10);
	
	ACK_Rspond = (ACK_Value_t)READ_SDA_SHT31;
	
	CLR_SCL_SHT31;
	I2C_Delay_us(1);
	
	//返回从机的应答信号
	return ACK_Rspond;
}

/*
	* @name   Read_Byte
	* @brief  I2C读字节
	* @param  ACK_Value -> 主机回应值
	* @retval 从机返回值      
*/
uint8_t Read_Byte (ACK_Value_t ACK_Value)
{
	uint8_t RD_Byte = 0,i;
		
	////接收数据
	//SCL为低电平时，SDA准备数据,接着SCL为高电平，读取SDA数据
	//数据按8位传输，高位在前，利用for循环逐个接收
	for(i=0;i<8;i++)
	{
		//准备接收下一比特位
		RD_Byte <<= 1;
		
		//SCL清零，从机SDA准备数据
		CLR_SCL_SHT31;
		I2C_Delay_us(10);
		
		//SCL置高，获取数据
		SET_SCL_SHT31;
		I2C_Delay_us(10);	

		RD_Byte |= READ_SDA_SHT31;		
	}
	
	
	//SCL清零，主机准备应答信号
	CLR_SCL_SHT31;
	I2C_Delay_us(1);
	
	//主机发送应答信号	
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
  //释放SDA数据线
	//SCL先清零，再释放SDA，防止连续传输数据时，从机错将SDA释放信号当成NACk信号
	CLR_SCL_SHT31;
  SET_SDA_SHT31; 	
	I2C_Delay_us(1);

	//返回数据
	return RD_Byte;
}

/*
	* @name   I2C_Delay
	* @brief  I2C延时
	* @param  None
	* @retval None      
*/
static void I2C_Delay_us(uint8_t us)
{
	uint8_t i = 0;
	
	//通过示波器测量进行校准
	while(us--)
	{
		for(i=0;i<7;i++);
	}
}

/******************************************************************************************************************/
static void i2c_Delay(void);

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */	
	CLR_SDA_BH1750;
  SET_SCL_BH1750;
  i2c_Delay();
	SET_SDA_BH1750;	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
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
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
static uint8_t i2c_ReadByte(void)
{
 	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
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
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
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
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_NAck(void)
{
	SET_SDA_BH1750;	/* CPU驱动SDA = 1 */
	i2c_Delay();
	SET_SCL_BH1750;	/* CPU产生1个时钟 */
	i2c_Delay();
	CLR_SCL_BH1750;
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
static uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	I2C_BH1750.i2c_Start();		/* 发送启动信号 */
	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | BH1750_I2C_WR);
	ucAck = I2C_BH1750.i2c_WaitAck();	/* 检测设备的ACK应答 */

	I2C_BH1750.i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}


/*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/********************************************************
  End Of File
********************************************************/
