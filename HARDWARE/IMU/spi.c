/**
  ******************************************************************************
  * @file    spi.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-15
  * @brief   本文件对spi1进行配置，来负责对mpu9250进行数据采集
	*			硬件资源：
  *          	SPI1  工作频率1Mhz			非中断方式
  *
	*			函数：
	*						SPI1_Init(void);     spi1配置函数
	*						SPI_RW(u8 byte);		 spi1读写函数，读取某寄存器地址的值
*/
#include "spi.h"
/****************************************************************************
*			SPI1 配置函数
*			PA5 ---- SCK	
*			PA6 ---- MISO   
*			PA7 ---- MOSI	
*			PC2 ---- CS 
*			PD15 ---- DRDY 
*			256Mhz/256
******************************************************************************/
void SPI1_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

		//开启时钟
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD ,ENABLE);
	
		//配置GPIO（复用）
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	
	  //配置GPIO（推挽输出）
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
  	GPIO_SetBits(GPIOC, GPIO_Pin_2);//不选中
		
		//SPI1配置
  	SPI_I2S_DeInit(SPI1);
  	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
  	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据模式
  	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//空闲模式下SCK为1
  	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//数据采样从第2个时间边沿开始
  	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS软件管理
  	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//波特率
  	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//大端模式
  	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRC多项式
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
  	SPI_Init(SPI1, &SPI_InitStructure);
  	SPI_Cmd(SPI1, ENABLE);
}

/****************************************************************************
*			SPI1 读寄存器值函数
*			输入：
*					byte -- 寄存器地址
*			返回：
*					寄存器的值
******************************************************************************/
u8 SPI_RW(u8 byte)
{
 	while((SPI1->SR&SPI_I2S_FLAG_TXE)==RESET);
 	SPI1->DR = byte;
 	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==RESET);
 	return(SPI1->DR);
}
