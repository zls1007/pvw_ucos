#include "iic.h"
#include "includes.h"


void I2C_Delay(void)   // 5us
{
    unsigned char a,b;
    for(b=51;b>0;b--)
        for(a=50;a>0;a--);
}
//=================================
//I2C��������
//=================================
void I2C_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	//��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//PB8 PB9 ��©��� �����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;	//ѡ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//����Ϊ��©���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//���Ƶ��Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//����Ϊ����   GPIO_PuPd_NOPULL(������)	GPIO_PuPd_UP
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

//==========================================
//I2C��ʼ�ź�
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
//I2Cֹͣ�ź�
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
//I2CӦ����ƣ�Ӧ���ź��ڵ�9��ʱ���ϳ���
//����������͵�ƽ��0��ΪӦ���źţ�A��������ߵ�ƽ��1��Ϊ��Ӧ���źţ�/A��
//����������ߵ�ƽ��1��ΪӦ���źţ�A��������͵�ƽ��0��Ϊ��Ӧ���źţ�/A��
//==========================================
//==========================================
//I2C����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)(0:Ӧ��͵�ƽ��1��Ӧ��ߵ�ƽ)
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
//I2C����Ӧ���ź�
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
//��I2C���߷���һ���ֽ����� SCL������1��0 ��Ƭ��дSDA��I2C�豸��ȡ
//ÿд��һ���ֽڶ�Ҫ�Ӵ��豸����Ӧ������
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
//��I2C���߽���һ���ֽ����� SCL������1��0��I2C�豸дSDA����Ƭ����ȡ
//��ȡ�ɹ��󣨷���ֵ�ɹ�����һ�������󣩣�����豸���ͷ�Ӧ���źš�
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
//��I2C�豸д��һ���ֽ�����
//==========================================
void I2C_Write_Reg(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                //��ʼ�ź�
    I2C_Write(SlaveAddress);   	//�����豸��ַ+д�źţ�+0��
    I2C_Write(REG_Address);    	//�ڲ��Ĵ�����ַ��
    I2C_Write(REG_data);       	//�ڲ��Ĵ������ݣ�
    I2C_Stop();                 //����ֹͣ�ź�
}

//==========================================
//��I2C�豸��ȡһ���ֽ�����
//==========================================
unsigned char I2C_Read_Reg(unsigned char REG_Address)
{
	unsigned char REG_data;
	//����豸���͵�Ƭ�������δ�ȡ����
	I2C_Start();                //��ʼ�ź�
	I2C_Write(SlaveAddress);    //�����豸��ַ+д�źţ�+0��
	I2C_Write(REG_Address);     //���ʹ洢��Ԫ��ַ
//	I2C_Stop();
	
	I2C_Start();                //��ʼ�ź�
	I2C_Write(SlaveAddress+1); 	//�����豸��ַ+���źţ�+1��
	REG_data=I2C_Read();       	//�����Ĵ�������
	I2C_SendACK(1);             //���ͷ�Ӧ���źŸ����豸 ������һ���ֽڣ�����豸���ͷ�Ӧ���ź�(1)
	I2C_Stop();                 //ֹͣ�ź�
	
	return REG_data;
}


//==========================================
//I2C CRC У��
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
