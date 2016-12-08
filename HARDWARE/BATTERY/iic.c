#include "iic.h"
#include "includes.h"


void I2C_Delay(void)   // 5us
{
    unsigned char a,b;
    for(b=51;b>0;b--)
        for(a=50;a>0;a--);
}
//=================================
//I2C引脚配置
//=================================
void I2C_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	//打开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//PB8 PB9 开漏输出 外接上拉电阻
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;	//选定引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//设置为开漏输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//输出频率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//设置为上拉   GPIO_PuPd_NOPULL(不上拉)	GPIO_PuPd_UP
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

//==========================================
//I2C起始信号
//==========================================
void I2C_Start(void)
{
	SDA_1;
	SCL_1;
	I2C_Delay();
	SDA_0;
	I2C_Delay();
	SCL_0;
}

//==========================================
//I2C停止信号
//==========================================
void I2C_Stop(void)
{
	SDA_0;
	SCL_1;
	I2C_Delay();
	SDA_1;
	I2C_Delay();
}

//==========================================
//I2C应答机制：应答信号在第9个时钟上出现
//接收器输出低电平（0）为应答信号（A），输出高电平（1）为非应答信号（/A）
//主控器输出高电平（1）为应答信号（A），输出低电平（0）为非应答信号（/A）
//==========================================
//==========================================
//I2C发送应答信号
//入口参数:ack (0:ACK 1:NAK)(0:应答低电平，1：应答高电平)
//==========================================
void I2C_SendACK(unsigned char ack)
{
	if(ack == 1)
		SDA_1;
	else
		SDA_0;
	SCL_1;
	I2C_Delay();
	SCL_0;
	I2C_Delay();
}

//==========================================
//I2C接收应答信号
//==========================================
unsigned char I2C_RecvACK(void)
{
	unsigned char ack;
	SDA_1;
	SCL_1;
	I2C_Delay();
	if(SDA_STATUS != 0)
		ack = 1;
	else
		ack = 0;
	SCL_0;
	I2C_Delay();
	return ack;
}

//==========================================
//向I2C总线发送一个字节数据 SCL引脚由1变0 单片机写SDA供I2C设备读取
//每写完一个字节都要从从设备接收应答数据
//==========================================
void I2C_Write(unsigned char dat)
{
	unsigned char i;
	for(i = 0;i<8;i++)
	{
		if((dat & 0x80) != 0)
			SDA_1;
		else
			SDA_0;
		SCL_1;
		I2C_Delay();
		SCL_0;
		I2C_Delay();
		
		dat = dat << 1;
	}
	I2C_RecvACK();
}

//==========================================
//从I2C总线接收一个字节数据 SCL引脚由1变0，I2C设备写SDA供单片机读取
//读取成功后（返回值成功赋给一个变量后），向从设备发送非应答信号。
//==========================================
unsigned char I2C_Read(void)
{
	unsigned char i;
    unsigned char dat = 0;
	SDA_1;
	for(i=0; i<8;i++)
	{
		dat = dat <<1;
		SCL_1;
		I2C_Delay();
		if(SDA_STATUS)
			dat |= 0x01;
		SCL_0;
		I2C_Delay();
	}
	return dat;
}

//==========================================
//向I2C设备写入一个字节数据
//==========================================
void I2C_Write_Reg(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                //起始信号
    I2C_Write(SlaveAddress);   	//发送设备地址+写信号（+0）
    I2C_Write(REG_Address);    	//内部寄存器地址，
    I2C_Write(REG_data);       	//内部寄存器数据，
    I2C_Stop();                 //发送停止信号
}

//==========================================
//从I2C设备读取一个字节数据
//==========================================
unsigned char I2C_Read_Reg(unsigned char REG_Address)
{
	unsigned char REG_data;
	//向从设备发送单片机即将何处取数据
	I2C_Start();                //起始信号
	I2C_Write(SlaveAddress);    //发送设备地址+写信号（+0）
	I2C_Write(REG_Address);     //发送存储单元地址
//	I2C_Stop();
	
	I2C_Start();                //起始信号
	I2C_Write(SlaveAddress+1); 	//发送设备地址+读信号（+1）
	REG_data=I2C_Read();       	//读出寄存器数据
	I2C_SendACK(1);             //发送非应答信号给从设备 接收完一个字节，向从设备发送非应答信号(1)
	I2C_Stop();                 //停止信号
	
	return REG_data;
}


//==========================================
//I2C CRC 校验
//==========================================
#define POLYNOME 0x07
u8 GetCrc8(u8 chkSum, u8 crcData)
{
	u8 j = 8;
	chkSum ^= crcData;
	do
	{
		if(!(chkSum & 0x80))
		{
			chkSum = chkSum << 1;
		}
		else
		{
			chkSum = (chkSum << 1)^POLYNOME;
		}
	}while(--j);
	return chkSum;
}
