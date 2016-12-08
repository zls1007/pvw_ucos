#ifndef _I2C_H_
#define _I2C_H_
#include "stm32f4xx.h"

//=========================
//从设备地址 +0 为写入；+1为读取 
//值和芯片有关，请注意根据实际情况更改
//MPU6050的SlaveAddress为 0xd0
//HMC5883的SlaveAddress为0x3C
//=========================
//电池设备地址
#define SlaveAddress 	0x16

//=========================
//I2C引脚定义
//=========================
#define SCL_0		GPIO_ResetBits(GPIOB,GPIO_Pin_8)	//PB8
#define SCL_1		GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define SDA_0		GPIO_ResetBits(GPIOB,GPIO_Pin_9)			//PB9
#define SDA_1		GPIO_SetBits(GPIOB,GPIO_Pin_9)

#define	SDA_STATUS	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
//=========================
//函数声明
//=========================
void I2C_Delay(void)  ;
void I2C_init(void);
void I2C_Write_Reg(unsigned char REG_Address,unsigned char REG_data);
unsigned char I2C_Read_Reg(unsigned char REG_Address);

void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(unsigned char ack);
unsigned char I2C_RecvACK(void);
void I2C_Write(unsigned char dat);
unsigned char I2C_Read(void);
u8 GetCrc8(u8 chkSum, u8 crcData);



#endif
